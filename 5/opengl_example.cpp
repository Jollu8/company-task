// Created by Jollu Emil on 11/7/23.

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <cmath>


// Структура для хранения параметров фигуры
struct ShapeParams {
    float x;
    float y;
    float width;
    float height;
    float radius;
};
class Shape {
public:
    virtual void draw(const ShapeParams &shape) = 0;
};

// Производные классы
class Circle : public Shape {
public:
    void draw(const ShapeParams &shape) override {

        float triangleAmount = 20;  // # of triangles used to draw circle

        // GLfloat radius = 0.8f; //radius
        GLfloat twicePi = 2.0f * M_PI;

        glBegin(GL_LINE_LOOP);
        for (auto i = 0; i <= triangleAmount; i++) {
            glVertex2f(shape.x + (shape.radius * cos((float) i *  twicePi / triangleAmount)),
                       shape.y + (shape.radius * sin((float ) i * twicePi / triangleAmount)));
        }
        glEnd();
    }
};

class Square : public Shape {
public:
    void draw(const ShapeParams &shape) override {
        glBegin(GL_LINE_LOOP);
        glVertex2f(shape.x, shape.y);
        glVertex2f(shape.x + shape.width, shape.y);
        glVertex2f(shape.x + shape.width, shape.y + shape.width);
        glVertex2f(shape.x, shape.y + shape.width);
        glEnd();
    }
};

class Triangle : public Shape {
public:
    void draw(const ShapeParams &shape) override {
        glBegin(GL_LINE_LOOP);
        glVertex2f(shape.x, shape.y);
        glVertex2f(shape.x + shape.width / 2, shape.y + shape.width);
        glVertex2f(shape.x + shape.width, shape.y);
        glEnd();
    }
};

class Rectangle : public Shape {
public:
    void draw(const ShapeParams &shape) override {
        glBegin(GL_LINE_LOOP);
        glVertex2f(shape.x, shape.y);
        glVertex2f(shape.x + shape.width, shape.y);
        glVertex2f(shape.x + shape.width, shape.y + shape.height);
        glVertex2f(shape.x, shape.y + shape.height);
        glEnd();
    }
};

void displayMe() {
    Circle circle;
    Square square;
    Triangle triangle;
    Rectangle rectangle;
    glClear(GL_COLOR_BUFFER_BIT);
    ShapeParams shape_circle{};
    shape_circle.x = 0.5;
    shape_circle.y = 0.5;
    shape_circle.radius = 0.5;
    circle.draw(shape_circle);

    ShapeParams shape_square{};
    shape_square.x = -0.5; // Исправлено здесь
    shape_square.y = -0.5;
    shape_square.width = 1.0;
    square.draw(shape_square);

    ShapeParams shape_triangle{};
    shape_triangle.x = -0.5;
    shape_triangle.y = 0.5;
    shape_triangle.width = 1.0;
    triangle.draw(shape_triangle);

    ShapeParams shape_rectangle{};
    shape_rectangle.x = 0.5;
    shape_rectangle.y = -0.5;
    shape_rectangle.width = 0.5;
    shape_rectangle.height = 1.0;
    rectangle.draw(shape_rectangle);
    glFlush();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello world!");
    glutDisplayFunc(displayMe);
    glutMainLoop();

    return 0;
}
