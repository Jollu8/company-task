//
// Created by Jollu Emil on 10/27/23.
//

#pragma once

#include "myTypes.h"
#include "json.hpp"
#include "Common.h"
#include <chrono>

using Time = std::chrono::time_point<std::chrono::system_clock>;


class Deal {
public: // Destructor
   virtual ~Deal() = default;

public: // Constructors
    Deal(std::size_t uid_, my_long volume_, my_long price_, Time time_) :
            uid(uid_),
            volume(volume_),
            price(price_),
            dealTime(time_) {
        isClosed = false;
    }

    Deal(std::size_t uid_, my_long volume_, my_long price_) :
            uid(uid_),
            volume(volume_),
            price(price_) {
        this->dealTime = std::chrono::system_clock::now();
        this->isClosed = false;
    }
    Deal& operator=(const Deal &other) {
        if (this != &other) {
            uid = other.uid;
            volume = other.volume;
            price = other.price;
            dealTime = other.dealTime;
            isClosed = other.isClosed;


        }
        return *this;
    }

public: // Getter
    [[nodiscard]] my_long GetPrice() const {
        return price;
    }

    [[nodiscard]] my_long GetVolume() const {
        return volume;
    }

    [[nodiscard]] std::size_t GetUid() const {
        return uid;
    }

    [[nodiscard]] Time GetTime() const {
        return dealTime;
    }

    [[nodiscard]] bool GetIsClosed() const {
        return isClosed;
    }

    [[nodiscard]] virtual std::string GetInfo() const ;

public: // Methods
    void SubVolume(my_long volumeToSub) {
        volume -= volumeToSub;
    }

    void close() {
        isClosed = true;
    }

protected: // Fields
    size_t uid;
    my_long volume;
    my_long price;
    Time dealTime;
    bool isClosed;
};


class SellDeal;

class BuyDeal : public Deal {
public: // Constructors
    BuyDeal(std::size_t uid_, my_long volume_, my_long price_) :
            Deal(uid_, volume_, price_) {

    }

    BuyDeal(std::size_t uid_, my_long volume_, my_long price_, Time time_) :
            Deal(uid_, volume_, price_, time_) {}

    BuyDeal(const BuyDeal& other_) : Deal(other_.GetUid(),
                                          other_.GetVolume(),
                                          other_.GetPrice(),
                                          other_.GetTime()) {}

    BuyDeal& operator=(BuyDeal &&other)  noexcept {
        if (this != &other) {
            uid = other.uid;
            volume = other.volume;
            price = other.price;
            dealTime = other.dealTime;
            isClosed = other.isClosed;


    }
        return *this;
}

public: // Operator
    bool operator==(const BuyDeal &rhs) const;

    bool operator<(const BuyDeal &rhs) const;

public: // Method
    [[nodiscard]] bool IsCompatible(const SellDeal &deal) const;

    [[nodiscard]] std::string GetInfo() const override;

};

class SellDeal : public Deal {
public: // Constructors
    SellDeal(std::size_t uid_, my_long volume_, my_long price_) :
            Deal(uid_, volume_, price_) {}

    SellDeal(std::size_t uid_, my_long volume_, my_long price_, Time time) :
            Deal(uid_, volume_, price_, time) {}

    SellDeal(const SellDeal &other_) :
            Deal(other_.GetUid(),
                 other_.GetVolume(),
                 other_.GetPrice(),
                 other_.GetTime()) {}

    SellDeal& operator=(SellDeal &&other)  noexcept {
        if (this != &other) {
            uid = other.uid;
            volume = other.volume;
            price = other.price;
            dealTime = other.dealTime;
            isClosed = other.isClosed;


        }
        return *this;
    }

public: // Operators
    bool operator==(const SellDeal &rhs) const;

    bool operator<(const SellDeal &rhs) const;

public: // Methods

    [[nodiscard]] bool IsCompatible(const BuyDeal& deal) const;

    [[nodiscard]] std::string GetInfo() const override;

};

using DealPair = std::pair<BuyDeal, SellDeal>;

DealPair completeDeals(BuyDeal &buy, SellDeal &sellDeal);


my_long getDealPrice(BuyDeal &buy, SellDeal &sell);

my_long getDealVolume(BuyDeal &buy, SellDeal &sell);