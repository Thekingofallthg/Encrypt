#include <emscripten/bind.h>
#include <string>
#include <vector>
#include <algorithm>

const std::string PASSWORD = "$igmas123@";
const std::string ADMIN_USER = "Anthonerules1";
const std::string ADMIN_PASS = "HeilH1tler123@";

std::vector<std::string> users = {"@nth0ne1", "N@than2", "Fl0re$3",
                                  "XxGimenezxX4", "K@duK@ki5", "Mor3n06",
                                  "B@cal@n7", "R@ni3l8", "G@bij@n9", "S0lit@ire10"};

const int shifts[100] = {5, 2, 1, 7, 3, 6, 7, 2, 2, 4, 5, 9, 2, 5, 5, 6, 8, 3, 4, 2,
                         8, 7, 5, 2, 7, 2, 1, 2, 3, 4, 5, 2, 6, 7, 8, 9, 2, 3, 6, 4,
                         7, 2, 3, 4, 5, 6, 7, 9, 2, 4, 1, 8, 2, 3, 6, 4, 3, 4, 8, 7,
                         9, 2, 3, 1, 4, 5, 6, 8, 9, 7, 6, 2, 3, 3, 3, 1, 3, 3, 1, 2,
                         5, 6, 8, 9, 2, 3, 1, 2, 6, 3, 4, 7, 8, 3, 4, 3, 1, 2, 1, 2};

bool login(std::string user, std::string pass) {
    if (pass != PASSWORD) return false;
    return std::find(users.begin(), users.end(), user) != users.end();
}

bool adminLogin(std::string user, std::string pass) {
    return user == ADMIN_USER && pass == ADMIN_PASS;
}

std::string getUserList(std::string aUser, std::string aPass) {
    if (!adminLogin(aUser, aPass)) return "";
    std::string list;
    for (const auto& u : users) list += u + "\n";
    return list;
}

bool addUser(std::string aUser, std::string aPass, std::string newUser) {
    if (!adminLogin(aUser, aPass)) return false;
    if (newUser.empty() || newUser.find('\n') != std::string::npos) return false;
    if (std::find(users.begin(), users.end(), newUser) != users.end()) return false;
    users.push_back(newUser);
    return true;
}

bool removeUser(std::string aUser, std::string aPass, std::string name) {
    if (!adminLogin(aUser, aPass)) return false;
    auto it = std::find(users.begin(), users.end(), name);
    if (it == users.end()) return false;
    users.erase(it);
    return true;
}

// Loads a saved newline-separated list (used on page load)
void setUsers(std::string data) {
    users.clear();
    std::string line;
    for (char c : data) {
        if (c == '\n') {
            if (!line.empty()) users.push_back(line);
            line.clear();
        } else {
            line += c;
        }
    }
    if (!line.empty()) users.push_back(line);
}

std::string encryptText(std::string text) {
    for (size_t i = 0; i < text.length(); i++) {
        if (text[i] >= 'a' && text[i] <= 'z') {
            text[i] = ((text[i] - 'a' + shifts[i % 100]) % 26) + 'a';
        } else if (text[i] >= 'A' && text[i] <= 'Z') {
            text[i] = ((text[i] - 'A' + shifts[i % 100]) % 26) + 'A';
        }
    }
    return text;
}

std::string decryptText(std::string text) {
    for (size_t i = 0; i < text.length(); i++) {
        if (text[i] >= 'a' && text[i] <= 'z') {
            text[i] = ((text[i] - 'a' - shifts[i % 100] + 26) % 26) + 'a';
        } else if (text[i] >= 'A' && text[i] <= 'Z') {
            text[i] = ((text[i] - 'A' - shifts[i % 100] + 26) % 26) + 'A';
        }
    }
    return text;
}

EMSCRIPTEN_BINDINGS(cipher_module) {
    emscripten::function("login", &login);
    emscripten::function("adminLogin", &adminLogin);
    emscripten::function("getUserList", &getUserList);
    emscripten::function("addUser", &addUser);
    emscripten::function("removeUser", &removeUser);
    emscripten::function("setUsers", &setUsers);
    emscripten::function("encryptText", &encryptText);
    emscripten::function("decryptText", &decryptText);
}
