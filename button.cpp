//
// Created by tropi on 4/4/2023.
//

#include "button.h"
#include "globalVariables.h"
#include "iostream"

ButtonState checkForClickAndDisplayButton(SDL_Rect buttonRect, SDL_Surface *windowSurf, SDL_Surface *buttonIMG, SDL_Surface * hoveredIMG){

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if ((mouseX > buttonRect.x && mouseX < buttonRect.x + buttonRect.w) && (mouseY > buttonRect.y &&
                                                                            mouseY < buttonRect.y + buttonRect.h)) {
        SDL_BlitScaled(hoveredIMG, nullptr, windowSurf, &buttonRect);

        if (mouseDown && !mouseHeldDown) {
            mouseHeldDown = true;
            return PRESSED;
        }
        else if (!mouseDown) {
            mouseHeldDown = false;
        }
        return HOVER;
    }
    else {
        SDL_BlitScaled(buttonIMG, nullptr, windowSurf, &buttonRect);
    }

    return NOT_PRESSED;
}
