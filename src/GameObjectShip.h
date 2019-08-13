/* File:            GameObjectShip.h
 * Author:          Vish Potnis
 * Description:     - Derived class for ship objects 
 */

#pragma once

#include "GameObject.h"
#include <vector>

enum class ShipMovement
{
    MOVE_FORWARD,
    MOVE_BACKWARD,
    ROTATE_LEFT,
    ROTATE_RIGHT
};

class GameObjectShip : public GameObject
{
    public:

        GameObjectShip(const Point& pos, const CTexture& tex, CVector velocity);
        
        void render(SDL_Renderer& renderer) override;   // render ship to the screen
        void update(const Uint32 updateTime) override;  // update ship position and direction based on movement booleans
        
        // setter functions for ship movement
        void setRotateLeft(bool val);
        void setRotateRight(bool val);
        void setMoveForward(bool val);
        void setMoveBackward(bool val);

        // getter
        const SDL_Rect& getBoundingBox();

    private:

        int _width;             // resize original texture
        int _height;            // resize original texture
        SDL_Rect _boundingBox;  // bounding box for laser used for collision detection

        // used for movement update based on keyboard input
        bool _rotateLeft;
        bool _rotateRight;
        bool _moveForward;
        bool _moveBackward;
};