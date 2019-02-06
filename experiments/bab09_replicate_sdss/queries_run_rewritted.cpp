#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <utility>

#include "DB.hpp"
#include "queries_run_rewritted.hpp"
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

