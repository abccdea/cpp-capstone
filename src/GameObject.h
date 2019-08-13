/* File:            GameObject.h
 * Author:          Vish Potnis
 * Description:     - Parent class for game objects
 *                  - Manages the position, movement, and rendering of game objects
 */

#pragma once

#include <vector>
#include <memory>

#include "CTexture.h"
#include "CVector.h"
#include "utility.h"

class GameObject{

    public:

        
        GameObject(const Point& pos, const CTexture& tex);                                       // basic constructor that accepts position and texture of the object
        GameObject(const Point& pos, const CTexture& tex, CVector velocity);                     // additional parameter for initial velocity vector
        GameObject(const Point& pos, const CTexture& tex, CVector velocity, double rotation);    // additional parameter for rotation of the object texture
        virtual ~GameObject() = default;

        virtual void render(SDL_Renderer& renderer);      // render object to screen, overridden based on derived object type
        virtual void update(const Uint32 updateTime);           // update the object position and texture based on time passed, overridden based on derived object type
        
        // factory method for creating GameObjects based on ObjectType
        static std::unique_ptr<GameObject> Create(ObjectType type, Point pos, CTexture& tex, CVector velocity=CVector(), double rotation=0);

        // getter functions
        Point getPos() const;
        CVector getVelocity() const;
        double getRotation() const;
        int getID() const;
        
    protected:        

        Point _pos;             // position of center of object on screen
        const CTexture& _tex;   // reference to the texture for the object
        CVector _velocity;      // current velocity of the object
        double _rotation;       // rotation of the texture

        Uint32 _lastUpdated;    // time stamp denoting last update of object
                        
        int _id;            // unique ID for the object
        static int _count;  // static counter for assigning IDs
};

