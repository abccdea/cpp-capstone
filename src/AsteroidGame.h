/* File:            AsteroidGame.h
 * Author:          Vish Potnis
 * Description:     - Main class for the game
 *                  - Contains main game loop
 *                  - Handle input events
 *                  - Create and manage game onjects
 */

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <unordered_map>
#include <utility>
#include <string>
#include <sstream>
#include <random>

#include "constants.h"
#include "utility.h"
#include "CTexture.h"
#include "GameObject.h"
#include "GameObjectAsteroid.h"
#include "GameObjectShip.h"
#include "GameObjectLaser.h"
#include "GameObjectExplosion.h"
#include "GameObjectStatic.h"

#include "MenuMain.h"
#include "MenuGameOver.h"
#include "MenuPause.h"
#include "MenuNext.h"

class AsteroidGame{

    public:
        AsteroidGame();
        ~AsteroidGame();

        // top level call to run the game
        void run();                

    private:

        //////////// Private functions ///////////////

        bool init();                                        // initialize SDL assets
        bool loadFonts();                                   // load fonts with SDL_ttf
        bool loadSounds();                                  // load sounds with SDL_mixer
        std::string getSoundPath(SoundType type) const;     // utility function for getting file paths
        bool loadTextures();                                // load sprites into texture objects
        std::string getTexturePath(TextureType type) const; // utility function for getting file paths
        
        void runLevel();                    // main game loop

        void handleInput(SDL_Event &e);     // handle keyboard input             
        void renderObjects();               // render all active game objects
        void updateObjects();               // update all non-static game objects based on time delta
        void deleteExpiredObjects();        // delete offscreen lasers or expired explosion animation objects

        void initLevel();                   // initialize level with asteroids and ship based on current level

        // wrappers for static factory method for creating game objects
        void createShip();
        void createLaser(Point pos, CVector velocity);
        void createAsteroid(Point pos, CVector velocity, CTexture& tex, AsteroidSize size, AsteroidColor color);
        void createExplosion(Point pos, AsteroidSize size);

        void checkShipCollision();                                        // check ship <-> asteroid collision
        void checkAsteroidCollision();                                    // check laser <-> asteroid collision
        bool checkCollision(const SDL_Rect &a, const SDL_Rect &b) const;  // check collision between 2 SDL_Rect bounding boxes

        void shootLaser();                              // determine velocity vector to create laser after keyboard input
        void splitAsteroid(GameObjectAsteroid& asteroid);   // split current asteroid into 2 smaller asteroid

        void checkLevelCompleted();         // check if any asteroids are remaining in the level

        void cleanupLevel();                    // clean up game objects
        void cleanup();                         // clean up fonts/sounds and SDL assets

        Point getRandomCorner() const;              // utility function for determining initial position for asteroids
        void updateScore(int scoreIncrease);        // update score and regenerate score texture

        void runMainMenu();                         // display the main menu
        void runGameOverMenu();                     // display the game over menu
        void runNextMenu();                         // display the next level menu
        void runPauseMenu();                        // display the pause menu

        void playLaserSound();                      // play laser sound
        void playExplosionSound();                  // play explosion sound

        ////////// Private variables //////////////

        SDL_Window_unique_ptr _window;          // pointer to the main game window
        SDL_Renderer_unique_ptr _renderer;      // pointer to the GPU renderer

        std::vector<CTexture> _mainTextures;    // vector holding the main loaded textures
        std::vector<TTF_Font*> _mainFonts;      // vector holding the fonts converted by SDL_TTF
        std::vector<Mix_Chunk*> _mainSounds;    // vector holding the loaded sounds
        
        
        std::unique_ptr<GameObjectShip> _pShip;                                         // Game object for the ship
        std::unordered_map<int, std::unique_ptr<GameObjectLaser>> _laserHash;           // Hashmap for active laser objects with a unique ID as the key
        std::unordered_map<int, std::unique_ptr<GameObjectAsteroid>> _asteroidHash;     // Hashmap for active asteroid objects with a unique ID as the key
        std::unordered_map<int, std::unique_ptr<GameObjectExplosion>> _explosionHash;   // Hashmap for active explosion objects with a unique ID as the key        
        std::unique_ptr<GameObjectStatic> _backgroundObject;                            // Game object for the background image

        CTexture _fontTextureLevel;         // loaded font to display level        
        std::unique_ptr<GameObjectStatic> _fontObjectLevel;     // loaded texture/object to display level

        CTexture _fontTextureScore;         // loaded font to display score        
        std::unique_ptr<GameObjectStatic> _fontObjectScore;      // loaded texture/object to display score

        GameState _state;                   // Game state enum 
        AsteroidColor _currentColor;        // Asteroid color enum, determines color for current level

        int _currentLevel;
        int _score;

        

};