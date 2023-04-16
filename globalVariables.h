//
// Created by tropi on 4/4/2023.
//

#ifndef ADVPROGFINPROJ_GLOBALVARIABLES_H
#define ADVPROGFINPROJ_GLOBALVARIABLES_H

#include "SDL.h"
#include "SDL_ttf.h"
#include <string>
#include <vector>

using namespace std;

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

//So that holding down a key will not skip through messages
extern bool noSkip;

//Sets up key presses
extern const Uint8 *keystates;

//To send messages in battle;
extern vector<string> messageList;

extern int amountOfMovesPerType;

extern SDL_Surface* buttonIMG;

extern SDL_Surface* buttonHoverIMG;

extern SDL_Surface* backButtonIMG;

extern SDL_Surface* backButtonHoverIMG;

extern SDL_Surface* attackButtonIMG;
extern SDL_Surface* attackButtonHoverIMG;

extern SDL_Surface* itemsButtonIMG;
extern SDL_Surface* itemsButtonHoverIMG;

extern SDL_Surface* catchButtonIMG;
extern SDL_Surface* catchButtonHoverIMG;

extern SDL_Surface* swapButtonIMG;
extern SDL_Surface* swapButtonHoverIMG;

extern SDL_Surface* runButtonIMG;
extern SDL_Surface* runButtonHoverIMG;

extern SDL_Surface* leftButtonIMG;
extern SDL_Surface* leftButtonHoverIMG;

extern SDL_Surface* rightButtonIMG;
extern SDL_Surface* rightButtonHoverIMG;

extern SDL_Surface* pokeInfoBG;

#endif //ADVPROGFINPROJ_GLOBALVARIABLES_H
