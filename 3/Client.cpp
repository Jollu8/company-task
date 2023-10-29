//
// Created by Jollu Emil on 10/27/23.
//

#include "myTypes.h"
#include "Common.h"
#include "json.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <boost/asio.hpp>

using boost_tcp = boost::asio::ip::tcp;


// Отправка сообщения на сервер по шаблону.

void SendMessage(
        boost_tcp::socket &aSocket,
        const std::string &aRequestType,
        const std::string &aMessage
) {
    using  nlohmann::json;
    json req;
    req["ReqType"] = aRequestType;
    req["Message"] = aMessage;


    std::string request = req.dump();
    boost::asio::write(aSocket, boost::asio::buffer(request, request.size()));

}

// Возвращает строку с ответом сервера на последний запрос.

std::string ReadMessage(boost_tcp::socket &aSocket) {
    boost::asio::streambuf b;
    boost::asio::read_until(aSocket, b, "\0");
    std::istream is(&b);
    std::string line(std::istreambuf_iterator<char>(is), {});
    return line;
}


std::string RegistrationProcess(boost_tcp::socket &aSocket) {
    using namespace std::string_literals;
    using namespace nlohmann;

    std::string name, pass, pass2;
    std::cout << "Enter your login: ";
    std::cin >> name;
    while (true) {
        std::cout << "Enter your password: "s;
        std::cin >> pass;
        std::cout << "Enter your password again: "s;
        std::cin >> pass2;

        if (pass == pass2) break;
        std::cout << "Sorry, password do not match. Try again! "s << std::endl;
    }

    json msg;
    msg["login"] = name;
    msg["password"] = pass;
    SendMessage(aSocket, Requests::Reg, msg.dump());

    auto reply = json::parse(ReadMessage(aSocket));
    if (reply["err"] == Errors::NoError) {
        std::cout << "Registration successfully complete, "s << name << '\n';
        return reply["uid"];
    } else if (reply["err"] == Errors::LoginExists)
        std::cout << "Sorry, user with this login already exists. Log-in or register with another login\n"s;
    else
        std::cout << "Something strange have occurred. Contact admin. Error: REG_ERROR\n"s;
    return Fillers::FAKE_ID;

}

std::string LoginProcess(boost_tcp::socket &aSocket) {
    using namespace std::string_literals;
    using namespace nlohmann;

    std::string name, pass;
    std::cout << "Enter your login: "s;
    std::cin >> name;
    std::cout << "Enter your password: "s;
    std::cin >> pass;

    json msg;
    msg["login"] = name;
    msg["password"] = pass;

    SendMessage(aSocket, Requests::Login, msg.dump());

    auto reply = json::parse(ReadMessage(aSocket));
    if (reply["err"] == Errors::NoError) {
        std::cout << "Login successfully complete, "s << name << '\n';
        return reply["uid"];
    } else if (reply["err"] == Errors::WrongPass)
        std::cout << "Wrong password. Register new account or try another password\n"s;
    else if (reply["err"] == Errors::LoginDoesntExist)
        std::cout << "Login doen`t exist. Register new or try another  login\n"s;
    else
        std::cout << "Something strange have occurred. Contact admin. Error: LOGIN_ERROR\n"s;
    return Fillers::FAKE_ID;
}

void MakeDealProcess(boost_tcp::socket &aSocket, const std::string_view my_id, bool isBuy) {
    using namespace std::string_literals;
    using nlohmann::json;

    my_long volume, cost;
    if (isBuy)std::cout << "Creating buy request...\n"s;
    else
        std::cout << "Creating sell request...\n";

    std::cout << "Enter USD volume: "s;
    std::cin >> volume;
    std::cout << "Enter cost: "s;
    std::cin >> cost;

    json msg;
    msg["uid"] = my_id;
    msg["vol"] = volume;
    msg["cost"] = cost;
    SendMessage(aSocket, (isBuy ? Requests::Buy : Requests::Sell), msg.dump());

    auto reply = ReadMessage(aSocket);
    if (reply == Errors::NoError)
        std::cout << "Deal request successfully processed\n"s;
    else
        std::cout << "Something strange have occurred. Contact admin. Error: BUY_ERROR\n"s;
}


void CheckBalance(boost_tcp::socket &aSocket, const std::string_view my_id) {
    using namespace std::string_literals;
    using nlohmann::json;
    json msg;
    msg["uid"] = my_id;
    SendMessage(aSocket, Requests::Balance, msg.dump());
    auto reply = json::parse(ReadMessage(aSocket));
    if (reply["err"] == Errors::NoError)
        std::cout << "Your balance:\n"s << reply["usd"] << " USD\n"s << reply["rub"] << " RUB\n"s;
    else
        std::cout << "Something strange have occurred. Contact admin. Error: BALANCE_ERROR\n"s;

}

void PrintDeal(const std::string &jsonStr) {
    using nlohmann::json;
    using namespace std::string_literals;

    auto info = json::parse(jsonStr);
    if (info[DealInfo::KeyType] == DealInfo::TypeBuy)std::cout << "Buy "s;
    else std::cout << "Sell "s;
    std::cout << info[DealInfo::KeyVolume] << "USD for " << info[DealInfo::KeyPrice] << "RUB each\n";
}

void PrintDeals(boost_tcp::socket &aSocket, const std::string_view my_id, bool is_active) {
    using nlohmann::json;
    using namespace std::string_literals;

    json msg;
    msg["uid"] = my_id;
    SendMessage(aSocket, (is_active ? Requests::ActiveDeals : Requests::ClosedDeals), msg.dump());
    auto reply = json::parse(ReadMessage(aSocket));
    if (reply["err"] == Errors::NoError) {
        if (reply["deals"].empty())
            std::cout << "You have no " << (is_active ? "active"s : "closed"s) << " deals now" << std::endl;
        else {
            std::cout << (is_active ? "Your active deals: \n"s : "Your closed deals\n"s);
            int i = 1;
            for (auto &str: reply["deals"]) {
                std::cout << i++ << ") ";
                PrintDeal(str);
            }
        }
    } else
        std::cout << "Something strange have occurred. Contact admin. Error PRINTDEALS_ERROR" << std::endl;
}

void GetBestPrices(boost_tcp::socket &aSocket) {
    using nlohmann::json;
    using namespace std::string_literals;

    SendMessage(aSocket, Requests::BestPrices, "{}"s);
    auto reply = json::parse(ReadMessage(aSocket));
    std::cout << "BEst current prices"s << std::endl;

    if (reply["buy"] == Fillers::FAKE_NUMBER)
        std::cout << "There`s no active buy request"s << std::endl;
    else
        std::cout << "Best buy price is "s << std::stoll(static_cast<std::string>(reply["buy"])) << " RUB for 1 USD"s
                  << std::endl;


    if (reply["sell"] == Fillers::FAKE_NUMBER)
        std::cout << "There`s no active sell request"s << std::endl;
    else
        std::cout << "Best sell price is "s << std::stoll(static_cast<std::string>(reply["sell"])) << " RUB for 1 USD"s
                  << std::endl;

}

int main() {
    using namespace std::string_literals;

    try {
        boost::asio::io_service io_service;

        boost_tcp::resolver resolver(io_service);
        boost_tcp::resolver::query query(boost_tcp::v4(), "127.0.0.1", std::to_string(port));
        boost_tcp::resolver::iterator iterator = resolver.resolve(query);

        boost_tcp::socket s(io_service);
        s.connect(*iterator);
        std::string my_id = "fake_id";
        while (my_id == Fillers::FAKE_ID) {
            std::cout << "Greetings! Please, register or login\n"s
                    "1) Register new account\n"s
                            "2) Login to existing account\n"s
                      << std::endl;

            short menu_option;
            std::cin >> menu_option;
            switch (menu_option) {
                case 1: {
                    my_id = RegistrationProcess(s);
                    break;
                }
                case 2: {
                    my_id = LoginProcess(s);
                    break;
                }
                default:
                    std::cout << "Unknown menu option"s << std::endl;
            }

            std::cout << "\n----------------------------------------------"s << std::endl;

        }
        while (true) {
            std::cout << "Please, choose menu option:\n"
                         "1) Buy USD\n"
                         "2) Sell USD\n"
                         "3) Check balance\n"
                         "4) Check my active deals\n"
                         "5) Check my closed deals\n"
                         "6) Check best prices\n"
                         "0) Exit\n"
                      << std::endl;
            short menu_option;
            std::cin >> menu_option;

            switch (menu_option) {
                case 1: // Buy
                {
                    MakeDealProcess(s, my_id, /*isBuy = */true);
                    break;
                }
                case 2: // Sell
                {
                    MakeDealProcess(s, my_id, /*isBuy = */false);
                    break;
                }
                case 3: // Check balance
                    CheckBalance(s, my_id);
                    break;
                case 4: // Check active deals
                    PrintDeals(s, my_id, true);
                    break;
                case 5: // Check closed deals
                    PrintDeals(s, my_id, false);
                    break;
                case 6: // Check best price
                    GetBestPrices(s);
                    break;
                case 0: // Exit
                    exit(0);
                default:
                    std::cout << "Unknown menu option" << std::endl;
            }
        }
        std::cout << "\n---------------------------------------------------------"s << std::endl;

    } catch (std::exception &e) {
        std::cerr << "Server does not run or:\n";
        std::cerr  << e.what() << std::endl;
    }
}