//
// Created by Jollu Emil on 10/27/23.
//

#include "deal.h"
#include "Common.h"


std::string Deal::GetInfo() const {
    return Fillers::FAKE_INFO;
}

//==============================================================================
//=============================BuyDeal==========================================
//==============================================================================

bool BuyDeal::operator<(const BuyDeal &rhs) const {
    if (price != rhs.price)return price > rhs.price;
    else if (dealTime != rhs.dealTime)return dealTime < rhs.dealTime;
    else return uid < rhs.uid;
}

bool BuyDeal::operator==(const BuyDeal &rhs) const {
    return uid == rhs.uid &&
           price == rhs.price &&
           volume == rhs.volume &&
           dealTime == rhs.dealTime &&
           isClosed == rhs.isClosed;

}

bool BuyDeal::IsCompatible(const SellDeal &deal) const {
    return price >= deal.GetPrice();
}

std::string BuyDeal::GetInfo() const {
    using namespace nlohmann;
    json info;
    info[DealInfo::KeyType] = DealInfo::TypeBuy;
    info[DealInfo::KeyVolume] = GetVolume();
    info[DealInfo::KeyPrice] = GetPrice();
    return info.dump();
}

//==============================================================================
//=============================SellDeal=========================================
//==============================================================================

bool SellDeal::operator==(const SellDeal &rhs) const {
    return uid == rhs.uid
           && price == rhs.price
           && volume == rhs.volume
           && dealTime == rhs.dealTime
           && isClosed == rhs.isClosed;
}

bool SellDeal::operator<(const SellDeal &rhs) const {
    if (price != rhs.price)return price < rhs.price;
    else if (dealTime != rhs.dealTime)return dealTime < rhs.dealTime;
    else return uid < rhs.uid;
}

bool SellDeal::IsCompatible(const BuyDeal& deal) const {
    return price <= deal.GetPrice();
}

std::string SellDeal::GetInfo() const {
    using namespace nlohmann;
    json info;
    info[DealInfo::KeyType] = DealInfo::TypeSell;
    info[DealInfo::KeyVolume] = GetVolume();
    info[DealInfo::KeyPrice] = GetPrice();
    return info.dump();
}

DealPair completeDeals(BuyDeal &buy, SellDeal &sell) {
    my_long minVolume = getDealVolume(buy, sell);
    my_long dealPrice = getDealPrice(buy, sell);
    DealPair res(buy, sell);

    if (buy.GetVolume() == minVolume)buy.close();
    else buy.SubVolume(minVolume);

    res.first = BuyDeal(buy.GetUid(), minVolume, dealPrice, buy.GetTime());
    res.first.close();

    if (sell.GetVolume() == minVolume)sell.close();
    else sell.SubVolume(minVolume);

    res.second = SellDeal(sell.GetUid(), minVolume, dealPrice, sell.GetTime());
    res.second.close();
    return res;
}


my_long getDealPrice(BuyDeal &buy, SellDeal &sell) {
    return (buy.GetTime() < sell.GetTime() ? buy.GetPrice() : sell.GetPrice());
}

my_long getDealVolume(BuyDeal &buy, SellDeal &sell) {
    return std::min(buy.GetVolume(), sell.GetVolume());
}

