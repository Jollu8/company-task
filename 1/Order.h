#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

enum class OrderType {
    BUY, SELL
};

struct Order {
    Order(double price_, int volume_, OrderType orderType) :
            price(price_),
            volume(volume_),
            type(orderType) {}

    double price;
    int volume;
    OrderType type;
};

template<typename Container>
class Basic {
public:
    virtual ~Basic() = default;

    virtual void addOrder(Order order) = 0;

    virtual void modifyOrder(std::size_t index, Order newOrder) = 0;

    virtual void deleteOrder(std::size_t index) = 0;

    virtual void displayTopOrders() = 0;

protected:
    Container orders;
};

class OrderBookVector : public Basic<std::vector<Order>> {
public:
    void addOrder(Order order) override {
        orders.emplace_back(order);
    }

    void modifyOrder(std::size_t index, Order newOrder) override {
        orders.at(index) = newOrder;
    }

    void deleteOrder(std::size_t index) override {
        orders.erase(orders.begin() + index);
    }

    void displayTopOrders() override {
        std::sort(orders.begin(), orders.end(), [](auto &a, auto &b) { return a.price > b.price; });
        for (int i = 0; i < std::min(10, (int) orders.size()); ++i) {
            std::cout << "Price: " << orders[i].price << ", Volume: " << orders[i].volume << ", Type: "
                      << (orders[i].type == OrderType::BUY ? "BUY" : "SELL") << std::endl;
        }
    }

    [[nodiscard]] std::size_t get_size() const {
        return orders.size();
    }
};


class Compare {
public:
    bool operator()(const Order &a, const Order &b) const {
        return a.price > b.price;
    }
};

class OrderBookMultiset : public Basic<std::multiset<Order, Compare>> {
public:
    void addOrder(Order order) override {
        orders.insert(order);
    }

    void modifyOrder(std::size_t index, Order newOrder) override {
        if (index >= 0u && index < orders.size()) {
            auto it = orders.begin();
            std::advance(it, index);
            orders.erase(it);
            orders.insert(newOrder);
        }
    }

    void deleteOrder(std::size_t index) override {
        if (index >= 0u && index < orders.size()) {
            auto it = orders.begin();
            std::advance(it, index);
            orders.erase(it);
        }
    }

    void displayTopOrders() override {
        auto it = orders.begin();
        for (int i = 0; i < std::min(10, (int) orders.size()); ++i, ++it) {
            std::cout << "Price: " << it->price << ", Volume: " << it->volume << ", Type: "
                      << (it->type == OrderType::BUY ? "BUY" : "SELL") << std::endl;
        }
    }

    [[nodiscard]] std::size_t get_size() const {
        return orders.size();
    }
};
