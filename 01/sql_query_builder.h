#pragma once

#include <optional>
#include <string>
#include <vector>

class SqlSelectQueryBuilder {
public:
    SqlSelectQueryBuilder& AddColumn(std::string field);
    SqlSelectQueryBuilder& AddFrom(std::string table);
    SqlSelectQueryBuilder& AddWhere(std::string var, std::string val);
    std::string BuildQuery() const;
private:
    std::vector<std::string> projection;
    std::optional<std::string> table;
    std::vector<std::pair<std::string, std::string>> filter;
};
