/* File:            CTexture.h
 * Author:          Vish Potnis
 * Description:     - Wrapper class for managing SDL texture
 */

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <string>
#include <memory>

#include "constants.h"
#include "utility.h"


class CTexture
{
    public:
        CTexture();
        ~CTexture();

        // delete copy and assignment consturctors
        CTexture(const CTexture& a) = delete;
        CTexture& operator=(const CTexture& ) = delete;

        // move constructor for transferring ownership of texture 
        CTexture(CTexture&& o);
    
        // create texture from png file
        bool loadFromFile(SDL_Renderer& renderer, std::string path);

        // create texture from font file
        bool loadFromRenderedText(SDL_Renderer& renderer, TTF_Font* font, std::string text, SDL_Color textColor);

        // getter functions
        SDL_Texture& getTexture() const;        
        int getWidth() const;
        int getHeight() const;
        
        void free();        

    private:
        
        SDL_Texture_unique_ptr _texture;    // smart pointer holding the texture object
    
        int _width{0};      // width of texture
        int _height{0};     // height of texture

};