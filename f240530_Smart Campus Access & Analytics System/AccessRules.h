#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<fstream>
#include"ConsoleAndBufferEssentials.h"
#include"UserAndDoors.h"

using namespace std;

class AccessRule
{
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
