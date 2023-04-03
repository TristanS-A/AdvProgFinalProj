//
// Created by tropi on 4/1/2023.
//

#ifndef ADVPROGFINPROJ_PLAYER_H
#define ADVPROGFINPROJ_PLAYER_H

#include "SDL.h"
#include "pokemon.h"
#include <vector>

class Player{
public:
    Player(SDL_Rect playerPos);

    void movePlayer(int xUnits, int yUnits);

    void setPlayerPos(int x, int y);

    void setSprinting();

    void setWalking();

    int getPlayerSpeed() const;

    SDL_Rect getPlayerRect();

    void displayPlayer(SDL_Surface* windowSurf);

    void addToPlayersPokemon(Pokemon* pokemon);

    void removeFromPlayersPokemon(Pokemon* pokemon);

    Pokemon* getCurrPokemon();

private:
    SDL_Rect playerPos;
    SDL_Surface* playerImage;
    const int WALK_SPEED = 5;
    const int SPRINT_SPEED = 10;
    int playerSpeed;
    const int MAX_POKEMON = 20;
    vector<Pokemon*> playersPokemon;
    int currPokemon;
};

#endif //ADVPROGFINPROJ_PLAYER_H
