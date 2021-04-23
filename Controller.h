//
// Created by torchtheo on 2021/4/16.
//

#ifndef CLIONPROJECTS_CONTROLLER_H
#define CLIONPROJECTS_CONTROLLER_H
#include <iostream>
#include <vector>
#include "CTable.h"
#include "Sql.h"

enum SQLACTION {
    CREATE_TABLE_ERROR, DROP_TABLE_ERROR, CREATE_TABLE, SHOW_TABLES, EXIT_SYSTEM, DROP_TABLE, INSERT, SELECT, UPDATE, DELETE, UNKNOWNSQL_ERROR, ALTER
};

class Controller {
public:
    Controller();
    void doSql();
    void createTable(string name, string attributes);
    static void writeData();
    static string upper(string s);
    void showTables();
    void dropTable(string Tname);
    void insertToTable(string Tname, string attributes);
    void select(vector<string> sql);
    void update(vector<string> sql);
    void deleteRows(vector<string> sql);
    void alter(vector<string> sql);
    static vector<string> tableNames;
    static vector<CTable*> tables;
};


#endif //CLIONPROJECTS_CONTROLLER_H
