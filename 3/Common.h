//
// Created by Jollu Emil on 10/27/23.
//

#pragma once

#include <string>

static short port = 5555;
namespace Requests {
    using namespace std::string_literals;
    static std::string Reg = "res"s;
    static std::string Login = "log"s;
    static std::string Buy = "buy"s;
    static std::string Sell = "sell"s;
    static std::string Balance = "bal"s;
    static std::string ActiveDeals = "ad";
    static std::string ClosedDeals = "cd";
    static std::string BestPrices = "bp";
}

namespace Errors {
    using namespace std::string_literals;
    static std::string NoError = "ok"s;
    static std::string LoginExists = "loginexists"s;
    static std::string LoginDoesntExist = "trevor"s;
    static std::string UserDoesntExist = "404"s;
    static std::string WrongPass = "wrongpass"s;
}

namespace DealInfo {
    using namespace std::string_literals;
    static std::string KeyType = "t"s;
    static std::string KeyVolume = "v"s;
    static std::string KeyPrice = "p"s;
    static std::string TypeBuy = "b"s;
    static std::string TypeSell = "s"s;
}

namespace Fillers {
    using namespace std::string_literals;
    static std::string FAKE_ID = "fake_id"s;
    static std::string FAKE_INFO = "fake_info"s;
    static std::string FAKE_NUMBER = "fake_number"s;

}



