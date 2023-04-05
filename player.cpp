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
    for (Item* item : playersItems){
        delete item;
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

catchingState Player::tryCatchingPokemon(Pokemon* pokemonToCatch) {
    random_device random;

    mt19937 outputNum(random());
    uniform_real_distribution<double> randomRange(0, 1.0);

    if (catchingChancesCount != TOTAL_CATCHING_CHANCES){
        catchProbability = float(0.5 - pokemonToCatch->getHealthPercent()) + float(0.1 * (float(playersPokemon[currPokemon]->getLevel()) / float(pokemonToCatch->getLevel())));
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
}

void Player::removeFromPlayersPokemon(Pokemon *pokemon) {
    playersPokemon.erase(find(playersPokemon.begin(), playersPokemon.end(), pokemon));
}

void Player::addToPlayersItems(Item *item) {
    playersItems.push_back(item);
}

void Player::removeFromPlayersItems(Item *item) {
    playersItems.erase(find(playersItems.begin(), playersItems.end(), item));
}

void Player::useItem() {
    playersItems[currItem];
}

Pokemon* Player::getCurrPokemon() {
    return playersPokemon[currPokemon];
}

playerAction Player::displayBattleMenu(TTF_Font *font, SDL_Surface *windowSurf, vector<string> &messages) {
    ///////////////////////////////////////////////////////////Add switch pokemon option
    switch (chosenAction){
        case NOT_CHOSEN:
            if (checkForClickAndDisplayButton({0, 0, 100, 100}, font, windowSurf)){
                chosenAction = ATTACKING;
            }
            else if (checkForClickAndDisplayButton({200, 0, 100, 100}, font, windowSurf)){
                chosenAction = USE_ITEM;
            }
            else if (checkForClickAndDisplayButton({300, 0, 100, 100}, font, windowSurf)){
                chosenAction = CATCH;
            }
            else if (checkForClickAndDisplayButton({400, 0, 100, 100}, font, windowSurf)){
                chosenAction = RUN;
            }
            break;
        case ATTACKING:
            if (getCurrPokemon()->displayAndChooseMoves(font, windowSurf, messages)){
                return chosenAction;
            }
            else if (checkForClickAndDisplayButton({0, 0, 100, 100}, font, windowSurf)){
                resetBattleMenu();
            }
            break;
        case USE_ITEM:
            if (!playersItems.empty()){
                cout << currItem << endl;
                playersItems[currItem]->displayItem(windowSurf, {200, 200, 0, 0});
                if (currItem < playersItems.size() - 1 && checkForClickAndDisplayButton({300, 200, 90, 100}, font, windowSurf)){
                    currItem++;
                }
                else if (currItem > 0 && checkForClickAndDisplayButton({100, 200, 90, 100}, font, windowSurf)){
                    currItem--;
                }
                else if (checkForClickAndDisplayButton({200, 200, 100, 100}, font, windowSurf)){
                    playersItems[currItem]->use(playersPokemon[currPokemon]);
                    messages.push_back("You used " + playersItems[currItem]->getName());
                    return chosenAction;
                }
                else if (checkForClickAndDisplayButton({0, 0, 100, 100}, font, windowSurf)){
                    resetBattleMenu();
                }
            } else {
                messages.push_back("You don't have any items to use!");
                resetBattleMenu();
            }
            break;
        case CATCH:
            if (!canCatch) {
                if (playersPokemon.size() >= MAX_POKEMON) {
                    for (vector<Item*> vectorOfPokeballs: playersPokeballs) {
                        if (!vectorOfPokeballs.empty()) {
                            canCatch = true;
                        }
                    }
                    if (!canCatch) {
                        messages.push_back("You don't have any pokeballs...");
                        resetBattleMenu();
                    }
                } else {
                    messages.push_back("You can't carry any more pokemon...");
                    resetBattleMenu();
                }
            } else {
                for (int i = playersPokeballs.size() - 1; i >= 0; i--){
                    if (!playersPokeballs[i].empty()) {
                        playersPokeballs[i][0]->displayItem(windowSurf, {700 - 100 * i, 600, 0, 0});
                        if (checkForClickAndDisplayButton({700 - 100 * i, 600, 100, 100}, font, windowSurf)){
                            currPokeball = playersPokeballs[i][0];
                            canCatch = false;
                            return CATCH;
                        }
                    }
                }
                if (checkForClickAndDisplayButton({0, 0, 100, 100}, font, windowSurf)){
                    canCatch = false;
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
    return playersItems[currItem];
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
            delete pokeball;
            return;
        }
    }
}
