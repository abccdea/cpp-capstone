/* File:            utility.h
 * Author:          Vish Potnis
 * Description:     - Helper functions and enums
 */

#pragma once

#include <SDL.h>
#include <memory>

// smart pointer typedefs with approriate deleter functions for SDL
using SDL_Window_unique_ptr = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
using SDL_Renderer_unique_ptr = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;
using SDL_Texture_unique_ptr = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;

// helper function for casting a smart pointer from a Base class to a Derived class
template<typename Derived, typename Base>
std::unique_ptr<Derived> static_unique_ptr_cast( std::unique_ptr<Base>&& p )
{
    auto d = static_cast<Derived *>(p.release());
    return std::unique_ptr<Derived>(d);
}

// used for defining hash function for an enum
// used in unordered map
struct EnumClassHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};

// menu items for main, pause, next, and game over menus
enum class MenuItem
{
    TITLE,
    ITEM1,
    ITEM2,
    ITEM1_SELECT,
    ITEM2_SELECT,
    ITEM_TOTAL
};

// defined position of object
struct Point{
    double x;
    double y;
};

// game state used for determining menu interaction and level progression
enum class GameState
{
    QUIT,
    LEVEL_COMPLETE,
    GAMEOVER,
    RUNNING,
    PLAY_AGAIN
};

// used in the factory method for generating game objects
enum class ObjectType
{
    STATIC,
    ASTEROID,
    SHIP,
    LASER,
    EXPLOSION
};

// asteroid attribute
enum class AsteroidSize
{
    BIG,
    MED,
    SMALL
};

// asteroid attribute
enum class AsteroidColor
{
    GREY,
    RED,
    BROWN
};

// loaded sounds
enum class SoundType
{
    LASER,
    EXPLOSION,
    SOUND_TOTAL
};

// loaded fonts
enum class FontType
{
    TITLE1,
    TITLE2,
    MENU,
    TEXT,
    FONT_TOTAL
};

// loaded textures
enum class TextureType
{    
    TEX_ASTEROID_BIG_1,
    TEX_ASTEROID_MED_1,
    TEX_ASTEROID_SMALL_1,
    TEX_ASTEROID_BIG_2,
    TEX_ASTEROID_MED_2,
    TEX_ASTEROID_SMALL_2,
    TEX_ASTEROID_BIG_3,
    TEX_ASTEROID_MED_3,
    TEX_ASTEROID_SMALL_3,
    TEX_SHIP,
    TEX_LASER,
    TEX_BACKGROUND,
    TEX_EXPLOSION_SPRITE_SHEET,
    TEX_TOTAL
};