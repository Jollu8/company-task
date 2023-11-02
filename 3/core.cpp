//
// Created by Jollu Emil on 10/27/23.
//

#include "json.hpp"
#include "Common.h"
#include "core.h"
#include "User.h"

#include <memory>
#include <set>
#include <string>

using namespace nlohmann;

std::string Core::RegisterNewUser(const std::string &login, const std::string &pass) {
    json msg;
    if (userTable.IsUserInTable(login))msg["err"] = Errors::LoginExists;
    else {
        msg["err"] = Errors::NoError;
        msg["uid"] = std::to_string(userTable.AddUser(login, pass));
    }
    return msg.dump();
}

std::string Core::TryLogin(const std::string &login, const std::string &pass) {
    json msg;
    if (!userTable.IsUserInTable(login))msg["err"] = Errors::LoginDoesntExist;
    else if (!userTable.IsUserDataCorrect(login, pass)) msg["err"] = Errors::WrongPass;
    else {
        msg["err"] = Errors::NoError;
        msg["uid"] = std::to_string(userTable.GetUid(login));
    }
    return msg.dump();
}

std::string Core::TryBuy(std::size_t uid, my_long volume, my_long cost) {
    BuyDeal myDeal(uid, volume, cost);
    while (!myDeal.GetIsClosed() && !activeSells.empty() && myDeal.IsCompatible(*activeSells.begin())) {
        SellDeal sell = *activeSells.begin();
        activeSells.erase(activeSells.begin());
        my_long dealPrice = getDealPrice(myDeal, sell);
        my_long dealVolume = getDealVolume(myDeal, sell);
        DealPair deals = completeDeals(myDeal, sell);

        if (!userTable.AddUsdToUser(myDeal.GetUid(), dealVolume) ||
            !userTable.AddRubToUser(myDeal.GetUid(), -dealPrice * dealVolume) ||
            !userTable.AddUsdToUser(sell.GetUid(), -dealVolume) ||
            !userTable.AddRubToUser(sell.GetUid(), dealPrice * dealVolume)) {
            return Errors::UserDoesntExist;
        }

        closedDeals[deals.first.GetUid()].emplace_back(new BuyDeal(deals.first));
        closedDeals[deals.second.GetUid()].emplace_back(new SellDeal(deals.second));

        if (!sell.GetIsClosed()) {
            activeSells.insert(sell);
        }
    }

    if (!myDeal.GetIsClosed()) {
        activeBuys.insert(myDeal);
    }

    return Errors::NoError;


}

std::string Core::TrySell(std::size_t uid, my_long volume, my_long cost) {
    SellDeal myDeal(uid, volume, cost);
    while (!myDeal.GetIsClosed() && !activeBuys.empty() && myDeal.IsCompatible(*activeBuys.begin())) {
        BuyDeal buy = *activeBuys.begin();
        activeBuys.erase(activeBuys.begin());
        my_long dealPrice = getDealPrice(buy, myDeal);
        my_long dealVolume = getDealVolume(buy, myDeal);
        DealPair deals = completeDeals(buy, myDeal);

        if (!userTable.AddUsdToUser(myDeal.GetUid(), -dealVolume) ||
            !userTable.AddRubToUser(myDeal.GetUid(), dealPrice * dealVolume) ||
            !userTable.AddUsdToUser(buy.GetUid(), dealVolume) ||
            !userTable.AddRubToUser(buy.GetUid(), -dealPrice * dealVolume)) {
            return Errors::UserDoesntExist;
        }

        closedDeals[deals.first.GetUid()].emplace_back(new BuyDeal(deals.first));
        closedDeals[deals.second.GetUid()].emplace_back(new SellDeal(deals.second));

        if (!buy.GetIsClosed()) {
            activeBuys.insert(buy);
        }
    }

    if (!myDeal.GetIsClosed()) {
        activeSells.insert(myDeal);
    }

    return Errors::NoError;

}

std::string Core::GetUserBalance(std::size_t uid) {
    json msg;
    if (userTable.IsUserInTable(uid)) {
        msg["err"] = Errors::NoError;
        msg["rub"] = userTable.GetUserRubBalance(uid);
        msg["usd"] = userTable.GetUserUsdBalance(uid);

    } else msg["err"] = Errors::UserDoesntExist;
    return msg.dump();
}

std::string Core::GetUserActiveDeals(std::size_t uid) {
    json msg;
    if (userTable.IsUserInTable(uid)) {
        msg["err"] = Errors::NoError;
        for (auto &deal: activeBuys)if (deal.GetUid() == uid)msg["deals"].emplace_back(deal.GetInfo());
        for (auto &deal: activeSells) if (deal.GetUid() == uid) msg["deals"].emplace_back(deal.GetInfo());

    } else msg["err"] = Errors::UserDoesntExist;
    return msg.dump();
}

std::string Core::GetUserClosedDeals(std::size_t uid) {
    json msg;
    if (userTable.IsUserInTable(uid)) {
        msg["err"] = Errors::NoError;
        for (auto &dealPtr: closedDeals[uid])msg["deals"].emplace_back(dealPtr->GetInfo());
    } else msg["err"] = Errors::UserDoesntExist;
    return msg.dump();
}

std::string Core::GetBestPrices() {
    json msg;
    if (!activeBuys.empty())msg["buy"] = std::to_string(activeBuys.begin()->GetPrice());
    else msg["buy"] = Fillers::FAKE_NUMBER;

    if (!activeSells.empty())msg["sell"] = std::to_string(activeSells.begin()->GetPrice());
    else msg["sell"] = Fillers::FAKE_NUMBER;
    return msg.dump();
}

Core &GetCore() {
    static Core core;
    return core;
}

