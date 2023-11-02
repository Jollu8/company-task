//
// Created by Jollu Emil on 10/27/23.
//

#pragma once

#include "deal.h"

#include <string>
#include <unordered_map>
#include <utility>

#include <iostream>


class User {
public: //Constructor
    User(std::size_t uid_,  std::string login_, const std::string &password_) :
            uid(uid_),
            login(std::move(login_)),
            password(password_),
            usdBalance{0},
            rubBalance{0} {}

    User() :
            uid(0),

            login(std::string()),
            password(std::string()),
            usdBalance(0),
            rubBalance(0) {}

public: // Getter
     [[nodiscard]] std::string GetLogin() const {
    if(login.empty())
        std::cout << "Login is empty" <<std::endl;
    return login; }

    [[nodiscard]] std::size_t GetUid() const { return uid; }

    [[nodiscard]] my_long GetUsdBalance() const { return usdBalance; }

    [[nodiscard]] my_long GetRubBalance() const { return rubBalance; }

public: // Methods
    void AddUsd(my_long volume) { usdBalance += volume; }

    void AddRub(my_long volume) { rubBalance += volume; }

    bool IsPasswordCorrect(const std::string &pass) {
        return pass == password;
    }

private: // Fields
    std::size_t uid;
    std::string login;
    std::string password;
    my_long usdBalance, rubBalance;
};

class UserTable {
public: // Methods
    std::size_t AddUser(const std::string& login, const std::string& password);

    bool IsUserInTable(const std::string &);

    bool IsUserInTable(std::size_t uid);

    bool IsUserDataCorrect(const std::string& login, const std::string& password);

    bool AddUsdToUser(std::size_t uid, my_long volume);

    bool AddRubToUser(std::size_t uid, my_long volume);

public: // Getter
    std::size_t GetUid(const std::string& login);

    my_long GetUserUsdBalance(std::size_t uid);

    my_long GetUserRubBalance(std::size_t uid);

private: // Fields
    [[maybe_unused]]std::size_t maxUid = 1;
    std::unordered_map<std::size_t, User> users;
    std::unordered_map<std::string, std::size_t> logins;
};