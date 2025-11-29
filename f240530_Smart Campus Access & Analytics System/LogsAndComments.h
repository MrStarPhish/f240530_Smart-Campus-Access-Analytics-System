#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<fstream>
#include"ConsoleAndBufferEssentials.h"
#include"Graph.h"
#include"AccessRules.h"
#include"UserAndDoors.h"


using namespace std;




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
