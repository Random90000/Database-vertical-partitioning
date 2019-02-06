
#include <algorithm>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <utility>
#include <iomanip>

#include "queries_run_rewritted.hpp"
#include "DB.hpp"

using namespace std;

int main(){

DB database("host=localhost user=postgres password=postgres dbname=sdss_extract", 1);

float clusterized_summary_time = 0;

vector<Query> queries;
ifstream file("rewritted");
string line;
while(getline(file, line)) queries.push_back({line});
file.close();

for(auto& q : queries)
{
    cout << q << endl;
    auto t = database.timeExecution(q);
    cout << " : " << t << endl;
    clusterized_summary_time += t;
}
cout << "TOTAL CLUSTERIZED:\n TIME : " << clusterized_summary_time;

return 0;
}
