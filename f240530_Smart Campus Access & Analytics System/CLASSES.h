#pragma once

// All the classes and their function are declared here

#ifndef CLASSES_H
#define CLASSES_H

#include<iostream>
#include<string>
#include<vector>
#include<fstream>
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
        cout << "[System] loading users..." << endl;
        if (!loadUsersFromFile())
        {
			cout << "[System] Failed to load users from file." << endl;
        }
    }

    void loadDoors() 
    {
        cout << "[System] loading doors..." << endl;
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

    bool loadUsersFromFile() 
    {
		users.clear(); // clearing the existing users

        ifstream fin("users.txt");

        if (!fin) 
        {
            cout << "Error: users.txt not found.\n";
            return 0;
        }

        int id;
        string name, role;

        while (fin >> id >> name >> role) {
            users.push_back(User(id, name, role));
        }

        fin.close();
        cout << "[System] Loaded " << users.size() << " users.\n";
    }

    void loadDoors() {
        doors.clear();
        ifstream fin("doors.txt");

        if (!fin) {
            cout << "Error: doors.txt not found.\n";
            return;
        }

        int id;
        string loc, type;

        while (fin >> id >> loc >> type) {
            doors.push_back(Door(id, loc, type));
        }

        fin.close();
        cout << "[System] Loaded " << doors.size() << " doors.\n";
    }


};

#endif




