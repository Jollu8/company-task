#include "OrderBookTester.h"
#include <iostream>
int main() {

    // Создаем экземпляры классов OrderBookVector и OrderBookMultiset
    OrderBookVector orderBookVector;
    OrderBookMultiset orderBookMultiset;

    // Создаем экземпляры класса OrderBookTester для каждого из классов OrderBook
    OrderBookTester<OrderBookVector> testerVector(orderBookVector);
    OrderBookTester<OrderBookMultiset> testerMultiset(orderBookMultiset);

    // Добавляем заявки в стаканы
    Order order1 = {100.0, 10, OrderType::BUY};
    Order order2 = {200.0, 20, OrderType::SELL};
    orderBookVector.addOrder(order1);
    orderBookVector.addOrder(order2);
    orderBookMultiset.addOrder(order1);
    orderBookMultiset.addOrder(order2);

    // Проводим тестирование для каждого из классов OrderBook
    std::cout << "Testing OrderBookVector:" << std::endl;
    testerVector.testOrderBook();

    std::cout << "Testing OrderBookMultiset:" << std::endl;
    testerMultiset.testOrderBook();

    return 0;
}
