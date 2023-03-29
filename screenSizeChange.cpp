//
// Created by tropi on 3/29/2023.
//

#include "screenSizeChange.h"
#include <SDL.h>

//Handles the dimensions of the gameplay window if the window size is changed
void screenSizeChange(SDL_Rect &windowTextureSize, SDL_Window *window){

    //Gameplay window ratio
    int aspectRatio_X = 16;
    int aspectRatio_Y = 9;

    //Gets window height and width
    windowTextureSize.w = SDL_GetWindowSurface(window)->w;
    windowTextureSize.h = SDL_GetWindowSurface(window)->h;

    //Since I am keeping the dimensions at a 16:9 ratio, this checks weather the width or height is
    // unable to expand more and then sets the other side to the 16:9 ration with respects to the
    // restricted side
    if (windowTextureSize.w / aspectRatio_X > windowTextureSize.h / aspectRatio_Y) {

        //Sets dimension of the windowText if the height cannot expand more at a 16:9 ratio
        windowTextureSize.w = aspectRatio_X * (windowTextureSize.h / aspectRatio_Y);
        windowTextureSize.h = windowTextureSize.h;

    } else if (windowTextureSize.w / aspectRatio_X < windowTextureSize.h / aspectRatio_Y) {

        //Sets dimension of the windowText if the width cannot expand more at a 16:9 ration
        windowTextureSize.h = 9 * (windowTextureSize.w / aspectRatio_X);
        windowTextureSize.w = windowTextureSize.w;
    }

    //Creates equal length margins on the sides that have empty space, by centering the windowText
    windowTextureSize.x = (SDL_GetWindowSurface(window)->w - windowTextureSize.w) / 2;
    windowTextureSize.y = (SDL_GetWindowSurface(window)->h - windowTextureSize.h) / 2;
}