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
            resetBattleMenu();
            catchingChancesCount = 0;
            return CAUGHT;
        }
        catchingChancesCount++;
    }
    else {
        catchingChancesCount = 0;
        resetBattleMenu();
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
            }
            break;
        case CATCH:
            if (!canCatch) {
                if (playersPokemon.size() >= MAX_POKEMON) {
                    for (int playersPokeballCount: playersPokeballs) {
                        if (playersPokeballCount > 0) {
                            canCatch = true;
                        }
                    }
                    if (!canCatch) {
                        messages.push_back("You don't have any pokeballs...");
                    }
                } else {
                    messages.push_back("You can't carry any more pokemon...");
                }
                resetBattleMenu();
            } else {
                //////////////////////////////////Display pokeball options like items
                canCatch = false;
                return CATCH;
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