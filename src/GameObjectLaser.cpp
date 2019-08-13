/* File:            GameObjectLaser.cpp
 * Author:          Vish Potnis
 * Description:     - Derived class for laser objects 
 */

#include "GameObjectLaser.h"
#include "constants.h"

GameObjectLaser::GameObjectLaser(const Point& pos, const CTexture& tex, CVector velocity, double rotation)
    : GameObject(pos, tex, velocity, rotation)
{
    // rescale original texture
    _width = _tex.getWidth()/AsteroidConstants::SCALE_LASER_W;
    _height = _tex.getHeight()/AsteroidConstants::SCALE_LASER_H;
}

// render laser to the screen
void GameObjectLaser::render(SDL_Renderer& renderer)
{
    int xPosCenter = std::round(_pos.x);
    int yPosCenter = std::round(_pos.y);

    int left = xPosCenter - _width/2;
    int top = yPosCenter - _height/2;

    SDL_Rect dstRect{left, top, _width, _height};

    SDL_RenderCopyEx( &renderer, &_tex.getTexture(), nullptr, &dstRect, _rotation, nullptr, SDL_FLIP_NONE);

    // desRect (on screen rectangle) defines the bounding box for the laser
    _boundingBox = std::move(dstRect);
}

// update asteroid position based on velocity and time delta
void GameObjectLaser::update(const Uint32 updateTime) 
{
    double timeDelta = static_cast<double>(updateTime - _lastUpdated)/1000;

    _pos.x += _velocity.getXProjection() * timeDelta;
    _pos.y += _velocity.getYProjection() * timeDelta;

    _lastUpdated = updateTime;
}

// check if laser has gone off screen
bool GameObjectLaser::checkOffscreen() const
{
    if(_pos.x < -AsteroidConstants::OFFSCREEN_BOUNDARY || _pos.x > AsteroidConstants::SCREEN_WIDTH + AsteroidConstants::OFFSCREEN_BOUNDARY)
        return true;
    if(_pos.y < -AsteroidConstants::OFFSCREEN_BOUNDARY || _pos.y > AsteroidConstants::SCREEN_HEIGHT + AsteroidConstants::OFFSCREEN_BOUNDARY)
        return true;
    return false;
}

// getter
const SDL_Rect& GameObjectLaser::getBoundingBox() { return _boundingBox;}