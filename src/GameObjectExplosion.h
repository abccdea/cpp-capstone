/* File:            GameObjectExplosion.h
 * Author:          Vish Potnis
 * Description:     - Derived class for explosion objects 
 *                  - Render animation sprites over time
 */

#pragma once

#include "GameObject.h"
#include "constants.h"
#include "utility.h"

class GameObjectExplosion : public GameObject
{
    public:

        GameObjectExplosion(const Point& pos, const CTexture& tex);     // constructor that accepts position and texture of the object

        void render(SDL_Renderer& renderer) override;         // render explosion to screen
        void update(const Uint32 updateTime) override;        // update animation frame based on timer
        

        void setSize(AsteroidSize size);    // set the size of the animation sprite based on asteroid size
        bool isAnimationDone() const;       // check if animation has cycled through all the sprites

    private:

        static std::vector<SDL_Rect> _spriteClips;  // static variable defining the source rectandles for all the animation sprites on the texture
        int _width;             // resize original texture
        int _height;            // resize original texture
        int _currentClip;       // current animation sprite count
};