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

#include <SDL2/SDL_ttf.h>
#include <cstdlib>
#include <emscripten.h>
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

struct context {
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
  int controllerCheck;

  double thrust = 5 * 9.81;
  double angVel = 0;
  double xVel = 0;
  double yVel = 0;

  bool pause = false;
  bool quit = false;

  SDL_Event event;
};

static void mainloop(void *arg) {
  // chdir(SDL_GetBasePath());
  context *ctx = static_cast<context *>(arg);
  ctx->frameStart = SDL_GetTicks(); // limit framerate (see end of while loop)

  while (SDL_PollEvent(&ctx->event) != 0) {
    // stop when pressing "x" (?)
    if (ctx->event.type == SDL_QUIT) {
      ctx->quit = true;
    }
  }
  auto keys_down = ctx->ui.WhichKeysDown(); // saves which key was pressed

  ctx->keyPress = 0; // resetting the key press
  for (unsigned char c : keys_down) {
    ctx->keyPress = c;
    if (ctx->keyPress == 'Q') {
      ctx->quit = true;
    } else if (ctx->keyPress == 'R') {
      ctx->reset = true;
    }
    // else if (keyPress == 'X')
    // {
    // if (controllerCheck == 0)
    // {
    // controllerActive = !controllerActive;
    // controllerCheck++;
    // }
    // }
    else if (ctx->keyPress == 'X') {
      ctx->controllerActive = false;
    } else if (ctx->keyPress == 'Y') {
      ctx->controllerActive = true;
    } else if (ctx->choice1 == 1 &&
               ctx->choice2 == 1) // only start drone when choices were made
    {
      if (ctx->keyPress == 'u') {
        if (ctx->controllerActive == true) {
          ctx->yVel += 0.13;
        } else {
          ctx->thrust += 0.5;
        }
      } else if (ctx->keyPress == 'd') {
        if (ctx->controllerActive == true) {
          ctx->yVel -= 0.13;
        } else {
          ctx->thrust -= 0.5;
        }
      } else if (ctx->keyPress == 'l') {
        if (ctx->controllerActive == true) {
          ctx->xVel -= 0.15;
        } else {
          ctx->angVel += 0.1;
        }
      } else if (ctx->keyPress == 'r') {
        if (ctx->controllerActive == true) {
          ctx->xVel += 0.15;
        } else {
          ctx->angVel -= 0.1;
        }
      }
    } else if (ctx->keyPress == 'D') {
      if (ctx->choice1 == 0) {
        ctx->droneType = 0;
        ctx->choice1 = 1;
      }
    } else if (ctx->keyPress == 'C') {
      if (ctx->choice1 == 0) {
        ctx->droneType = 1;
        ctx->choice1 = 1;
      }
    } else if (ctx->keyPress == 'E') {
      if (ctx->choice2 == 0) {
        ctx->integrationMethod = 0;
        ctx->choice2 = 1;
      }
    } else if (ctx->keyPress == 'K') {
      if (ctx->choice2 == 0) {
        ctx->integrationMethod = 1;
        ctx->choice2 = 1;
      }
    }
  }
  // pause the game
  if (ctx->pause) {
    // do nothing
  } else {

    // reset
    if (ctx->reset == true) {
      if (ctx->droneType == 1) {
        ctx->thrust = 5 * 9.81;
        ctx->cargoDrone.setStates({0, 0, 0, 0, 0, 0, 0 - 1, 0, 0});
      } else if (ctx->droneType == 0) {
        ctx->thrust = 3 * 9.81;
        ctx->drone.setStates({0, 0, 0, 0, 0, 0, 0, 0, 0});
      }
      ctx->angVel = 0;
      ctx->xVel = 0;
      ctx->yVel = 0;
      ctx->reset = false;
      // set velocity inputs to Drone vel
    }

    // integration
    for (int i = 0; i < 2; ++i) {
      if (ctx->controllerActive == true) {
        ctx->controller.setDroneType(ctx->droneType);
        ctx->controller.setReceivedVelref({ctx->xVel, ctx->yVel});
        ctx->controller.calculateControllerOutput();
        if (ctx->droneType == 1) {
          ctx->controller.setReceivedUpdatedStates(ctx->cargoDrone.getStates());
          ctx->cargoDroneDynamics.setReceivedInputs(ctx->controller.getInput());
        } else if (ctx->droneType == 0) {
          ctx->controller.setReceivedUpdatedStates(ctx->drone.getStates());
          ctx->droneDynamics.setReceivedInputs(ctx->controller.getInput());
        }
      } else {
        if (ctx->droneType == 1) {
          ctx->cargoDroneDynamics.setReceivedInputs({ctx->thrust, ctx->angVel});
        } else if (ctx->droneType == 0) {
          ctx->droneDynamics.setReceivedInputs({ctx->thrust, ctx->angVel});
        }
      }
      if (ctx->droneType == 1) {
        ctx->cargoDroneDynamics.setReceivedStates(ctx->cargoDrone.getStates());
        if (ctx->integrationMethod == 1) {
          ctx->cargoDroneDynamics.rungeKutta();
        } else if (ctx->integrationMethod == 0) {
          ctx->cargoDroneDynamics.forwardEuler();
        }
        ctx->cargoDrone.setStates(ctx->cargoDroneDynamics.getUpdatedStates());
      } else if (ctx->droneType == 0) {
        ctx->droneDynamics.setReceivedStates(ctx->drone.getStates());
        if (ctx->integrationMethod == 1) {
          ctx->droneDynamics.rungeKutta();
        } else if (ctx->integrationMethod == 0) {
          ctx->droneDynamics.forwardEuler();
        }
        ctx->drone.setStates(ctx->droneDynamics.getUpdatedStates());
      }
    }

    // scaling coordinates to drone/cargo size
    if (ctx->droneType == 1) {
      ctx->x = ctx->lineLength * 2 * ctx->cargoDrone.getStates().at(0);
      ctx->y = -ctx->lineLength * 2 * ctx->cargoDrone.getStates().at(1);
      ctx->angle = ctx->cargoDrone.getStates().at(2);
      ctx->x_c = ctx->lineLength * 2 * ctx->cargoDrone.getStates().at(5);
      ctx->y_c = -ctx->lineLength * 2 * ctx->cargoDrone.getStates().at(6);
    } else if (ctx->droneType == 0) {
      ctx->x = ctx->lineLength * 2 * ctx->drone.getStates().at(0);
      ctx->y = -ctx->lineLength * 2 * ctx->drone.getStates().at(1);
      ctx->angle = ctx->drone.getStates().at(2);
      ctx->x_c = ctx->lineLength * 2 * ctx->drone.getStates().at(5);
      ctx->y_c = -ctx->lineLength * 2 * ctx->drone.getStates().at(6);
    }
  }
  // rendering screen
  ctx->ui.clear(); // clears screen

  // function to render all the bmp and the animation
  renderImages(ctx->ui.getRenderer(), ctx->x, ctx->y, ctx->angle, ctx->x_c,
               ctx->y_c, ctx->droneType, ctx->choice1, ctx->choice2);

  // draw Rope if cargo drone was chosen
  if (ctx->droneType == 1 && ctx->choice1 == 1 && ctx->choice2 == 1) {
    ctx->ui.drawLine(ctx->x, ctx->y, ctx->x_c, ctx->y_c);
  }

  ctx->ui.present(); // shows rendered objects

  // frame time to limit FPS
  ctx->frameTime = SDL_GetTicks() - ctx->frameStart;
  if (ctx->frameDelay > ctx->frameTime) {
    SDL_Delay(ctx->frameDelay - ctx->frameTime);
  }
  ctx->controllerCheck++;        // increases frame number
  if (ctx->controllerCheck > 15) // resets frameNumber if at end of animation
  {
    ctx->controllerCheck = 0;
  }
}

int main() {
  // chdir(SDL_GetBasePath());
  context ctx;

  ctx.controllerCheck = 0;

  emscripten_set_main_loop_arg(mainloop, &ctx, -1, 1);

  return 0;
}
