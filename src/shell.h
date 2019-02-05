#pragma once

#include <conio.h>
#include <windows.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "globals.h"
#include "utility.h"

using namespace std;

typedef map<string, string> conf;

class Shell {
public:
    bool RUNNING = true;

    Shell();
    ~Shell();

    void run();

    void setTitle(const char *);
    void setVersion(const char *);
    void setPROMPT(const char *);
    bool loadConfig();

private:
    string USERNAME;
    string COMPUTERNAME;

    string title = "Adventure";
    string version = "v1.0";
    string PROMPT = ">";

    conf config;

    concol TITLE_COLOR = gray;
    concol PROMPT_COLOR = white;
    concol COMMAND_COLOR = white;
    concol OUTPUT_COLOR = gray;
    concol CUSTOM_COLOR = magenta;
    concol ERROR_COLOR = red;
    concol PATH_COLOR = dark_aqua;

    enum keys {
        KEY_UP = 72,
        KEY_DOWN = 80,
        KEY_LEFT = 75,
        KEY_RIGHT = 77,
        KEY_BACKSPACE = 8,
        KEY_TAB = 9,
        KEY_ESC = 27,
        ESCAPE = -32
    };

    string input();
    int process(string);
    string exec(const char *);
    void printHeader(bool);
    string getCurrentWord(string &);
    void setCurrentPathFromCommand(string);
    void refreshCSBI();
    void cursorLeft();
    void cursorRight();
    void historyUp();
    void historyDown();

    vector<string> history;
    unsigned int historyPos = 0;
    unsigned int cursorPosInCommandLine;

    HANDLE hConsole;
    COORD cursorPosition;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    string commandLine;
};

/* UTILITY */
vector<string> split(string, char);
