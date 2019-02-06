#include <pqxx/connection>
#include <pqxx/connection_base>
#include <pqxx/transaction>
#include <pqxx/result>
#include <string>
#include <vector>


struct NoSuchTable {};

class DB {
    
public:
std::string connection_string;
    pqxx::connection conn;
    bool exec;
    DB(std::string connection_string, bool exec_queries)
        :connection_string(connection_string),
         conn(connection_string),
         exec(exec_queries){}
    DB(DB& other)
        :connection_string(other.connection_string),
         conn(other.connection_string),
         exec(other.exec){}
    ~DB();

    std::vector<std::string> columns(std::string table);
    std::vector<std::string> primary_column(std::string table);
    float timeExecution(std::string query);
    void delete_tables(std::vector<std::string> tables);
    std::vector<long> execute_memory_cost(std::vector<std::string> tables);
    friend class Table;
};

class Table {
    DB& db;
    std::string name;
    std::vector<std::string> prim_col;
    std::vector<std::string> cols;
public:
    Table(DB& database, std::string name);
    const std::string get_name() const { return name; }
    const std::vector<std::string>& primary_column() const { return prim_col; }
    const std::vector<std::string>& columns() const { return cols; }
    std::vector<std::string> partition(std::string suffix, std::vector<std::vector<std::string>>& partitions);
};
