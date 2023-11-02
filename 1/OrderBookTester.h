#pragma once
#include "Order.h"

#include <chrono>

template<typename T>
class OrderBookTester {
private:
    T& orderBook;

public:
    explicit OrderBookTester(T& ob) : orderBook(ob) {}

    void testOrderBook() {
        // Замеряем время выполнения метода добавления заявки
        auto start = std::chrono::high_resolution_clock::now();
        orderBook.addOrder(Order{100.0, 10, OrderType::BUY});
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Time taken to add order: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds" << std::endl;

        // Замеряем время выполнения метода изменения заявки
        start = std::chrono::high_resolution_clock::now();
        orderBook.modifyOrder(0, Order{200.0, 20, OrderType::SELL});
        end = std::chrono::high_resolution_clock::now();
        std::cout << "Time taken to modify order: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds" << std::endl;

        // Замеряем время выполнения метода удаления заявки
        start = std::chrono::high_resolution_clock::now();
        orderBook.deleteOrder(0);
        end = std::chrono::high_resolution_clock::now();
        std::cout << "Time taken to delete order: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds" << std::endl;

        // Замеряем время выполнения метода отображения заявок
        start = std::chrono::high_resolution_clock::now();
        orderBook.displayTopOrders();
        end = std::chrono::high_resolution_clock::now();
        std::cout << "Time taken to display orders: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds" << std::endl;
    }
};

