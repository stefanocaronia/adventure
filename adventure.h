#pragma once

#include <string>

using namespace std;

class Adventure {
public:
    string name = "";
    Character* player = nullptr;
    Room* room = nullptr;
    string start = "";

    GameObject* objects[];

    Adventure();

    void register(GameObject*);
    void command(string);

    bool lookList(string);

private:
}