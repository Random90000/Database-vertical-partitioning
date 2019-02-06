#include <iostream>
#include <string>
#include <vector>
#include <utility>

struct Query {
    std::vector<std::string> attributes;
    std::string query_text;
    std::vector<std::vector<std::string>> used_clusters;

    Query(){}
    Query(std::string line);
    Query(const std::vector<std::vector<std::string>>& clusters, const std::string& text)
        : used_clusters(clusters), query_text(text){}

    operator std::string() const { return query_text; }
};

std::ostream& operator<<(std::ostream& stream, const Query& query);
