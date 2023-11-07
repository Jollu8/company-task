//
// Created by Jollu Emil on 11/7/23.
//
#include <iostream>


class Shape {
public:
    virtual void draw() = 0;
};

class Circle : public Shape {
public:
    void draw() {
        std::cout << "Drawing a circle." << std::endl;
    }
};

class Square : public Shape {
public:
    void draw() {
        std::cout << "Drawing a square." << std::endl;
    }
};

class Triangle : public Shape {
public:
    void draw() {
        std::cout << "Drawing a triangle." << std::endl;
    }
};

class Rectangle : public Shape {
public:
    void draw() {
        std::cout << "Drawing a rectangle." << std::endl;
    }
};

int main() {
    Circle circle;
    Square square;
    Triangle triangle;
    Rectangle rectangle;

    circle.draw();
    square.draw();
    triangle.draw();
    rectangle.draw();

    return 0;
}
