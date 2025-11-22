#pragma once

// All the classes and their function are declared here

#ifndef CLASSES_H
#define CLASSES_H

#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include"ConsoleAndBufferEssentials.h"
using namespace std;

// =====================
// User
// =====================
class User {
public:
    int id;
    string name;
    string role;

    User(int id = 0, string name = "", string role = "")
    {
        this->id = id;
		this->name = name;
        this->role = role;
    }
};

// =====================
// Door
// =====================
class Door {
public:
    int id;
    string location;
    string type;

    Door(int id = 0, string location = "", string type = "")
    {
        this->id = id;
        this->location = location;
        this->type = type;
    }
};

// =====================
// AccessRule
// =====================
class AccessRule {
public:
    string ruleString;

    AccessRule(string r = "") { this->ruleString = r; }
};

// =====================
// AccessLog Entry
// =====================
class AccessLogEntry {
public:
    string timestamp;
    int userID;
    int doorID;
    bool granted;

    AccessLogEntry(string t = "", int u = 0, int d = 0, bool g = false)
    {
        timestamp = t;
		userID = u;
        doorID = d;
		granted = g;
    }
};

// =====================
// Campus Graph (simple for now)
// =====================
class CampusGraph {
public:
    // adjacency list will be implemented later
    CampusGraph() {}
};

// =====================
// MAIN SYSTEM
// =====================
class System {
public:
    vector<User> users;
    vector<Door> doors;
    vector<AccessLogEntry> logs;

    System() {}

    void loadUsers() 
    {
		generateSystemResponse("loading users...");
        if (!loadUsersFromFile())
        {
			generateSystemResponse("Failed to load users from file.");
        }
    }

    void loadDoors() 
    {
		generateSystemResponse("loading doors...");
        if(!loadDoorsFromFile()) 
        {
			generateSystemResponse("Failed to load doors from file.");
		}
    }

    void printUsers() 
    {
        cout << endl;
        cout << "--- Users ---";
        cout << endl;
        for (auto& u : users) 
        {
            cout << u.id << " " << u.name << " " << u.role << endl;
        }
    }


    void generateSystemResponse(string resp)
    {
        setColor(L_BLUE);
        cout << "[System] ";
		setColor(DEFAULT);
        cout << resp << endl;
    }

    void generateErrorResponse(string resp)
    {
        setColor(L_RED);
        cout << "[Error] ";
        setColor(DEFAULT);
        cout << resp << endl;
    }

private:

    bool loadUsersFromFile() 
    {
		users.clear(); // clearing the existing users

        ifstream fin("users.txt");

        if (!fin) 
        {
			generateErrorResponse("users.txt file is missing.");
            return 0;
        }

        int id;
        string name, role;

        while (fin >> id >> name >> role)
        {
			User temp(id, name, role);
            users.push_back(temp);
        }

        fin.close();

        string temp = "[System] Loaded " + users.size();
        temp += " users.";
		generateSystemResponse(temp);

        return 1;
    }

    bool loadDoorsFromFile() {
        doors.clear();
        ifstream fin("doors.txt");

        if (!fin) 
        {
			generateErrorResponse("doors.txt file is missing.");
            return 0;
        }

        int id;
        string loc, type;

        while (fin >> id >> loc >> type)
        {
			Door temp(id, loc, type);  
            doors.push_back(temp);
        }

        fin.close();

        string temp = "[System] Loaded " + doors.size();
        temp += " doors.";
		generateSystemResponse(temp);

        return 1;
    }


};

#endif




