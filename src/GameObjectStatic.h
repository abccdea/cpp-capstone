/* File:            GameObjectStatic.h
 * Author:          Vish Potnis
 * Description:     - Derived class for static objects (text and background)
 *                  - Only render function is overriden
 */

#pragma once

#include "GameObject.h"

class GameObjectStatic : public GameObject
{
    public:

        GameObjectStatic(const Point& pos, const CTexture& tex);        // basic constructor that accepts position and texture of the object

        void render(SDL_Renderer& renderer) override;         // render object to screen, destination rectangle is size of texture
        void render(SDL_Renderer& renderer, SDL_Rect& dest) const;  // render object to screen, destination is given by dest


    private:

};