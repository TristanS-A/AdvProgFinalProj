//
// Created by tropi on 4/4/2023.
//

#ifndef ADVPROGFINPROJ_GLOBALVARIABLES_H
#define ADVPROGFINPROJ_GLOBALVARIABLES_H

#include "SDL.h"
#include "SDL_ttf.h"

//Small font for game
extern TTF_Font *smallFont;

//Medium font for game
extern TTF_Font *mediumFont;

//Large font for game
extern TTF_Font *largeFont;

//Pixel sizes for fonts
extern const int SMALL_FONT_SIZE;
extern const int MEDIUM_FONT_SIZE;
extern const int LARGE_FONT_SIZE;

//To convert the pixel size of the font to width instead of hight (I think)
extern const float FONT_PIXEL_HEIGHT_TO_WIDTH;

//To keep track of if the mouse is pressed
extern bool mouseDown;

//To keep track of if the mouse button is being held down
extern bool mouseHeldDown;

extern SDL_Surface* buttonIMG;

extern SDL_Surface* buttonHoverIMG;

extern SDL_Surface* backButtonIMG;

extern SDL_Surface* backButtonHoverIMG;

#endif //ADVPROGFINPROJ_GLOBALVARIABLES_H
