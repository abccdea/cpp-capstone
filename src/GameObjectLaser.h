/* File:            GameObjectLaser.h
 * Author:          Vish Potnis
 * Description:     - Derived class for laser objects 
 */

#pragma once

#include "GameObject.h"

class GameObjectLaser : public GameObject
{
    public:

        GameObjectLaser(const Point& pos, const CTexture& tex, CVector velocity, double rotation) ;

        void render(SDL_Renderer &renderer) override;       // render laser to the screen
        void update(const Uint32 updateTime) override;      // update laser position based on velocity and time delta
        
        bool checkOffscreen() const;        // check if laser has gone off screen

        // getter
        const SDL_Rect& getBoundingBox();
        
    private:

        int _width;             // resize original texture
        int _height;            // resize original texture
        SDL_Rect _boundingBox;  // bounding box for laser used for collision detection
};