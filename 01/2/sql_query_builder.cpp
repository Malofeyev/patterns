#include "sql_query_builder.h"


SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddColumn(std::string field) {
    projection.push_back(std::move(field));
    return *this;
}

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddFrom(std::string table) {
    this->table = std::move(table);
    return *this;
}

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddWhere(std::string var, std::string val) {
    filter.push_back(std::make_pair(std::move(var), std::move(val)));
    return *this;
}


SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddWhere(const std::map<std::string, std::string>& kv) noexcept {
    filter.reserve(filter.size() + kv.size());
    filter.insert(filter.end(), kv.begin(), kv.end());
    return *this;
}

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddColumns(const std::vector<std::string>& columns) noexcept {
    projection.reserve(projection.size() + columns.size());
    projection.insert(projection.end(), columns.begin(), columns.end());
    return *this;
}

std::string SqlSelectQueryBuilder::BuildQuery() const {
    std::string result = "SELECT ";
    if (projection.empty()) {
        result += "*";
    } else {
        for (int i = 0; i + 1 < projection.size(); i++) {
            result += projection[i] + ", ";
        }
        result += projection.back();
    }

    if (!table) {
        return result + ";";
    } else {
       result += " FROM "+ table.value();
    }

    if (!filter.empty()) {
        result += " WHERE ";
        for (int i = 0; i + 1 < filter.size(); i++) {
            result += filter[i].first + "=" + filter[i].second + " AND "; 
        }
        const auto& eq = filter.back();
        result += eq.first + "=" + eq.second;
    }
    result += ";";
    return result;


}
