/* File:            MenuMain.cpp
 * Author:          Vish Potnis
 * Description:     - Derived class for main menu
 */

#include "MenuMain.h"

// constructor accepts initilized renderer, background image object, and all the loaded fonts
MenuMain::MenuMain(SDL_Renderer& renderer, const GameObjectStatic& backgroundObject, const std::vector<TTF_Font*>& mainFonts)
 : Menu(renderer, backgroundObject, mainFonts), _state(true)
{
    initMenuItems();
}

// run menu loop
GameState MenuMain::run()
{
    SDL_Event event;

    while(true){
        // handle keyboard input
        while(SDL_PollEvent(&event) != 0){
            if(event.type == SDL_QUIT){
                return GameState::QUIT;
            }
            else if(event.type == SDL_KEYUP){
                switch(event.key.keysym.sym)
                {
                    case SDLK_w:
                    case SDLK_s:
                    case SDLK_UP:
                    case SDLK_DOWN:
                        toggleState();
                        break;
                    case SDLK_RETURN:
                        return select();
                        break;
                    default:
                        break;
                }
            }
        }
        // render items
        render();
    }
}

// initialize static objects to be rendered
void MenuMain::initMenuItems()
{

    SDL_Color whiteTextColor{255,255,255,255};
    SDL_Color selectTextColor{245,227,66,255};

    const int numItems = 5;

     // initilize CTexture objects
    for(int i = 0; i < numItems; i++){
        _textTextureHash.insert(std::make_pair(static_cast<MenuItem>(i), CTexture()) );
    }

    // generate texture from loaded font using given string and color
    _textTextureHash[MenuItem::TITLE].loadFromRenderedText(_renderer, _mainFonts[static_cast<int>(FontType::TITLE1)], "Asteroids", whiteTextColor);
    _textTextureHash[MenuItem::ITEM1].loadFromRenderedText(_renderer, _mainFonts[static_cast<int>(FontType::MENU)], "Play Game", whiteTextColor);
    _textTextureHash[MenuItem::ITEM2].loadFromRenderedText(_renderer, _mainFonts[static_cast<int>(FontType::MENU)], "Quit", whiteTextColor);
    _textTextureHash[MenuItem::ITEM1_SELECT].loadFromRenderedText(_renderer, _mainFonts[static_cast<int>(FontType::MENU)], "Play Game", selectTextColor);
    _textTextureHash[MenuItem::ITEM2_SELECT].loadFromRenderedText(_renderer, _mainFonts[static_cast<int>(FontType::MENU)], "Quit", selectTextColor);

    // position of text objects
    Point titlePos{ static_cast<double>((AsteroidConstants::SCREEN_WIDTH -  _textTextureHash[MenuItem::TITLE].getWidth())/2),
                    static_cast<double>((AsteroidConstants::SCREEN_HEIGHT - _textTextureHash[MenuItem::TITLE].getHeight())/3)};
    
    Point item1Pos{  static_cast<double>((AsteroidConstants::SCREEN_WIDTH -  _textTextureHash[MenuItem::ITEM1].getWidth())/2),
                    static_cast<double>((AsteroidConstants::SCREEN_HEIGHT - _textTextureHash[MenuItem::ITEM1].getHeight())/2)};

    Point item2Pos{  static_cast<double>((AsteroidConstants::SCREEN_WIDTH - _textTextureHash[MenuItem::ITEM2].getWidth())/2),
                    static_cast<double>((AsteroidConstants::SCREEN_HEIGHT - _textTextureHash[MenuItem::ITEM2].getHeight())/1.8)};

    // generate static text objects from textures
    _textObjectHash[MenuItem::TITLE] = createStaticTextObject(titlePos, _textTextureHash[MenuItem::TITLE]); 
    _textObjectHash[MenuItem::ITEM1] = createStaticTextObject(item1Pos, _textTextureHash[MenuItem::ITEM1]);
    _textObjectHash[MenuItem::ITEM2] = createStaticTextObject(item2Pos, _textTextureHash[MenuItem::ITEM2]);
    _textObjectHash[MenuItem::ITEM1_SELECT] = createStaticTextObject(item1Pos, _textTextureHash[MenuItem::ITEM1_SELECT]);
    _textObjectHash[MenuItem::ITEM2_SELECT] = createStaticTextObject(item2Pos, _textTextureHash[MenuItem::ITEM2_SELECT]);

}

// render text objects based on selection state
void MenuMain::renderMenuItems()
{
    
    _textObjectHash[MenuItem::TITLE]->render(_renderer);
    if(_state){
        _textObjectHash[MenuItem::ITEM1_SELECT]->render(_renderer);
        _textObjectHash[MenuItem::ITEM2]->render(_renderer);
    }
    else{
        _textObjectHash[MenuItem::ITEM1]->render(_renderer);
        _textObjectHash[MenuItem::ITEM2_SELECT]->render(_renderer);
    }
}

// toggle menu selection
void MenuMain::toggleState()
{
    _state = !_state;
}

// return menu selection
GameState MenuMain::select()
{
    if(_state){
        return GameState::RUNNING;
    }
    else{
        return GameState::QUIT;
    }
}

