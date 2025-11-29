#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<fstream>
#include"ConsoleAndBufferEssentials.h"

using namespace std;

class CampusGraph
{
public:
    vector<vector<pair<int, int>>> adj;  // adjacency list 
    int size = 0;
    CampusGraph(int n = 0)
    {
        size = n;
        adj.resize(n + 1); // 1-based indexing
    }
    void setSize(int sz)
    {
        size = sz;
    }
    void addEdge(int a, int b, int w)
    {
        if (a < 1 || b < 1 || a > size || b > size)
            return;

        adj[a].push_back({ b, w });
        adj[b].push_back({ a, w });   // undirected
    }
    string getAdjList()
    {
        string out;

        for (int i = 1; i <= size; i++)
        {
            out += to_string(i) + ": ";
            for (auto& p : adj[i]) {
                out += "(" + to_string(p.first) + ", w=" + to_string(p.second) + ") ";
            }
            out += "\n";
        }

        return out;
    }

    string prim()
    {
        vector<int> key(size + 1, INT_MAX);
        vector<bool> inMST(size + 1, false);
        vector<int> parent(size + 1, -1);

        key[1] = 0; // start from node 1

        for (int i = 1; i <= size; i++)
        {
            int u = -1;


            for (int v = 1; v <= size; v++)  // pick min key vertex not in MST
            {
                if (!inMST[v] && (u == -1 || key[v] < key[u]))
                    u = v;
            }

            inMST[u] = true;


            for (auto& p : adj[u]) // checking every neighbors here
            {
                int v = p.first;
                int w = p.second;

                if (!inMST[v] && w < key[v])
                {
                    key[v] = w;
                    parent[v] = u;
                }
            }
        }

        string out = "--- MST (Prim's) ---\n";
        int total = 0;

        for (int v = 2; v <= size; v++)
        {
            if (parent[v] != -1)
            {
                out += to_string(parent[v]) + " - " + to_string(v) +
                    " (w=" + to_string(key[v]) + ")\n";
                total += key[v];
            }
        }

        out += "\tTotal cost = " + to_string(total) + "\n";
        return out;
    }

    bool hamDFS(int u, vector<bool>& vis, vector<int>& path)
    {
        if ((int)path.size() == size)
            return true;

        for (auto& p : adj[u])
        {
            int v = p.first;
            if (!vis[v])
            {
                vis[v] = true;
                path.push_back(v);
                if (hamDFS(v, vis, path))
                    return true;
                path.pop_back();
                vis[v] = false;
            }
        }
        return false;
    }

    string findHamiltonianPath()
    {
        for (int start = 1; start <= size; start++)
        {
            vector<bool> vis(size + 1, false);
            vector<int> path;

            vis[start] = true;
            path.push_back(start);

            if (hamDFS(start, vis, path))
            {
                string out = "Hamiltonian Path:\n";
                int i = 0;
                for (int v : path)
                {
                    out += to_string(v) + " -> ";
                    if (i % 2 == 0)
                        out += "\n";
                    i++;
                }
                out.pop_back(); out.pop_back(); out.pop_back(); out.pop_back(); out.pop_back();
                out += ".\n";
                return out;
            }
        }
        return "No Hamiltonian Path exists in this graph.\n";
    }


    string findEulerPath()
    {
        vector<int> degree(size + 1, 0);

        for (int u = 1; u <= size; u++) // compute degrees
            degree[u] = adj[u].size();


        vector<int> odd;         // count odd degree vertices
        for (int i = 1; i <= size; i++)
        {
            if (degree[i] % 2 == 1)
                odd.push_back(i);
        }

        if (odd.size() != 0 && odd.size() != 2) // No Euler path if odd > 2
            return "No Euler Path or Circuit exists.\n";

        int start = 1; // pick start node
        if (odd.size() == 2)
            start = odd[0];

        vector<vector<int>> g(size + 1);

        // copy adjacency list
        for (int u = 1; u <= size; u++)
            for (auto& p : adj[u])
                g[u].push_back(p.first);

        auto removeEdge = [&](int a, int b)
            {
                for (int i = 0; i < g[a].size(); i++)
                {
                    if (g[a][i] == b)
                    {
                        g[a].erase(g[a].begin() + i);
                        break;
                    }
                }
            };


        stack<int> st;
        vector<int> path;
        st.push(start);


        while (!st.empty()) // Hierholzer's Algorithm
        {
            int v = st.top();
            if (!g[v].empty())
            {
                int u = g[v].back();
                g[v].pop_back();
                removeEdge(u, v);
                st.push(u);
            }
            else
            {
                path.push_back(v);
                st.pop();
            }
        }

        string out;
        if (odd.size() == 0)
            out += "Euler Circuit exists:\n";
        else
            out += "Euler Path exists:\n";

        int i = 0;
        for (int v : path)
        {
            out += to_string(v) + " -> ";
            if (i % 2 == 0)
                out += "\n";
            i++;
        }
        out.pop_back(); out.pop_back(); out.pop_back(); out.pop_back(); // remove arrow

        out += ".\n";
        return out;
    }

};