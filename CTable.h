//
// Created by torchtheo on 2021/4/16.
//

#ifndef CLIONPROJECTS_CTABLE_H
#define CLIONPROJECTS_CTABLE_H
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;
class Attribute {
public:
    Attribute(string name);
    string name;
    bool isPrimaryKey = false;
};

class Object {
public:
    map<Attribute*, string> attributes_val;
};

class CTable {
public:
    void insertObject(string attribute);
    CTable(string name, string attributes);
    void writeData();
    void showAttributes();
    void showTable_Distinct(string column);
    void showTable(string column);
    void showTable_Conditioned(string column, string condition);
    void showTable_Conditioned_Distinct(string column, string condition);
    void updateAttribute(string column, string condition="");
    void deleteRows(string condition="");
    void alterAdd(string attribute);
    void alterDrop(string attribute);
    vector<Attribute*> attributes;
    string TName;
    vector<Object*> objects;
};


#endif //CLIONPROJECTS_CTABLE_H
