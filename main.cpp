#include<iostream>
#include "Controller.h"
using namespace std;
int main() {
    Controller controller;
    while(true) {
        cout<<"sql> ";
        controller.doSql();
    }

    return 0;
}
/*
create TaBLe students (id PrIMaRy_kEY,name,age);
create table department (name);
create table test (col_1,col_2 primary_key,col_3);

INSERT INTO students VALUES (1,test1,18);
INSERT INTO students VALUES (1,test2,18);
 */