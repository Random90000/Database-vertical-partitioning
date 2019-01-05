#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <utility>

#include "DB.hpp"
#include "queries.hpp"
#include "util.hpp"

using namespace std;

Query::Query(string line){
    size_t colon = line.find(":");
    attributes = split(line.substr(0, colon), ' ');
    query_text = line.substr(colon+1);
}

ostream& operator<<(ostream& stream, const Query& query){
    return stream << query.query_text;
}

string Query::rewrite(const Table& table, const vector<string>& new_tables) const {
    string query = query_text;
    const vector<vector<string>>& clusters = used_clusters;
    cout << "Looking at: " << query << endl;
    string select, from, where, rest;

    int pos = find_not_quoted(query, "from");
    select = query.substr(0, pos);
    query = query.substr(pos);

    pos = find_not_quoted(query, "where");
    if(pos == query.size()) pos = find_not_quoted(query, "group by");
    if(pos == query.size()) pos = find_not_quoted(query, "order by");
    from = query.substr(0, pos);
    query = query.substr(pos);

    pos = find_not_quoted(query, "group by");
    if(pos == query.size()) pos = find_not_quoted(query, "order by");
    where = query.substr(0, pos);
    rest = query.substr(pos);

    #ifdef DEBUG
    cout << "select : " << select << endl;
    cout << "from : " << from << endl;
    cout << "where : " << where << endl;
    cout << "rest : " << rest << endl;
    #endif

    // check the from clause for subqueries
    // and handle them recursively
    {
        int counter = 0;
        int start;
        for(int i = 0; i < from.size(); ++i){
            if(from[i] == '(')
                if(++counter == 1) start = i;
            if(from[i] == ')')
                if(--counter == 0){
                    string sub = from.substr(start+1, i-start-1);
                    Query subq(clusters, sub);
                    from.replace(start+1, i-start-1, subq.rewrite(table, new_tables));
                }
        }
    }
    // check the where clause for subqueries
    // and handle them recursively
    {
        int counter = 0;
        int subquery_brace = -1;
        int start;
        for(int i = 0; i < where.size(); ++i){
            if(where[i] == '('){
                ++counter;
                do ++i;
                while(where[i] == ' ');
                if(subquery_brace == -1 && where.substr(i, 6) == "select"){
                    subquery_brace = counter;
                    start = i;
                }
            }
            if(where[i] == ')'){
                if(counter-- == subquery_brace){
                    string sub = where.substr(start, i-start-1);
                    Query subq(clusters, sub);
                    string new_sub = subq.rewrite(table, new_tables);
                    where.replace(start, i-start-1, new_sub);
                    i += new_sub.size() - sub.size();
                    subquery_brace = -1;
                }
            }
        }
    }

    vector<string> used_tables;

    // table + '.' + clusters[i][j] -> new_tables[i] + '.' + clusters[i][j]
    for(int i = 0; i < clusters.size(); ++i)
        for(auto& column : clusters[i])
            if((replaceAll(select, table.get_name() + '.' + column,
                                   new_tables[i] + '.' + column)
              | replace_not_quoted(where,  table.get_name() + '.' + column,
                                   new_tables[i] + '.' + column)
              | replaceAll(rest,   table.get_name() + '.' + column,
                                   new_tables[i] + '.' + column))
            && (used_tables.size() == 0 || used_tables.back() != new_tables[i]))
                used_tables.push_back(new_tables[i]);

    #ifdef DEBUG
    cout << "used tables: " << endl;
    for(auto& t : used_tables) cout << t << endl;
    #endif

    if(used_tables.size()){
        // the primary column is copied to every table, so we'll just use the first we see
        for(auto& prim : table.primary_column()){
            replaceAll(select, table.get_name() + '.' + prim,
                               used_tables[0] + '.' + prim);
            replace_not_quoted(where,  table.get_name() + '.' + prim,
                               used_tables[0] + '.' + prim);
            replaceAll(rest,   table.get_name() + '.' + prim,
                               used_tables[0] + '.' + prim);
        }
        // replace the table name with a collection of relevant tables
        replace_not_quoted(from, ' ' + table.get_name() + ' ', ' ' + join(used_tables, " , ") + ' ');
        replace_not_quoted(from, ' ' + table.get_name() + ',', ' ' + join(used_tables, " , ") + ',');
        replace_not_quoted(from, ' ' + table.get_name() + ';', ' ' + join(used_tables, " , ") + ';');
    }

    if(used_tables.size() > 1){
        // put braces around the WHERE clause and connect subrelations
        bool was_empty = false;
        int offset = 0;
        if(where.size() == 0){
            was_empty = true;
            where = "where ";
            if(from.back() == ';'){
                from.back() = ' ';
                where += ';';
                offset = 1;
            }
        } else if(where.back() == ';') offset = 1;
        where.insert(6, "(");
        if(!was_empty){
            where.insert(where.size()-offset, ")");
        }
        vector<string> primary = table.primary_column();
        for(int i = 1; i < used_tables.size(); ++i){
            for(auto& col : primary)
                where.insert(where.size() - offset, (where.size() > 8 ? " and ( " : " ") +
                                                    used_tables[i-1] + '.' + col + " = " +
                                                    used_tables[i] + '.' + col + " ) ");
        }
    }

    cout << "Result: " + select + from + where + rest << endl;
    return select + from + where + rest;
}
