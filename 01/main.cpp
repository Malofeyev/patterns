#include <iostream>
#include "sql_query_builder.h"

int main(int argc, char *argv[]) {
    std::cout << "Hello, World!\n";

    
    SqlSelectQueryBuilder query_builder;
    query_builder.AddColumn("name").AddColumn("phone");
    query_builder.AddFrom("students");
    query_builder.AddWhere("id", "42").AddWhere("name", "John");
    std::cout << query_builder.BuildQuery();
    return 0;
}
