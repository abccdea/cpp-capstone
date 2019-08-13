/* File:            Menu.h
 * Author:          Vish Potnis
 * Description:     - Parent class for menus
 *                  - Initialize menu object
 *                  - render items and handle keyboard input
 */

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <vector>
#include <unordered_map>
#include <utility>

#include "CTexture.h"
#include "GameObject.h"
#include "GameObjectStatic.h"
#include "constants.h"
#include "utility.h"

class Menu
{
    public:
        // constructor accepts initilized renderer, background image object, and all the loaded fonts
        Menu(SDL_Renderer& renderer, const GameObjectStatic& backgroundObject, const std::vector<TTF_Font*>& mainFonts);
        virtual ~Menu() = default;

        virtual GameState run();    // run menu loop
    
    protected:

        virtual void initMenuItems(){};             // initialize static objects to be rendered
        void render();                        // initialize renderer and render objects
        virtual void renderMenuItems();       // render text objects

        // wrapper for factory method for creating static game objects
        std::unique_ptr<GameObjectStatic> createStaticTextObject(Point pos, CTexture &tex);

        SDL_Renderer& _renderer;                // reference to the renderer
        const GameObjectStatic& _backgroundObject;    // reference to the background image object
        const std::vector<TTF_Font*> &_mainFonts;     // referecen to all the loaded fonts

        // hash table to map menu item to texture
        std::unordered_map<MenuItem, CTexture, EnumClassHash> _textTextureHash;     

        // hash table to map menu item to static object to be rendered
        std::unordered_map<MenuItem, std::unique_ptr<GameObjectStatic>, EnumClassHash> _textObjectHash; 

};

