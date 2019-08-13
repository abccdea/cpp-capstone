/* File:            MenuMain.h
 * Author:          Vish Potnis
 * Description:     - Derived class for main menu
 */

#pragma once

#include "Menu.h"


class MenuMain : public Menu
{
    public:
        // constructor accepts initilized renderer, background image object, and all the loaded fonts
        MenuMain(SDL_Renderer& renderer, const GameObjectStatic& backgroundObject, const std::vector<TTF_Font*>& mainFonts);

        GameState run() override;    // run menu loop
    
    private:

        void initMenuItems() override;      // initialize static objects to be rendered
        void renderMenuItems() override;    // render text objects

        void toggleState();      // toggle menu selection
        GameState select();      // return menu selection

        bool _state;     // current selection (binary choice)
};

