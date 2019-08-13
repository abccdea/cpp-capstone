/* File:            constants.h
 * Author:          Vish Potnis
 * Description:     - Constants used in the game
 */

#pragma once 

namespace AsteroidConstants
{
    constexpr int FPS{60};                          // frames per sec limit
    constexpr int TICKS_PER_FRAME = 1000 / FPS;
    
    constexpr int SCREEN_WIDTH{800};
    constexpr int SCREEN_HEIGHT{600};        
    constexpr double PI{3.14159265};

    // init laser and asteroid attributes
    constexpr int INIT_ASTEROID_VELOCITY{100};
    constexpr double ASTEROID_VELOCITY_MULTIPLIER{1.1};
    constexpr int LASER_VELOCITY{500};

    // off screen boundary for deleting laser objects
    constexpr int OFFSCREEN_BOUNDARY{50};

    // font size
    constexpr int FONTSIZE_TITLE1{100};
    constexpr int FONTSIZE_TITLE2{64};
    constexpr int FONTSIZE_MENU{32};
    constexpr int FONTSIZE_TEXT{24};

    // object scaling for ship and laser textures
    constexpr double SCALE_SHIP_W{1.5};
    constexpr double SCALE_SHIP_H{1.5};
    constexpr double SCALE_LASER_W{20};
    constexpr double SCALE_LASER_H{24};

    // position for text objects
    constexpr int FONT_LEVEL_POS_X{10};
    constexpr int FONT_LEVEL_POS_Y{10};
    constexpr int FONT_SCORE_POS_X{SCREEN_WIDTH-210};
    constexpr int FONT_SCORE_POS_Y{10};

    // sprite sheet dimensions
    constexpr int EXPLOSION_SPRITE_WIDTH{64};
    constexpr int EXPLOSION_SPRITE_HEIGHT{64};
    constexpr int EXPLOSION_SPRITE_NUM{25};


} 
