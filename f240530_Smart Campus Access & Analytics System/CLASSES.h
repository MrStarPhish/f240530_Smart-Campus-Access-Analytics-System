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

// ========================================== USER =======================
class User {
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

class Door {
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

// =====================
// AccessRule                                    AND EVALUATION OF RULE
// =====================
class AccessRule {
public:
    int id;
    string ruleString;

    AccessRule(int id = 0, string r = "") { this->id = id; this->ruleString = r; }
    bool evaluateRule(User u, Door d)
    {
        return evaluateRuleHelper(ruleString, u, d);
    }
    
private:
    bool evaluateRuleHelper(string rule, User u, Door d)
    {
        int hour = 12; // setting to 12 by default

        string replaced = replaceConditions(rule, u, d, hour); // generate the BOOLEAN EXPRESSION of the Rule String

        return evaluateBoolExpression(replaced); // final evaluation 
    }


    string replaceConditions(string expr, User u, Door d, int hour) // input: NOT (role=student)
    {
        string token; // "NOT" + "(" + "role=student" + ")"
        string result; // "false" + "(" "true" +")"

        for (int i = 0; i < expr.size(); i++)
        {
            char c = expr[i];

            if (c == ' ' || c == '(' || c == ')') 
            {
                if (!token.empty()) 
                {
                    int val = evaluateToken(token, u, d, hour);
                    if (val == 0)
                        result += "false";
                    else if (val == 1)
                        result += "true";
                    else if (token == "AND" || token == "OR" || token == "NOT")
                    {
                        result += token;
                    }
                    token.clear();
                }
                result += c;
            }
            else 
            {
                token += c;
            }
        }

        if (!token.empty()) 
        {
            int val = evaluateToken(token, u, d, hour);
            if (val == 0)
                result += "false";
            else if (val == 1)
                result += "true";
            else if (token == "AND" || token == "OR" || token == "NOT")
            {
                result += token;
            }
        }

        return result;
    }


    int evaluateToken(string tok, User u, Door d, int hour)  // inputs each token and translates it.
    {
        if (tok == "AND" || tok == "OR" || tok == "NOT")
            return -1; // not a condition

        if (tok == "true") return 1; // constant T/F
        if (tok == "false") return 0;

        return evaluateCondition(tok, u, d, hour); // a proper conditional token i.e role=student
    }


    bool evaluateCondition(string c, User u, Door d, int hour)  // to evaluate a chunk of a condition
    {
        // trim spaces from both sides
        while (c[0] == ' ') c = c.substr(1);
        while (c.back() == ' ') c.pop_back();

        if (c.rfind("role=", 0) == 0) // if input:  "role=student" , then it compares and returns the result whether it matches or not... whether given condition is true or not
            return u.role == c.substr(5);

        if (c.rfind("type=", 0) == 0)
            return d.type == c.substr(5);

        if (c.rfind("door=", 0) == 0)
            return d.location == c.substr(5);

        if (c.rfind("time>=", 0) == 0)
            return hour >= stoi(c.substr(6));

        if (c.rfind("time<=", 0) == 0)
            return hour <= stoi(c.substr(6));

        return 0;
    }

  

    bool evaluateBoolExpression(string expr) 
    {
        
        string s;
        for (int i = 0; i < expr.size(); i++) // Remove spaces for easier parsing
        {
            char c = expr[i];
            if (c != ' ')
                s += c;
        }


        return evaluateOR(s); // lowest precedence, calling OR
    }

    bool evaluateOR(string s) 
    {
        int pos = s.rfind("OR");
        if (pos != string::npos) 
        {
            return evaluateOR(s.substr(0, pos)) || evaluateAND(s.substr(pos + 2)); // performs OR... [ A || (next equation)]
        }
        return evaluateAND(s);
    }

    bool evaluateAND(string s) 
    {
        int pos = s.rfind("AND");
        if (pos != string::npos)
        {
            return evaluateAND(s.substr(0, pos)) && evaluateNOT(s.substr(pos + 3));
        }
        return evaluateNOT(s);
    }

    bool evaluateNOT(string s) 
    {
        if (s.rfind("NOT", 0) == 0) 
        {
            return !evaluateNOT(s.substr(3));
        }
        return evaluateTerm(s);
    }

    bool evaluateTerm(string s) 
    {
        if (s[0] == '(' && s.back() == ')')
            return evaluateBoolExpression(s.substr(1, s.size() - 2));
        
        if (s == "(true" || s == "true)") // had to hardcode this part due to a bug...
            return true;
        if (s == "(false" || s == "false)")
            return false;
        return (s == "true");
    }



};

// =====================
// AccessLog Entry
// =====================
class AccessLogEntry
{
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

    string logToString()
    {
        string temp = "[" + timestamp + "] UID=" + to_string(userID) + " DID=" + to_string(doorID) + " ACCESS=";
        temp += (granted ? "GRANTED" : "DENIED");
        return temp;
    }
    
    bool appendToFile()
    {
        ofstream fout("accesslog.txt", ios::app);

        if (!fout)
        {
            return 0;
        }

        fout << logToString() << "\n";
        fout.close();
        return 1;
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

class CommentEntry
{
public:
    string timestamp;
    string message;

    CommentEntry(string t = "", string msg = "")
    {
        timestamp = t; message = msg;
    }

    string commentToString()
    {
        return "[" + timestamp + "] " + message;
    }

    bool appendToFile()
    {
        ofstream fout("comments.txt", ios::app);

        if (!fout)
        {
            return 0;
        }

        fout << commentToString() << "\n";
        fout.close();
        return 1;
    }
};

// =====================
// MAIN SYSTEM
// =====================
class System {
public:
    vector<User> users;
    vector<Door> doors;
    vector<AccessLogEntry> accessLogs;
    vector<CommentEntry> comments;
    vector<AccessRule> rules;
	bool loginStatus = 0;
    int offsetOfCommandLine = 0;
    int countOfSystemMsg = 0;
	int x, y; // cursor position storage

    System() {}

    void start()
    {
        gotoxy(0, 0);
        generateSystemResponse("Smart Campus System Started. Awaiting commands...");
        while (true)
        {
            inputCommand();
        }
    }


    bool initialize() 
    {
        drawBoundary();
        gotoxy(0, 0);
		displayHUDLabels();
		generateSystemResponse("Initializing system...");
        if (!loadUsers())
            return 0;
        if (!loadDoors())
			return 0;
        loadLogs(); 
        loadComments();
        loadRules();
		generateSystemResponse("System initialized successfully.");
        return 1;

	}

    // ===================== LOADING RESOURCES HERE =====================



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

    bool loadLogs()
    {
        generateSystemResponse("loading accesslogs...");
        if (!loadAccessLogsFromFile())
        {
            generateSystemResponse("Failed to load logs from file.");
            return 0;
        }
        return 1;
    }

    bool loadComments()
    {
        generateSystemResponse("loading comments...");
        if (!loadCommentsFromFile())
        {
            generateSystemResponse("Failed to load comments from file.");
            return 0;
        }
        return 1;
    }

    bool loadRules()
    {
        generateSystemResponse("loading access rules...");
        if (!loadRulesFromFile())
        {
            generateSystemResponse("Failed to load access rules from file.");
            return 0;
        }
        return 1;
    }

    


    // ===================== PREVIEW PANEL MESSAGE GENERATION =====================

    void preview(string msg)
    {
        clearPreviewPanel();

        string temp = "";

        int start = 0;
        int pos = 0;
        int i = 0;
        while ((pos = msg.find('\n', start)) != string::npos)  // printing each line manually, by dividing and seaprating substrings on every instance of '\n' 
        {
            temp = msg.substr(start, pos - start);
            start = pos + 1; // skip the newline, ready for next instance

            gotoxy(PREVIEW_PANEL_X + 1, 1+i);
            cout << temp;
            i++;
        }

    }


    // PREVIEW PANEL DEDICATED PRINTING FUNCTIONS


    void printCommandsList()
    {
        preview
        (
            "Commands List:\n"
            "/help, /cmds\n"
            "/comment, #<comment>\n"
            "/load users, /load doors\n"
            "/show users, /show doors\n"
            "/cls, /previewcls, /systemcls, /clsall, /restart\n"
            "/access <userID> <doorID>\n"
        );
    }

    void printUsersList()
    {
        if (users.size() > 18)
            printUsersListManual();
        else
        {
            string msg = "Users List:\n";
            for (auto& u : users)
            {
                msg += to_string(u.id) + " " + u.name + " " + u.role + "\n";
            }
            preview(msg);
        }
    }

    void printDoorsList()
    {
        if (doors.size() > 18)
            printDoorsListManual();
        else
        {
            string msg = "Doors List:\n";
            for (auto& u : doors)
            {
                msg += to_string(u.id) + " " + u.location + " " + u.type + "\n";
            }
            preview(msg);
        }
    }


    void printComments(int n = 0)
    {
    
        string msg = "Comments List (Page " + to_string(n) + ") :\n";
        int start = 0;

        if (comments.size() == 0) 
        {
            msg += "  [Comments List Empty]  \n";
        }
        
        else if (comments.size() < 18)
        {
            start = 0;
        }
        else if ((comments.size()/18) >= n)
        {
            start = 18 * n;
        }
        
        for (int i = start; i < comments.size() && i < start + 18; i++)
        {
            msg += to_string(i) + ". " + comments[i].message + "\n";
        }
        preview(msg);
        
    }

    void printLogs(int n = 0)
    {
        clearPreviewPanel();
        string msg = "Access Logs (Page " + to_string(n) + ") :\n";
        int start = 0;

        if (accessLogs.size() == 0)
        {
            msg += "  [Comments List Empty]  \n";
        }

        else if (accessLogs.size() < 18)
        {
            start = 0;
        }
        else if ((accessLogs.size() / 18) >= n)
        {
            start = 18 * n;
        }

        for (int i = start; i < accessLogs.size() && i < start + 18; i++)
        {
            msg += to_string(i) + ". " + accessLogs[i].logToString() + "\n";
        }
        preview(msg);

    }
    
    void printUsersListManual()
    {
        clearPreviewPanel();

        int k = 0;
        int j = 0;
        string temp = "";
        for (int i = 0; i < users.size(); i++)
        {
            temp = " ";
            temp += to_string(users[i].id) + " ";
            temp += users[i].name + " ";
            temp += users[i].role;

            if (k < 18)
            {
                gotoxy(PREVIEW_PANEL_X + 1, 1 + k);
                cout << temp;
                k++;
            }
            else
            {
                gotoxy(PREVIEW_PANEL_X + 1 + 25, 1 + j);
                cout << temp;
                j++;
            }

        }
    }

    void printDoorsListManual()
    {
        clearPreviewPanel();

        int k = 0;
        int j = 0;
        string temp = "";
        for (int i = 0; i < doors.size(); i++)
        {
            temp = " ";
            temp += to_string(users[i].id) + " ";
            temp += doors[i].location + " ";
            temp += doors[i].type;

            if (k < 18)
            {
                gotoxy(PREVIEW_PANEL_X + 1, 1 + k);
                cout << temp;
                k++;
            }
            else
            {
                gotoxy(PREVIEW_PANEL_X + 1 + 25, 1 + j);
                cout << temp;
                j++;
            }

        }
    }

    


	// ===================== SYSTEM MESSAGES GENERATION =====================

    void generateSystemResponse(string resp) // BLUE ONEs
    {
		int x, y;
		getCursorPosition(x, y);
		gotoxy(0, 22 + countOfSystemMsg%12);

        cout << countOfSystemMsg << ". ";
        setColor(L_BLUE);
        cout << "[System] ";
		setColor(DEFAULT);
        cout << resp << endl;
		countOfSystemMsg++;
        
		gotoxy(x, y); // return to old cursor position
    }

    void generateErrorResponse(string resp) // Red ones
    {
        saveCoords();
        gotoxy(0, 22 + countOfSystemMsg % 12);

		cout << countOfSystemMsg << ". ";
        setColor(L_RED);
        cout << "[Error] ";
        setColor(DEFAULT);
        cout << resp << endl;

		countOfSystemMsg++;
        loadCoords();
    }

    void generateWarningResponse(string resp) // Yellow ones
    {
        saveCoords();

        gotoxy(0, 22 + countOfSystemMsg % 12);

		cout << countOfSystemMsg << ". ";
        setColor(YELLOW);
        cout << "[Warning] ";
        setColor(DEFAULT);
		cout << resp << endl;

		countOfSystemMsg++;
        loadCoords(); // return to old cursor position

    }

    void generateLogResponse(string resp)
    {
        saveCoords();

        gotoxy(0, 22 + countOfSystemMsg % 12);

        cout << countOfSystemMsg << ". ";
        setColor(L_GREEN);
        cout << "[Logs] ";
        setColor(DEFAULT);
        cout << resp << endl;

        countOfSystemMsg++;
        loadCoords(); // return to old cursor position
    }
    
    void generateAccessLog(int uid, int did, bool access)
    {
        AccessLogEntry temp(getCurrentTime(), uid, did, access);
        if (temp.appendToFile())
        {
            accessLogs.push_back(temp);
        }
        else
        {
            generateErrorResponse("accesslogs.txt file not found.");
        }
    }

    void generateComment(string msg)
    {
        CommentEntry temp(getCurrentTime(), msg);
        if (temp.appendToFile())
        {
            comments.push_back(temp);
        }
        else
        {
            generateErrorResponse("comments.txt file not found.");
        }
    }


    // ===================== DISPLAYS AND CLEARS =====================


  
    void clearCommandPanel()
    {           
        saveCoords();
        for (int i = 1; i < 20; i++) 
        {
            gotoxy(0, i);
            clearLine(0, i, 50);
        }
        loadCoords();
	}

    void clearSystemPanel()
    {
        saveCoords();
        for (int i = 22; i < 34; i++)
        {
            gotoxy(0, i);
            clearLine(0, i, 110);
        }
        loadCoords();
    }

    void clearPreviewPanel()
    {
        saveCoords();
        for (int i = 1; i < 20; i++)
        {
            gotoxy(PREVIEW_PANEL_X, i);
            clearLine(PREVIEW_PANEL_X, i, 66);
        }
		loadCoords();
    }
    
    void displayHUDLabels()
    {
		displayCommandPanelLabel();
        displaySystemPanelLabel();
		displayPreviewPanelLabel();
    }

    void displayCommandPanelLabel()
    {
		gotoxy(0, 0);
        setColor(L_CYAN);
        cout << " === Command Panel ===";
		setColor(DEFAULT);
    }

    void displaySystemPanelLabel()
    {
        gotoxy(0, 21);
        setColor(L_CYAN);
		cout << " === System Messages ===";
        setColor(DEFAULT);
    }

    void displayPreviewPanelLabel()
    {
        gotoxy(PREVIEW_PANEL_X, 0);
        setColor(L_CYAN);
        cout << " === Preview Panel ===";
		setColor(DEFAULT);
    }

    // COORDS ESSENTIAL

    void saveCoords()
    {
        getCursorPosition(x, y);
	}
    void loadCoords()
    {
        gotoxy(x, y);
    }

    // ===================== FILE AND DATA HANDLING =====================
// #LOAD #FILE #DATA #FILEHANDLING #DATAHANDLING #LOADUSER #LOADDOORS

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

    bool loadDoorsFromFile() 
    {
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

    bool loadAccessLogsFromFile()
    {
        accessLogs.clear();
        ifstream fin("accesslog.txt");
        if (!fin)
        {
            generateWarningResponse(" accesslog.txt not found.");
            return 0;
        }

        string line;
        while (getline(fin, line))
        {
            if (line.size() < 5) continue;

            // timestamp = between [ ]
            int l = line.find('[');
            int r = line.find(']');
            string timestamp = line.substr(l + 1, r - l - 1);

            // UID=
            int uidPos = line.find("UID=") + 4;
            int uidEnd = line.find(" ", uidPos);
            int uid = stoi(line.substr(uidPos, uidEnd - uidPos));

            // DID=
            int didPos = line.find("DID=") + 4;
            int didEnd = line.find(" ", didPos);
            int did = stoi(line.substr(didPos, didEnd - didPos));

            // RESULT=
            int resPos = line.find("RESULT=") + 7;
            string res = line.substr(resPos);
            bool granted = (res == "GRANTED");

            accessLogs.push_back(AccessLogEntry(timestamp, uid, did, granted));
        }

        fin.close();
        generateSystemResponse("Access Logs Loaded.");
        return 1;
    }

    bool loadCommentsFromFile() 
    {
        comments.clear();
        ifstream fin("comments.txt");
        if (!fin) 
        {
            generateWarningResponse(" comments.txt not found.");
            return 0;
        }

        string line;
        while (getline(fin, line)) {
            if (line.size() < 5) continue;

            int l = line.find('[');
            int r = line.find(']');
            if (l == string::npos || r == string::npos) continue;

            string timestamp = line.substr(l + 1, r - l - 1);
            string msg = line.substr(r + 2); // skip "] "

            comments.push_back(CommentEntry(timestamp, msg));
        }

        fin.close();
        generateSystemResponse("Comments Loaded.");
        return 1;
    }

    bool loadRulesFromFile() 
    {
        rules.clear();
        ifstream fin("rules.txt");

        if (!fin) 
        {
            generateWarningResponse(" rules.txt not found.");
            return 0;
        }

        int id;
        string rest;

        while (fin >> id) 
        {
            getline(fin, rest);
            if (rest.size() > 1 && rest[0] == ' ')
                rest = rest.substr(1);
            rules.push_back(AccessRule(id, rest));
        }

        fin.close();
        generateSystemResponse("Access Rules Loaded.");
        return 1;
    }



    // ===================== ACCESS RULE TEMP =====================

    


    bool canAccess(User u, Door d)
    {
        if (u.role == "student") // classroom only
            return (d.type == "classroom");

        if (u.role == "teacher") // classroom and examhall
            return d.type == "classroom" || d.type == "examhall";

        if (u.role == "staff") // datacenter and examhall
            return d.type == "datacenter" || d.type == "examhall";

        if (u.role == "guard") // everywhere allowed
            return 1;

        if (u.role == "labatt")
            return (d.type == "lab");

        return false;
    }


	// ===================== COMMAND PROCESSING =====================
//#COMMAND #COMMANDPROCESSING #COMMANDINPUT

    void inputCommand()
    {
        if (offsetOfCommandLine >= 16)
        {
            generateWarningResponse("Command section crowded. Consider using '/cls' to clear."); // warning message
        }

        gotoxy(0, 1 + offsetOfCommandLine % 18);

        cout << "> ";
        string command;
        getline(cin, command);

        offsetOfCommandLine++;
        processCommand(command);

        // Command processing will be implemented later
    }

    void processCommand(string command) // sab commands idhar process hongy
    {
        if (command[0] != '/' && command[0] != '#') // every command should start with '/' or '#'
        {
            generateWarningResponse("Commands must start with '/'. Type '/help' for a list of commands.");
            return;
        }
        else if (command[0]=='#') // comments kay liye
        {
            generateComment(command.substr(1));
            generateLogResponse("Added comment: " + command.substr(1));
            return;
		}
        else if (command == "/exit")
        {
            generateSystemResponse("Exiting the system...");
            // exit code
        }
        else if (command == "/help")
        {
            generateSystemResponse("Available commands: help, load, show, cls. For more commands in Preview Panel, type /cmds");
        }
        else if (command == "/cmds")
        {
            printCommandsList();
            generateSystemResponse("Commands List displayed in Preview Panel.");
        }
        else if (command == "/show users")
        {
            //printUsersListManual(); // for backup
            printUsersList();
            generateSystemResponse("Users List displayed in Preview Panel.");
        }
        else if (command == "/show doors")
        {
            //printDoorsListManual(); // for backup
            printDoorsList();
            generateSystemResponse("Doors List displayed in Preview Panel.");
        }
        else if (command == "/load users")
        {
            loadUsers();
			generateSystemResponse("Users reloaded from file.");

        }
        else if (command == "/load doors")
        {
            loadDoors();
            generateSystemResponse("Doors reloaded from file.");
        }
        else if (command == "/cls") // screen clear karnay kay liye
        {
            clearCommandPanel();
            offsetOfCommandLine = 0; // reset command line offset
            generateSystemResponse("Command section cleared.");
            gotoxy(0, 0);
        }
		else if (command == "/systemcls") // clears system panel
        {
            clearSystemPanel();
            countOfSystemMsg = 0; // reset system message count
            generateSystemResponse("System section cleared.");
            gotoxy(0, 0);
        }
		else if (command == "/previewcls") // clears preview panel
        {
            clearPreviewPanel();
            generateSystemResponse("Preview section cleared.");
            gotoxy(0, 0);
        }
        else if (command == "/clsall") // clears all 3 panels
        {
            clearCommandPanel();
            clearSystemPanel();
            clearPreviewPanel();
            offsetOfCommandLine = 0;
            countOfSystemMsg = 0;
            generateSystemResponse("All sections cleared.");
            gotoxy(0, 0);
        }
        else if (command == "/restart") // incase visuals get bugged :>
        {
            system("CLS");
			drawBoundary();
            displayHUDLabels();
            offsetOfCommandLine = 0;
            countOfSystemMsg = 0;
            generateSystemResponse("System restarted.");
			gotoxy(0, 0);

        }   
        else if (command[0] == '/') // some command                      GENERAL CASE HERE ==========
        {
            stringstream ss(command);
            string cmd = "";
            int i = 0;
            ss >> cmd;

            if (cmd == "/access") // access command
            {
                vector<int> parameters;
                int temp = 0;
                while (ss >> temp) // store the parameters
                {
                    parameters.push_back(temp);
                }

            
                if (parameters.size() == 2) // correct usage
                {
                    if (handleAccess(parameters[0], parameters[1]))
                    {
                        generateSystemResponse("Access Command Used.");

                    }
                    else
                    {
                        generateSystemResponse("Access Command Failed.");
                    }
                }
                else
                {
                    generateWarningResponse("Missing Command Parameters. /access <UserID> <DoorID>");
                }
            }
            else if (cmd == "/comment")
            {
                generateComment(command.substr(9));
                generateLogResponse("Added comment: " + command.substr(1));
            }
            else if (cmd == "/show")
            {
                string para1;
                ss >> para1;
                if (para1 == "comments")
                {
                    int para2 = 0;
                    if (ss >> para2)
                    {
                        printComments(para2);
                    }
                    else
                    {
                        printComments(0);
                    }
                    generateSystemResponse("Printed Comments in Preview Panel.");
                }
                else if (para1 == "logs")
                {
                    int para2 = 0;
                    if (ss >> para2)
                    {
                        printLogs(para2);
                    }
                    else
                    {
                        printLogs(0);
                    }
                    generateSystemResponse("Printed Access Logs in Preview Panel.");
                }
            }
            else
            {
                generateErrorResponse("Unknown command. Type 'help' for a list of commands.");
            }

        }

        else
        {
            generateErrorResponse("Unknown command. Type 'help' for a list of commands.");
        }

    }


    // HANDLING THE /ACCESS command here

    bool handleAccess(int uid, int did) 
    {
        User* u = nullptr;
        Door* d = nullptr;

        // find user and dooor
        for (auto& x : users)
            if (x.id == uid) u = &x;

        for (auto& x : doors)
            if (x.id == did) d = &x;

        if (!u)
        {
            generateErrorResponse("User not found.");
            return 0;
        }

        if (!d) 
        {
            generateErrorResponse("Door not found.");
            return 0;
        }

        //bool allowed = canAccess(*u, *d);
        bool allowed = 1;
        // Rule Engine (deny-only rules)
        for (auto& r : rules)
        {
            if (r.evaluateRule(*u, *d)) 
            {
                generateSystemResponse("[Rule Denied] Rule " + to_string(r.id) + " triggered.");
                preview("Rule denied access: " + r.ruleString);
                allowed = false;
                break;
            }
        }


        
        generateAccessLog(uid, did, allowed);

        if (allowed) 
        {
            generateLogResponse("ACCESS GRANTED " + u->name +" ("+ to_string(uid) + ")" + " entered " + d->location + " " + d->type + " (" + to_string(did) + ")");
        }
        else 
        {
            generateLogResponse("ACCESS DENIED " + u->role + " cannot enter " + d->type);
        }
        return 1;
    }


};

#endif




