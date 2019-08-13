/* File:            GameObject.cpp
 * Author:          Vish Potnis
 * Description:     - Parent class for game objects
 *                  - Manages the position, movement, and rendering of game objects
 */

#include "GameObject.h"
#include "GameObjectAsteroid.h"
#include "GameObjectShip.h"
#include "GameObjectLaser.h"
#include "GameObjectStatic.h"
#include "GameObjectExplosion.h"

int GameObject::_count = 0;     // initialize static counter

// basic constructor that accepts position and texture of the object
GameObject::GameObject(const Point& pos, const CTexture& tex)
    : GameObject(pos, tex, CVector()){}

// additional parameter for initial velocity vector
GameObject::GameObject(const Point& pos, const CTexture& tex, CVector velocity)
    : GameObject(pos, tex, velocity, 0){}

// additional parameter for rotation of the object texture. Initialize _lasUpdated with the time of creation
GameObject::GameObject(const Point& pos, const CTexture& tex, CVector velocity, double rotation)
    : _pos(pos), _tex(tex), _velocity(velocity), _rotation(rotation), _lastUpdated(SDL_GetTicks()), _id(++_count){}


// render object to screen, overridden based on derived object type
void GameObject::render(SDL_Renderer& renderer)
{
    // calculate target destination rectangle
    SDL_Rect renderQuad{static_cast<int>(_pos.x), static_cast<int>(_pos.y), _tex.getWidth(), _tex.getHeight()};
    SDL_RenderCopy( &renderer, &_tex.getTexture(), nullptr, &renderQuad );    
}

// update the object position and texture based on time passed, overridden based on derived object type
void GameObject::update(const Uint32 updateTime)
{
    _lastUpdated = updateTime;
}

// factory method for creating GameObjects based on ObjectType
std::unique_ptr<GameObject> GameObject::Create(ObjectType type, Point pos, CTexture& tex, CVector velocity, double rotation)
{
    switch(type){
        case ObjectType::STATIC:    return std::unique_ptr<GameObject>(new GameObjectStatic(pos, tex));
        case ObjectType::ASTEROID:  return std::unique_ptr<GameObject>(new GameObjectAsteroid(pos, tex, velocity));
        case ObjectType::SHIP:      return std::unique_ptr<GameObject>(new GameObjectShip(pos, tex, velocity));
        case ObjectType::LASER:     return std::unique_ptr<GameObject>(new GameObjectLaser(pos, tex, velocity, rotation));
        case ObjectType::EXPLOSION: return std::unique_ptr<GameObject>(new GameObjectExplosion(pos, tex));
        default: 
            return nullptr;
    }
}

// getter functions
CVector GameObject::getVelocity() const { return _velocity;}
int GameObject::getID() const { return _id;}
Point GameObject::getPos() const { return _pos;}
double GameObject::getRotation() const { return _rotation;}

