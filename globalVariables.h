//
// Created by tropi on 4/4/2023.
//

#ifndef ADVPROGFINPROJ_GLOBALVARIABLES_H
#define ADVPROGFINPROJ_GLOBALVARIABLES_H

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "pokemon.h"
#include "player.h"
#include <string>
#include <vector>
#include <random>

using namespace std;

//Dimensions for the screen
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

//Creates Rect for windowSurf surface so that its dimensions can be rescaled and repositioned
extern SDL_Rect windowTextureSize;

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

extern uniform_real_distribution<double> randomChanceRange;
extern uniform_real_distribution<double> randomLevelRange;

//To keep track of if the mouse is pressed
extern bool mouseDown;

//To keep track of if the mouse button is being held down
extern bool mouseHeldDown;

//So that holding down a key will not skip through messages
extern bool noSkip;

//Sets up key presses
extern const Uint8 *keystates;

//Quadrants of the map
extern int topLeftQuadrant[2];
extern int topRightQuadrant[2];
extern int bottomLeftQuadrant[2];
extern int bottomRightQuadrant[2];

//Images for the battle backgrounds in each quadrant
extern SDL_Surface* battleBackground_Forest;
extern SDL_Surface* battleBackground_Swamp;
extern SDL_Surface* battleBackground_Snow;
extern SDL_Surface* battleBackground_Fire;

//The current battle background
extern SDL_Surface* currBattleBackground;

//Image for the bakcground
extern SDL_Surface* background;

//Pointer for the player
extern Player* player;

//Position and size of the background
extern SDL_Rect bgPos;

//To send messages in battle;
extern vector<string> messageList;

extern int amountOfMovesPerType;

extern int amountOfNamesPerType;

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

extern SDL_Surface* titleScreenImage;

extern SDL_Surface* startButtonIMG;

extern SDL_Surface* startButtonHoverIMG;

extern SDL_Surface* chooseScreen;

extern SDL_Surface* curtain;

extern SDL_Surface* pokecenter;

extern SDL_Surface* spaceButton;

extern SDL_Surface* textboxIMG;

extern SDL_Surface* textSurf;

extern Mix_Music* titleMusic;

extern Mix_Music* worldMusic;

extern Mix_Music* battleMusic;

extern Mix_Chunk* currSound;

extern Mix_Chunk* battleIntro;

extern Mix_Chunk* critHit;

extern Mix_Chunk* weakHit;

extern Mix_Chunk* regularHit;

extern Mix_Chunk* buttonSound;

extern Mix_Chunk* healSound;

extern Mix_Chunk* pokecenterSound;

#endif //ADVPROGFINPROJ_GLOBALVARIABLES_H
