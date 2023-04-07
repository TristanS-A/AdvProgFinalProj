//
// Created by tropi on 4/4/2023.
//

#ifndef ADVPROGFINPROJ_BUTTON_H
#define ADVPROGFINPROJ_BUTTON_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>

enum ButtonState {
    NOT_PRESSED, PRESSED, HOVER
};

ButtonState checkForClickAndDisplayButton(SDL_Rect buttonRect, TTF_Font *font, SDL_Surface *windowSurf, SDL_Surface *buttonIMG, SDL_Surface * hoveredIMG);

#endif //ADVPROGFINPROJ_BUTTON_H
