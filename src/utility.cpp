#include "utility.h"

std::vector<std::string> split(std::string text, char delim) {
    std::vector<std::string> elements;
    std::stringstream stream(text);
    std::string item;
    while (getline(stream, item, delim)) {
        item.erase(0, item.find_first_not_of(" \n\r\t"));
        item.erase(item.find_last_not_of(" \n\r\t") + 1);
        elements.push_back(item);
    }
    return elements;
}

std::string toLower(const std::string& txt) {
    std::string lowered = txt;
    std::transform(lowered.begin(), lowered.end(), lowered.begin(), ::tolower);
    return lowered;
}

std::string replaceChar(std::string str, char search, char replace) {
    for (int i = 0; i < str.length(); ++i) {
        if (str[i] == search)
            str[i] = replace;
    }

    return str;
}

std::string trim(std::string str) {
    str.erase(str.find_last_not_of(" \n\r\t") + 1);
    return str;
}