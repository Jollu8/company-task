//
// Created by Jollu Emil on 10/27/23.
//


#include "User.h"
#include <string>

bool UserTable::IsUserInTable(const std::string &login_) {
    return logins.contains(login_);
}

bool UserTable::IsUserInTable(std::size_t uid) {
    return users.contains(uid);
}

bool UserTable::IsUserDataCorrect(const std::string &login, const std::string &password) {
    return IsUserInTable(login) && users[logins.at(login)].IsPasswordCorrect(password);
}

std::size_t UserTable::GetUid(const std::string &login) {
    return (IsUserInTable(login) ? logins.at(login) : 0);
}

std::size_t UserTable::AddUser(const std::string& login, const std::string &password) {
    if (!IsUserInTable(login)) {
        std::size_t myUid = maxUid++;
        User newUser(myUid, login, password);
        logins[login] = myUid;
        users[myUid] = newUser;
        return myUid;
    }
    return 0;
}

bool UserTable::AddUsdToUser(std::size_t uid, my_long volume) {
    if (!IsUserInTable(uid))return false;
    users[uid].AddUsd(volume);
    return true;
}

bool UserTable::AddRubToUser(std::size_t uid, my_long volume) {
    if (!IsUserInTable(uid)) return false;
    users[uid].AddRub(volume);
    return true;
}

my_long UserTable::GetUserUsdBalance(std::size_t uid) {
    return users.at(uid).GetUsdBalance();
}

my_long UserTable::GetUserRubBalance(std::size_t uid) {
    return users.at(uid).GetRubBalance();
}
