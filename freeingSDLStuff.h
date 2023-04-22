//
// Created by tropi on 4/20/2023.
//

#ifndef ADVPROGFINPROJ_FREEINGSDLSTUFF_H
#define ADVPROGFINPROJ_FREEINGSDLSTUFF_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "globalVariables.h"

//Function for freeing SDL surfaces and music/sounds to avoid memory leaks
void freeingSDLStuff();

#endif //ADVPROGFINPROJ_FREEINGSDLSTUFF_H
