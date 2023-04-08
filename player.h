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

enum PlayerAction {
    NOT_CHOSEN, ATTACKING, USE_ITEM, CATCH, SWITCH_POKEMON, RUN
};

enum CatchingState {
    CAUGHT, NOT_CAUGHT, ANIMATION_NOT_FINISHED
};

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

    CatchingState tryCatchingPokemon(Pokemon* pokemonToCatch);

    void addToPlayersPokemon(Pokemon* pokemon);

    void removeFromPlayersPokemon(Pokemon* pokemon);

    void addToPlayersItems(Item* item);

    void removeFromPlayersItems(Item* item);

    void addToPlayersPokeballs(Item* pokeball);

    void removeFromPlayersPokeballs(Item* pokeball);

    Item* getCurrItem();

    void useItem();

    Pokemon* getCurrPokemon();

    bool noOtherHealthyPokemon();

    bool switchingPokemon();

    PlayerAction displayBattleMenu(SDL_Surface* windowSurf, vector<string> &messages);

    void resetBattleMenu();

private:
    SDL_Rect playerPos;
    SDL_Surface* playerImage;
    const int WALK_SPEED = 5;
    const int SPRINT_SPEED = 10;
    int playerSpeed;
    const int MAX_POKEMON = 6;
    vector<Pokemon*> playersPokemon;
    Pokemon* currPokemon;
    Pokemon* pokemonToSwapTo;
    vector<vector<Item*>> playersPokeballs;
    bool canDoAction = false;
    int catchingChancesCount = 0;
    const int TOTAL_CATCHING_CHANCES = 3;
    float catchProbability;
    Item* currPokeball;
    PlayerAction chosenAction;
    const int MAX_ITEMS = 20;
    vector<vector<Item*>> playersItems;
    int currItem = 0;
};

#endif //ADVPROGFINPROJ_PLAYER_H
