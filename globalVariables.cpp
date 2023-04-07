//
// Created by tropi on 4/4/2023.
//

#include "globalVariables.h"
#include "SDL_image.h"

//To keep track of if the mouse is pressed
bool mouseDown = false;

//To keep track of if the mouse button is being held down
bool mouseHeldDown = false;

SDL_Surface* buttonIMG = IMG_Load("images/button.png");

SDL_Surface* buttonHoverIMG = IMG_Load("images/buttonHover.png");