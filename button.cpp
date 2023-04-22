//
// Created by tropi on 4/4/2023.
//

#include "button.h"
#include "globalVariables.h"

ButtonState checkForClickAndDisplayButton(SDL_Rect buttonRect, SDL_Surface *windowSurf, SDL_Surface *buttonIMG, SDL_Surface * hoveredIMG){

    //Get mouse position
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    //If the user changed the screen dimension, this will translate the mouse position to the
    // new screen/window dimensions
    mouseY = mouseY * (SCREEN_HEIGHT) / windowTextureSize.h - windowTextureSize.y * (SCREEN_HEIGHT) /
                                                                      windowTextureSize.h;
    mouseX = mouseX * (SCREEN_WIDTH) / windowTextureSize.w - windowTextureSize.x * (SCREEN_WIDTH) /
                                                                     windowTextureSize.w;

    //Test if mouse is over button to display hovered image, but otherwise displays regular image
    if ((mouseX > buttonRect.x && mouseX < buttonRect.x + buttonRect.w) && (mouseY > buttonRect.y &&
                                                                            mouseY < buttonRect.y + buttonRect.h)) {
        SDL_BlitScaled(hoveredIMG, nullptr, windowSurf, &buttonRect);

        if (mouseDown && !mouseHeldDown) {
            mouseHeldDown = true;
            Mix_PlayChannel(2, buttonSound, 0);
            return PRESSED;
        }
        else if (!mouseDown) { //So that the player has to let go of the mouse button to click again
            mouseHeldDown = false;
        }
        return HOVER;
    }
    else {
        SDL_BlitScaled(buttonIMG, nullptr, windowSurf, &buttonRect);
    }

    return NOT_PRESSED;
}
