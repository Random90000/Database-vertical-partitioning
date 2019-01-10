#include <algorithm>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <utility>

#include "DB.hpp"
#include "matrix.hpp"
#include "queries.hpp"
#include "pbab.hpp"
#include "bab11.hpp"
#include "util.hpp"

using namespace std;

void usage(){
    cout << "vpart [-u username] [-p password] [-h host] [-d dbname] [-t table]" << endl;
    cout << "--first - return the first solution (may be lacking in quality)" << endl;
    cout << "--no-exec - do not run the workload" << endl;
    cout << "Algorithms:" << endl;
    cout << "--CI - use the CI algorithm" << endl;
    cout << "--optimal bound - use the 'optimal' from 95" << endl;
    cout << "--blocking bound - algorithm from 94" << endl;
    cout << "--void bound - heuristic algorithm from 95" << endl;
    cout << "--bab09 cohesion - algorithm from 09" << endl;
    cout << "Intercluster columns handling:" << endl;
    cout << "--replicate - placed in all clusters" << endl;
    cout << "--separate - dedicate a cluster" << endl;
    cout << "--nearest - place in a closest cluster" << endl;
    cout << "Defaults:" << endl;
    cout << "\tHost: localhost" << endl;
    cout << "\tUser: postgres" << endl;
    cout << "\tDBName: postgres" << endl;
}

int main(int argc, char* argv[]){
    string host = "localhost";
    string user = "postgres";
    string password = "12qw12qw";
    string dbname = "tpch";
    string table_name = "";

    int CI = 0;
    int OPTIMAL = 0;
    int OPTIMAL_UPPER_BOUND;
    int VOID = 0;
    int VOID_UPPER_BOUND;
    int BLOCKING = 0;
    int BLOCKING_UPPER_BOUND;
    int BAB09 = 0;
    int BAB11 = 0;
    int REPLICATE = 0, SEPARATE = 0, NEAREST = 0;
    int RETURN_FIRST = 0;
    int EXEC_QUERIES = 1;
    float BAB09_THRESHOLD;
    float BAB11_THRESHOLD;
    {
        struct option long_opts[] = {
            {"CI", 0, &CI, 1},
            {"optimal", 1, &OPTIMAL, 1},
            {"void", 1, &VOID, 1},
            {"blocking", 1, &BLOCKING, 1},
            {"bab09", 1, &BAB09, 1},
            {"bab11", 1, &BAB11, 1},
	    {"replicate", 0, &REPLICATE, 1},
            {"separate", 0, &SEPARATE, 1},
            {"nearest", 0, &NEAREST, 1},
            {"no-exec", 0, &EXEC_QUERIES, 0},
            {"first", 0, &RETURN_FIRST, 1},
            {0, 0, 0, 0}
        };
        int c;
        int index;
        opterr = 1;
        do {
            c = getopt_long(argc, argv, "u:p:h:d:t:", long_opts, &index);
            if(!c) switch(index){
                case 1:
                    OPTIMAL_UPPER_BOUND = atoi(optarg);
                    if(OPTIMAL_UPPER_BOUND < 1){
                        cerr << "Wrong --optimal argument" << endl;
                        return 1;
                    }
                    break;
                case 2:
                    VOID_UPPER_BOUND = atoi(optarg);
                    if(VOID_UPPER_BOUND < 1){
                        cerr << "Wrong --void argument" << endl;
                        return 1;
                    }
                    break;
                case 3:
                    BLOCKING_UPPER_BOUND = atoi(optarg);
                    if(BLOCKING_UPPER_BOUND < 1){
                        cerr << "Wrong --blocking argument" << endl;
                        return 1;
                    }
                    break;
                case 4:
                    BAB09_THRESHOLD = atof(optarg);
                    if(BAB09_THRESHOLD < 0 || BAB09_THRESHOLD > 1){
                        cerr << "Wrong --bab09 argument" << endl;
                        return 1;
                    }
                    break;
                case 5:
		    BAB11_THRESHOLD = atof(optarg);
		    if(BAB11_THRESHOLD < 0 || BAB11_THRESHOLD > 1){
			cerr << "Wron --bab11 argument" << endl;
			return 1;
		    }
		    break;
            } else switch(c){
                case 'u': user = optarg; break;
                case 'p': password = optarg; break;
                case 'h': host = optarg; break;
                case 'd': dbname = optarg; break;
                case 't': table_name = optarg; break;
                case '?': cerr << "getopt error" << endl; usage(); return 1;
            }
        } while (c != -1);
    }

    if(CI + OPTIMAL + BLOCKING + VOID + BAB09 + BAB11 != 1){
        cerr << "You should specify exactly one algorithm\n";
        return 1;
    }

    if(REPLICATE + SEPARATE + NEAREST != 1){
        cerr << "You should specify exactly one column handling scheme\n";
        return 1;
    }

    if(table_name == ""){
        cerr << "You must specify a table name\n";
        return 1;
    }

    string conn_str = "host=" + host + " user=" + user +
                      (password.size() ? " password=" + password : "") +
                      " dbname=" + dbname;

    try {
        // set up the connection
        DB database(conn_str, EXEC_QUERIES);
        Table table(database, table_name);

        // print out some info about the table
        const vector<string>& columns = table.columns();
        const vector<string>& primary = table.primary_column();
        bool  hasPrimary              = primary.size();
        float not_clusterized_time    = 0;
        float clusterized_time        = 0;
        if(hasPrimary){
            cout << "Primary columns: " << join(primary, " ") << endl;
        } else cout << "No primary column, won't be able to partition correctly" << endl;
        cout << "Number of other columns: " << columns.size() << endl;
        cout << "Other columns:" << endl;
        for(string col : columns) cout << col << endl;

        // read the queries
        vector<Query> queries;
        {
            ifstream file("workload");
            string line;
            while(getline(file, line)) queries.push_back({line});
            file.close();
        }
        
        if(EXEC_QUERIES){
            cout << "Running queries:" << endl;
            for(auto& q : queries){
                auto t = database.timeExecution(q);
                cout << q << endl; cout << " : " << t << endl;
                not_clusterized_time += t;
            }
        }

        // construct the matrix
        Matrix m(queries.size(), columns.size());
        for(int i = 0; i < queries.size(); ++i){
            vector<string>& fields = queries[i].attributes;
            for(int j = 0; j < columns.size(); ++j)
                m[j][i] = find(fields.begin(), fields.end(), table_name + "." + columns[j]) == fields.end() ? 0 : 1;
        }
        {
            // remove empty rows and cols
            auto clusters = m.clusters();
            if(clusters.size() == 1)
                m = std::move(clusters[0]);
        }
        cout << "Matrix:" << endl << m << endl;

        // clusterize
        vector<vector<string>> clusters;
        {
            Solution output;
            // run the chosen algorithm
            if(CI)       output.clusters = m.clusters();
            if(OPTIMAL)  output = PBAB<Optimal>   (m, RETURN_FIRST, OPTIMAL_UPPER_BOUND);
            if(BLOCKING) output = PBAB<Blocking>  (m, RETURN_FIRST, BLOCKING_UPPER_BOUND);
            if(VOID)     output = PBAB<Void>      (m, RETURN_FIRST, VOID_UPPER_BOUND);
            if(BAB09)    output = PBAB<Strategy09>(m, RETURN_FIRST, BAB09_THRESHOLD);
            if(BAB11)    { BabNode::threshold = BAB11_THRESHOLD; output = Bab11(m); }
	    auto& intercluster = output.intercluster;
            // print out clusters and distribute columns appropriately
            cout << "Intercluster:" << endl;
            for(int i : intercluster)
                cout << (i+1) << ": " << columns[i] << endl;
            cout << "Submatrices:" << endl;
            for(Matrix& cluster : output.clusters){
                cout << cluster << endl;
                clusters.push_back(vector<string>());
                vector<string>& current = clusters.back();
                for(int i = 0; i < cluster.C; ++i){
                    // only the Strategy09 leaves all columns in clusters
                    // the whole point of the NEAREST strategy is to keep it that way
                    // so if we are Strategy09 and a different strategy,
                    // then we need to pick them out
                    if(!BAB09 || NEAREST || intercluster.find(cluster.col_id[i]) == intercluster.end())
                        // it's not an intercluster column so we can add it
                        current.push_back(columns[cluster.col_id[i]]);
                }
                if(current.size() == 0){
                    // it's possible to have an empty cluster if all columns are intercluster
                    // in that case queries will have to use other clusters that have these
                    clusters.pop_back();
                }
            }
            // replicating strategy puts a copy of a column everywhere it's used
            if(REPLICATE && !BAB11)
                for(int k = 0; k < clusters.size(); ++k){
                    auto& c = output.clusters[k];
                    // check if col is used by this cluster's transactions
                    for(int i : intercluster){
                        // but first, find a corresponding column index
                        int col = 0;
                        for(; col < m.C; ++col)
                            if(m.col_id[col] == i) break;

                        for(int j = 0; j < c.R; ++j){
                            if(m[col][c.row_id[j]]){
                                // if so, add it
                                clusters[k].push_back(columns[i]);
                                break;
                            }
                        }
                    }
                }
            // just treat intercluster columns as another cluster
            if(SEPARATE && !BAB11){
                vector<string> inter;
                inter.reserve(intercluster.size());
                for(int i : intercluster) inter.push_back(columns[i]);
                clusters.push_back(std::move(inter));
            }
            // sensible for the Strategy09 algortihm only, handled above
            // TODO: design and implement for other algorithms
            if(NEAREST && !BAB09 && !BAB11)
                for(int i : intercluster)
                    clusters[0].push_back(columns[i]);
            // associate the clusters with queries
            {
                for(int i = 0; i < queries.size(); ++i)
                    queries[i].used_clusters.resize(clusters.size());
                // add the "main" cluster from a submatrix
                for(int i = 0; i < output.clusters.size(); ++i)
                for(int j = 0; j < output.clusters[i].R; ++j){
                    queries[output.clusters[i].row_id[j]].
                        used_clusters[i] = clusters[i];
                    cout << output.clusters[i].row_id[j] << ' ' << i << endl;
                }
                // if replicating, we are done
                // if using a separate cluster, we go through the queries
                if(SEPARATE && !BAB11){
                    const vector<string>& last = clusters.back();
                    for(Query& q: queries){
                        for(int i = 0; i < last.size(); ++i)
                        for(int j = 0; j < q.attributes.size(); ++j)
                            if(table_name + '.' + last[i] == q.attributes[j]){
                                q.used_clusters.back() = last;
                                goto next_query;
                            }
                        next_query:;
                    }
                } else if(NEAREST && !BAB11){
                    for(Query& q: queries){
                        for(int k = 0; k < clusters.size(); ++k){
                            if(q.used_clusters[k].size() > 0) continue;
                            const vector<string>& current = clusters[k];
                            for(int i = 0; i < current.size(); ++i)
                            for(int j = 0; j < q.attributes.size(); ++j)
                                if(table_name + '.' + current[i] == q.attributes[j]){
                                    q.used_clusters[k] = current;
                                }
                        }
                    }
                }
            }
        }


        cout << "Clusters:" << endl;
        for(auto& cluster : clusters){
            for(auto& col: cluster) cout << col << ' ';
            cout << endl;
        }

        cout << "Partitioning..." << endl;
        vector<string> new_tables = table.partition("test", clusters);
        cout << "Done" << endl;

        cout << "Rewriting queries" << endl;
        vector<string> new_queries;
        for(const Query& q : queries)
            new_queries.push_back(q.rewrite(table, new_tables));
        cout << "Done" << endl;
        if(EXEC_QUERIES){
            cout << "Running queries:" << endl;
            for(auto& q : new_queries){
                auto t = database.timeExecution(q);
                cout << q << endl; cout << " : " << t << endl;
                clusterized_time += t;
            }
        }
        database.delete_tables(new_tables);
        cout << "TOTAL NOT CLUSTERIZED TIME : " << not_clusterized_time << "\nTOTAL CLUSTERIZED TIME     : " << clusterized_time << "\n"; 
    } catch (pqxx::broken_connection){
        cerr << "Failed to connect to the database" << endl;
    } catch (NoSuchTable){
        cerr << "There is no table called " << table_name << endl;
    }
    return 0;
}
