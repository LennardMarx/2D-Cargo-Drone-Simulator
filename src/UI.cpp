#include <SDL2/SDL.h>
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <utility>
// #include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "../include/UI.h"

using namespace std::chrono_literals;

UI::UI(int sizeX, int sizeY) : sizeX(sizeX), sizeY(sizeY) {
  initialize(sizeX, sizeY);
}

UI::~UI() {
  if (renderer)
    SDL_DestroyRenderer(renderer);
  if (window)
    SDL_DestroyWindow(window);
  SDL_Quit();
  // IMG_Quit();
  // SDL_DestroyTexture(_drone_texture);
}

void UI::clear() {
  setDrawColor(255, 255, 255, 255);
  SDL_RenderClear(renderer);
  // setDrawColor(0, 0, 0, 255);
  setDrawColor(249, 245, 215, 255); // gruv-light (lightest)
}

void UI::present() {
  // SDL_Delay(10);
  SDL_RenderPresent(renderer);
}
// function to draw pixel on screen (not used)
void UI::drawPixel(int x, int y) {
  SDL_RenderDrawPoint(renderer, x + sizeX / 2, y + sizeY / 2);
}
// function to draw line between two points
void UI::drawLine(int x1, int y1, int x2, int y2) {
  SDL_RenderDrawLine(renderer, x1 + sizeX / 2, y1 + sizeY / 2, x2 + sizeX / 2,
                     y2 + sizeY / 2);
}
// get set drone type
bool UI::getDroneType() { return droneType; }
void UI::setDroneType(bool _droneType) { droneType = _droneType; }
// get set integration method
bool UI::getIntegrationMethod() { return integrationMethod; }
void UI::setIntegrationMethod(bool _integrationMethod) {
  integrationMethod = _integrationMethod;
}
// get set choices (made at start of game - drone type, integration method)
bool UI::getChoice1() { return choice1; }
void UI::setChoice1(bool _choice1) { choice1 = _choice1; }
bool UI::getChoice2() { return choice2; }
void UI::setChoice2(bool _choice2) { choice2 = _choice2; }

void UI::setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

SDL_Renderer *&UI::getRenderer() // pointer reference to the renderer
{
  return renderer;
}
SDL_Window *UI::getWindow() // pointer to the window
{
  return window;
}

// initializing the UI
void UI::initialize(int sizeX, int sizeY) {
  SDL_Init(SDL_INIT_EVERYTHING);

  // Create a Window
  window = SDL_CreateWindow("Drone Simulation", 0, 0, sizeX, sizeY,
                            SDL_WINDOW_SHOWN);

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  // scale window
  SDL_SetWindowSize(window, sizeX, sizeY);
  // adjust render scale
  SDL_RenderSetScale(renderer, 1, 1);
  // place window in middle of screen after scaling
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

std::vector<unsigned char> UI::WhichKeysDown() {
  // Event handler
  SDL_Event e;
  std::vector<unsigned char> KeysDown;

  // Handle events on queue
  while (SDL_PollEvent(&e) != 0) // Required to update KeyboardState
  {
    // User requests quit
    if (e.type == SDL_QUIT) {
      quit = true;
    }
  }

  // Add any keys that you also want to capture.
  // For simplicity, I have given all arrow keys a character code (an enum would
  // have been better here...)
  const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
  if (currentKeyStates[SDL_SCANCODE_UP])
    KeysDown.push_back('u');
  if (currentKeyStates[SDL_SCANCODE_DOWN])
    KeysDown.push_back('d');
  if (currentKeyStates[SDL_SCANCODE_LEFT])
    KeysDown.push_back('l');
  if (currentKeyStates[SDL_SCANCODE_RIGHT])
    KeysDown.push_back('r');
  if (currentKeyStates[SDL_SCANCODE_W])
    KeysDown.push_back('u');
  if (currentKeyStates[SDL_SCANCODE_S])
    KeysDown.push_back('d');
  if (currentKeyStates[SDL_SCANCODE_A])
    KeysDown.push_back('l');
  if (currentKeyStates[SDL_SCANCODE_D])
    KeysDown.push_back('r');
  if (currentKeyStates[SDL_SCANCODE_Q])
    KeysDown.push_back('Q');
  if (currentKeyStates[SDL_SCANCODE_R])
    KeysDown.push_back('R');
  // if (currentKeyStates[SDL_SCANCODE_D])
  //   KeysDown.push_back('D');
  if (currentKeyStates[SDL_SCANCODE_C])
    KeysDown.push_back('C');
  if (currentKeyStates[SDL_SCANCODE_E])
    KeysDown.push_back('E');
  if (currentKeyStates[SDL_SCANCODE_K])
    KeysDown.push_back('K');
  if (currentKeyStates[SDL_SCANCODE_X])
    KeysDown.push_back('X');
  if (currentKeyStates[SDL_SCANCODE_Y])
    KeysDown.push_back('Y');
  return KeysDown;
}
