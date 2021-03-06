#include "shell.h"
#include "colors.h"

Shell::Shell() {
    SetConsoleTitleA("Adventure");
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    HWND console = GetConsoleWindow();
    MoveWindow(console, 200, 200, 800, 600, true);
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
            if (result > 0) {
                cout << ERROR_COLOR << "ERROR " << result << endl;
            }
        }
    }
}

string Shell::input() {
    commandLine = "";
    string currentWord = "";
    cursorPosInCommandLine = 0;
    char ch;

    while (((ch = _getch()) != '\r')) {
        refreshCSBI();

        //cout << "[" << (int)ch << "|"<< ch << "] ";

        switch (ch) {
            case ESCAPE:
                switch (_getch()) {
                    case KEY_LEFT:
                        cursorLeft();
                        break;
                    case KEY_RIGHT:
                        cursorRight();
                        break;
                    case KEY_UP:
                        historyUp();
                        break;
                    case KEY_DOWN:
                        historyDown();
                        break;
                }
                break;
            case KEY_BACKSPACE:
                if (commandLine.size() > 0) {
                    cout << ch << " " << ch;
                    commandLine.erase(commandLine.size() - 1);
                }
                cursorPosInCommandLine = commandLine.length();
                break;
            case KEY_ESC:
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
    string command = "";
    vector<string> words;

    // pulisco la line
    cline = toLower(cline);
    cline = replaceChar(cline, '?', ' ');
    cline = trim(cline);

    // ottengo le parole
    words = split(cline, ' ');

    // pulisco il command
    command = toLower(words[0]);
    command = replaceChar(command, '?', ' ');
    command = trim(command);

    // cout << command;

    cout << OUTPUT_COLOR;

    history.push_back(cline);
    historyPos = history.size() - 1;

    if (command == "cls" || command == "clear") {
        //result = system(cline.c_str());
        result = system("cls");
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
    cout << TITLE_COLOR << "Welcome to " << title << " " << version << " by s.caronia" << endl << endl;
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

void Shell::historyUp() {
    if (history.size() == 0) {
        return;
    }
    for (unsigned int c = 0; c < commandLine.length(); c++) {
        cout << char(KEY_BACKSPACE) << " " << char(KEY_BACKSPACE);
    }
    commandLine = history[historyPos];
    cout << commandLine;
    if (historyPos > 0) historyPos--;
    cursorPosInCommandLine = commandLine.length();
}

void Shell::historyDown() {
    if (history.size() == 0) {
        return;
    }
    for (unsigned int c = 0; c < commandLine.length(); c++) {
        cout << char(KEY_BACKSPACE) << " " << char(KEY_BACKSPACE);
    }
    commandLine = history[historyPos];
    cout << commandLine;
    if (historyPos < history.size() - 1) historyPos++;
    cursorPosInCommandLine = commandLine.length();
}

void Shell::cursorRight() {
    if (cursorPosInCommandLine == commandLine.length()) return;
    COORD newCoord;
    newCoord.Y = csbi.dwCursorPosition.Y;
    newCoord.X = csbi.dwCursorPosition.X + 1;
    SetConsoleCursorPosition(hConsole, newCoord);
    cursorPosInCommandLine++;
}

