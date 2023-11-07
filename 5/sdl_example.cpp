//
// Created by Jollu Emil on 11/7/23.
//

#include <SDL2/SDL.h>

#include <cmath>
#include <cstdio>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Структура для хранения параметров фигуры
struct ShapeParams {
    int x;
    int y;
    int width;
    int height;
    int radius;
    int size;
};

class Shape {
public:
    virtual void draw(SDL_Renderer* renderer, const ShapeParams& shape) = 0;
};

// Производные классы
class Circle : public Shape {
public:
    void draw(SDL_Renderer* renderer, const ShapeParams& shape) override {
        for (auto dy = 1; dy <= shape.radius; dy += 1.0) {
            double dx = floor(sqrt((2.0 * shape.radius * dy) - (dy * dy)));
            SDL_RenderDrawLine(renderer, shape.x - dx, shape.y + dy - shape.radius, shape.x + dx,
                               shape.y + dy - shape.radius);
            SDL_RenderDrawLine(renderer, shape.x - dx, shape.y - shape.radius - dy, shape.x + dx,
                               shape.y - shape.radius - dy);
        }
    }
};

class Square : public Shape {
public:
    void draw(SDL_Renderer* renderer, const ShapeParams& shape) override {
        SDL_Rect rect = {shape.x - shape.size / 2, shape.y - shape.size / 2, shape.size, shape.size};
        SDL_RenderDrawRect(renderer, &rect);
    }
};

class Triangle : public Shape {
public:
    void draw(SDL_Renderer* renderer, const ShapeParams& shape) override {
        SDL_RenderDrawLine(renderer, shape.x, shape.y - shape.size / 2, shape.x - shape.size / 2,
                           shape.y + shape.size / 2);
        SDL_RenderDrawLine(renderer, shape.x, shape.y - shape.size / 2, shape.x + shape.size / 2,
                           shape.y + shape.size / 2);
        SDL_RenderDrawLine(renderer, shape.x - shape.size / 2, shape.y + shape.size / 2, shape.x + shape.size / 2,
                           shape.y + shape.size / 2);
    }
};

class Rectangle : public Shape {
public:
    void draw(SDL_Renderer* renderer, const ShapeParams& shape) override {
        SDL_Rect rect = {shape.x - shape.width / 2, shape.y - shape.height / 2, shape.width, shape.height};
        SDL_RenderDrawRect(renderer, &rect);
    }
};

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("SDL Shapes", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                          SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        ShapeParams shape_circle{};
        shape_circle.x = 4;
        shape_circle.y = SCREEN_HEIGHT / 4;
        shape_circle.radius = 50;
        Circle circle{};
        circle.draw(renderer, shape_circle);

        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
        ShapeParams shape_square{};
        shape_square.x = SCREEN_WIDTH / 4;
        shape_square.y = 3 * SCREEN_HEIGHT / 4;
        shape_square.size = 100;
        Square square{};
        square.draw(renderer, shape_square);

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
        ShapeParams shape_triangle{};
        shape_triangle.x = 3 * SCREEN_WIDTH / 4;
        shape_triangle.y = SCREEN_HEIGHT / 4;
        shape_triangle.size = 100;
        Triangle triangle;
        triangle.draw(renderer, shape_triangle);

        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);
        ShapeParams shape_rectangle{};
        shape_rectangle.x = 3 * SCREEN_WIDTH / 4;
        shape_rectangle.y = 3 * SCREEN_HEIGHT / 4;
        shape_rectangle.width = 100;
        shape_rectangle.height = 50;
        Rectangle rectangle;
        rectangle.draw(renderer, shape_rectangle);

        SDL_RenderPresent(renderer);

        SDL_Delay(5000);

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    return 0;
}
