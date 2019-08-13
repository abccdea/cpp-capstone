/* File:            GameObjectAsteroid.cpp
 * Author:          Vish Potnis
 * Description:     - Derived class for asteroid objects
 */


#include "GameObjectAsteroid.h"
#include "constants.h"
#include <cmath>

GameObjectAsteroid::GameObjectAsteroid(const Point& pos, const CTexture& tex, CVector velocity)
    : GameObject(pos, tex, velocity)
{}

// render asteroid to screen
void GameObjectAsteroid::render(SDL_Renderer& renderer)
{
    int xPosCenter = std::round(_pos.x);
    int yPosCenter = std::round(_pos.y);

    std::vector<SDL_Rect> srcRect;  // source rectangles defining texture boundary for wrap around the screen
    std::vector<SDL_Rect> dstRect;  // destination rectangles for the screen for source rectangles

    int width = _tex.getWidth();
    int height = _tex.getHeight();

    // calculate screen wrap around based on position and texture dimensions
    calculateRenderRectangles(xPosCenter, yPosCenter, width, height, AsteroidConstants::SCREEN_WIDTH, AsteroidConstants::SCREEN_HEIGHT, srcRect, dstRect);    

    // render texture in potential parts
    for(unsigned long i = 0; i < srcRect.size(); i++){
        SDL_RenderCopy( &renderer, &_tex.getTexture(), &srcRect[i], &dstRect[i]);
    }

    // desRect (on screen rectangles) define the bounding box for the asteroid
    _boundingBoxes = std::move(dstRect);    
}


// update asteroid position based on velocity and time delta
void GameObjectAsteroid::update(Uint32 updateTime)
{

    double timeDelta = static_cast<double>(updateTime - _lastUpdated)/1000;

    // update position    
    _pos.x += _velocity.getXProjection() * timeDelta;
    _pos.y += _velocity.getYProjection() * timeDelta;

    // wrap around the screen
    if(_pos.x >= AsteroidConstants::SCREEN_WIDTH){
        _pos.x = 0;
    }
    if(_pos.x < 0){
        _pos.x = AsteroidConstants::SCREEN_WIDTH;
    }

    if(_pos.y >= AsteroidConstants::SCREEN_HEIGHT){
        _pos.y = 0;
    }
    if(_pos.y < 0){
        _pos.y = AsteroidConstants::SCREEN_HEIGHT;
    }

    _lastUpdated = updateTime;

}

void GameObjectAsteroid::setAsteroidAttr(AsteroidSize size, AsteroidColor color)
{
    _asteroidSize = size;
    _asteroidColor = color;
}

// getters
const std::vector<SDL_Rect>& GameObjectAsteroid::getBoundingBoxes() { return _boundingBoxes;}
AsteroidSize GameObjectAsteroid::getSize() const { return _asteroidSize;}
AsteroidSize GameObjectAsteroid::getNextSize() const
{
    if(_asteroidSize == AsteroidSize::BIG) return AsteroidSize::MED;
    return AsteroidSize::SMALL;
}

///// static helpers /////

// static function to determine next color based on input color (cycle through colors)
AsteroidColor GameObjectAsteroid::getNextColor(AsteroidColor color)
{
    switch(color){
        case AsteroidColor::GREY:   return AsteroidColor::RED;
        case AsteroidColor::RED:   return AsteroidColor::BROWN;
        case AsteroidColor::BROWN:   return AsteroidColor::GREY;
    }
    return AsteroidColor::GREY;
}

// static function to get asteroid texture enum based on size and clor
TextureType GameObjectAsteroid::getAsteroidTexture(AsteroidSize size, AsteroidColor color)
{
    switch(size){
        case AsteroidSize::BIG:
            switch(color){
                case AsteroidColor::GREY:   return TextureType::TEX_ASTEROID_BIG_1;
                case AsteroidColor::RED:    return TextureType::TEX_ASTEROID_BIG_2;
                case AsteroidColor::BROWN:  return TextureType::TEX_ASTEROID_BIG_3;
            }
            break;
        case AsteroidSize::MED:
            switch(color){
                case AsteroidColor::GREY:   return TextureType::TEX_ASTEROID_MED_1;
                case AsteroidColor::RED:    return TextureType::TEX_ASTEROID_MED_2;
                case AsteroidColor::BROWN:  return TextureType::TEX_ASTEROID_MED_3;
            }
            break;
        case AsteroidSize::SMALL:
            switch(color){
                case AsteroidColor::GREY:   return TextureType::TEX_ASTEROID_SMALL_1;
                case AsteroidColor::RED:    return TextureType::TEX_ASTEROID_SMALL_2;
                case AsteroidColor::BROWN:  return TextureType::TEX_ASTEROID_SMALL_3;
            }
            break;
    }
    return TextureType::TEX_ASTEROID_BIG_1;
}


// calculate offscreen wrap arounds for textures
// srcRect contains the rectangles defining the texture area 
// destRect contains the recatangles defining the destination area
void GameObjectAsteroid::calculateRenderRectangles(int objPosX, int objPosY, int objWidth, int objHeight, int screenWidth, int screenHeight, 
                                        std::vector<SDL_Rect> &srcRect, std::vector<SDL_Rect> &dstRect)
{
    
    int left = objPosX - objWidth/2;
    int top = objPosY - objHeight/2;
    int right = left + objWidth - 1;
    int bottom = top + objHeight - 1;
    
    // entire object fits on screen without wrapping
    if(left >= 0 && right < screenWidth && top >= 0 && bottom < screenHeight){
        srcRect.push_back(SDL_Rect{0, 0, objWidth, objHeight});
        dstRect.push_back(SDL_Rect{left, top, objWidth, objHeight});        
        return;
    }
    // object height is within the screen
    if(top >= 0 && bottom < screenHeight){
        // object wraps on the left side
        if(left < 0){
            srcRect.push_back(SDL_Rect{0, 0, 0-left, objHeight});
            srcRect.push_back(SDL_Rect{0-left, 0, right+1, objHeight});

            dstRect.push_back(SDL_Rect{left+screenWidth, top, 0-left, objHeight});
            dstRect.push_back(SDL_Rect{0, top, right+1, objHeight});
            return;
        }
        // object wraps on the right side
        else{
            srcRect.push_back(SDL_Rect{0, 0, screenWidth-left, objHeight});
            srcRect.push_back(SDL_Rect{screenWidth-left, 0, right-screenWidth+1, objHeight});

            dstRect.push_back(SDL_Rect{left, top, screenWidth-left, objHeight});
            dstRect.push_back(SDL_Rect{0, top, right-screenWidth+1, objHeight});
            return;
        }
    }
    // object width is within the screen
    if(left >= 0 && right < screenWidth){
        // object wraps on the top
        if(top < 0){
            srcRect.push_back(SDL_Rect{0, 0, objWidth, 0-top});
            srcRect.push_back(SDL_Rect{0, 0-top, objWidth, bottom+1});

            dstRect.push_back(SDL_Rect{left, top+screenHeight, objWidth, 0-top});
            dstRect.push_back(SDL_Rect{left, 0, objWidth, bottom+1});
            return;
        }
        // object wraps on the bottom
        else{
            srcRect.push_back(SDL_Rect{0, 0, objWidth, screenHeight-top});
            srcRect.push_back(SDL_Rect{0, screenHeight-top, objWidth, bottom-screenHeight+1});

            dstRect.push_back(SDL_Rect{left, top, objWidth, screenHeight-top});
            dstRect.push_back(SDL_Rect{left, 0, objWidth, bottom-screenHeight+1});
            return;
        }
    }
    // object wraps top left corner
    if(left < 0 && top < 0){
        srcRect.push_back(SDL_Rect{0, 0, 0-left, 0-top});
        srcRect.push_back(SDL_Rect{0, 0-top, 0-left, bottom+1});
        srcRect.push_back(SDL_Rect{0-left, 0, right+1, 0-top});
        srcRect.push_back(SDL_Rect{0-left, 0-top, right+1, bottom+1});

        dstRect.push_back(SDL_Rect{left+screenWidth, top+screenHeight, 0-left, 0-top});
        dstRect.push_back(SDL_Rect{left+screenWidth, 0, 0-left, bottom+1});
        dstRect.push_back(SDL_Rect{0, top+screenHeight, right+1, 0-top});
        dstRect.push_back(SDL_Rect{0, 0, right+1, bottom+1});
        return;
    }
    // object wraps bottom left corner
    if(left < 0 && bottom >= screenHeight){
        srcRect.push_back(SDL_Rect{0, 0, 0-left, screenHeight-top});
        srcRect.push_back(SDL_Rect{0, screenHeight-top, 0-left, bottom-screenHeight+1});
        srcRect.push_back(SDL_Rect{0-left, 0, right+1, screenHeight-top});
        srcRect.push_back(SDL_Rect{0-left, screenHeight-top, right+1, bottom-screenHeight+1});

        dstRect.push_back(SDL_Rect{left+screenWidth, top, 0-left, screenHeight-top});
        dstRect.push_back(SDL_Rect{left+screenWidth, 0, 0-left, bottom-screenHeight+1});
        dstRect.push_back(SDL_Rect{0, top, right+1, screenHeight-top});
        dstRect.push_back(SDL_Rect{0, 0, right+1, bottom-screenHeight+1});
        return;
    }
    // object wraps top right corner
    if(top < 0 && right >= screenWidth){
        srcRect.push_back(SDL_Rect{0, 0, screenWidth-left, 0-top});
        srcRect.push_back(SDL_Rect{0, 0-top, screenWidth-left, bottom+1});
        srcRect.push_back(SDL_Rect{screenWidth-left, 0, right-screenWidth+1, 0-top});
        srcRect.push_back(SDL_Rect{screenWidth-left, 0-top, right-screenWidth+1, bottom+1});

        dstRect.push_back(SDL_Rect{left, top+screenHeight, screenWidth-left, 0-top});
        dstRect.push_back(SDL_Rect{left, 0, screenWidth-left, bottom+1});
        dstRect.push_back(SDL_Rect{0, top+screenHeight, right-screenWidth+1, 0-top});
        dstRect.push_back(SDL_Rect{0, 0, right-screenWidth+1, bottom+1});
        return;
    }
    // object wraps bottom right corner
    if(right >= screenWidth && bottom >= screenHeight){
        srcRect.push_back(SDL_Rect{0, 0, screenWidth-left, screenHeight-top});
        srcRect.push_back(SDL_Rect{0, screenHeight-top, screenWidth-left, bottom-screenHeight+1});
        srcRect.push_back(SDL_Rect{screenWidth-left, 0, right-screenWidth+1, screenHeight-top});
        srcRect.push_back(SDL_Rect{screenWidth-left, screenHeight-top, right-screenWidth+1, bottom-screenHeight+1});

        dstRect.push_back(SDL_Rect{left, top, screenWidth-left, screenHeight-top});
        dstRect.push_back(SDL_Rect{left, 0, screenWidth-left, bottom-screenHeight+1});
        dstRect.push_back(SDL_Rect{0, top, right-screenWidth+1, screenHeight-top});
        dstRect.push_back(SDL_Rect{0, 0, right-screenWidth+1, bottom-screenHeight+1});
        return;
    }            
}
