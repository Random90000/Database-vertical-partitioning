#include <chrono>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "cstdlib"
#include <unistd.h>

#include "DB.hpp"
#include "util.hpp"

using namespace std;

DB::~DB(){
    // to drop temporary tables
    conn.disconnect();
}

vector<string> DB::columns(string table){
    pqxx::work work(conn);
    pqxx::result res = work.exec("select column_name from information_schema.columns where table_name = '" + table + "';");
    vector<string> result;
    for(const auto& row : res)
        result.push_back(row[0].c_str());
    return result;
}

vector<string> DB::primary_column(string table){
    pqxx::work work(conn);
    pqxx::result res = work.exec("SELECT a.attname\
        FROM   pg_index i\
        JOIN   pg_attribute a ON a.attrelid = i.indrelid\
            AND a.attnum = ANY(i.indkey)\
        WHERE  i.indrelid = '" + table + "'::regclass\
        AND    i.indisprimary;");
    vector<string> result;
    for(int i = 0; i < res.size(); ++i)
        result.push_back(res[i][0].c_str());
    return result;
}

float DB::timeExecution(string query){
    if(exec){
        float sum = 0;
        int iterations_count = 10;    
        for (int i = 0; i < iterations_count; i++)
        {
            //this->conn.pqxx::connection::~basic_connection();
            this->conn.deactivate();
            auto sys = system("service postgresql restart");
            //auto sys = system("sudo -u postgres psql -c \"select pg_stat_reset();select pg_stat_statements_reset();\"");
            std::ofstream("/proc/sys/vm/drop_caches") << 3;
            this->conn.activate();
            pqxx::work work(conn);
            auto disable_parallelism = work.exec(" set max_parallel_workers_per_gather to 0;");
            //auto drop_stat = work.exec("select pg_stat_reset();select pg_stat_statements_reset();");
            auto start = chrono::system_clock::now();
            auto res = work.exec(query);
            auto t = chrono::duration_cast<chrono::milliseconds>
                       (chrono::system_clock::now() - start).count();
            std::cout << "  " << t << "\n"; 
            sum += t;
            work.commit(); 
        }
        return sum/iterations_count;
    }
    else
    {
        return 0;
    }
}

std::vector<long> DB::execute_memory_cost(std::vector<std::string> tables){
    std::vector<long> tables_size;
    pqxx::work work(conn);
    pqxx::result r = work.exec("SELECT table_name, pg_total_relation_size(quote_ident(table_name))"
                               "FROM information_schema.tables WHERE table_schema = 'public' ORDER BY 1;");
    cout << "TABLES SIZE: \n";
    for (pqxx::result::size_type i=0; i < r.size(); ++i)
    {
        cout << r[i]["table_name"] << " " << r[i]["pg_total_relation_size"] << "\n";
        tables_size.push_back(std::atol(r[i]["pg_total_relation_size"].c_str()));
    }
    work.commit();
    return tables_size;
}

void DB::delete_tables(std::vector<std::string> tables){
    pqxx::work work(conn);
    for (auto t : tables)
    {
        work.exec("DROP TABLE " + t);
    }
    work.commit();
}

Table::Table(DB& database, string name): db(database) {
    this->name = name;
    prim_col = database.primary_column(name);
    cols = database.columns(name);
    for(auto& prim : prim_col)
        cols.erase(std::remove(cols.begin(), cols.end(), prim), cols.end());
}

vector<string> Table::partition(string suffix, vector<vector<string>>& partitions){
    int n = 0;
    vector<string> result;
    for(auto& part : partitions){
        pqxx::work work(db.conn);
        string new_table = name + "_" + suffix + "_" + to_string(n++);
        string query = "SELECT " + (prim_col.size() ? join(prim_col, ", ") + ", " : "") + join(part, ", ") + " INTO " + new_table + " FROM " + name + ";";
        if(prim_col.size())
            query += " ALTER TABLE " + new_table + " ADD PRIMARY KEY (" + join(prim_col, ",") + ");analyze " + new_table + ";";
        cout << query << endl;
        if(db.exec){
            work.exec(query);
            work.commit();
        }
        result.push_back(new_table);
    }
    return result;
}
