#!/bin/bash

# obj
g++ -c src/main.cpp -o obj/main.o
g++ -c src/AnimatedSprite.cpp -o obj/AnimatedSprite.o
g++ -c src/ResourceManager.cpp -o obj/ResourceManager.o
g++ -c src/TexturedRectangle.cpp -o obj/TexturedRectangle.o
g++ -c src/UI.cpp -o obj/UI.o
g++ -c src/cargo_drone.cpp -o obj/cargo_drone.o
g++ -c src/cargo_drone_dynamics.cpp -o obj/cargo_drone_dynamics.o
g++ -c src/controller.cpp -o obj/controller.o
g++ -c src/drone.cpp -o obj/drone.o
g++ -c src/drone_dynamics.cpp -o obj/drone_dynamics.o

# program
g++ obj/*.o -I/usr/include/SDL2/ -I. -Iinclude/ -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -o bin/dronesim


# # obj
# emcc -O2 -c src/main.cpp -o obj/main.o
# emcc -O2 -c src/AnimatedSprite.cpp -o obj/AnimatedSprite.o
# emcc -O2 -c src/ResourceManager.cpp -o obj/ResourceManager.o
# emcc -O2 -c src/TexturedRectangle.cpp -o obj/TexturedRectangle.o
# emcc -O2 -c src/UI.cpp -o obj/UI.o
# emcc -O2 -c src/cargo_drone.cpp -o obj/cargo_drone.o
# emcc -O2 -c src/cargo_drone_dynamics.cpp -o obj/cargo_drone_dynamics.o
# emcc -O2 -c src/controller.cpp -o obj/controller.o
# emcc -O2 -c src/drone.cpp -o obj/drone.o
# emcc -O2 -c src/drone_dynamics.cpp -o obj/drone_dynamics.o
#
# # program
# # g++ obj/*.o -I/usr/include/SDL2/ -I. -Iinclude/ -lSDL2main -lSDL2 -g -o bin/game
# # emcc obj/*.o -s WASM=1 -s USE_SDL=2 -O2 -o renderingC.js
# emcc obj/*.o -s WASM=1 -s USE_SDL=2 -s SDL2_IMAGE_FORMATS='["bmp"]' -s USE_SDL_TTF=2 --preload-file resources -O2 -o dronesim.js
# # emcc main.cpp -s WASM=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_TTF=2 --preload-file res -o index.js
