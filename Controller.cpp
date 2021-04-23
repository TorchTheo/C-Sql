//
// Created by torchtheo on 2021/4/16.
//

#include <fstream>
#include <cstring>
#include "Controller.h"
#include "CTable.h"

vector<CTable*> Controller::tables;
vector<string> Controller::tableNames;


void Controller::doSql() {
    string str;
    getline(cin, str);
    if(str[str.length() - 1] != ';') {
        cout<<"Error!\n";
        return;
    }
    str.pop_back();
    Sql *sql = new Sql(str);
    switch (sql->anaSql()) {
        case CREATE_TABLE_ERROR:
            cout<<"!!!CREATE TABLE ERROR!!!\n";
            return;
        case DROP_TABLE_ERROR:
            cout<<"DROP TABLE ERROR\n";
            return;
        case EXIT_SYSTEM:
            exit(0);
        case CREATE_TABLE:
            createTable(sql->targetSql[2], sql->targetSql[3]);
            break;
        case SHOW_TABLES:
            showTables();
            break;
        case DROP_TABLE:
            dropTable(sql->targetSql[2]);
            break;
        case INSERT:
            insertToTable(sql->targetSql[2], sql->targetSql[4]);
            break;
        case SELECT:
            select(sql->targetSql);
            break;
        case UPDATE:
            update(sql->targetSql);
            break;
        case DELETE:
            deleteRows(sql->targetSql);
            break;
        case ALTER:
            alter(sql->targetSql);
            break;
        case UNKNOWNSQL_ERROR:
            cout<<"ERROR: UNKNOWN SQL INSTRUCTION\n";
    }
}

void Controller::createTable(string name, string attributes) {
    //预处理
    string sub = "PRIMARY_KEY";
    int idx = 0;        //开始查找的下标 & 找到子串的首字母下标
    int cnt = 0;        //子串出现次数
    while ((idx = upper(attributes).find(sub, idx)) != upper(attributes).npos){
        idx++;
        cnt++;
    }
    if(cnt >= 2) {
        cout<<"ERROR: A TABLE ONLY HAVE AT MOST ONE PRIMARY_KEY\n";
        return;
    }
    for(int i = 0; i < tableNames.size(); i++)
        if(tableNames[i] == name) {
            cout<<"ERROR: TABLE " + name +  " EXISTS\n";
            return;
        }
    CTable *table = new CTable(name, attributes);
    tables.push_back(table);
    tableNames.push_back(table->TName);
    writeData();
    cout<<"TABLE " + table->TName + " HAS BEEN CREATED\n";
}

Controller::Controller() {
    ifstream ifs1("tables.txt", ios::in);
    system("if [ ! -d \"db\" ];then mkdir db; fi");
    string tableName;
    while(ifs1>>tableName) {
        tableNames.push_back(tableName);
        CTable *table = new CTable(tableName, "");
        tables.push_back(table);
    }
}

void Controller::writeData() {
    ofstream ofs("tables.txt", ios::out);
    for(int i = 0; i < tableNames.size(); i++)
        ofs<<tableNames[i]<<" ";
    ofs.close();
}

void Controller::showTables() {
    cout<<tables.size()<<" table(s):\n";
    for(int i = 0; i < tables.size(); i++) {
        cout<<tables[i]->TName<<"(";
        for(int j = 0; j < tables[i]->attributes.size(); j++) {
            cout<<tables[i]->attributes[j]->name;
            if(tables[i]->attributes[j]->isPrimaryKey)
                cout<<" PRIMARY_KEY";
            if(j != tables[i]->attributes.size() - 1)
                cout<<",";
        }
        cout<<")\n";
    }
}

string Controller::upper(string s) {
    for(int i = 0; i < s.length(); i++)
        if(s[i] >= 'a' && s[i] <= 'z')
            s[i] -= 32;
    return s;
}

void Controller::dropTable(string Tname) {
    bool flag = false;
    for(vector<string>::iterator it = tableNames.begin(); it != tableNames.end(); it++)  {
        if(*it == Tname) {
            flag = true;
            tableNames.erase(it);
            break;
        }
    }
    if(!flag) {
        cout<<Tname + " TABLE DOESN'T EXIST\n";
        return;
    }
    for(vector<CTable*>::iterator it = tables.begin(); it != tables.end(); it++)
        if((*it)->TName == Tname) {
            tables.erase(it);
            break;
        }
    char fileName[100];
    strcpy(fileName, ("db/" + Tname).c_str());
    remove(strcat(fileName, ".txt"));
    cout<<"DROP " + Tname + " TABLE SUCCESSFULLY\n";
    writeData();
}

void Controller::insertToTable(string Tname, string attributes) {
    CTable *table = nullptr;
    for(int i = 0; i < tables.size(); i++) {
        if(tables[i]->TName ==  Tname) {
            table = tables[i];
            break;
        }
    }
    if(!table) {
        cout<<"INSERT ERROR: TABLE " + Tname + " DOESN'T EXIST\n";
        return;
    }
    table->insertObject(attributes);
}

void Controller::select(vector<string> sql) {
    int index = -1, indexOfWhere =  -1;
    bool flag = false;
    for (int i = 0; i < sql.size(); i++) {
        if(upper(sql[i]) == "FROM")
            index = i;
        if(upper(sql[i]) == "WHERE") {
            flag =  true;
            indexOfWhere = i;
        }
    }
    if(index == -1) {
        cout<<"SELECT ERROR: WRONG FORMAT SQL\n";
        return;
    }
    CTable *table = nullptr;
    for(int i = 0; i < tables.size(); i++) {
        if(tables[i]->TName ==  sql[index + 1]) {
            table = tables[i];
            break;
        }
    }
    if(!table) {
        cout<<"SELECT ERROR: TABLE " + sql[index + 1] + " DOESN'T EXIST\n";
        return;
    }
    string condition("");
    if(flag)
        for(int i = indexOfWhere + 1; i < sql.size(); i++)
            condition += sql[i] + " ";
    if(upper(sql[1]) == "DISTINCT") {
        if(!flag)
            table->showTable_Distinct(sql[index - 1]);
        else
            table->showTable_Conditioned_Distinct(sql[index - 1], condition);
    }
    else {
        if(!flag)
            table->showTable(sql[index - 1]);
        else
            table->showTable_Conditioned(sql[index - 1], condition);
    }

}

void Controller::update(vector<string> sql) {
    CTable* table = nullptr;
    for(int i = 0; i < tables.size(); i++)
        if(tables[i]->TName == sql[1]) {
            table = tables[i];
            break;
        }
    if(!table) {
        cout<<"UPDATE ERROR: TABLE " + sql[1] + " DOESN'T EXIST\n";
        return;
    }
    if(sql.size() < 4 || sql.size() == 5 || upper(sql[2]) != "SET") {
        cout<<"UPDATE ERROR: WRONG FORMAT SQL\n";
        return;
    }
    else if(sql.size() == 4)
        table->updateAttribute(sql[3]);
    else {
        string str("");
        for(int i = 5; i < sql.size(); i++)
            str += sql[i] + " ";
        table->updateAttribute(sql[3], str);
    }
}

void Controller::deleteRows(vector<string> sql) {
    if(sql.size() < 3 || upper(sql[1]) != "FROM" || upper(sql[3]) != "WHERE")  {
        cout<<"DELETE ERROR: WRONG FORMAT SQL\n";
        return;
    }
    CTable *table = nullptr;
    for(int i = 0; i < tables.size(); i++)
        if(tables[i]->TName == sql[2]) {
            table =  tables[i];
            break;
        }
    if(!table) {
        cout<<"DELETE ERROR: TABLE " + sql[2] + " DOESN'T EXIST\n";
        return;
    }
    if(sql.size() == 3)
        table->deleteRows();
    else {
        string str("");
        for(int i = 4; i < sql.size(); i++)
            str += sql[i] + " ";
        table->deleteRows(str);
    }
}

void Controller::alter(vector<string> sql) {
    CTable *table = nullptr;
    for(int i = 0; i < tables.size(); i++)
        if(tables[i]->TName == sql[2]) {
            table = tables[i];
            break;
        }
    if(!table) {
        cout<<"ALTER ERROR: TABLE " + table->TName + " DOESN'T EXIST\n";
        return;
    }
    if(upper(sql[3]) == "ADD")
        table->alterAdd(sql[4]);
    else if(upper(sql[3]) == "DROP")
        table->alterDrop(sql[4]);
}
