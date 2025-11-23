#pragma once

// All Console related functions and Buffer/Output related functions are here.
#include<iostream>
#include<Windows.h>
#include<ctime>
#include<sstream>
using namespace std;

const int WIDTH = 125;
const int HEIGHT = 40;

const int X_BOUND = 118;
const int Y_BOUND = 33; // Boundaries Coords

const int COMMAND_PANEL_X = 0;
const int COMMAND_PANEL_Y = 1;

const int SYSTEM_PANEL_X = 0;
const int SYSTEM_PANEL_Y = 24;

const int PREVIEW_PANEL_X = 52;
const int PREVIEW_PANEL_Y = 1;

CHAR_INFO buffer[WIDTH * HEIGHT]; // character + color info

enum ColorCode
{
    BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE, GREY, L_BLUE, L_GREEN, L_CYAN, L_RED, L_MAGENTA, L_YELLOW, L_WHITE, DEFAULT = 7
};


void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clearLine(int x, int y, int length = 1)
{
    gotoxy(x, y);
    for (int i = 0; i < length; i++)
    {
        cout << " ";
    }
}

void setColor(int textColor = DEFAULT)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), textColor);
}

// Function to get current cursor position
void getCursorPosition(int& x, int& y) 
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) 
    {
        x = csbi.dwCursorPosition.X;
        y = csbi.dwCursorPosition.Y;
    }
    else 
    {
        // If for some reason it fails, return (0,0)
        x = 0;
        y = 0;
    }
}

void clearBuffer() // to clear screen ; instead of "CLS"
{
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        buffer[i].Char.AsciiChar = ' ';
        buffer[i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // Default aka White
    }
}



void bufferChar(int x, int y, char ch, WORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) // coords, char, RGBA
{
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
    {
        int index = y * WIDTH + x;
        buffer[index].Char.AsciiChar = ch;
        buffer[index].Attributes = color;
    }
}

void bufferString(int x, int y, string line, WORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) // coords, string, RGBA
{
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
    {
        int index = y * WIDTH + x;
        for (int i = 0; i < line.length(); i++)
        {
            buffer[index + i].Char.AsciiChar = line[i];

            buffer[index + i].Attributes = color;
        }

    }
}

void eraseChar(int x, int y) // the character to be removed ; coords
{
    bufferChar(x, y, ' ');
}

void eraseString(int x, int y, int length) // the string of characters to be removed ; coords , length
{
    for (int i = 0; i < length; i++)
    {
        eraseChar(x + i, y);
    }
}

void renderBuffer() // render the whole buffer[] 
{
    COORD bufferSize = { WIDTH, HEIGHT };
    COORD bufferCoord = { 0, 0 };
    SMALL_RECT writeRegion = { 0, 0, WIDTH - 1, HEIGHT - 1 };

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsoleOutputA(hConsole, buffer, bufferSize, bufferCoord, &writeRegion);
}

void SetConsoleBufferSize(int width, int height)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD bufferSize; // setting buffer size here
    bufferSize.X = width;
    bufferSize.Y = height;
    SetConsoleScreenBufferSize(hOut, bufferSize);

    SMALL_RECT windowSize; // setting windows size here
    windowSize.Left = 0;
    windowSize.Top = 0;
    windowSize.Right = width - 1;
    windowSize.Bottom = height - 1;
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);
}

void SetConsoleWindowSize(int width, int height)
{
    HWND consoleWindow = GetConsoleWindow();      // Window handle
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);     // Output handle

    CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) }; // getting current font size to calculate pixel dimensions
    GetCurrentConsoleFontEx(hOut, FALSE, &fontInfo);

    int fontWidth = fontInfo.dwFontSize.X;
    int fontHeight = fontInfo.dwFontSize.Y;

    COORD newSize = { (SHORT)width, (SHORT)height }; // Resisizing Buffer first
    SetConsoleScreenBufferSize(hOut, newSize);

    SMALL_RECT windowSize = { 0, 0, (SHORT)(width - 1), (SHORT)(height - 1) }; // Resizing visible window
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);

    MoveWindow(consoleWindow, 100, 100, width * fontWidth, height * fontHeight, TRUE); // Physical resizing 
}

string getCurrentTime()
{
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);

    stringstream ss;
    ss << 1900 + ltm.tm_year << "-"
        << (ltm.tm_mon + 1) << "-"
        << ltm.tm_mday << " "
        << ltm.tm_hour << ":"
        << ltm.tm_min << ":"
        << ltm.tm_sec;

    return ss.str();  // returning time-stamp string like "2025-07-02 23:18:15"
}





void drawBoundary() 
{
    gotoxy(0, 20);

    for (int i = 0; i < 5; i++)
        cout << "=========================";

    gotoxy(0, Y_BOUND + 2);

    for (int i = 0; i < 5; i++)
        cout << "=========================";

    for (int i = 0; i <= 33 + 2; i++) // left boundary
    {
        gotoxy(X_BOUND, i);
        cout << "||";
    }

    for (int i = 0; i < 20; i++) // middle boundary
    {
        gotoxy(50, i);
        cout << "^^";
    }


}