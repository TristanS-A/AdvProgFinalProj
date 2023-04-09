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

//To send messages in battle;
vector<string> messageList;

SDL_Surface* buttonIMG = IMG_Load("images/button.png");

SDL_Surface* buttonHoverIMG = IMG_Load("images/buttonHover.png");

SDL_Surface* backButtonIMG = IMG_Load("images/backButton.png");

SDL_Surface* backButtonHoverIMG = IMG_Load("images/backButtonHover.png");