//
// Created by Jollu Emil on 10/27/23.
//
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "json.hpp"
#include "doctest.h"


#include "Common.h"
#include "core.h"
#include "deal.h"
#include "User.h"

#include <cassert>
#include <chrono>
#include <thread>

TEST_CASE("Test User methods") {
    std::size_t uid = 42;
    std::string login = "my_login";
    std::string password = "qwerty", another_password = "123456";
    User user(uid, login, password);
    SUBCASE("Test user getters") {
        CHECK(user.GetUid() == uid);
        CHECK(user.GetLogin() == login);
        CHECK(user.GetRubBalance() == 0);
        CHECK(user.GetUsdBalance() == 0);
    }
    SUBCASE("Test user password") {
        CHECK(user.IsPasswordCorrect(password));
        CHECK_FALSE(user.IsPasswordCorrect(another_password));
    }

    SUBCASE("Test money addition") {
        CHECK(user.GetUsdBalance() == 0);
        CHECK(user.GetRubBalance() == 0);

    }

    SUBCASE("Test default constructor") {
        User default_user;
        CHECK(default_user.GetUid() == 0);
        CHECK(default_user.GetLogin() == "");
        CHECK(default_user.IsPasswordCorrect(""));
        CHECK(default_user.GetRubBalance() == 0);
        CHECK(default_user.GetUsdBalance() == 0);
    }
}


TEST_CASE("Test UserTable methods") {
    std::string login = "my_login", another_login = "foo";
    std::string password = "qwerty", another_password = "12345";
    UserTable userTable;

    SUBCASE("Test user addition") {
        CHECK_FALSE(userTable.IsUserInTable(1));
        CHECK_FALSE(userTable.IsUserInTable(login));

        CHECK(userTable.AddUser(login, password) == 1);
        CHECK(userTable.AddUser(login, another_password) == 0);

        CHECK(userTable.IsUserInTable(1));
        CHECK(userTable.IsUserInTable(login));

        CHECK(userTable.AddUser(another_login, another_password) == 2);
    }

    SUBCASE("Test user data checking") {
        REQUIRE(userTable.AddUser(login, password) == 1);
        CHECK(userTable.IsUserDataCorrect(login, password));
        CHECK_FALSE(userTable.IsUserDataCorrect(another_login, password));
        CHECK_FALSE(userTable.IsUserDataCorrect(login, another_password));
        CHECK_FALSE(userTable.IsUserDataCorrect(another_login, another_password));

        REQUIRE(userTable.AddUser(another_login, another_password) == 2);
        CHECK(userTable.IsUserDataCorrect(another_login, another_password));
    }

    SUBCASE("Test getUid") {
        REQUIRE(userTable.AddUser(login, password) == 1);
        CHECK(userTable.GetUid(login) == 1);
        CHECK(userTable.GetUid(another_login) == 0);

        REQUIRE(userTable.AddUser(another_login, another_password) == 2);
        CHECK(userTable.GetUid(login) == 1);
        CHECK(userTable.GetUid(another_login) == 2);
    }

    SUBCASE("Test money addition") {
        REQUIRE(userTable.AddUser(login, password) == 1);
        CHECK(userTable.GetUserRubBalance(1) == 0);
        CHECK(userTable.GetUserUsdBalance(1) == 0);

        CHECK(userTable.AddRubToUser(1, 42));
        CHECK(userTable.AddUsdToUser(1, -37));
        CHECK(userTable.GetUserRubBalance(1) == 42);
        CHECK(userTable.GetUserUsdBalance(1) == -37);

        CHECK_FALSE(userTable.AddRubToUser(2, 1));
        CHECK_FALSE(userTable.AddUsdToUser(2, 1));
        CHECK_THROWS(userTable.GetUserRubBalance(2));
    }
}


TEST_CASE("Test Deal methods") {
    size_t uid = 42;
    long long price = 37, volume = 13;
    Time time = std::chrono::system_clock::now();
    Deal deal(uid, volume, price, time);

    SUBCASE("Test Deal getters") {
        CHECK(deal.GetUid() == uid);
        CHECK(deal.GetPrice() == price);
        CHECK(deal.GetVolume() == volume);
        CHECK(deal.GetTime() == time);
        CHECK(deal.GetInfo() == Fillers::FAKE_INFO);
        CHECK_FALSE(deal.GetIsClosed());
    }

    SUBCASE("Test Deal close") {
        CHECK_FALSE(deal.GetIsClosed());
        deal.close();
        CHECK(deal.GetIsClosed());
    }

    SUBCASE("Test Deal subVolume") {
        CHECK(deal.GetVolume() == volume);
        deal.SubVolume(2);
        CHECK(deal.GetVolume() == volume - 2);
    }
}

TEST_CASE("Test BuyDeal and SellDeal methods") {
    size_t uid = 42;
    long long price = 37, volume = 13;
    Time time = std::chrono::system_clock::now();

    BuyDeal buy(uid, volume, price, time), buyH(uid, volume, price + 1, time), buyL(uid, volume, price - 1, time);
    SellDeal sell(uid, volume, price, time), sellH(uid, volume, price + 1, time), sellL(uid, volume, price - 1, time);

    SUBCASE("Test BuyDeal getters") {
        CHECK(buy.GetUid() == uid);
        CHECK(buy.GetPrice() == price);
        CHECK(buy.GetVolume() == volume);
        CHECK(buy.GetTime() == time);
        CHECK_FALSE(buy.GetIsClosed());

        nlohmann::json info;
        info[DealInfo::KeyType] = DealInfo::TypeBuy;
        info[DealInfo::KeyVolume] = volume;
        info[DealInfo::KeyPrice] = price;
        CHECK(buy.GetInfo() == info.dump());
    }

    SUBCASE("Test SellDeal getters") {
        CHECK(sell.GetUid() == uid);
        CHECK(sell.GetPrice() == price);
        CHECK(sell.GetVolume() == volume);
        CHECK(sell.GetTime() == time);
        CHECK_FALSE(sell.GetIsClosed());

        nlohmann::json info;
        info[DealInfo::KeyType] = DealInfo::TypeSell;
        info[DealInfo::KeyVolume] = volume;
        info[DealInfo::KeyPrice] = price;
        CHECK(sell.GetInfo() == info.dump());
    }

    SUBCASE("Test compatibility") {
        CHECK(buy.IsCompatible(sell));
        CHECK(sell.IsCompatible(buy));

        CHECK(buy.IsCompatible(sellL));
        CHECK_FALSE(buy.IsCompatible(sellH));

        CHECK(sell.IsCompatible(buyH));
        CHECK_FALSE(sell.IsCompatible(buyL));
    }

    SUBCASE("Test operator <") {
        CHECK(buyH < buy);
        CHECK(buy < buyL);

        CHECK(sell < sellH);
        CHECK(sellL < sell);
    }
}


TEST_CASE("Test Deal functions") {
    size_t uid = 42;
    long long price = 37, volume = 13;
    Time time = std::chrono::system_clock::now();

    BuyDeal buy(uid, volume, price, time);
    SellDeal sell(uid, volume, price, time);

    SUBCASE("Test getDealVolume") {
        CHECK(getDealVolume(buy, sell) == volume);

        BuyDeal another_buy(uid, volume - 1, price, time);
        SellDeal another_sell(uid, volume - 1, price, time);

        CHECK(getDealVolume(buy, another_sell) == volume - 1);
        CHECK(getDealVolume(another_buy, sell) == volume - 1);
    }

    SUBCASE("Test getDealPrice") {
        CHECK(getDealPrice(buy, sell) == price);

        time -= std::chrono::system_clock::duration(1);
        BuyDeal another_buy(uid, volume, price - 5, time);
        SellDeal another_sell(uid, volume, price + 5, time);

        CHECK(getDealPrice(another_buy, sell) == price - 5);
        CHECK(getDealPrice(buy, another_sell) == price + 5);
    }

    SUBCASE("Test completeDeals") {
        SUBCASE("Test full completion") {
            long long dealVolume = getDealVolume(buy, sell);
            DealPair deals = completeDeals(buy, sell);

            CHECK(buy.GetIsClosed());
            CHECK(buy.GetVolume() == dealVolume);
            CHECK(deals.first == buy);

            CHECK(sell.GetIsClosed());
            CHECK(sell.GetVolume() == dealVolume);
            CHECK(deals.second == sell);
        }

        SUBCASE("Test buy completion") {
            SellDeal another_sell(uid, volume + 10, price, time);
            long long dealVolume = getDealVolume(buy, another_sell);
            DealPair deals = completeDeals(buy, another_sell);

            CHECK(buy.GetIsClosed());
            CHECK(buy.GetVolume() == dealVolume);
            CHECK(deals.first == buy);

            CHECK_FALSE(deals.second == another_sell);
            CHECK(deals.second.GetIsClosed());
            CHECK_FALSE(another_sell.GetIsClosed());
            CHECK(deals.second.GetVolume() == dealVolume);
            CHECK(another_sell.GetVolume() == 10);
        }

        SUBCASE("Test sell completion") {
            BuyDeal another_buy(uid, volume + 10, price, time);
            long long dealVolume = getDealVolume(another_buy, sell);
            DealPair deals = completeDeals(another_buy, sell);

            CHECK_FALSE(deals.first == another_buy);
            CHECK(deals.first.GetIsClosed());
            CHECK_FALSE(another_buy.GetIsClosed());
            CHECK(deals.first.GetVolume() == dealVolume);
            CHECK(another_buy.GetVolume() == 10);

            CHECK(sell.GetIsClosed());
            CHECK(sell.GetVolume() == dealVolume);
            CHECK(deals.second == sell);
        }
    }
}

TEST_CASE("Test registration and login") {
    Core core;
    std::string login = "Gandarf", password = "qwerty";
    std::string another_login = "login", another_password = "123456";

    SUBCASE("Test multiple registrations") {
        auto msg = nlohmann::json::parse(core.RegisterNewUser(login, password));
        CHECK(msg["err"] == Errors::NoError);
        CHECK(msg["uid"] == "1");

        msg = nlohmann::json::parse(core.RegisterNewUser(login, password));
        CHECK(msg["err"] == Errors::LoginExists);

        msg = nlohmann::json::parse(core.RegisterNewUser(login, another_password));
        CHECK(msg["err"] == Errors::LoginExists);

        msg = nlohmann::json::parse(core.RegisterNewUser(another_login, password));
        CHECK(msg["err"] == Errors::NoError);
        CHECK(msg["uid"] == "2");
    }

    SUBCASE("Test registration and login") {
        auto msg = nlohmann::json::parse(core.RegisterNewUser(login, password));
        CHECK(msg["err"] == Errors::NoError);
        CHECK(msg["uid"] == "1");

        msg = nlohmann::json::parse(core.TryLogin(another_login, password));
        CHECK(msg["err"] == Errors::LoginDoesntExist);

        msg = nlohmann::json::parse(core.TryLogin(login, another_password));
        CHECK(msg["err"] == Errors::WrongPass);

        msg = nlohmann::json::parse(core.TryLogin(login, password));
        CHECK(msg["err"] == Errors::NoError);
        CHECK(msg["uid"] == "1");
    }
}

TEST_CASE("Test buy/sell") {
    Core core;
    core.RegisterNewUser("login1", "pass1");
    core.RegisterNewUser("login2", "pass2");

    SUBCASE("Test casual GetUserBalance") {
        auto msg = nlohmann::json::parse(core.GetUserBalance(42));
        CHECK(msg["err"] == Errors::UserDoesntExist);

        msg = nlohmann::json::parse(core.GetUserBalance(1));
        CHECK(msg["err"] == Errors::NoError);
        CHECK(msg["rub"] == 0);
        CHECK(msg["usd"] == 0);
    }

    SUBCASE("Test sell/buy") {
        core.TryBuy(1, 10, 10);
        core.TrySell(2, 10, 10);

        auto msg = nlohmann::json::parse(core.GetUserBalance(1));
        CHECK(msg["err"] == Errors::NoError);
        CHECK(msg["rub"] == -100);
        CHECK(msg["usd"] == 10);

        msg = nlohmann::json::parse(core.GetUserBalance(2));
        CHECK(msg["err"] == Errors::NoError);
        CHECK(msg["rub"] == 100);
        CHECK(msg["usd"] == -10);
    }

    SUBCASE("Test sell/buy with different prices") {
        SUBCASE("Buy first") {
            core.TryBuy(1, 10, 10);
            // Price between two deals is dependent on deal time, so sleep a bit for confidence
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            core.TrySell(2, 10, 9);

            auto msg = nlohmann::json::parse(core.GetUserBalance(1));
            CHECK(msg["err"] == Errors::NoError);
            CHECK(msg["rub"] == -100);
            CHECK(msg["usd"] == 10);

            msg = nlohmann::json::parse(core.GetUserBalance(2));
            CHECK(msg["err"] == Errors::NoError);
            CHECK(msg["rub"] == 100);
            CHECK(msg["usd"] == -10);
        }

        SUBCASE("Sell first") {
            core.TrySell(2, 10, 9);
            // Price between two deals is dependent on deal time, so sleep a bit for confidence
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            core.TryBuy(1, 10, 10);

            auto msg = nlohmann::json::parse(core.GetUserBalance(1));
            CHECK(msg["err"] == Errors::NoError);
            CHECK(msg["rub"] == -90);
            CHECK(msg["usd"] == 10);

            msg = nlohmann::json::parse(core.GetUserBalance(2));
            CHECK(msg["err"] == Errors::NoError);
            CHECK(msg["rub"] == 90);
            CHECK(msg["usd"] == -10);
        }
    }

    SUBCASE("Test partial sell/buy") {
        core.TryBuy(1, 10, 10);
        core.TrySell(2, 5, 10);

        auto msg = nlohmann::json::parse(core.GetUserBalance(1));
        CHECK(msg["err"] == Errors::NoError);
        CHECK(msg["rub"] == -50);
        CHECK(msg["usd"] == 5);

        msg = nlohmann::json::parse(core.GetUserBalance(2));
        CHECK(msg["err"] == Errors::NoError);
        CHECK(msg["rub"] == 50);
        CHECK(msg["usd"] == -5);
    }
}

TEST_CASE("Test deals info") {
    Core core;
    core.RegisterNewUser("login1", "pass1");
    core.RegisterNewUser("login2", "pass2");

    SUBCASE("Test ActiveDeals info") {
        auto msg = nlohmann::json::parse(core.GetUserActiveDeals(42));
        CHECK(msg["err"] == Errors::UserDoesntExist);

        msg = nlohmann::json::parse(core.GetUserActiveDeals(1));
        CHECK(msg["err"] == Errors::NoError);
        CHECK(msg["deals"].size() == 0);

        BuyDeal buyDeal(1, 10, 10);
        core.TryBuy(1, 10, 10);
        msg = nlohmann::json::parse(core.GetUserActiveDeals(1));
        CHECK(msg["err"] == Errors::NoError);
        CHECK(msg["deals"].size() == 1);
        CHECK(msg["deals"][0] == buyDeal.GetInfo());

        SellDeal sellDeal(1, 10, 11);
        core.TrySell(1, 10, 11);
        msg = nlohmann::json::parse(core.GetUserActiveDeals(1));
        CHECK(msg["err"] == Errors::NoError);
        CHECK(msg["deals"].size() == 2);
        CHECK(msg["deals"][0] == buyDeal.GetInfo());
        CHECK(msg["deals"][1] == sellDeal.GetInfo());
    }

    SUBCASE("Test ClosedDeals info") {
        auto msg = nlohmann::json::parse(core.GetUserClosedDeals(42));
        CHECK(msg["err"] == Errors::UserDoesntExist);

        msg = nlohmann::json::parse(core.GetUserClosedDeals(1));
        CHECK(msg["err"] == Errors::NoError);
        CHECK(msg["deals"].size() == 0);

        BuyDeal buyDeal(1, 10, 10);
        core.TryBuy(1, 10, 10);
        SellDeal sellDeal(2, 10, 10);
        core.TrySell(2, 10, 10);

        msg = nlohmann::json::parse(core.GetUserClosedDeals(1));
        CHECK(msg["err"] == Errors::NoError);
        CHECK(msg["deals"].size() == 1);
        CHECK(msg["deals"][0] == buyDeal.GetInfo());

        msg = nlohmann::json::parse(core.GetUserClosedDeals(2));
        CHECK(msg["err"] == Errors::NoError);
        CHECK(msg["deals"].size() == 1);
        CHECK(msg["deals"][0] == sellDeal.GetInfo());
    }

    SUBCASE("Test deals info with partial deal") {
        BuyDeal buyDeal(1, 10, 10);
        core.TryBuy(1, 10, 10);
        SellDeal sellDeal(2, 4, 10);
        core.TrySell(2, 4, 10);

        BuyDeal closedBuyDeal(1, 4, 10);
        auto msg = nlohmann::json::parse(core.GetUserClosedDeals(1));
        CHECK(msg["err"] == Errors::NoError);
        CHECK(msg["deals"].size() == 1);
        CHECK(msg["deals"][0] == closedBuyDeal.GetInfo());

        msg = nlohmann::json::parse(core.GetUserClosedDeals(2));
        CHECK(msg["err"] == Errors::NoError);
        CHECK(msg["deals"].size() == 1);
        CHECK(msg["deals"][0] == sellDeal.GetInfo());

        BuyDeal remainder(1, 6, 10);
        msg = nlohmann::json::parse(core.GetUserActiveDeals(1));
        CHECK(msg["err"] == Errors::NoError);
        CHECK(msg["deals"].size() == 1);
        CHECK(msg["deals"][0] == remainder.GetInfo());
    }
}

TEST_CASE("Test GetBestPrices") {
    Core core;
    core.RegisterNewUser("login1", "pass1");
    core.RegisterNewUser("login2", "pass2");

    auto msg = nlohmann::json::parse(core.GetBestPrices());
    CHECK(msg["buy"] == Fillers::FAKE_NUMBER);
    CHECK(msg["sell"] == Fillers::FAKE_NUMBER);

    core.TryBuy(1, 10, 10);
    msg = nlohmann::json::parse(core.GetBestPrices());
    CHECK(msg["buy"] == "10");
    CHECK(msg["sell"] == Fillers::FAKE_NUMBER);

    core.TrySell(2, 10, 11);
    msg = nlohmann::json::parse(core.GetBestPrices());
    CHECK(msg["buy"] == "10");
    CHECK(msg["sell"] == "11");

    core.TrySell(2, 10, 10);
    msg = nlohmann::json::parse(core.GetBestPrices());
    CHECK(msg["buy"] == Fillers::FAKE_NUMBER);
    CHECK(msg["sell"] == "11");
}