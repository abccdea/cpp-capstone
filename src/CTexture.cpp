/* File:            CTexture.cpp
 * Author:          Vish Potnis
 * Description:     - Wrapper class for managing SDL texture
 */


#include "CTexture.h"

CTexture::CTexture() : _texture(nullptr, SDL_DestroyTexture)
{}

CTexture::~CTexture()
{
    free();
}

// move constructor for transferring ownership of texture 
CTexture::CTexture(CTexture&& o) : _texture(std::move(o._texture))
{
    _width = o._width;
    _height = o._height;

    o._width = 0;
    o._height = 0;    
}

// create texture from png file
bool CTexture::loadFromFile(SDL_Renderer& renderer, std::string path)
{
    // destroy existing texture
    free();

    // load png into SDL surface
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == nullptr){
        std::cout << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << "\n";
        return false;
    }
    
    // convert SDL surface to SDL texture
    _texture.reset(SDL_CreateTextureFromSurface(&renderer, loadedSurface));
    if(_texture == nullptr){
        std::cout << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << "\n";
        return false;
    }
    
    //get image dimensions
    _width = loadedSurface->w;
    _height = loadedSurface->h;       
    SDL_FreeSurface(loadedSurface);        
    
    return true;

}

// create texture from font file
bool CTexture::loadFromRenderedText(SDL_Renderer& renderer, TTF_Font* font, std::string text, SDL_Color textColor)
{
    free();

    // create SDL surface from text using loaded font and specified color
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if(textSurface == nullptr){
        std::cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << "\n";
        return false;
    }

    // convert SDL surface to SDL texture
    _texture.reset(SDL_CreateTextureFromSurface(&renderer, textSurface));
    if(_texture == nullptr){
        std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << "\n";
        return false;
    }
    
    //get image dimensions
    _width = textSurface->w;
    _height = textSurface->h;       
    SDL_FreeSurface(textSurface);        
    
    return true;
}


// getter functions
SDL_Texture& CTexture::getTexture() const { return *_texture;}
int CTexture::getWidth() const { return _width;}
int CTexture::getHeight() const { return _height;}

// reset class
void CTexture::free()
{
    _texture = nullptr;
    _width = 0;
    _height = 0;
}