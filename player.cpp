//
// Created by tropi on 4/1/2023.
//
#include "player.h"
#include "SDL.h"
#include "SDL_image.h"
#include "globalVariables.h"
#include "button.h"
#include <iostream>
#include <vector>
#include <bits/stdc++.h>

Player::Player(SDL_Rect playerPos) {
    playerImage = IMG_Load("images/p.png");

    if (!playerImage){
        std::cout << "Could not load player image.";
    }

    this->playerPos = {playerPos.x - playerImage->w / 2, playerPos.y - playerImage->h / 2, playerImage->w,
                       playerImage->h};

    playerSpeed = WALK_SPEED;
}

Player::~Player() {
    for (vector<Item*> vectorOfItems: playersItems){
        for (Item* item : vectorOfItems){
            delete item;
        }
    }

    for (vector<Item*> vectorOfPokeballs: playersPokeballs){
        for (Item* pokeball : vectorOfPokeballs){
            delete pokeball;
        }
    }

    for (Pokemon* pokemon : playersPokemon){
        delete pokemon;
    }

    playersPokemon.clear();
    playersItems.clear();
}

void Player::movePlayer(int xUnits, int yUnits) {
    playerPos.x += xUnits;
    playerPos.y += yUnits;
}

void Player::setPlayerPos(int x, int y) {
    playerPos.x = x;
    playerPos.y = y;
}

void Player::setSprinting() {
    playerSpeed = SPRINT_SPEED;
}

void Player::setWalking() {
playerSpeed = WALK_SPEED;
}

int Player::getPlayerSpeed() const {
    return playerSpeed;
}

SDL_Rect Player::getPlayerRect() {
    return playerPos;
}

void Player::displayPlayer(SDL_Surface* windowSurf) {
    SDL_BlitSurface(playerImage, nullptr, windowSurf, &playerPos);
}

CatchingState Player::tryCatchingPokemon(Pokemon* pokemonToCatch) {
    random_device random;

    mt19937 outputNum(random());
    uniform_real_distribution<double> randomRange(0, 1.0);

    if (catchingChancesCount != TOTAL_CATCHING_CHANCES){
        catchProbability = float(0.5 - pokemonToCatch->getHealthPercent()) + float(0.1 * (float(currPokemon->getLevel()) / float(pokemonToCatch->getLevel())));
        cout << catchProbability << endl;
        if (randomRange(outputNum) < catchProbability){
            addToPlayersPokemon(pokemonToCatch);
            catchingChancesCount = 0;
            return CAUGHT;
        }
        catchingChancesCount++;
    }
    else {
        removeFromPlayersPokeballs(currPokeball);

        //This is that if the player used their last pokeball the menu would reset
        bool outOfPokeballs = true;
        for (vector<Item*> vectorOfPokeballs : playersPokeballs){
            if (!vectorOfPokeballs.empty()){
                outOfPokeballs = false;
            }
        }
        if (outOfPokeballs){
            resetBattleMenu();
        }
        catchingChancesCount = 0;
        return NOT_CAUGHT;
    }

    return ANIMATION_NOT_FINISHED;
}

void Player::addToPlayersPokemon(Pokemon* pokemon) {
    playersPokemon.push_back(pokemon);

    ///////////////////////////////////////Maybe place this in main when starter pokemon are given
    if (playersPokemon.size() == 1){
        currPokemon = pokemon;
    }
}

void Player::removeFromPlayersPokemon(Pokemon *pokemon) {
    //////////////////////////////////////////////////////////////Decide on boxes storage or delete
    if (currPokemon == pokemon){
        if (playersPokemon[0] == pokemon){
            currPokemon = playersPokemon[1];
        }
        else if (playersPokemon[playersPokemon.size() - 1] == pokemon){
            currPokemon = playersPokemon[playersPokemon.size() - 2];
        }
        else {
            for (int i = 0; i < playersPokemon.size(); i++){
                if (playersPokemon[i] == pokemon){
                    currPokemon = playersPokemon[i - 1];
                    break;
                }
            }
        }
    }

    playersPokemon.erase(find(playersPokemon.begin(), playersPokemon.end(), pokemon));
}

void Player::addToPlayersItems(Item *item) {
    for (vector<Item*> &vectorOfItems : playersItems){
        if (vectorOfItems[0]->getName() == item->getName()){
            vectorOfItems.push_back(item);
            return;
        }
    }

    playersItems.push_back({});
    playersItems[playersItems.size() - 1].push_back(item);
}

///////////////////////////////////////////////////Maybe make a template for this and pokeball deletion and adding
void Player::removeFromPlayersItems(Item *item) {
    for (vector<Item*> &vectorOfItems : playersItems){
        if (vectorOfItems[0]->getName() == item->getName()){
            vectorOfItems.erase(find(vectorOfItems.begin(), vectorOfItems.end(), item));
            if (vectorOfItems.empty()){
                playersItems.erase(find(playersItems.begin(), playersItems.end(), vectorOfItems));
            }
            delete item;
            return;
        }
    }
}

void Player::useItem() {
    playersItems[currItem][0]->use(currPokemon);
    removeFromPlayersItems(playersItems[currItem][0]);
    currItem = 0;
    resetBattleMenu();
    if (playersItems.empty()){
        resetBattleMenu();
    }
}

Pokemon* Player::getCurrPokemon() {
    return currPokemon;
}

bool Player::noOtherHealthyPokemon() {
    bool noMorePokemon = true;
    for (Pokemon* pokemon : playersPokemon){
        if (pokemon->getHealth() > 0 && pokemon != currPokemon){
            noMorePokemon = false;
        }
    }

    return noMorePokemon;
}

PlayerAction Player::displayBattleMenu(TTF_Font *font, SDL_Surface *windowSurf, vector<string> &messages) {
    ///////////////////////////////////////////////////////////Maybe make buttons with a global variable for the texboxPos
    switch (chosenAction){
        case NOT_CHOSEN:
            if (checkForClickAndDisplayButton({100, 650, 100, 100}, font, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED && currPokemon->getHealth() > 0){
                chosenAction = ATTACKING;
            }
            else if (checkForClickAndDisplayButton({200, 650, 100, 100}, font, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED && currPokemon->getHealth() > 0){
                chosenAction = USE_ITEM;
            }
            else if (checkForClickAndDisplayButton({300, 650, 100, 100}, font, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED && currPokemon->getHealth() > 0){
                chosenAction = CATCH;
            }
            else if (checkForClickAndDisplayButton({400, 650, 100, 100}, font, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED || currPokemon->getHealth() <= 0){
                chosenAction = SWITCH_POKEMON;
            }
            else if (checkForClickAndDisplayButton({500, 650, 100, 100}, font, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED && currPokemon->getHealth() > 0){
                chosenAction = RUN;
            }
            break;
        case ATTACKING:
            if (getCurrPokemon()->displayAndChooseMoves(font, windowSurf, messages)){
                return chosenAction;
            }
            else if (checkForClickAndDisplayButton({0, 0, 100, 100}, font, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED){
                resetBattleMenu();
            }
            break;
        case USE_ITEM:
            //////////////////////////////////////////////////////Display item descriptions
            if (!playersItems.empty()){
                cout << currItem << endl;
                //Not if else statements because the other button images would dispensary if another one was pressed
                if (currItem < playersItems.size() - 1 && checkForClickAndDisplayButton({1200, 650, 90, 100}, font, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED){
                    currItem++;
                }
                if (currItem > 0 && checkForClickAndDisplayButton({1000, 650, 90, 100}, font, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED){
                    currItem--;
                }
                if (checkForClickAndDisplayButton({1100, 650, 100, 100}, font, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED){
                    messages.push_back("You used " + playersItems[currItem][0]->getName());
                    return chosenAction;
                }
                if (checkForClickAndDisplayButton({0, 0, 100, 100}, font, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED){
                    resetBattleMenu();
                }
                playersItems[currItem][0]->displayItem(windowSurf, {1100, 650, 0, 0});
            } else {
                messages.push_back("You don't have any items to use!");
                resetBattleMenu();
            }
            break;
        case CATCH:
            if (!canDoAction) {
                canDoAction = true;
                if (playersPokemon.size() >= MAX_POKEMON) {
                    if (playersPokeballs.empty()) {
                        canDoAction = false;
                        messages.push_back("You don't have any pokeballs...");
                        resetBattleMenu();
                    }
                } else {
                    canDoAction = false;
                    messages.push_back("You can't carry any more pokemon...");
                    resetBattleMenu();
                }
            } else {
                for (int i = playersPokeballs.size() - 1; i >= 0; i--){
                    if (!playersPokeballs[i].empty()) {
                        if (checkForClickAndDisplayButton({1200 - 100 * i, 650, 100, 100}, font, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED){
                            currPokeball = playersPokeballs[i][0];
                            canDoAction = false;
                            return CATCH;
                        }
                        playersPokeballs[i][0]->displayItem(windowSurf, {1200 - 100 * i, 650, 0, 0});
                    }
                }
                if (checkForClickAndDisplayButton({0, 0, 100, 100}, font, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED){
                    canDoAction = false;
                    resetBattleMenu();
                }
            }
            break;
        case SWITCH_POKEMON:
            if (!canDoAction){
                if (!noOtherHealthyPokemon()){
                    canDoAction = true;
                }
                else {
                    messages.push_back("You don't have any other pokemon you can bring out!");
                    resetBattleMenu();
                }
            }
            else {
                int spacing = 0;
                for (int i = playersPokemon.size() - 1; i >= 0; i--) {
                    if (playersPokemon[i] != currPokemon && playersPokemon[i]->getHealth() > 0) {
                        if (checkForClickAndDisplayButton({1200 - spacing, 650, 100, 100}, font, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED) {
                            pokemonToSwapTo = playersPokemon[i];
                            pokemonToSwapTo->setImagePos(currPokemon->getImagePos().x, currPokemon->getImagePos().y);
                            pokemonToSwapTo->setInfoPos(currPokemon->getInfoPos().x, currPokemon->getInfoPos().y);
                            messages.push_back(
                                    "You swapped out " + currPokemon->getName() + " and brought out " +
                                    pokemonToSwapTo->getName() + ".");
                            canDoAction = false;
                            return SWITCH_POKEMON;
                        }

                        playersPokemon[i]->displayPokemonInfoButton(windowSurf, {1200 - spacing, 650, 100, 100});
                        spacing += 100;
                    }
                }
                if (checkForClickAndDisplayButton({0, 0, 100, 100}, font, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED && currPokemon->getHealth() > 0) {
                    canDoAction = false;
                    resetBattleMenu();
                }
            }
            break;
        case RUN:
            return RUN;
            break;
    }

    return NOT_CHOSEN;
}

Item* Player::getCurrItem() {
    return playersItems[currItem][0];
}

void Player::resetBattleMenu() {
    chosenAction = NOT_CHOSEN;
}

void Player::addToPlayersPokeballs(Item* pokeball) {
    for (vector<Item*> vectorOfPokeballs : playersPokeballs){
        if (vectorOfPokeballs[0]->getName() == pokeball->getName()){
            vectorOfPokeballs.push_back(pokeball);
            return;
        }
    }

    playersPokeballs.push_back({});
    playersPokeballs[playersPokeballs.size() - 1].push_back(pokeball);
}

void Player::removeFromPlayersPokeballs(Item* pokeball) {
    for (vector<Item*> &vectorOfPokeballs : playersPokeballs){
        if (vectorOfPokeballs[0]->getName() == pokeball->getName()){
            vectorOfPokeballs.erase(find(vectorOfPokeballs.begin(), vectorOfPokeballs.end(), pokeball));
            if (vectorOfPokeballs.empty()){
                playersPokeballs.erase(find(playersPokeballs.begin(), playersPokeballs.end(), vectorOfPokeballs));
            }
            delete pokeball;
            return;
        }
    }
}

bool Player::switchingPokemon() {
    currPokemon = pokemonToSwapTo;
    resetBattleMenu();
    return false;
}
