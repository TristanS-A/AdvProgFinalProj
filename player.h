//
// Created by tropi on 4/1/2023.
//

#ifndef ADVPROGFINPROJ_PLAYER_H
#define ADVPROGFINPROJ_PLAYER_H

#include "SDL.h"
#include "pokemon.h"
#include "items.h"
#include <vector>

enum playerAction {
    NOT_CHOSEN, ATTACKING, USE_ITEM, CATCH, RUN
};

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

    void addToPlayersItems(Item* item);

    void removeFromPlayersItems(Item* item);

    Pokemon* getCurrPokemon();

    playerAction displayBattleMenu(TTF_Font* font, SDL_Surface* windowSurf, vector<string> &messages);

    void resetBattleMenu();

private:
    SDL_Rect playerPos;
    SDL_Surface* playerImage;
    const int WALK_SPEED = 5;
    const int SPRINT_SPEED = 10;
    int playerSpeed;
    const int MAX_POKEMON = 20;
    vector<Pokemon*> playersPokemon;
    int currPokemon = 0;
    playerAction chosenAction;
    const int MAX_ITEMS = 20;
    vector<Item*> playersItems;
};

#endif //ADVPROGFINPROJ_PLAYER_H
