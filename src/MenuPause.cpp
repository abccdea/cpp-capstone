/* File:            MenuPause.cpp
 * Author:          Vish Potnis
 * Description:     - Derived class for pause menu
 */

#include "MenuPause.h"

// constructor accepts initilized renderer, background image object, and all the loaded fonts
MenuPause::MenuPause(SDL_Renderer& renderer, const GameObjectStatic& backgroundObject, const std::vector<TTF_Font*>& mainFonts)
 :Menu(renderer, backgroundObject, mainFonts)
{
    initMenuItems();
}

// initialize static objects to be rendered
void MenuPause::initMenuItems()
{
    SDL_Color whiteTextColor{255,255,255,255};

    const int numItems = 2;

    // initilize CTexture objects
    for(int i = 0; i < numItems; i++){
        _textTextureHash.insert(std::make_pair(static_cast<MenuItem>(i), CTexture()) );
    }

    // generate texture from loaded font using given string and color
    _textTextureHash[MenuItem::TITLE].loadFromRenderedText(_renderer, _mainFonts[static_cast<int>(FontType::TITLE2)], "Paused", whiteTextColor);
    _textTextureHash[MenuItem::ITEM1].loadFromRenderedText(_renderer, _mainFonts[static_cast<int>(FontType::MENU)], "Press Enter...", whiteTextColor);

    // position of text objects
    Point titlePos{ static_cast<double>((AsteroidConstants::SCREEN_WIDTH -  _textTextureHash[MenuItem::TITLE].getWidth())/2),
                    static_cast<double>((AsteroidConstants::SCREEN_HEIGHT - _textTextureHash[MenuItem::TITLE].getHeight())/3)};
    
    Point item1Pos{  static_cast<double>((AsteroidConstants::SCREEN_WIDTH -  _textTextureHash[MenuItem::ITEM1].getWidth())/2),
                    static_cast<double>((AsteroidConstants::SCREEN_HEIGHT - _textTextureHash[MenuItem::ITEM1].getHeight())/2)};

    // generate static text objects from textures
    _textObjectHash[MenuItem::TITLE] = createStaticTextObject(titlePos, _textTextureHash[MenuItem::TITLE]); 
    _textObjectHash[MenuItem::ITEM1] = createStaticTextObject(item1Pos, _textTextureHash[MenuItem::ITEM1]); 

}
