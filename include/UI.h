#pragma once
#include <SDL2/SDL.h>
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <utility>
// #include <SDL2/SDL_image.h>
#include <vector>

using namespace std::chrono_literals;

class UI
{
public:
    const int sizeX;
    const int sizeY;

    UI(int, int);
    ~UI();

    void clear();
    void present();
    void drawPixel(int, int);
    void drawLine(int, int, int, int);
    // void drawImage(double, double);
    void setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    SDL_Renderer*& getRenderer(); // pointer reference to the renderer
    SDL_Window* getWindow();      // pointer to the window

    // get and set methods
    bool getDroneType();
    void setDroneType(bool);
    bool getIntegrationMethod();
    void setIntegrationMethod(bool);

    bool getChoice1();
    void setChoice1(bool);
    bool getChoice2();
    void setChoice2(bool);

    std::vector<unsigned char> WhichKeysDown();

private:
    void initialize(int, int);

private:
    SDL_Window* window = nullptr;     // create window pointer
    SDL_Renderer* renderer = nullptr; // create renderer pointer

    // bools to save chosen drone type and integration method as well as if choices were made
    bool droneType;
    bool integrationMethod;
    bool choice1 = 0;
    bool choice2 = 0;
    bool quit;
    // rclcpp::TimerBase::SharedPtr timer_;
};
