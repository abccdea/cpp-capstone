/* File:            GameObjectAsteroid.h
 * Author:          Vish Potnis
 * Description:     - Derived class for asteroid objects
 */

#pragma once

#include "GameObject.h"
#include <vector>

class GameObjectAsteroid : public GameObject
{

    public:
        GameObjectAsteroid(const Point& pos, const CTexture& tex, CVector velocity);
        
        void render(SDL_Renderer& renderer) override;       // render asteroid to screen
        void update(const Uint32 updateTime) override;      // update asteroid position based on velocity and time delta

         void setAsteroidAttr(AsteroidSize size, AsteroidColor color);
        
        // getters
        const std::vector<SDL_Rect>& getBoundingBoxes();
        AsteroidSize getSize() const;
        AsteroidSize getNextSize() const;
               
        static AsteroidColor getNextColor(AsteroidColor color);     // static function to determine next color based on input color (cycle through colors)
        static TextureType getAsteroidTexture(AsteroidSize size, AsteroidColor color);  // static function to get asteroid texture enum based on size and clor

    private:
        
        // calculate offscreen wrap arounds for textures
        void calculateRenderRectangles(int objPosX, int objPosY, int objWidth, int objHeight, int screenWidth, int screenHeight, 
                                        std::vector<SDL_Rect> &srcRect, std::vector<SDL_Rect> &dstRect);

        AsteroidSize _asteroidSize;
        AsteroidColor _asteroidColor;           

        std::vector<SDL_Rect> _boundingBoxes;       // bounding boxes for asteroid used for collision detection
};