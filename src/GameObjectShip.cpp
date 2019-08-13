/* File:            GameObjectShip.cpp
 * Author:          Vish Potnis
 * Description:     - Derived class for ship objects 
 */

#include "GameObjectShip.h"
#include "constants.h"

GameObjectShip::GameObjectShip(const Point& pos, const CTexture& tex, CVector velocity)
    : GameObject(pos, tex, velocity), _rotateLeft(false), _rotateRight(false), _moveForward(false), _moveBackward(false)
{
    // rescale original texture
    _width = _tex.getWidth()/AsteroidConstants::SCALE_SHIP_W;
    _height = _tex.getHeight()/AsteroidConstants::SCALE_SHIP_H;
}

// render ship to the screen
void GameObjectShip::render(SDL_Renderer& renderer)
{
    int xPosCenter = std::round(_pos.x);
    int yPosCenter = std::round(_pos.y);

    int left = xPosCenter - _width/2;
    int top = yPosCenter - _height/2;


    SDL_Rect dstRect{left, top, _width, _height};

    SDL_RenderCopyEx( &renderer, &_tex.getTexture(), nullptr, &dstRect, _rotation, nullptr, SDL_FLIP_NONE);

    _boundingBox = std::move(dstRect);

}

// update ship position and direction based on movement booleans
void GameObjectShip::update(Uint32 updateTime)
{

    // if move forward or move backward is true set current velocity
    if(_moveForward || _moveBackward){
        if(_moveForward){
            _velocity = CVector(150, _rotation-90, VectorType::POLAR);
        }
        else{
            _velocity = CVector(-150, _rotation-90, VectorType::POLAR);
        }
    }
    // otherwise set current velocity to 0
    else{
        _velocity = CVector(0, 0, VectorType::POLAR);
    }

    // if rorate left or rotate right change the velocity angle
    if(_rotateLeft ^ _rotateRight){
        if(_rotateLeft){
            _rotation -= 5;
        }
        else{
            _rotation += 5;
        }
        if(_rotation < 0) _rotation += 360;
        if(_rotation >= 360) _rotation -= 360;
    }

    // compute new position based on velocity vector and time delta
    double timeDelta = static_cast<double>(updateTime - _lastUpdated)/1000;

    _pos.x += _velocity.getXProjection() * timeDelta;
    _pos.y += _velocity.getYProjection() * timeDelta;

    _lastUpdated = updateTime;
}


// setter functions for ship movement
void GameObjectShip::setRotateLeft(bool val) { _rotateLeft = val;}
void GameObjectShip::setRotateRight(bool val) { _rotateRight = val;}
void GameObjectShip::setMoveForward(bool val) { _moveForward = val;}
void GameObjectShip::setMoveBackward(bool val) { _moveBackward = val;}

// getter
const SDL_Rect& GameObjectShip::getBoundingBox() { return _boundingBox;}
