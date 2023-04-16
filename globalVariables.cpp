//
// Created by tropi on 4/4/2023.
//

#include "globalVariables.h"
#include "SDL_image.h"

//Small font for game
TTF_Font *smallFont;

//Medium font for game
TTF_Font *mediumFont;

//Large font for game
TTF_Font *largeFont;

//Pixel sizes for fonts
const int SMALL_FONT_SIZE = 20;
const int MEDIUM_FONT_SIZE = 28;
const int LARGE_FONT_SIZE = 50;

//To convert the pixel size of the font to width instead of hight (I think)
const float FONT_PIXEL_HEIGHT_TO_WIDTH = 1.3;

//To keep track of if the mouse is pressed
bool mouseDown = false;

//To keep track of if the mouse button is being held down
bool mouseHeldDown = false;

//So that holding down a key will not skip through messages
bool noSkip = true;

//Sets up key presses
const Uint8 *keystates{SDL_GetKeyboardState(nullptr)};

//To send messages in battle;
vector<string> messageList;

int amountOfMovesPerType = 0;

SDL_Surface* buttonIMG = IMG_Load("images/button.png");

SDL_Surface* buttonHoverIMG = IMG_Load("images/buttonHover.png");

SDL_Surface* backButtonIMG = IMG_Load("images/backButton.png");

SDL_Surface* backButtonHoverIMG = IMG_Load("images/backButtonHover.png");

SDL_Surface* attackButtonIMG = IMG_Load("images/attackButton.png");
SDL_Surface* attackButtonHoverIMG = IMG_Load("images/attackButtonHover.png");

SDL_Surface* itemsButtonIMG = IMG_Load("images/itemsButton.png");
SDL_Surface* itemsButtonHoverIMG = IMG_Load("images/itemsButtonHover.png");

SDL_Surface* catchButtonIMG = IMG_Load("images/catchButton.png");
SDL_Surface* catchButtonHoverIMG = IMG_Load("images/catchButtonHover.png");

SDL_Surface* swapButtonIMG = IMG_Load("images/swapButton.png");
SDL_Surface* swapButtonHoverIMG = IMG_Load("images/swapButtonHover.png");

SDL_Surface* runButtonIMG = IMG_Load("images/runButton.png");
SDL_Surface* runButtonHoverIMG = IMG_Load("images/runButtonHover.png");

SDL_Surface* leftButtonIMG = IMG_Load("images/leftButton.png");
SDL_Surface* leftButtonHoverIMG = IMG_Load("images/leftButtonHover.png");

SDL_Surface* rightButtonIMG = IMG_Load("images/rightButton.png");
SDL_Surface* rightButtonHoverIMG = IMG_Load("images/rightButtonHover.png");

SDL_Surface* pokeInfoBG = IMG_Load("images/pokeInfoBG.png");