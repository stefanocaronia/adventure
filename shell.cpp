#include <conio.h>
#include <windows.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "colors.h"
#include "globals.h"
#include "shell.h"

using namespace std;

Shell::Shell() {
    SetConsoleTitle("Adventure");
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

Shell::~Shell() {}

void Shell::run() {
    string commandLine = "";
    int result = 0;

    printHeader(true);

    while (RUNNING) {
        cout << PROMPT_COLOR << PROMPT << " " << COMMAND_COLOR;

        commandLine = input();

        cout << "\r\n";

        if (commandLine.length() > 0) {
            result = process(commandLine);
            if (result > 0) cout << ERROR_COLOR << "ERROR " << result << endl;
            cout << endl;
        }
    }
}

string Shell::input() {
    commandLine = "";
    string currentWord = "";
    cursorPosInCommandLine = 0;
    string currentWordCompl = "";
    char ch;

    while (((ch = _getch()) != '\r')) {
        refreshCSBI();

        switch (ch) {
            case KEY_UP:
                for (unsigned int c = 0; c < commandLine.length(); c++) {
                    cout << char(KEY_BACKSPACE) << " " << char(KEY_BACKSPACE);
                }
                commandLine = history[history_pos];
                cout << commandLine;
                if (history_pos > 0) history_pos--;
                cursorPosInCommandLine = commandLine.length();
                break;
            case KEY_DOWN:
                for (unsigned int c = 0; c < commandLine.length(); c++) {
                    cout << char(KEY_BACKSPACE) << " " << char(KEY_BACKSPACE);
                }
                commandLine = history[history_pos];
                cout << commandLine;
                if (history_pos < history.size() - 1) history_pos++;
                cursorPosInCommandLine = commandLine.length();
                break;
            /*case KEY_LEFT:
                    cursorLeft();
                    break;*/
            case -32:
                if (_getch() == KEY_LEFT) cursorLeft();
                if (_getch() == KEY_RIGHT) cursorRight();
                break;
            case KEY_RIGHT:
                cursorRight();
                break;
            case KEY_BACKSPACE:
                if (commandLine.size() > 0) {
                    cout << ch << " " << ch;
                    commandLine.erase(commandLine.size() - 1);
                }
                cursorPosInCommandLine = commandLine.length();
                break;
            default:
                if (cursorPosInCommandLine == commandLine.length()) {
                    commandLine += ch;
                    cout << ch;
                }
                cursorPosInCommandLine = commandLine.length();
        }
    }

    return commandLine;
}

int Shell::process(string cline) {
    int result = 0;
    vector<string> cparts;
    cparts = split(cline, ' ');
    string command = cparts[0];

    cout << OUTPUT_COLOR;

    history.push_back(cline);
    history_pos = history.size() - 1;

    if (command == "cls") {
        result = system(cline.c_str());
        printHeader(false);
    } else if (command == "ciao") {
        cout << CUSTOM_COLOR << "Cazzo vuoi?" << endl;
    } else if (command == "exit") {
        RUNNING = false;
    } else {
        // il gioco
    }

    return result;
}

void Shell::printHeader(bool with_endl = true) {
    cout << TITLE_COLOR << "Welcome to " << title << " " << version << " by s.caronia" << endl;
}

string Shell::getCurrentWord(string& cline) {
    std::size_t found = cline.find_last_of(" ");
    return cline.substr(found + 1);
}

void Shell::setTitle(const char* s) { title = string(s); }

void Shell::setVersion(const char* s) { version = string(s); }

void Shell::setPROMPT(const char* ti) { PROMPT = string(ti); }

void Shell::refreshCSBI() {
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    cursorPosition = csbi.dwCursorPosition;
}

void Shell::cursorLeft() {
    if (cursorPosInCommandLine == 0) return;
    COORD newCoord;
    newCoord.Y = csbi.dwCursorPosition.Y;
    newCoord.X = csbi.dwCursorPosition.X - 1;
    if ((cursorPosInCommandLine + PROMPT.length()) % csbi.dwMaximumWindowSize.X == 1) {
        newCoord.Y--;
    }
    cursorPosInCommandLine--;
    SetConsoleCursorPosition(hConsole, newCoord);
}

void Shell::cursorRight() {
    if (cursorPosInCommandLine == commandLine.length()) return;
    COORD newCoord;
    newCoord.Y = csbi.dwCursorPosition.Y;
    newCoord.X = csbi.dwCursorPosition.X + 1;
    SetConsoleCursorPosition(hConsole, newCoord);
    cursorPosInCommandLine++;
}

/* UTILITY FUNCTIONS */

vector<string> split(string text, char delim) {
    vector<string> elements;
    stringstream stream(text);
    string item;
    while (getline(stream, item, delim)) {
        item.erase(0, item.find_first_not_of(" \n\r\t"));
        item.erase(item.find_last_not_of(" \n\r\t") + 1);
        elements.push_back(item);
    }
    return elements;
}
