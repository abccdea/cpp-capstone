/* File:            GameObjectStatic.cpp
 * Author:          Vish Potnis
 * Description:     - Derived class for static objects (text and background)
 *                  - Only render function is overriden
 */

#include "GameObjectStatic.h"

// basic constructor that accepts position and texture of the object
GameObjectStatic::GameObjectStatic(const Point& pos, const CTexture& tex)
    : GameObject(pos, tex)
{}

// render object to screen, destination rectangle is size of texture
void GameObjectStatic::render(SDL_Renderer& renderer) 
{
    SDL_Rect renderQuad{static_cast<int>(_pos.x), static_cast<int>(_pos.y), _tex.getWidth(), _tex.getHeight()};
    SDL_RenderCopy( &renderer, &_tex.getTexture(), nullptr, &renderQuad);    
}

// render object to screen, destination is given by dest
void GameObjectStatic::render(SDL_Renderer& renderer, SDL_Rect& dest) const
{
    SDL_RenderCopy( &renderer, &_tex.getTexture(), nullptr, &dest);    
}