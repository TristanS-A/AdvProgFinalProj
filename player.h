//
// Created by tropi on 4/1/2023.
//

#ifndef ADVPROGFINPROJ_PLAYER_H
#define ADVPROGFINPROJ_PLAYER_H

#include "SDL.h"

class Player{
public:
    Player(SDL_Rect playerPos);

    void movePlayer(int xUnits, int yUnits);

    void setPlayerPos(int x, int y);

    void setSprinting();

    void setWakling();

    int getPlayerSpeed();

    SDL_Rect getPlayerRect();

    void displayPlayer(SDL_Surface* windowSurf);
private:
    SDL_Rect playerPos;
    SDL_Surface* playerImage;
    const int walkSpeed = 5;
    const int sprintSpeed = 10;
    int playerSpeed;
};

#endif //ADVPROGFINPROJ_PLAYER_H
