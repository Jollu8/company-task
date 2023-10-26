//
// Created by Jollu Emil on 10//23.
//

#include <algorithm>
#include <cmath>
#include <iostream>
#include <list>
#include <vector>

class Curve {
public:
    virtual std::tuple<double, double, double> getPoint(double t) = 0;

    virtual std::tuple<double, double, double> getDerivative(double t) = 0;

    virtual double getter() = 0;
};

class Circle : public Curve {
public:
    Circle(double radius) : radius(radius) {}

    std::tuple<double, double, double> getPoint(double t) override {
        return {radius * cos(t), radius * sin(t), 0};
    }

    std::tuple<double, double, double> getDerivative(double t) override {
        return {-radius * sin(t), radius * cos(t), 0};
    }

    double getter() override { return radius; }

private:
    double radius;
};

class Ellipse : public Curve {
public:
    Ellipse(double radiusX, double radiusY)
            : radiusX(radiusX), radiusY(radiusY) {}

    std::tuple<double, double, double> getPoint(double t) override {
        return {radiusX * cos(t), radiusY * sin(t), 0};
    }

    std::tuple<double, double, double> getDerivative(double t) override {
        return {-radiusX * sin(t), radiusY * cos(t), 0};
    }

    double getter() override { return (radiusX + radiusY) / 2.0; }

private:
    double radiusX;
    double radiusY;
};

class Spiral : public Curve {
public:
    Spiral(double radius, double step) : radius(radius), step(step) {}

    std::tuple<double, double, double> getPoint(double t) override {
        return {radius * cos(t), radius * sin(t), t * step};
    }

    std::tuple<double, double, double> getDerivative(double t) override {
        return {-radius * sin(t), radius * cos(t), step};
    }

    double getter() override { return radius; }


private:
    double radius;
    double step;
};

int main() {
    std::vector<Curve *> curves;
    curves.push_back(new Circle(5));
    curves.push_back(new Ellipse(10, 15));
    curves.push_back(new Spiral(20, 0.1));
    for (auto curve: curves) {
        auto point = curve->getPoint(M_PI / 4);
        auto derivative = curve->getDerivative(M_PI / 4);
        std::cout << "Point: (" << std::get<0>(point) << ", " << std::get<1>(point) << ", " << std::get<2>(point) << ")"
                  << std::endl;
        std::cout << "Derivative: (" << std::get<0>(derivative) << ", " << std::get<1>(derivative) << ", "
                  << std::get<2>(derivative) << ")" << std::endl;
    }
    std::list<Circle *> circles;
    for (auto curve: curves) {
        Circle *circle = dynamic_cast<Circle *>(curve);
        if (circle)
            circles.push_back(circle);
    }
    circles.sort([](Circle *a, Circle *b) { return a->getter() < b->getter(); });

    double totalRadius = 0;
    for (auto circle: circles) {
        totalRadius += circle->getter();
    }

    std::cout << "Total Radius: " << totalRadius << std::endl;

    return 0;
}

