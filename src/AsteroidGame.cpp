/* File:            AsteroidGame.cpp
 * Author:          Vish Potnis
 * Description:     - Main class for the game
 *                  - Contains main game loop
 *                  - Handle input events
 *                  - Create and manage game onjects
 */

#include "AsteroidGame.h"

//////////// Public functions ////////////

// initalize SDL assets, load textures, load fonts, create background image object
AsteroidGame::AsteroidGame()
    : _window(nullptr, SDL_DestroyWindow), _renderer(nullptr, SDL_DestroyRenderer),
      _state(GameState::RUNNING), _currentColor(AsteroidColor::GREY), _currentLevel(1), _score(0)
{
    if(!init())
        exit(0);
    if(!loadTextures())
        exit(0);
    if(!loadSounds())
        exit(0);
    if(!loadFonts())
        exit(0);

    Point backgroundPos{0,0};
    std::unique_ptr<GameObject> pGameObject = GameObject::Create(ObjectType::STATIC, backgroundPos, _mainTextures[static_cast<int>(TextureType::TEX_BACKGROUND)]);
    _backgroundObject = static_unique_ptr_cast<GameObjectStatic, GameObject>(std::move(pGameObject));    

}

AsteroidGame::~AsteroidGame()
{
    cleanup();
}


// top level call to run the game
void AsteroidGame::run()
{
    // initialize the game with a main menu
    runMainMenu();
    
    // run level, if level is complete show next level menu
    // if game over during the level then show game over menu
    while(_state == GameState::RUNNING){
        initLevel();        
        runLevel();
        if(_state == GameState::LEVEL_COMPLETE){
            runNextMenu();
        }
        if(_state == GameState::GAMEOVER){
            runGameOverMenu();
        }
    }

}

//////////// Private functions ////////////


// initialize SDL assets
bool AsteroidGame::init()
{

    // initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }
    // set texture filtering to linear
    if(!SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" )){
		std::cout << "Warning: Linear texture filtering not enabled!\n";
	}

    // initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)){
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << "\n";
        return false;
    }

    // initialize SDL_mixer
    if( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << "\n";
        return false;
    }

    // initialize SDL_ttf
    if(TTF_Init() == -1){
        std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " <<  TTF_GetError() << "\n";
        return false;
    }

    // Create window
    _window.reset(SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, AsteroidConstants::SCREEN_WIDTH, AsteroidConstants::SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
    if(_window == nullptr){
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    // Create renderer for window
    _renderer.reset(SDL_CreateRenderer(_window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    if(_renderer == nullptr){
        std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << "\n";
        return false;
    }        

    return true;
}

// load fonts with SDL_ttf
bool AsteroidGame::loadFonts()
{
    bool success = true;

    for(unsigned int i = 0; i < static_cast<unsigned int>(FontType::FONT_TOTAL); i++){
        TTF_Font *pFont = nullptr;
        switch(static_cast<FontType>(i)){
            case FontType::TITLE1:
                pFont = TTF_OpenFont( "fonts/Alexis Laser Italic.ttf", AsteroidConstants::FONTSIZE_TITLE1);
                break;
            case FontType::TITLE2:
                pFont = TTF_OpenFont( "fonts/Alexis Italic.ttf", AsteroidConstants::FONTSIZE_TITLE2);
                break;
            case FontType::MENU:
                pFont = TTF_OpenFont( "fonts/Alexis.ttf", AsteroidConstants::FONTSIZE_MENU);
                break;
            case FontType::TEXT:
                pFont = TTF_OpenFont( "fonts/Alexis.ttf", AsteroidConstants::FONTSIZE_TEXT);
                break;
            default: break;
        }
        if(pFont == nullptr){
            std::cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << "\n";		
		    success &= false;
	    }
        else{
            _mainFonts.push_back(pFont);
        }        
    }

    return success;
}

// load sounds with SDL_mixer
bool AsteroidGame::loadSounds()
{
    bool success = true;

    for(unsigned int i = 0; i < static_cast<unsigned int>(SoundType::SOUND_TOTAL); i++){
        Mix_Chunk *pSound = Mix_LoadWAV(getSoundPath(static_cast<SoundType>(i)).c_str());

        if(pSound == nullptr){
            std::cout << "Failed to load sound effect! SDL_mixer Error: " << Mix_GetError() << "\n";    
            success &= false;
        }
        else{
            _mainSounds.push_back(pSound);
        }
    }    
    return success;
}

// utility function for getting file paths
std::string AsteroidGame::getSoundPath(SoundType type) const
{
    switch(type){
        case SoundType::LASER:      return "sounds/laser.wav";
        case SoundType::EXPLOSION:  return "sounds/explosion.wav";
        default:                    return "";
    }
}   

// load sprites into texture objects
bool AsteroidGame::loadTextures()
{
    bool success = true;

    for(unsigned int i = 0; i < static_cast<unsigned int>(TextureType::TEX_TOTAL); i++){
        CTexture tmp;
        success &= tmp.loadFromFile(*_renderer, getTexturePath(static_cast<TextureType>(i)));
        _mainTextures.push_back(std::move(tmp));
    }
    return success;
}

// utility function for getting file paths
std::string AsteroidGame::getTexturePath(TextureType type) const
{
    switch(type){
        case TextureType::TEX_BACKGROUND:               return "img/background.png";
        case TextureType::TEX_ASTEROID_BIG_1:           return "img/asteroid_big1.png";
        case TextureType::TEX_ASTEROID_BIG_2:           return "img/asteroid_big2.png";
        case TextureType::TEX_ASTEROID_BIG_3:           return "img/asteroid_big3.png";
        case TextureType::TEX_ASTEROID_MED_1:           return "img/asteroid_med1.png";
        case TextureType::TEX_ASTEROID_MED_2:           return "img/asteroid_med2.png";
        case TextureType::TEX_ASTEROID_MED_3:           return "img/asteroid_med3.png";
        case TextureType::TEX_ASTEROID_SMALL_1:         return "img/asteroid_small1.png";
        case TextureType::TEX_ASTEROID_SMALL_2:         return "img/asteroid_small2.png";
        case TextureType::TEX_ASTEROID_SMALL_3:         return "img/asteroid_small3.png";
        case TextureType::TEX_LASER:                    return "img/laser.png";
        case TextureType::TEX_SHIP:                     return "img/ship.png";
        case TextureType::TEX_EXPLOSION_SPRITE_SHEET:   return "img/explosion_sheet.png";
        default:                                        return "";
    }
}

// main game loop
void AsteroidGame::runLevel()
{
    SDL_Event event;

    while(_state == GameState::RUNNING){

        Uint32 startTick = SDL_GetTicks();
            
        handleInput(event);

        updateObjects();
        renderObjects();
        deleteExpiredObjects();

        checkShipCollision();            
        checkAsteroidCollision();      

        checkLevelCompleted(); 

        // limit FPS
        Uint32 endTick = SDL_GetTicks();
        Uint32 frameTicks = endTick - startTick;        
        if(frameTicks < AsteroidConstants::TICKS_PER_FRAME){
            SDL_Delay(AsteroidConstants::TICKS_PER_FRAME - frameTicks);
        }
    }
    cleanupLevel();
}

// handle keyboard input             
void AsteroidGame::handleInput(SDL_Event &event)
{

    while(SDL_PollEvent(&event) != 0){
        if(event.type == SDL_QUIT){
            _state = GameState::QUIT;
        }
        else if(event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_a:    _pShip->setRotateLeft(true);    break;
                case SDLK_d:    _pShip->setRotateRight(true);   break;
                case SDLK_w:    _pShip->setMoveForward(true);   break;
                case SDLK_s:    _pShip->setMoveBackward(true);  break;
                default:                                        break;
            }
        }
        else if(event.type == SDL_KEYUP)
        {
            switch(event.key.keysym.sym)
            {
                case SDLK_a:        _pShip->setRotateLeft(false);   break;
                case SDLK_d:        _pShip->setRotateRight(false);  break;
                case SDLK_w:        _pShip->setMoveForward(false);  break;
                case SDLK_s:        _pShip->setMoveBackward(false); break;
                case SDLK_SPACE:    shootLaser();                   break;
                case SDLK_ESCAPE:   runPauseMenu();                 break;
                default:                                            break;

            }
        }
    }
}

// render all active game objects
void AsteroidGame::renderObjects()
{
    // clear screen
    SDL_SetRenderDrawColor( _renderer.get(), 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear( _renderer.get() );

    // render background image
    SDL_Rect backgroundRect{0,0,AsteroidConstants::SCREEN_WIDTH, AsteroidConstants::SCREEN_HEIGHT};
    _backgroundObject->render(*_renderer, backgroundRect);

    // render explosions
    for(auto& explosion: _explosionHash){
        explosion.second->render(*_renderer);        
    }

    // render asteroids
    for(auto const& asteroid: _asteroidHash){
        asteroid.second->render(*_renderer);
    }

    // render lasers
    for(auto const& laser: _laserHash){
        laser.second->render(*_renderer);
    }    
    // render ship
    _pShip->render(*_renderer);

    // render level and score text
    _fontObjectLevel->render(*_renderer);
    _fontObjectScore->render(*_renderer);

    // update screen
    SDL_RenderPresent( _renderer.get() );
}

// update all non-static game objects based on time delta
void AsteroidGame::updateObjects()
{
    Uint32 time = SDL_GetTicks();

    // update asteroid position
    for(auto& asteroid: _asteroidHash){
        asteroid.second->update(time);
    }
    // update laser position
    for(auto& laser: _laserHash){
        laser.second->update(time);
    }
    // update explosion animation
    for(auto& explosion: _explosionHash){
        explosion.second->update(time);
    }
    // update ship position based on current movement booleans
    _pShip->update(time);

}

// delete offscreen lasers or expired explosion animation objects
void AsteroidGame::deleteExpiredObjects()
{
    // check for expired explosion animations and delete the game object
    std::vector<int> expiredExplosionID;
    for(auto& explosion: _explosionHash){
        if(explosion.second->isAnimationDone()){
            expiredExplosionID.push_back(explosion.first);
        }
    }
    for(int id: expiredExplosionID){
        _explosionHash.erase(id);
    }

    // check for offscreen lasers and delete the game object
    std::vector<int> expiredLaserID;
    for(auto& laser: _laserHash){
        if(laser.second->checkOffscreen()){
            expiredLaserID.push_back(laser.first);
        }
    }
    for(int id: expiredLaserID){
        _laserHash.erase(id);
    }
}

// initialize level with asteroids and ship based on current level
void AsteroidGame::initLevel()
{

    // set number of asteroids equal to current level 
    int numAsteroid = _currentLevel;

    // velocity is based on current level multiplier
    double asteroidVelocity = AsteroidConstants::INIT_ASTEROID_VELOCITY * std::pow(AsteroidConstants::ASTEROID_VELOCITY_MULTIPLIER, _currentLevel-1);

    // random starting position
    Point pos = getRandomCorner();

    // random angle for the velocity vector
    std::random_device rd;
    std::uniform_int_distribution<> randomAngle(0, 360);                

    // create the asteroids and ship
    AsteroidSize size = AsteroidSize::BIG;
    CTexture& tex = _mainTextures[static_cast<int>(GameObjectAsteroid::getAsteroidTexture(size, _currentColor))];    

    for(int i = 0; i < numAsteroid; i++){
        double angle = static_cast<double>(randomAngle(rd));
        CVector velocity{asteroidVelocity, angle, VectorType::POLAR};

        createAsteroid(pos, velocity, tex, size, _currentColor);
    }
    createShip();

    SDL_Color whiteTextColor{255,255,255,255};

    // create font object for level text
    std::stringstream ss("");
    ss << "Level: " << _currentLevel;
    _fontTextureLevel.loadFromRenderedText(*_renderer, _mainFonts[static_cast<int>(FontType::MENU)], ss.str(), whiteTextColor);

    Point levelPos{AsteroidConstants::FONT_LEVEL_POS_X, AsteroidConstants::FONT_LEVEL_POS_Y};
    std::unique_ptr<GameObject> pGameObject = GameObject::Create(ObjectType::STATIC, levelPos, _fontTextureLevel); 
    _fontObjectLevel = static_unique_ptr_cast<GameObjectStatic, GameObject>(std::move(pGameObject));    

    // create font object for score text
    ss.str("");
    ss << "Score: " << std::setw(5) << _score;
    _fontTextureScore.loadFromRenderedText(*_renderer, _mainFonts[static_cast<int>(FontType::MENU)], ss.str(), whiteTextColor);

    Point scorePos{AsteroidConstants::FONT_SCORE_POS_X, AsteroidConstants::FONT_SCORE_POS_Y};    
    std::unique_ptr<GameObject> pGameObject2 = GameObject::Create(ObjectType::STATIC, scorePos, _fontTextureScore); 
    _fontObjectScore = static_unique_ptr_cast<GameObjectStatic, GameObject>(std::move(pGameObject2));

}

// wrapper for factory method for creating ship object
void AsteroidGame::createShip()
{
    Point pos{AsteroidConstants::SCREEN_WIDTH/2, AsteroidConstants::SCREEN_HEIGHT/2};
    CVector velocity{0,0,VectorType::POLAR};

    CTexture& tex = _mainTextures[static_cast<int>(TextureType::TEX_SHIP)];

    std::unique_ptr<GameObject> pGameObject = GameObject::Create(ObjectType::SHIP, pos, tex, velocity);
    _pShip = static_unique_ptr_cast<GameObjectShip, GameObject>(std::move(pGameObject));

}

// wrapper for factory method for creating laser objects
void AsteroidGame::createLaser(Point pos, CVector velocity)
{
    CTexture& tex = _mainTextures[static_cast<int>(TextureType::TEX_LASER)];

    std::unique_ptr<GameObject> pLaserGO = GameObject::Create(ObjectType::LASER, pos, tex, velocity, velocity.getAngle() + 90);         
    std::unique_ptr<GameObjectLaser> pLaser = static_unique_ptr_cast<GameObjectLaser, GameObject>(std::move(pLaserGO));

    _laserHash.insert(std::make_pair( pLaser->getID(), std::move(pLaser)) );
}

// wrapper for factory method for creating asteroid objects
void AsteroidGame::createAsteroid(Point pos, CVector velocity, CTexture& tex, AsteroidSize size, AsteroidColor color)
{
    std::unique_ptr<GameObject> pAsteroidGO = GameObject::Create(ObjectType::ASTEROID, pos, tex, velocity);
    std::unique_ptr<GameObjectAsteroid> pAsteroid = static_unique_ptr_cast<GameObjectAsteroid, GameObject>(std::move(pAsteroidGO));

    pAsteroid->setAsteroidAttr(size, color);

    _asteroidHash.insert(std::make_pair(pAsteroid->getID(), std::move(pAsteroid)) );
}

// wrapper for factory method for creating explosion objects
void AsteroidGame::createExplosion(Point pos, AsteroidSize size)
{   
    CTexture& tex = _mainTextures[static_cast<int>(TextureType::TEX_EXPLOSION_SPRITE_SHEET)];
    
    std::unique_ptr<GameObject> pExplosionGO = GameObject::Create(ObjectType::EXPLOSION, pos, tex);
    std::unique_ptr<GameObjectExplosion> pExplosion = static_unique_ptr_cast<GameObjectExplosion, GameObject>(std::move(pExplosionGO));

    pExplosion->setSize(size);

    _explosionHash.insert(std::make_pair(pExplosion->getID(), std::move(pExplosion)) );
}


// check ship <-> asteroid collision
void AsteroidGame::checkShipCollision()
{
    // check if the bounding box for the ship overlaps with any of the asteroid bounding boxes
    const SDL_Rect &shipRect = _pShip->getBoundingBox();
    
    for(auto const &asteroid: _asteroidHash){
        const std::vector<SDL_Rect> &boxes = asteroid.second->getBoundingBoxes();
        for(const SDL_Rect &box: boxes){
            if(checkCollision(shipRect, box)){
                _state = GameState::GAMEOVER;
                return;
            }
                
        }
    }
}

// check laser <-> asteroid collision
void AsteroidGame::checkAsteroidCollision()
{
    std::vector<int> asteroidCollideIdx;
    std::vector<int> laserCollideIdx;

    // iterate through all the onscreen active lasers
    for(const auto &laser: _laserHash){
        const SDL_Rect &laserRect = laser.second->getBoundingBox();
        bool collide = false;

        // check if current laser collides with an asteroid
        // if there is a collision store the asteroid and laser IDs
        for(const auto &asteroid: _asteroidHash){
            const std::vector<SDL_Rect> &boxes = asteroid.second->getBoundingBoxes();
            for(const SDL_Rect &box: boxes){
                if(checkCollision(laserRect, box)){
                    asteroidCollideIdx.push_back(asteroid.first);
                    collide = true;
                    break;
                }                    
            }
            if(collide){
                laserCollideIdx.push_back(laser.first);
                break;
            }

        }
    }

    // for every destroyed asteroid split it into smaller ones and update score
    for(int idx: asteroidCollideIdx){
        splitAsteroid(*_asteroidHash[idx].get());
        _asteroidHash.erase(idx);
        updateScore(10);
    }
    // delete colided lasers
    for(int idx: laserCollideIdx){
        _laserHash.erase(idx);
    }
   
}

// check collision between 2 SDL_Rect bounding boxes
bool AsteroidGame::checkCollision(const SDL_Rect &a, const SDL_Rect &b) const
{
    // calculate the sides of rect A
    int leftA = a.x;
    int rightA = a.x + a.w;
    int topA = a.y;
    int bottomA = a.y + a.h;

    // calculate the sides of rect B
    int leftB = b.x;
    int rightB = b.x + b.w;
    int topB = b.y;
    int bottomB = b.y + b.h;

    // if any of the sides from A are outside of B then there is no collision
    if(bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB){
        return false;
    }
    return true;
}


// determine velocity vector to create laser after keyboard input
void AsteroidGame::shootLaser()
{
    Point laserPos = _pShip->getPos();
    double velocityAngle = _pShip->getRotation() - 90;

    CVector velocity{AsteroidConstants::LASER_VELOCITY, velocityAngle, VectorType::POLAR};

    createLaser(laserPos, velocity);
    playLaserSound();    
}

// split current asteroid into 2 smaller asteroids
void AsteroidGame::splitAsteroid(GameObjectAsteroid& asteroid)
{
    playExplosionSound();
    AsteroidSize currentSize = asteroid.getSize();
    Point pos = asteroid.getPos();

    // if current asteroid is the smallest size then only create an explosion
    if(currentSize == AsteroidSize::SMALL){
        createExplosion(pos, currentSize);
        return;
    }

    // otherwise create 2 asteroids that split off at 45 degree angles and create an explosion

    AsteroidSize nextSize = asteroid.getNextSize();

    int nextTexIdx = static_cast<int>(GameObjectAsteroid::getAsteroidTexture(nextSize, _currentColor));
    CTexture& tex = _mainTextures[nextTexIdx];
    
    CVector currentVelocity = asteroid.getVelocity();
    CVector velocity1(currentVelocity.getMag(), currentVelocity.getAngle() - 45, VectorType::POLAR);
    CVector velocity2(currentVelocity.getMag(), currentVelocity.getAngle() + 45, VectorType::POLAR);

    createExplosion(pos, currentSize);
    createAsteroid(pos, velocity1, tex, nextSize, _currentColor);
    createAsteroid(pos, velocity2, tex, nextSize, _currentColor);

}

// level is completed if no asteroids are remaining in the level
void AsteroidGame::checkLevelCompleted()
{
    if(_asteroidHash.size() == 0){
        _state = GameState::LEVEL_COMPLETE;
        _currentLevel++;
        _currentColor = GameObjectAsteroid::getNextColor(_currentColor);
    }
}


// clean up game objects
void AsteroidGame::cleanupLevel()
{
    _explosionHash.clear();
    _laserHash.clear();
    _asteroidHash.clear();
}


// clean up fonts/sounds and SDL assets
void AsteroidGame::cleanup()
{
    cleanupLevel();

    for(auto& sound: _mainSounds){
        Mix_FreeChunk(sound);
    }

    for(auto& font: _mainFonts){
        TTF_CloseFont(font);
    }

    // Quit SDL subsystems    
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

// utility function for determining initial position for asteroids
// randomly give one of the four corners
Point AsteroidGame::getRandomCorner() const
{
    std::random_device rd;
    std::uniform_int_distribution<> rdCorner(0, 3);

    int corner = rdCorner(rd);

    switch(corner){
        case 0:     return Point{100, 100};
        case 1:     return Point{100, AsteroidConstants::SCREEN_HEIGHT-100};
        case 2:     return Point{AsteroidConstants::SCREEN_WIDTH-100, 100};
        case 3:     return Point{AsteroidConstants::SCREEN_WIDTH-100, AsteroidConstants::SCREEN_HEIGHT-100};
        default:    return Point{0, 0};
    }
}

// update score and regenerate score texture
void AsteroidGame::updateScore(int scoreIncrease)
{
    _score += scoreIncrease;

    SDL_Color whiteTextColor{255,255,255,255};
    std::stringstream ss("");
    ss << "Score: " << std::setw(5) << _score;
    _fontTextureScore.loadFromRenderedText(*_renderer, _mainFonts[static_cast<int>(FontType::MENU)], ss.str(), whiteTextColor);
}


// display the main menu
void AsteroidGame::runMainMenu()
{
    MenuMain mainMenu(*_renderer, *_backgroundObject, _mainFonts);
    _state = mainMenu.run();
}

// display the gave over menu
void AsteroidGame::runGameOverMenu()
{
    MenuGameOver gameOverMenu(*_renderer, *_backgroundObject, _mainFonts);
    _state = gameOverMenu.run();
    if(_state == GameState::PLAY_AGAIN){
        _currentLevel = 1;
        _score = 0;
        _state = GameState::RUNNING;
    }
}

// display the next level menu
void AsteroidGame::runNextMenu()
{
    MenuNext nextMenu(*_renderer, *_backgroundObject, _mainFonts);
    _state = nextMenu.run();
}
    
// display the pause menu
void AsteroidGame::runPauseMenu()
{
    MenuPause pauseMenu(*_renderer, *_backgroundObject, _mainFonts);
    _state = pauseMenu.run();
}

// play laser sound
void AsteroidGame::playLaserSound()
{
    Mix_PlayChannel(-1, _mainSounds[static_cast<int>(SoundType::LASER)], 0);
}                      
    
// play explosion sound
void AsteroidGame::playExplosionSound()
{
    Mix_PlayChannel(-1, _mainSounds[static_cast<int>(SoundType::EXPLOSION)], 0);
}