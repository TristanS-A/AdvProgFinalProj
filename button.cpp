//
// Created by tropi on 4/4/2023.
//

#include "button.h"
#include "globalVariables.h"

bool checkForClickAndDisplayButton(SDL_Rect buttonRect, TTF_Font *font, SDL_Surface *windowSurf){

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (mouseDown) {
        if ((mouseX > buttonRect.x && mouseX < buttonRect.x + buttonRect.w) && (mouseY > buttonRect.y &&
            mouseY < buttonRect.y + buttonRect.h)) {
            return true;
        }
    }

    return false;
}