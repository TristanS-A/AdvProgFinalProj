//
// Created by tropi on 4/4/2023.
//

#ifndef ADVPROGFINPROJ_BUTTON_H
#define ADVPROGFINPROJ_BUTTON_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>

//Button states
enum ButtonState {
    NOT_PRESSED, PRESSED, HOVER
};

//Function to display a button and check for button click or hover
ButtonState checkForClickAndDisplayButton(SDL_Rect buttonRect, SDL_Surface *windowSurf, SDL_Surface *buttonIMG, SDL_Surface * hoveredIMG);

#endif //ADVPROGFINPROJ_BUTTON_H
