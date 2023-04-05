//
// Created by tropi on 4/1/2023.
//

#ifndef ADVPROGFINPROJ_PLAYER_H
#define ADVPROGFINPROJ_PLAYER_H

#include "SDL.h"
#include "pokemon.h"
#include "items.h"
#include <vector>

///////////////////////////////Maybe capitalize the enums

enum playerAction {
    NOT_CHOSEN, ATTACKING, USE_ITEM, CATCH, RUN
};

enum catchingState {CAUGHT, NOT_CAUGHT, ANIMATION_NOT_FINISHED};

class Player{
public:
    Player(SDL_Rect playerPos);

    ~Player();

    void movePlayer(int xUnits, int yUnits);

    void setPlayerPos(int x, int y);

    void setSprinting();

    void setWalking();

    int getPlayerSpeed() const;

    SDL_Rect getPlayerRect();

    void displayPlayer(SDL_Surface* windowSurf);

    catchingState tryCatchingPokemon(Pokemon* pokemonToCatch);

    void addToPlayersPokemon(Pokemon* pokemon);

    void removeFromPlayersPokemon(Pokemon* pokemon);

    void addToPlayersItems(Item* item);

    void removeFromPlayersItems(Item* item);

    void addToPlayersPokeballs(Item* pokeball);

    void removeFromPlayersPokeballs(Item* pokeball);

    Item* getCurrItem();

    void useItem();

    Pokemon* getCurrPokemon();

    playerAction displayBattleMenu(TTF_Font* font, SDL_Surface* windowSurf, vector<string> &messages);

    void resetBattleMenu();

private:
    SDL_Rect playerPos;
    SDL_Surface* playerImage;
    const int WALK_SPEED = 5;
    const int SPRINT_SPEED = 10;
    int playerSpeed;
    const int MAX_POKEMON = 1;
    vector<Pokemon*> playersPokemon;
    /////////////////////////////////////////Look into making the curr variables into class pointers like currPokeball
    int currPokemon = 0;
    vector<vector<Item*>> playersPokeballs;
    bool canCatch = false;
    int catchingChancesCount = 0;
    const int TOTAL_CATCHING_CHANCES = 3;
    float catchProbability;
    Item* currPokeball;
    playerAction chosenAction;
    const int MAX_ITEMS = 20;
    vector<Item*> playersItems;
    int currItem = 0;
};

#endif //ADVPROGFINPROJ_PLAYER_H
