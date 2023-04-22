//
// Created by tropi on 4/1/2023.
//

#ifndef ADVPROGFINPROJ_PLAYER_H
#define ADVPROGFINPROJ_PLAYER_H

#include "SDL.h"
#include "pokemon.h"
#include "items.h"
#include <vector>
#include <algorithm>

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

    void playWalkAnimation();

    void setPlayerVel(int xVel, int yVel);

    void movePlayer();

    void setPlayerPos(int x, int y);

    void setSprinting();

    void setWalking();

    int getPlayerSpeed() const;

    SDL_Rect getPlayerRect();

    void displayPlayer(SDL_Surface* windowSurf);

    CatchingState tryCatchingPokemon(Pokemon* pokemonToCatch);

    void addToPlayersPokemon(Pokemon* pokemon);

    //void removeFromPlayersPokemon(Pokemon* pokemon); //Function to remove a pokemon from player pokemon (unimplemented)

    void addToPlayersItems(Item* item);

    void removeFromPlayersItems(Item* item);

    void addToPlayersPokeballs(Pokeball* pokeball);

    void removeFromPlayersPokeballs(Pokeball* pokeball);

    void useItem();

    Pokemon* getCurrPokemon();

    vector<Pokemon*> getAllPokemon();

    bool allPokemonHealthy();

    bool noOtherHealthyPokemon();

    bool switchingPokemon();

    PlayerAction displayBattleMenu(SDL_Surface* windowSurf);

    void resetBattleMenu();

    int getTeamAverageLevel();

    void calculateTeamExperience(Pokemon* pokemonDefeated);

    void checkForLevelUps();

private:
    SDL_Rect playerPos;
    SDL_Surface* playerImage;
    vector<SDL_Surface*> forwardWalk;
    vector<SDL_Surface*> leftWalk;
    vector<SDL_Surface*> rightWalk;
    vector<SDL_Surface*> backWalk;
    int walkTimeStep;
    int walkPrevTime;
    int walkIndex_X;
    int walkIndex_Y;
    int xVel = 0;
    int yVel = 0;
    const int WALK_SPEED = 5;
    const int SPRINT_SPEED = 10;
    int playerSpeed;
    const int MAX_POKEMON = 6;
    vector<Pokemon*> playersPokemon;
    Pokemon* currPokemon;
    Pokemon* pokemonToSwapTo;
    vector<vector<Pokeball*>> playersPokeballs;
    bool canDoAction = false;
    int catchingChancesCount = 0;
    const int TOTAL_CATCHING_CHANCES = 3;
    float catchProbability;
    Pokeball* currPokeball;
    PlayerAction chosenAction;
    const int MAX_ITEMS = 10;
    const int MAX_POKEBALLS = 10;
    vector<vector<Item*>> playersItems;
    int currItem = 0;
};

#endif //ADVPROGFINPROJ_PLAYER_H
