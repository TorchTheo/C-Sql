//
// Created by torchtheo on 2021/4/16.
//

#include "CTable.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <set>
#include "Controller.h"

using namespace std;


CTable::CTable(string name, string attributes) {
    ifstream ifs("db/" + name + ".txt", ios::in);
    if(ifs.is_open()) {
        this->TName = name;
        string str;
        getline(ifs, str);
        vector<string> t;
        istringstream t1(str);
        string t2;
        while(t1 >> t2)
            t.push_back(t2);
        for(int i = 0; i < t.size(); i++) {
            if(i && Controller::upper(t[i]) == "PRIMARY_KEY"){
                this->attributes[this->attributes.size() - 1]->isPrimaryKey = true;
                continue;
            }
            Attribute *attribute = new Attribute(t[i]);
            this->attributes.push_back(attribute);
        }
        while(getline(ifs, str)) {
            vector<string> _t;
            Object *obj = new Object();
            istringstream _t1(str);
            string _t2;
            while(_t1 >> t2)
                _t.push_back(t2);
            for(int i = 0; i < _t.size();  i++)
                obj->attributes_val.insert(pair<Attribute*, string>(this->attributes[i],  _t[i]));
            this->objects.push_back(obj);
        }
    }
    else {
        this->TName = name;
        attributes.erase(0, 1);
        attributes.pop_back();
        for(int i = 0; i < attributes.length(); i++)
            if(attributes[i] == ',')
                attributes[i] = ' ';
        vector<string> temp;
        istringstream s(attributes);
        string out;
        while(s >> out)
            temp.push_back(out);
        bool flag = true;
        for(int i = 0; i < temp.size(); i++) {
            if(i && Controller::upper(temp[i]) == "PRIMARY_KEY") {
                if(!flag)
                    return;
                else  {
                    this->attributes[this->attributes.size() - 1]->isPrimaryKey = true;
                    flag = false;
                    continue;
                }
            }
            Attribute* attribute = new Attribute(temp[i]);
            this->attributes.push_back(attribute);
        }
        this->writeData();
    }
    ifs.close();
}

void CTable::showAttributes() {
    for(int i = 0; i < this->attributes.size(); i++)
        cout<<attributes[i]->name<<attributes[i]->isPrimaryKey<<endl;
}

void CTable::writeData() {
    ofstream ofs("db/" + this->TName + ".txt", ios::out);
    for(int i = 0; i < this->attributes.size(); i++) {
        ofs<<this->attributes[i]->name<<" ";
        if(this->attributes[i]->isPrimaryKey)
            ofs<<"PRIMARY_KEY ";
    }
    ofs<<endl;
    for(int i = 0; i < this->objects.size(); i++) {
        for(int j = 0; j < this->objects[i]->attributes_val.size(); j++)
            ofs<<this->objects[i]->attributes_val[this->attributes[j]]<<" ";
        ofs<<endl;
    }
    ofs.close();
}

void CTable::insertObject(string attribute) {
    attribute.erase(0, 1);
    attribute.pop_back();
    for(int i = 0; i < attribute.length(); i++)
        if(attribute[i] == ',')
            attribute[i] = ' ';
    vector<string> temp;
    istringstream s(attribute);
    string out;
    while(s >> out)
        temp.push_back(out);
    if(temp.size() != this->attributes.size()) {
        cout<<"INSERT ERROR: WRONG NUMBERS OF ATTRIBUTE\n";
        return;
    }
    Object* obj = new Object();
    for(int i = 0; i < temp.size(); i++) {
        if(this->attributes[i]->isPrimaryKey) {
            for(int j = 0; j < this->objects.size(); j++) {
                if(this->objects[j]->attributes_val[this->attributes[i]] == temp[i]) {
                    cout<<"INSERT ERROR: ATTRIBUTE " + this->attributes[i]->name + " OF TABLE " + this->TName + " IS PRIMARY_KEY, VALUE " + temp[i] + " EXIST\n";
                    return;
                }
            }
        }
        obj->attributes_val.insert(pair<Attribute*, string>(this->attributes[i], temp[i]));
    }
    cout<<"TABLE " + this->TName  + " HAVE INSERTED A NEW RECORD SUCCESSFULLY\n";
    this->objects.push_back(obj);
    this->writeData();
}

void CTable::showTable(string column) {
    if(column == "*") {
        for(int i = 0; i < this->attributes.size(); i++)
            cout<<this->attributes[i]->name + "\t";
        cout<<endl;
        for(int i = 0; i < this->objects.size(); i++) {
            for(int j = 0; j < this->attributes.size(); j++) {
                cout<<this->objects[i]->attributes_val[this->attributes[j]] + "\t";
            }
            cout<<endl;
        }
        return;
    }
    for(int i = 0; i < column.length(); i++)
        if(column[i] == ',')
            column[i] = ' ';
    vector<string> temp;
    istringstream s(column);
    string out;
    while(s >> out)
        temp.push_back(out);
    for(int j = 0; j < temp.size(); j++)
        for(int i = 0; i < this->attributes.size();  i++)
            if(this->attributes[i]->name == temp[j]) {
                cout<<this->attributes[i]->name + "\t";
                break;
            }
    cout<<endl;
    for(int i = 0; i < this->objects.size(); i++) {
        for(int k = 0; k < temp.size(); k++)
            for(int j = 0; j < this->attributes.size(); j++) {
                if(this->attributes[j]->name == temp[k]) {
                    cout << this->objects[i]->attributes_val[this->attributes[j]] + "\t";
                    break;
                }
        }
        cout<<endl;
    }
}

void CTable::showTable_Distinct(string column) {
    if(column == "*") {
        set<string>rows;
        for(int i = 0; i < this->attributes.size(); i++)
            cout<<this->attributes[i]->name + "\t";
        cout<<endl;
        for(int i = 0; i < this->objects.size(); i++) {
            string str("");
            for(int j = 0; j < this->attributes.size(); j++)
                str += this->objects[i]->attributes_val[this->attributes[j]] + "\t";
            rows.insert(str);
        }
        for(set<string>::iterator it = rows.begin(); it != rows.end(); it++)
            cout<<*it<<endl;
        return;
    }
    for(int i = 0; i < column.length(); i++)
        if(column[i] == ',')
            column[i] = ' ';
    vector<string> temp;
    istringstream s(column);
    string out;
    while(s >> out)
        temp.push_back(out);

    for(int j = 0; j < temp.size(); j++)
        for(int i = 0; i < this->attributes.size();  i++)
            if(this->attributes[i]->name == temp[j]) {
                cout<<this->attributes[i]->name + "\t";
                break;
            }
    cout<<endl;
    set<string>rows;
    for(int i = 0; i < this->objects.size(); i++) {
        string str;
        for(int k = 0; k < temp.size(); k++)
            for (int j = 0; j < this->attributes.size(); j++)
                if (this->attributes[j]->name == temp[k]) {
                    str += this->objects[i]->attributes_val[this->attributes[j]] + "\t";
                    break;
                }
        rows.insert(str);
    }
    for(set<string>::iterator it = rows.begin(); it != rows.end(); it++)
        cout<<*it<<endl;
}

void CTable::showTable_Conditioned(string column, string condition) {
    int flag = 0;//0 denotes only one condition, 1 denotes AND, 2 denotes OR
    vector<string> temp;
    istringstream s(condition);
    string out;
    while(s >> out)
        temp.push_back(out);
    vector<string >::iterator it_And;
    vector<string >::iterator it_Or;
    map<Attribute*, string>_m;
    if(temp.size() != 1) {
        for(vector<string >::iterator it = temp.begin(); it != temp.end(); it++) {
            if (Controller::upper(*it) == "AND") {
                it_And = it;
                flag =  1;
                temp.erase(it_And);
                break;
            }
            if(Controller::upper(*it) == "OR") {
                it_Or = it;
                flag =  2;
                temp.erase(it_Or);
                break;
            }
        }
    }
    for(int i = 0; i < temp.size(); i++) {
        bool f = false;//Is attribute exist?
        for(int j = 0; j < temp[i].length(); j++)
            if(temp[i][j] == '=')
                temp[i][j] = ' ';
        s = istringstream(temp[i]);
        string o, _o;
        s>>o>>_o;
        for(int j = 0; j < this->attributes.size(); j++)
            if(this->attributes[j]->name == o) {
                _m.insert(make_pair(this->attributes[j], _o));
                f = true;
                break;
            }
        if(!f) {
            cout<<"SELECT ERROR: TABLE " + this->TName + " DOESN'T HAVE ATTRIBUTE " + o + "\n";
            return;
        }

    }
    if(column == "*") {
        for(int i = 0; i < this->attributes.size(); i++)
            cout<<this->attributes[i]->name + "\t";
        cout<<endl;
        for(int i = 0; i < this->objects.size(); i++) {
            bool f = false;
            if(flag == 0 || flag == 2) {
                for(map<Attribute*, string>::iterator it = _m.begin(); it != _m.end(); it++) {
                    for(int j = 0; j < this->attributes.size(); j++) {
                        if(this->attributes[j] == it->first && this->objects[i]->attributes_val[it->first] == it->second) {
                            f = true;
                            break;
                        }
                    }
                    if(f)
                        break;
                }
            }
            if(flag == 1) {
                f = true;
                for(map<Attribute*, string>::iterator it = _m.begin(); it != _m.end(); it++) {
                    for(int j = 0; j < this->attributes.size(); j++) {
                        if(this->attributes[j] == it->first && this->objects[i]->attributes_val[it->first] != it->second) {
                            f = false;
                            break;
                        }
                    }
                    if(!f)
                        break;
                }
            }
            if(f) {
                for(int j = 0; j < this->attributes.size(); j++)
                    cout<<this->objects[i]->attributes_val[this->attributes[j]] + "\t";
                cout<<endl;
            }

        }
        return;
    }

    for(int i = 0; i < column.length(); i++)
        if(column[i] == ',')
            column[i] = ' ';
    temp.clear();
    s = istringstream(column);
    while(s >> out)
        temp.push_back(out);
    for(int j = 0; j < temp.size(); j++)
        for(int i = 0; i < this->attributes.size();  i++)
            if(this->attributes[i]->name == temp[j]) {
                cout<<this->attributes[i]->name + "\t";
                break;
            }
    cout<<endl;
    for(int i = 0; i < this->objects.size(); i++) {
        bool f = false;
        if(flag == 0 || flag == 2) {
            for(map<Attribute*, string>::iterator it = _m.begin(); it != _m.end(); it++) {
                for(int j = 0; j < this->attributes.size(); j++) {
                    if(this->attributes[j] == it->first && this->objects[i]->attributes_val[it->first] == it->second) {
                        f = true;
                        break;
                    }
                }
                if(f)
                    break;
            }
        }
        if(flag == 1) {
            f = true;
            for(map<Attribute*, string>::iterator it = _m.begin(); it != _m.end(); it++) {
                for(int j = 0; j < this->attributes.size(); j++) {
                    if(this->attributes[j] == it->first && this->objects[i]->attributes_val[it->first] != it->second) {
                        f = false;
                        break;
                    }
                }
                if(!f)
                    break;
            }
        }
        if(f) {
            for(int k = 0; k < temp.size(); k++)
                for (int j = 0; j < this->attributes.size(); j++)
                    if (this->attributes[j]->name == temp[k]){
                        cout << this->objects[i]->attributes_val[this->attributes[j]] + "\t";
                        break;
                    }
            cout << endl;
        }
    }
}

void CTable::showTable_Conditioned_Distinct(string column, string condition) {
    int flag = 0;//0 denotes only one condition, 1 denotes AND, 2 denotes OR
    vector<string> temp;
    istringstream s(condition);
    string out;
    while(s >> out)
        temp.push_back(out);
    vector<string >::iterator it_And;
    vector<string >::iterator it_Or;
    map<Attribute*, string>_m;
    if(temp.size() != 1) {
        for(vector<string >::iterator it = temp.begin(); it != temp.end(); it++) {
            if (Controller::upper(*it) == "AND") {
                it_And = it;
                flag =  1;
                temp.erase(it_And);
                break;
            }
            if(Controller::upper(*it) == "OR") {
                it_Or = it;
                flag =  2;
                temp.erase(it_Or);
                break;
            }
        }
    }
    for(int i = 0; i < temp.size(); i++) {
        bool f = false;//Is attribute exist?
        for(int j = 0; j < temp[i].length(); j++)
            if(temp[i][j] == '=')
                temp[i][j] = ' ';
        s = istringstream(temp[i]);
        string o, _o;
        s>>o>>_o;
        for(int j = 0; j < this->attributes.size(); j++)
            if(this->attributes[j]->name == o) {
                _m.insert(make_pair(this->attributes[j], _o));
                f = true;
                break;
            }
        if(!f) {
            cout<<"SELECT ERROR: TABLE " + this->TName + " DOESN'T HAVE ATTRIBUTE " + o + "\n";
            return;
        }

    }
    if(column == "*") {
        for(int i = 0; i < this->attributes.size(); i++)
            cout<<this->attributes[i]->name + "\t";
        cout<<endl;
        set<string>rows;
        for(int i = 0; i < this->objects.size(); i++) {
            bool f = false;
            if(flag == 0 || flag == 2) {
                for(map<Attribute*, string>::iterator it = _m.begin(); it != _m.end(); it++) {
                    for(int j = 0; j < this->attributes.size(); j++) {
                        if(this->attributes[j] == it->first && this->objects[i]->attributes_val[it->first] == it->second) {
                            f = true;
                            break;
                        }
                    }
                    if(f)
                        break;
                }
            }
            if(flag == 1) {
                f = true;
                for(map<Attribute*, string>::iterator it = _m.begin(); it != _m.end(); it++) {
                    for(int j = 0; j < this->attributes.size(); j++) {
                        if(this->attributes[j] == it->first && this->objects[i]->attributes_val[it->first] != it->second) {
                            f = false;
                            break;
                        }
                    }
                    if(!f)
                        break;
                }
            }
            if(f) {
                string str("");
                for(int j = 0; j < this->attributes.size(); j++)
                    str += this->objects[i]->attributes_val[this->attributes[j]] + "\t";
                rows.insert(str);
            }
        }
        for(set<string>::iterator it = rows.begin(); it != rows.end(); it++)
            cout<<*it<<endl;
        return;
    }

    for(int i = 0; i < column.length(); i++)
        if(column[i] == ',')
            column[i] = ' ';
    temp.clear();
    s = istringstream(column);
    set<string> rows;
    while(s >> out)
        temp.push_back(out);
    for(int j = 0; j < temp.size(); j++)
        for(int i = 0; i < this->attributes.size();  i++)
            if(this->attributes[i]->name == temp[j]) {
                cout<<this->attributes[i]->name + "\t";
                break;
            }
    cout<<endl;
    for(int i = 0; i < this->objects.size(); i++) {
        bool f = false;
        if(flag == 0 || flag == 2) {
            for(map<Attribute*, string>::iterator it = _m.begin(); it != _m.end(); it++) {
                for(int j = 0; j < this->attributes.size(); j++) {
                    if(this->attributes[j] == it->first && this->objects[i]->attributes_val[it->first] == it->second) {
                        f = true;
                        break;
                    }
                }
                if(f)
                    break;
            }
        }
        if(flag == 1) {
            f = true;
            for(map<Attribute*, string>::iterator it = _m.begin(); it != _m.end(); it++) {
                for(int j = 0; j < this->attributes.size(); j++) {
                    if(this->attributes[j] == it->first && this->objects[i]->attributes_val[it->first] != it->second) {
                        f = false;
                        break;
                    }
                }
                if(!f)
                    break;
            }
        }
        if(f) {
            string str("");
            for(int k = 0; k < temp.size(); k++)
                for (int j = 0; j < this->attributes.size(); j++)
                    if (this->attributes[j]->name == temp[k]) {
                        str += this->objects[i]->attributes_val[this->attributes[j]] + "\t";
                        break;
                    }
            rows.insert(str);
        }
    }
    for(set<string>::iterator it = rows.begin(); it != rows.end(); it++)
        cout<<*it<<endl;
}

void CTable::updateAttribute(string column, string condition) {
    int flag = 0;//0 denotes only one condition, 1 denotes AND, 2 denotes OR, -1 denotes non-condition
    map<Attribute*, string>_m;
    vector<string> temp;
    istringstream s(condition);
    string out;
    vector<string>::iterator it_And;
    vector<string>::iterator it_Or;
    if(condition == "")
        flag = -1;
    else {
        while (s >> out)
            temp.push_back(out);
        if (temp.size() != 1) {
            for (vector<string>::iterator it = temp.begin(); it != temp.end(); it++) {
                if (Controller::upper(*it) == "AND") {
                    it_And = it;
                    flag = 1;
                    temp.erase(it_And);
                    break;
                }
                if (Controller::upper(*it) == "OR") {
                    it_Or = it;
                    flag = 2;
                    temp.erase(it_Or);
                    break;
                }
            }
        }
    }
    for(int i = 0; i < temp.size(); i++) {
        bool f = false;//Is attribute exist?
        for(int j = 0; j < temp[i].length(); j++)
            if(temp[i][j] == '=')
                temp[i][j] = ' ';
        s = istringstream(temp[i]);
        string o, _o;
        s>>o>>_o;
        for(int j = 0; j < this->attributes.size(); j++)
            if(this->attributes[j]->name == o) {
                _m.insert(make_pair(this->attributes[j], _o));
                f = true;
                break;
            }
        if(!f) {
            cout<<"SELECT ERROR: TABLE " + this->TName + " DOESN'T HAVE ATTRIBUTE " + o + "\n";
            return;
        }
    }
    for(int i = 0; i < column.length(); i++)
        if(column[i] == '=')
            column[i] = ' ';
    s = istringstream(column);
    bool f = false;
    Attribute* attribute = nullptr;
    string col, newVal;
    s >> col >> newVal;
    for(int i = 0; i < this->attributes.size(); i++) {
        if(this->attributes[i]->name  == col) {
            attribute = this->attributes[i];
            f = true;
            break;
        }
    }
    if(!f) {
        cout<<"UPDATE ERROR: TABLE " + this->TName + " DOESN'T HAVE ATTRIBUTE" + col + "\n";
        return;
    }
    int numOfUpdatedRecords = 0;
    for(int i = 0; i < this->objects.size(); i++) {
        bool f = false;
        if(flag == -1)
            f = true;
        if(flag == 0 || flag == 2) {
            for(map<Attribute*, string>::iterator it = _m.begin(); it != _m.end(); it++) {
                for(int j = 0; j < this->attributes.size(); j++) {
                    if(this->attributes[j] == it->first && this->objects[i]->attributes_val[it->first] == it->second) {
                        f = true;
                        break;
                    }
                }
                if(f)
                    break;
            }
        }
        if(flag == 1) {
            f = true;
            for(map<Attribute*, string>::iterator it = _m.begin(); it != _m.end(); it++) {
                for(int j = 0; j < this->attributes.size(); j++) {
                    if(this->attributes[j] == it->first && this->objects[i]->attributes_val[it->first] != it->second) {
                        f = false;
                        break;
                    }
                }
                if(!f)
                    break;
            }
        }
        if(f) {
            if(attribute->isPrimaryKey) {
                for(int m = 0; m < this->objects.size(); m++) {
                    if(this->objects[m]->attributes_val[attribute] == newVal) {
                        cout<<"UPDATE ERROR: ATTRIBUTE " + col + " OF TABLE " + this->TName + " IS PRIMARY_KEY, YOU HAVE SET A REPEATED VALUE\n";
                        break;
                    }
                }
            }
            else {
                this->objects[i]->attributes_val[attribute] = newVal;
                numOfUpdatedRecords++;
            }
        }
    }
    this->writeData();
    cout<<numOfUpdatedRecords<<" RECORD(S) OF TABLE " + this->TName + " HAVE BEEN UPDATED\n";
}

void CTable::deleteRows(string condition) {
    int flag = 0;//0 denotes only one condition, 1 denotes AND, 2 denotes OR, -1 denotes non-condition
    map<Attribute*, string>_m;
    vector<string> temp;
    istringstream s(condition);
    string out;
    vector<string>::iterator it_And;
    vector<string>::iterator it_Or;
    if(condition == "")
        flag = -1;
    else {
        while (s >> out)
            temp.push_back(out);
        if (temp.size() != 1) {
            for (vector<string>::iterator it = temp.begin(); it != temp.end(); it++) {
                if (Controller::upper(*it) == "AND") {
                    it_And = it;
                    flag = 1;
                    temp.erase(it_And);
                    break;
                }
                if (Controller::upper(*it) == "OR") {
                    it_Or = it;
                    flag = 2;
                    temp.erase(it_Or);
                    break;
                }
            }
        }
    }
    for(int i = 0; i < temp.size(); i++) {
        bool f = false;//Is attribute exist?
        for(int j = 0; j < temp[i].length(); j++)
            if(temp[i][j] == '=')
                temp[i][j] = ' ';
        s = istringstream(temp[i]);
        string o, _o;
        s>>o>>_o;
        for(int j = 0; j < this->attributes.size(); j++)
            if(this->attributes[j]->name == o) {
                _m.insert(make_pair(this->attributes[j], _o));
                f = true;
                break;
            }
        if(!f) {
            cout<<"SELECT ERROR: TABLE " + this->TName + " DOESN'T HAVE ATTRIBUTE " + o + "\n";
            return;
        }
    }
    int numOfDeletedRows = 0;
    vector<Object*> t;
    for(vector<Object*>::iterator i = this->objects.begin(); i != this->objects.end(); i++) {
        bool f = false;
        if(flag == -1)
            f = true;
        if(flag == 0 || flag == 2) {
            for(map<Attribute*, string>::iterator it = _m.begin(); it != _m.end(); it++) {
                for(int j = 0; j < this->attributes.size(); j++) {
                    if(this->attributes[j] == it->first && (*i)->attributes_val[it->first] == it->second) {
                        f = true;
                        break;
                    }
                }
                if(f)
                    break;
            }
        }
        if(flag == 1) {
            f = true;
            for(map<Attribute*, string>::iterator it = _m.begin(); it != _m.end(); it++) {
                for(int j = 0; j < this->attributes.size(); j++) {
                    if(this->attributes[j] == it->first && (*i)->attributes_val[it->first] != it->second) {
                        f = false;
                        break;
                    }
                }
                if(!f)
                    break;
            }
        }
        if(f) {
            t.push_back(*i);
        }
    }
    for(int i = 0; i < t.size(); i++) {
        for(vector<Object*>::iterator it = this->objects.begin(); it != this->objects.end(); it++)  {
            if((*it) == t[i]) {
                this->objects.erase(it);
                numOfDeletedRows++;
                break;
            }
        }
    }
    this->writeData();
    cout<<numOfDeletedRows<<" ROW(S) OF TABLE " + this->TName + " HAVE BEEN DELETED\n";
}

void CTable::alterDrop(string attribute) {
    vector<Attribute*>::iterator it;
    bool flag = false;
    for(vector<Attribute*>::iterator i = this->attributes.begin(); i != this->attributes.end(); i++) {
        if((*i)->name == attribute) {
            it = i;
            flag = true;
            break;
        }
    }
    if(!flag) {
        cout<<"ALTER ERROR: TABLE " + this->TName + " DOESN'T HAVE ATTRIBUTE " + attribute;
        return;
    }
    for(int i = 0; i < this->objects.size(); i++) {
        map<Attribute*, string>::iterator it_map;
        for(map<Attribute*, string>::iterator j = this->objects[i]->attributes_val.begin(); j != this->objects[i]->attributes_val.end(); j++)
            if(j->first->name == attribute) {
                it_map = j;
            }
        this->objects[i]->attributes_val.erase(it_map);
    }

    this->attributes.erase(it);
    this->writeData();
}

void CTable::alterAdd(string attribute) {
    Attribute* attribute1 = nullptr;
    bool flag = true;
    for(int i = 0; i < this->attributes.size(); i++)
        if(this->attributes[i]->name == attribute) {
            flag = false;
            break;
        }
    if(!flag) {
        cout<<"ALTER ERROR: TABLE " + this->TName + " HAD ALREADY HAVE ATTRIBUTE " + attribute + "\n";
        return;
    }
    attribute1 = new Attribute(attribute);
    this->attributes.push_back(attribute1);
    for(int i = 0; i < this->objects.size(); i++)
        this->objects[i]->attributes_val.insert(make_pair(attribute1, "NULL"));
    this->writeData();
}

Attribute::Attribute(string name) {
    this->name = name;
}
