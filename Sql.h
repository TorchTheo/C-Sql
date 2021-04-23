//
// Created by torchtheo on 2021/4/16.
//

#ifndef CLIONPROJECTS_SQL_H
#define CLIONPROJECTS_SQL_H
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include "CTable.h"
using namespace std;
class Sql {
public:
    Sql(string str);
    vector<string> readSql();
    int anaSql();
    string originSql;
    vector<string> targetSql;
};


#endif //CLIONPROJECTS_SQL_H
