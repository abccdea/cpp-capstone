/* File:            GameObjectExplosion.cpp
 * Author:          Vish Potnis
 * Description:     - Derived class for explosion objects 
 *                  - Render animation sprites over time
 */


#include "GameObjectExplosion.h"

// initialize static variable
std::vector<SDL_Rect> GameObjectExplosion::_spriteClips = std::vector<SDL_Rect>();

GameObjectExplosion::GameObjectExplosion(const Point &pos, const CTexture& tex)
    : GameObject(pos, tex), _width(AsteroidConstants::EXPLOSION_SPRITE_WIDTH), _height(AsteroidConstants::EXPLOSION_SPRITE_HEIGHT), _currentClip(0)
{
    // if _spriteClips has not been set then determine the sprite clip rectangles based on sprite sheet dimensions
    if(_spriteClips.size() == 0){

        for(int y = 0; y < tex.getHeight(); y += AsteroidConstants::EXPLOSION_SPRITE_HEIGHT){
            for(int x = 0; x < tex.getWidth(); x += AsteroidConstants::EXPLOSION_SPRITE_WIDTH){
                SDL_Rect rect{x, y, AsteroidConstants::EXPLOSION_SPRITE_WIDTH, AsteroidConstants::EXPLOSION_SPRITE_HEIGHT};
                _spriteClips.push_back(rect);
            }
        }
        
    }
}

// render explosion to screen based on current current clip
void GameObjectExplosion::render(SDL_Renderer& renderer)
{
    if(_currentClip < AsteroidConstants::EXPLOSION_SPRITE_NUM){
         
        int xPosCenter = std::round(_pos.x);
        int yPosCenter = std::round(_pos.y);

        int left = xPosCenter - _width/2;
        int top = yPosCenter - _height/2;

        SDL_Rect dstRect{left, top, _width, _height};

        SDL_RenderCopy( &renderer, &_tex.getTexture(), &_spriteClips[_currentClip], &dstRect);
    }
}

// update explosion animation frame based on timer (50ms)
void GameObjectExplosion::update(const Uint32 updateTime)
{

    if(_currentClip < AsteroidConstants::EXPLOSION_SPRITE_NUM){
        Uint32 timeDelta = updateTime - _lastUpdated;
        if(timeDelta > 50){
            _currentClip++;        
            _lastUpdated = updateTime;
        }            
    }
    
}

// set the size of the animation sprite based on asteroid size
void GameObjectExplosion::setSize(AsteroidSize size)
{
    switch(size)
    {
        case AsteroidSize::BIG:
            _width = 128;
            _height = 128;
            break;
        case AsteroidSize::MED:
            _width = 64;
            _height = 64;
            break;
        case AsteroidSize::SMALL:
            _width = 32;
            _height = 32;
            break;
    }
    
}

 // check if animation has cycled through all the sprites
bool GameObjectExplosion::isAnimationDone() const
{
    return _currentClip >= AsteroidConstants::EXPLOSION_SPRITE_NUM;
}