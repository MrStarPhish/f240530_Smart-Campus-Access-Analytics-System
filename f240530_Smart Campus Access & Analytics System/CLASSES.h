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
	bool loginStatus = 0;
    int countOfCommandLines = 0;
    int countOfSystemMsg = 0;
	int x, y; // cursor position storage

    System() {}

    bool initialize() 
    {
		generateSystemResponse("Initializing system...");
        if (!loadUsers())
            return 0;
        if (!loadDoors())
			return 0;
		generateSystemResponse("System initialized successfully.");
        return 1;

	}

    bool loadUsers() 
    {
		generateSystemResponse("loading users...");
        if (!loadUsersFromFile())
        {
			generateSystemResponse("Failed to load users from file.");
            return 0;
        }
        return 1;
    }

    bool loadDoors() 
    {
		generateSystemResponse("loading doors...");
        if(!loadDoorsFromFile()) 
        {
			generateSystemResponse("Failed to load doors from file.");
            return 0;
		}
        return 1;
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
		int x, y;
		getCursorPosition(x, y);
		gotoxy(0, 22 + countOfSystemMsg%10);

        cout << countOfSystemMsg << ". ";
        setColor(L_BLUE);
        cout << "[System] ";
		setColor(DEFAULT);
        cout << resp << endl;
		countOfSystemMsg++;
        
		gotoxy(x, y); // return to old cursor position
    }

    void generateErrorResponse(string resp)
    {
        saveCoords();
        gotoxy(0, 22 + countOfSystemMsg % 10);

		cout << countOfSystemMsg << ". ";
        setColor(L_RED);
        cout << "[Error] ";
        setColor(DEFAULT);
        cout << resp << endl;

		countOfSystemMsg++;
        loadCoords();
    }

    void generateWarningResponse(string resp)
    {
        saveCoords();

        gotoxy(0, 22 + countOfSystemMsg % 10);

		cout << countOfSystemMsg << ". ";
        setColor(L_YELLOW);
        cout << "[Warning] ";
        setColor(DEFAULT);
		cout << resp << endl;

		countOfSystemMsg++;
        loadCoords(); // return to old cursor position

    }

    void inputCommand()
    {
        if (countOfCommandLines >= 16)
        {
			generateWarningResponse("Command section crowded. Consider using '/cls' to clear."); // warning message
        }

        gotoxy(0, 1 + countOfCommandLines%18);
        
        cout<<"> ";
        string command;
        getline(cin, command);

        countOfCommandLines++;

		// Command processing will be implemented later
    }

    void processCommand(string command)
    {
        if (command[0]!='/') 
        {
            generateWarningResponse("Commands must start with '/'. Type '/help' for a list of commands.");
            return;
		}
        if (command == "/exit") 
        {
            generateSystemResponse("Exiting the system...");
            // exit code
		}
        if (command == "/help") 
        {
            generateSystemResponse("Available commands: help, exit, list_users, list_doors");
        }
        else if (command == "/list_users") 
        {
           // printUsers();
        }
        else if (command == "/list_doors") 
        {
            // printDoors(); // to be implemented
        }
        else if (command == "/cls") 
        {
            clearCommandSection();
			generateSystemResponse("Command section cleared.");
            gotoxy(0, 0);
        }
        else 
        {
            generateErrorResponse("Unknown command. Type 'help' for a list of commands.");
		}
        // Command processing logic will be implemented later
	}

private:

    void clearCommandSection()
    {           
        saveCoords();
        for (int i = 0; i < 21; i++) 
        {
            gotoxy(0, i);
            clearLine(0, i, 50);
        }
        loadCoords();
	}

    void saveCoords()
    {
        getCursorPosition(x, y);
	}
    void loadCoords()
    {
        gotoxy(x, y);
    }

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

        string temp = "Loaded ";
        temp += to_string(users.size());
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

        string temp = "Loaded ";
        temp += to_string(doors.size());
        temp += " doors.";
		generateSystemResponse(temp);

        return 1;
    }


};

#endif




