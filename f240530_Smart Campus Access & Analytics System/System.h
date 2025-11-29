#pragma once

// All the classes and their function are declared here

#ifndef System_H
#define System_H

#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<fstream>
#include"ConsoleAndBufferEssentials.h"
#include"LogsAndComments.h"
#include"AccessRules.h"
#include"UserAndDoors.h"
#include"Graph.h"

using namespace std;


// =====================
// MAIN SYSTEM // Sab kuch idhar se control kr rhay :)
// =====================
class System 
{
public:
    vector<User> users;
    vector<Door> doors;
    vector<AccessLogEntry> accessLogs;
    vector<CommentEntry> comments;
    vector<AccessRule> rules;
    CampusGraph graph;
	bool loginStatus = 0;
    int offsetOfCommandLine = 0;
    int countOfSystemMsg = 0;
	int x, y; // cursor position storage

    System() {}

    void start()
    {
        gotoxy(0, 0);
        generateSystemResponse("Smart Campus System Started. Awaiting commands...");
        loginStatus = 1;
        while (loginStatus)
        {
            inputCommand();
        }

        generateSystemResponse("System Exited.");
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
        loadGraph(1); // 0 for edges.txt || 1 for edges2.txt that has EULAR Path
		generateSystemResponse("System initialized successfully.");
        return 1;

	}

    void insertCommandManually(string cmd)
    {
        processCommand(cmd); // processing the given command here
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

    bool loadGraph(int i = 0)
    {
        generateSystemResponse("loading graphs...");
        graph.setSize(doors.size());
        if (i == 0)
        {
            if (!loadGraphFromFile())
            {
                generateSystemResponse("Failed to load graph from file.");
                return 0;
            }
        }
        else if (i == 1)
        {
            if (!loadGraphFromFile1())
            {
                generateSystemResponse("Failed to load graph from file2.");
                return 0;
            }
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
            "/graph <display/mst/eular/ham>\n"
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

    void printAdjacencyList()
    {
        clearPreviewPanel();
        string temp = graph.getAdjList();
        preview(temp);
    }
    
    void printMST()
    {
        clearPreviewPanel();
        string temp = graph.prim();
        preview(temp);
    }

    void printHamiltonian()
    {
        clearPreviewPanel();
        string temp = graph.findHamiltonianPath();
        preview(temp);
    }

    void printEular()
    {
        clearPreviewPanel();
        string temp = graph.findEulerPath();
        preview(temp);
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

    bool loadGraphFromFile()
    {
        graph.adj.clear();
        graph.adj.resize(graph.size + 1);

        ifstream fin("edges.txt");
        if (!fin)
        {
            generateErrorResponse("edges.txt file is missing.");
            return 0;
        }
        


        int a, b, w;

        while (fin >> a >> b >> w)
        {
            graph.addEdge(a, b, w);
        }

        fin.close();

        string temp = "Loaded ";
        temp += to_string(graph.size);
        temp += " edges from file.";
        generateSystemResponse(temp);

        return 1;
    }

    bool loadGraphFromFile1() // this file will have EULAR PATH/CIRCUIT
    {
        graph.adj.clear();
        graph.adj.resize(graph.size + 1);

        ifstream fin("edges2.txt");
        if (!fin)
        {
            generateErrorResponse("edges2.txt file is missing.");
            return 0;
        }



        int a, b, w;

        while (fin >> a >> b >> w)
        {
            graph.addEdge(a, b, w);
        }

        fin.close();

        string temp = "Loaded ";
        temp += to_string(graph.size);
        temp += " edges from file2.";
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


    // ===================== ACCESS RULE SECONDARY =====================

    


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
        processCommand(command); // processing the given command here

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
        else if (command == "/exit" || command == "/quit")
        {
            generateSystemResponse("Exiting the system...");
            loginStatus = 0;
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
                else
                {
                    generateErrorResponse("Invalid Parameters. /show <entity> . /cmds for more info.");
                }
            }
            else if (cmd == "/graph")
            {
                string para1;
                ss >> para1;
                if (para1 == "display")
                {
                    printAdjacencyList();
                    generateSystemResponse("Printed Adjacency List in Preview Panel.");
                }
                else if (para1 == "mst" || para1 == "MST")
                {
                    printMST();
                    generateSystemResponse("Printed MST in Preview Panel.");
                }
                else if (para1 == "ham" || para1 == "hamiltonian")
                {
                    printHamiltonian();
                    generateSystemResponse("Printed Hamiltonian in Preview Panel.");
                }
                else if (para1 == "eular" || para1 == "EULAR")
                {
                    printEular();
                    generateSystemResponse("Printed Eular Path in Preview Panel.");
                }
                else
                {
                    generateErrorResponse("Invalid Parameters. /graph <display/mst/ham/eular>");
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

        bool allowed = 1; // by default allowed
        
        for (auto& r : rules) // checks every rule , Rule Engine (deny-only rules)
        {
            if (r.evaluateRule(*u, *d)) 
            {
                generateSystemResponse("[Rule Denied] Rule " + to_string(r.id) + " triggered.");
                preview("Rule denied access: " + r.ruleString);
                allowed = false;
                break;
            }
        }

        //bool allowed = canAccess(*u, *d); // secondary access rule processor

        
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




