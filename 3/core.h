//
// Created by Jollu Emil on 10/27/23.
//
#pragma once

#include "deal.h"
#include "User.h"
#include "myTypes.h"

#include <memory>
#include <set>
#include <string>


class Core {
public: // Methods

    std::string RegisterNewUser(const std::string &login, const std::string &pass);

    std::string TryLogin(const std::string &login, const std::string &pass);

    std::string TryBuy(std::size_t uid, my_long volume, my_long cost);

    std::string TrySell(std::size_t uid, my_long volume, my_long cost);

public: // Getter

    std::string GetUserBalance(std::size_t uid);

    std::string GetUserActiveDeals(std::size_t uid);

    std::string GetUserClosedDeals(std::size_t uid);

    std::string GetBestPrices();

private:
    UserTable userTable;
    std::multiset<BuyDeal> activeBuys;
    std::multiset<SellDeal> activeSells;
    std::unordered_map<std::size_t, std::vector<std::unique_ptr<Deal>>> closedDeals;

};

Core &GetCore();