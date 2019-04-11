#include <string>
#include <vector>

std::vector<std::string> split(std::string str, char delim);
std::string join(std::vector<std::string> strings, std::string delim);
int replaceAll(std::string& str, std::string sub_old, std::string sub_new);
int find_not_quoted(std::string&, std::string, int = 0);
int replace_not_quoted(std::string&, std::string, std::string);
