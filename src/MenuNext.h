/* File:            MenuNext.h
 * Author:          Vish Potnis
 * Description:     - Derived class for next level menu
 */

#pragma once

#include "Menu.h"


class MenuNext : public Menu
{
    public:
        // constructor accepts initilized renderer, background image object, and all the loaded fonts
        MenuNext(SDL_Renderer& renderer, const GameObjectStatic& backgroundObject, const std::vector<TTF_Font*>& mainFonts);        
    
    private:

        void initMenuItems() override;  // initialize static objects to be rendered

};
