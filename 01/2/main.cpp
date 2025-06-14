#include <iostream>
#include "sql_query_builder.h"

int main(int argc, char *argv[]) {
    std::cout << "Hello, World!\n";

    SqlSelectQueryBuilder query_builder;
    query_builder.AddColumn("id");

    std::vector<std::string> fields = {"name", "phone"};
    query_builder.AddColumns(fields);

    query_builder.AddFrom("students");

    std::map<std::string, std::string> eqs = {{"id", "42"}, {"name", "John"}};
    query_builder.AddWhere(eqs);
    std::cout << query_builder.BuildQuery();
    return 0;
}
