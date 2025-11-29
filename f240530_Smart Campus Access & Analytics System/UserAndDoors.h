#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<fstream>
#include"ConsoleAndBufferEssentials.h"
#include"AccessRules.h"

using namespace std;

// ========================================== USER =======================
class User
{
public:
    int id;
    string name;
    string role; // student, teacher, staff, guard, labat (lab attendant)

    User(int id = 0, string name = "", string role = "")
    {
        this->id = id;
        this->name = name;
        this->role = role;
    }
};

// ========================================== DOOR =======================

class Door
{
public:
    int id;
    string location;
    string type; // classroom, examhall, datacenter, lab

    Door(int id = 0, string location = "", string type = "")
    {
        this->id = id;
        this->location = location;
        this->type = type;
    }
};



