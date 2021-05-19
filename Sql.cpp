//
// Created by torchtheo on 2021/4/16.
//

#include "Sql.h"
#include "Controller.h"


Sql::Sql(string str) {
    this->originSql = str;
    this->targetSql = this->readSql();
}

vector<string> Sql::readSql() {
    vector<string> sql;
    istringstream s(this->originSql);
    string out;
    while(s >> out)
        sql.push_back(out);
    return sql;
}

int Sql::anaSql() {
    if(Controller::upper(this->targetSql[0]) == "CREATE") {
        if(this->targetSql.size() <= 3)
            return SQLACTION::CREATE_TABLE_ERROR;
        else if (Controller::upper(this->targetSql[1]) == "TABLE") {
            string str("");
            int len = this->targetSql.size();
            for(int i = 3; i < len; i++) {
                str += this->targetSql[i];
                if(i == 3 && len > 4)
                    str += " ";
            }
            for(int i = 0; i < len - 3; i++)
                this->targetSql.pop_back();
            this->targetSql.push_back(str);
            return SQLACTION::CREATE_TABLE;
        }
    }
    else if(this->targetSql.size() == 1 && (Controller::upper(this->targetSql[0]) == "EXIT" || Controller::upper(this->targetSql[0]) == "QUIT"))
        return SQLACTION::EXIT_SYSTEM;
    else if (this->targetSql.size() == 2 && Controller::upper(this->targetSql[0])  == "SHOW" && Controller::upper(this->targetSql[1])  == "TABLES")
        return SQLACTION::SHOW_TABLES;
    else if(Controller::upper(this->targetSql[0]) == "DROP" && Controller::upper(this->targetSql[1]) == "TABLE") {
        if(this->targetSql.size() != 3)
            return SQLACTION::DROP_TABLE_ERROR;
        return SQLACTION::DROP_TABLE;
    }
    else if(this->targetSql.size()  == 5 && Controller::upper(this->targetSql[0]) == "INSERT" && Controller::upper(this->targetSql[1]) == "INTO" && Controller::upper(this->targetSql[3]) == "VALUES")
        return SQLACTION::INSERT;
    else if(this->targetSql.size() >= 4 && Controller::upper(this->targetSql[0]) ==  "SELECT")
        return SQLACTION::SELECT;
    else if(Controller::upper(this->targetSql[0]) == "UPDATE")
        return SQLACTION::UPDATE;
    else if(Controller::upper(this->targetSql[0]) == "DELETE")
        return SQLACTION::DELETE;
    else if(Controller::upper(this->targetSql[0]) == "ALTER")
        return SQLACTION::ALTER;
    return SQLACTION::UNKNOWNSQL_ERROR;
}
