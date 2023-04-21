//
// Created by tropi on 4/20/2023.
//

#include "globalVariables.h"

void freeingSDLStuff(){

    SDL_FreeSurface(battleBackground_Forest);
    SDL_FreeSurface(battleBackground_Swamp);
    SDL_FreeSurface(battleBackground_Snow);
    SDL_FreeSurface(battleBackground_Fire);

    SDL_FreeSurface(background);

    SDL_FreeSurface(buttonIMG);

    SDL_FreeSurface(buttonHoverIMG);

    SDL_FreeSurface(backButtonIMG);

    SDL_FreeSurface(backButtonHoverIMG);

    SDL_FreeSurface(attackButtonIMG);
    SDL_FreeSurface(attackButtonHoverIMG);

    SDL_FreeSurface(itemsButtonIMG);
    SDL_FreeSurface(itemsButtonHoverIMG);

    SDL_FreeSurface(catchButtonIMG);
    SDL_FreeSurface(catchButtonHoverIMG);

    SDL_FreeSurface(swapButtonIMG);
    SDL_FreeSurface(swapButtonHoverIMG);

    SDL_FreeSurface(runButtonIMG);
    SDL_FreeSurface(runButtonHoverIMG);

    SDL_FreeSurface(leftButtonIMG);
    SDL_FreeSurface(leftButtonHoverIMG);

    SDL_FreeSurface(rightButtonIMG);
    SDL_FreeSurface(rightButtonHoverIMG);

    SDL_FreeSurface(pokeInfoBG);

    SDL_FreeSurface(titleScreenImage);

    SDL_FreeSurface(startButtonIMG);

    SDL_FreeSurface(startButtonHoverIMG);

    SDL_FreeSurface(chooseScreen);

    SDL_FreeSurface(curtain);

    SDL_FreeSurface(pokecenter);

    SDL_FreeSurface(spaceButton);

    SDL_FreeSurface(textboxIMG);

    Mix_FreeMusic(titleMusic);

    Mix_FreeMusic(worldMusic);

    Mix_FreeMusic(battleMusic);

    Mix_FreeChunk(battleIntro);
}
