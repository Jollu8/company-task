//
// Created by Jollu Emil on 10/27/23.
//

#include "Common.h"
#include "core.h"
#include "deal.h"
#include "User.h"

#include "json.hpp"

#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <set>
#include <string>


#define NDB

using boost_tcp = boost::asio::ip::tcp;
using boost_io_service = boost::asio::io_service;
using boost_error_code = boost::system::error_code;


class session {
private:
    boost_tcp::socket socket_;
    enum {
        max_length = 1024
    };
    chain data_[max_length]{};
public: // Constructor
    explicit session(boost_io_service &io_service) : socket_(io_service) {}

public: // Methods
    boost_tcp::socket &socket() { return socket_; }

    void start() {
#ifdef NDB
        std::cout << "Call session start" << std::endl;
#endif
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                boost::bind(&session::handle_read, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    }

    void handle_read(const boost_error_code &error, std::size_t bytes_transferred) {
#ifdef NDB
        std::cout << "Call session handle_read" << std::endl;
#endif

        using namespace nlohmann;
        if (!error) {
            data_[bytes_transferred] = '\0';

            // Парсим json, который пришёл нам в сообщении.
            auto j = json::parse(data_);
            auto reqType = j["ReqType"];

            std::string reply = "Error! Unknown request type";

            auto msg = json::parse(static_cast<std::string>(j["Message"]));

            if (reqType == Requests::Reg)reply = GetCore().RegisterNewUser(msg["login"], msg["password"]);
            else if (reqType == Requests::Login) reply = GetCore().TryLogin(msg["login"], msg["password"]);
            else if (reqType == Requests::Buy) {
                std::string uid = msg["uid"];
                reply = GetCore().TryBuy(std::stoi(uid), msg["vol"], msg["cost"]);
            } else if (reqType == Requests::Sell) {
                std::string uid = msg["uid"];
                reply = GetCore().TrySell(std::stoi(uid), msg["vol"], msg["cost"]);
            } else if (reqType == Requests::Balance) {
                std::string uid = msg["uid"];

                reply = GetCore().GetUserBalance(std::stoi(uid));
            } else if (reqType == Requests::ActiveDeals) {
                std::string uid = msg["uid"];
                reply = GetCore().GetUserActiveDeals(std::stoi(uid));
            } else if (reqType == Requests::ClosedDeals) {
                std::string uid = msg["uid"];
                reply = GetCore().GetUserClosedDeals(std::stoi(uid));
            } else if (reqType == Requests::BestPrices)
                reply = GetCore().GetBestPrices();

            boost::asio::async_write(socket_, boost::asio::buffer(reply, reply.size()),
                                     boost::bind(&session::handle_write, this,
                                                 boost::asio::placeholders::error));
        } else delete this;
    }

    void handle_write(const boost_error_code &error) {
#ifdef NDB
        std::cout << "Call session handle_write" << std::endl;
#endif
        if (!error) {
            socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                    boost::bind(&session::handle_read, this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        } else delete this;
    }
};


using boost::asio::ip::tcp;

class server {
public:
    explicit server(boost_io_service &io_service)
            : io_service_(io_service),
              acceptor_(io_service, boost_tcp::endpoint(tcp::v4(), port)) {
        std::cout << "Sever started! Listen  " << port << std::endl;

        auto *new_session = new session(io_service_);
        acceptor_.async_accept(new_session->socket(),
                               boost::bind(&server::handle_accept, this, new_session,
                                           boost::asio::placeholders::error));
    }

    void handle_accept(session *new_session,
                       const boost_error_code &error) {
#ifdef NDB
        std::cout << "Server call handle_accept" << std::endl;
#endif
        if (!error) {
            new_session->start();
            new_session = new session(io_service_);
            acceptor_.async_accept(new_session->socket(),
                                   boost::bind(&server::handle_accept, this, new_session,
                                               boost::asio::placeholders::error));
        } else {
            delete new_session;
        }
    }

private:
    boost_io_service &io_service_;
    boost_tcp::acceptor acceptor_;
};

int main() {
    try {
        boost_io_service io_service;
        static Core core;
        server s(io_service);
        io_service.run();
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}