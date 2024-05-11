#include <SDL2/SDL.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include <ratio>

#include "../include/UI.h"
#include "../include/cargo_drone.h"
#include "../include/cargo_drone_dynamics.h"
#include "../include/controller.h"
#include "../include/drone.h"
#include "../include/drone_dynamics.h"

#include "../include/AnimatedSprite.hpp"
#include "../include/ResourceManager.hpp"
#include "../include/TexturedRectangle.hpp"

#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

// #include <SDL2/SDL_image.h>

// fuction to render bmp images
void renderImages(SDL_Renderer *&_renderer, double _x_drone, double _y_drone,
                  double _angle, double _x_cargo, double _y_cargo,
                  bool _droneType, bool _choice1, bool _choice2) {
  int droneW = 80;                        // width of the drone
  int droneH = 20;                        // height of the drone
  int cargoW = 25;                        // wifth of cargo
  int cargoH = 20;                        // height of cargo
  double x = _x_drone + 600 - droneW / 2; // scale the coordinates
  double y = _y_drone + 400 - droneH / 2;
  double angle = -_angle * 180 / 3.1415; // scale angle (from rad to degrees)
  double x_c = _x_cargo + 600 - cargoW / 2;
  double y_c = _y_cargo + 400 - cargoH / 2;
  TexturedRectangle droneType(_renderer, "../resources/droneType.bmp");
  droneType.Draw(0, 0, 1200, 800); // background for choice of drone
  TexturedRectangle integrationMethod(_renderer,
                                      "../resources/integrationMethod.bmp");
  integrationMethod.Draw(0, 0, 1200,
                         800); // backdround for choice if integration method
  TexturedRectangle background(_renderer, "../resources/background.bmp");
  background.Draw(0, 0, 1200, 800); // game background
  AnimatedSprite drone(_renderer, "../resources/drone_animated.bmp");
  drone.Draw(x, y, droneW,
             droneH); // animated bmp of the drone (propellors turning)
  TexturedRectangle cargo(_renderer, "../resources/cargo.bmp");
  cargo.Draw(x_c, y_c, cargoW, cargoH); // image of the cargo

  if (_choice1 == 0 && _choice2 == 0) {
    droneType.Render(_renderer); // show if no choice was taken yet
  }
  if (_choice1 == 1 && _choice2 == 0) {
    integrationMethod.Render(_renderer); // show if first choice was taken
  }
  if (_choice1 == 1 && _choice2 == 1) // show if both choices were taken
  {
    static int frameNumber = 0; // static int to not reset framenumber everytime

    background.Render(_renderer); // renders background
    drone.PlayFrame(0, 0, 416, 107,
                    frameNumber);   // show correct "slot" of animation bmp
    drone.Render(_renderer, angle); // renders drone at given angle
    if (_droneType == 1) {
      cargo.Render(_renderer); // render cargo if cargo drone was chosen
    }

    frameNumber++;       // increases frame number
    if (frameNumber > 6) // resets frameNumber if at end of animation
    {
      frameNumber = 0;
    }
  }
}

int main() {
  chdir(SDL_GetBasePath());

  const int FPS = 80;                // set FPS
  const int frameDelay = 1000 / FPS; // delay according to FPS
  Uint32 frameStart;                 // keeps track of time (?)
  int frameTime;

  UI ui{1200, 800};
  CargoDroneDynamics cargoDroneDynamics;
  DroneDynamics droneDynamics;
  CargoDrone cargoDrone{0, 0};
  Drone drone{0, 0};
  DroneController controller;

  int lineLength = 30;
  double pi = 3.141593;
  double x, y, x_c, y_c, angle;

  char keyPress = 0;

  bool choice1 = 0;
  bool choice2 = 0;
  bool choice3 = 0;
  bool droneType = 1;
  bool integrationMethod = 1;
  bool reset = false;
  bool controllerActive = true;
  static int controllerCheck = 0;

  double thrust = 5 * 9.81;
  double angVel = 0;
  double xVel = 0;
  double yVel = 0;

  bool pause = false;
  bool quit = false;

  SDL_Event event;

  while (!quit) {
    frameStart = SDL_GetTicks(); // limit framerate (see end of while loop)

    while (SDL_PollEvent(&event) != 0) {
      // stop when pressing "x" (?)
      if (event.type == SDL_QUIT) {
        quit = true;
      }
    }
    auto keys_down = ui.WhichKeysDown(); // saves which key was pressed

    keyPress = 0; // resetting the key press
    for (unsigned char c : keys_down) {
      keyPress = c;
      if (keyPress == 'Q') {
        quit = true;
      } else if (keyPress == 'R') {
        reset = true;
      }
      // else if (keyPress == 'X')
      // {
      // if (controllerCheck == 0)
      // {
      // controllerActive = !controllerActive;
      // controllerCheck++;
      // }
      // }
      else if (keyPress == 'X') {
        controllerActive = false;
      } else if (keyPress == 'Y') {
        controllerActive = true;
      } else if (choice1 == 1 &&
                 choice2 == 1) // only start drone when choices were made
      {
        if (keyPress == 'u') {
          if (controllerActive == true) {
            yVel += 0.13;
          } else {
            thrust += 0.5;
          }
        } else if (keyPress == 'd') {
          if (controllerActive == true) {
            yVel -= 0.13;
          } else {
            thrust -= 0.5;
          }
        } else if (keyPress == 'l') {
          if (controllerActive == true) {
            xVel -= 0.15;
          } else {
            angVel += 0.1;
          }
        } else if (keyPress == 'r') {
          if (controllerActive == true) {
            xVel += 0.15;
          } else {
            angVel -= 0.1;
          }
        }
      } else if (keyPress == 'D') {
        if (choice1 == 0) {
          droneType = 0;
          choice1 = 1;
        }
      } else if (keyPress == 'C') {
        if (choice1 == 0) {
          droneType = 1;
          choice1 = 1;
        }
      } else if (keyPress == 'E') {
        if (choice2 == 0) {
          integrationMethod = 0;
          choice2 = 1;
        }
      } else if (keyPress == 'K') {
        if (choice2 == 0) {
          integrationMethod = 1;
          choice2 = 1;
        }
      }
    }
    // pause the game
    if (pause) {
      // do nothing
    } else {

      // reset
      if (reset == true) {
        if (droneType == 1) {
          thrust = 5 * 9.81;
          cargoDrone.setStates({0, 0, 0, 0, 0, 0, 0 - 1, 0, 0});
        } else if (droneType == 0) {
          thrust = 3 * 9.81;
          drone.setStates({0, 0, 0, 0, 0, 0, 0, 0, 0});
        }
        angVel = 0;
        xVel = 0;
        yVel = 0;
        reset = false;
        // set velocity inputs to Drone vel
      }

      // integration
      for (int i = 0; i < 2; ++i) {
        if (controllerActive == true) {
          controller.setDroneType(droneType);
          controller.setReceivedVelref({xVel, yVel});
          controller.calculateControllerOutput();
          if (droneType == 1) {
            controller.setReceivedUpdatedStates(cargoDrone.getStates());
            cargoDroneDynamics.setReceivedInputs(controller.getInput());
          } else if (droneType == 0) {
            controller.setReceivedUpdatedStates(drone.getStates());
            droneDynamics.setReceivedInputs(controller.getInput());
          }
        } else {
          if (droneType == 1) {
            cargoDroneDynamics.setReceivedInputs({thrust, angVel});
          } else if (droneType == 0) {
            droneDynamics.setReceivedInputs({thrust, angVel});
          }
        }
        if (droneType == 1) {
          cargoDroneDynamics.setReceivedStates(cargoDrone.getStates());
          if (integrationMethod == 1) {
            cargoDroneDynamics.rungeKutta();
          } else if (integrationMethod == 0) {
            cargoDroneDynamics.forwardEuler();
          }
          cargoDrone.setStates(cargoDroneDynamics.getUpdatedStates());
        } else if (droneType == 0) {
          droneDynamics.setReceivedStates(drone.getStates());
          if (integrationMethod == 1) {
            droneDynamics.rungeKutta();
          } else if (integrationMethod == 0) {
            droneDynamics.forwardEuler();
          }
          drone.setStates(droneDynamics.getUpdatedStates());
        }
      }

      // scaling coordinates to drone/cargo size
      if (droneType == 1) {
        x = lineLength * 2 * cargoDrone.getStates().at(0);
        y = -lineLength * 2 * cargoDrone.getStates().at(1);
        angle = cargoDrone.getStates().at(2);
        x_c = lineLength * 2 * cargoDrone.getStates().at(5);
        y_c = -lineLength * 2 * cargoDrone.getStates().at(6);
      } else if (droneType == 0) {
        x = lineLength * 2 * drone.getStates().at(0);
        y = -lineLength * 2 * drone.getStates().at(1);
        angle = drone.getStates().at(2);
        x_c = lineLength * 2 * drone.getStates().at(5);
        y_c = -lineLength * 2 * drone.getStates().at(6);
      }
    }
    // rendering screen
    ui.clear(); // clears screen

    // function to render all the bmp and the animation
    renderImages(ui.getRenderer(), x, y, angle, x_c, y_c, droneType, choice1,
                 choice2);

    // draw Rope if cargo drone was chosen
    if (droneType == 1 && choice1 == 1 && choice2 == 1) {
      ui.drawLine(x, y, x_c, y_c);
    }

    ui.present(); // shows rendered objects

    // frame time to limit FPS
    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime) {
      SDL_Delay(frameDelay - frameTime);
    }
    controllerCheck++;        // increases frame number
    if (controllerCheck > 15) // resets frameNumber if at end of animation
    {
      controllerCheck = 0;
    }
  }
  return 0;
}
