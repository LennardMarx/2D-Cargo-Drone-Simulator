//===============================================================
// credit for this code to Mike Shah
// https://github.com/MikeShah/SDL2_Tutorials
//
// non-trivial changes were done to adjust to code for our needs
// e.g. rotation of the animated image
//===============================================================

#include <string>
// Third Party
#include <SDL2/SDL.h> // For Mac, use <SDL.h>
// #include <SDL2/SDL_image.h>

#include "../include/AnimatedSprite.hpp"
#include "../include/ResourceManager.hpp"

// Constructor
AnimatedSprite::AnimatedSprite(SDL_Renderer *&renderer, std::string filepath)
{

    SDL_Surface *retrieveSurface = ResourceManager::GetInstance().GetSurface(filepath);
    m_texture = SDL_CreateTextureFromSurface(renderer, retrieveSurface);
}
// Destructor
AnimatedSprite::~AnimatedSprite()
{
    SDL_DestroyTexture(m_texture);
}

void AnimatedSprite::Draw(int x, int y, int w, int h)
{
    // Create a rectangle
    m_dst.x = x;
    m_dst.y = y;
    m_dst.w = w;
    m_dst.h = h;

    pt.x = w / 2; // center point of rotation
    pt.y = h / 2;
}

void AnimatedSprite::PlayFrame(int x, int y, int w, int h, int frame)
{
    m_src.x = x + w * frame;
    m_src.y = y;
    m_src.w = w;
    m_src.h = h;
}

void AnimatedSprite::Update()
{
}

void AnimatedSprite::Render(SDL_Renderer *&renderer, double angle)
{
    // render copy to allow for rotation
    SDL_RenderCopyEx(renderer, m_texture, &m_src, &m_dst, angle, &pt, SDL_FLIP_NONE);
}
