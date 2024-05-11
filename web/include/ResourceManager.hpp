//===============================================================
// credit for this code to Mike Shah
// https://github.com/MikeShah/SDL2_Tutorials
//
// non-trivial changes were done to adjust to code for our needs
// e.g. rotation of the animated image
//===============================================================

#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <string>
#include <unordered_map>

// Third party library
#include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>

class ResourceManager
{
private:
    ResourceManager();
    ResourceManager(ResourceManager const &);
    ResourceManager operator=(ResourceManager const &);

    std::unordered_map<std::string, SDL_Surface *> m_surfaces;

public:
    static ResourceManager &GetInstance();

    SDL_Surface *GetSurface(std::string filepath);
};

#endif
