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
#include <random>
#include <algorithm>
#include <string>

Player::Player(SDL_Rect playerPos) {
    forwardWalk = {IMG_Load("images/forward0.png"), IMG_Load("images/forward1.png"), IMG_Load("images/forward2.png")};
    leftWalk = {IMG_Load("images/left0.png"), IMG_Load("images/left1.png"), IMG_Load("images/left2.png")};
    rightWalk = {IMG_Load("images/right0.png"), IMG_Load("images/right1.png"), IMG_Load("images/right2.png")};
    backWalk = {IMG_Load("images/back0.png"), IMG_Load("images/back1.png"), IMG_Load("images/back2.png")};
    walkIndex_X = 0;
    walkIndex_Y = 0;
    walkTimeStep = 5;
    walkPrevTime = 0;
    playerSpeed = WALK_SPEED;

    playerImage = forwardWalk[walkIndex_X];

    if (!playerImage){
        std::cout << "Could not load player image.";
    } else {
        this->playerPos = {playerPos.x - playerImage->w / 2, playerPos.y - playerImage->h / 2, playerImage->w,
                           playerImage->h};
    }
}

Player::~Player() {
    for (SDL_Surface* surf : forwardWalk){
        SDL_FreeSurface(surf);
    }

    for (SDL_Surface* surf : leftWalk){
        SDL_FreeSurface(surf);
    }

    for (SDL_Surface* surf : rightWalk){
        SDL_FreeSurface(surf);
    }

    for (SDL_Surface* surf : backWalk){
        SDL_FreeSurface(surf);
    }

    playerImage = nullptr;

    for (vector<Item*> vectorOfItems: playersItems){
        for (Item* item : vectorOfItems){
            delete item;
        }
    }

    for (vector<Pokeball*> vectorOfPokeballs: playersPokeballs){
        for (Item* pokeball : vectorOfPokeballs){
            delete pokeball;
        }
    }

    for (Pokemon* pokemon : playersPokemon){
        delete pokemon;
    }

    playersPokemon.clear();
    playersItems.clear();
    playersPokeballs.clear();

    SDL_FreeSurface(playerImage);
}

void Player::playWalkAnimation() {
    Uint32 currTime = SDL_GetTicks();

    if (xVel != 0){
        if (currTime > walkPrevTime + 1000 / walkTimeStep) {
            walkPrevTime = currTime;
            if (walkIndex_X == rightWalk.size() - 1) {
                walkIndex_X = 0;
            } else {
                walkIndex_X++;
            }
        }
        if (xVel > 0){
            playerImage = rightWalk[walkIndex_X];
        } else if (xVel < 0){
            playerImage = leftWalk[walkIndex_X];
        }
    } else {
        walkIndex_X = 0;
        if (find(rightWalk.begin(), rightWalk.end(), playerImage) != rightWalk.end()){
            playerImage = rightWalk[walkIndex_X];
        }
        else if (find(leftWalk.begin(), leftWalk.end(), playerImage) != leftWalk.end()){
            playerImage = leftWalk[walkIndex_X];
        }
    }
    if (yVel != 0){
        if (currTime > walkPrevTime + 1000 / walkTimeStep) {
            walkPrevTime = currTime;
            if (walkIndex_Y == backWalk.size() - 1) {
                walkIndex_Y = 0;
            } else {
                walkIndex_Y++;
            }
        }
        if (yVel < 0){
            playerImage = backWalk[walkIndex_Y];
        } else if (yVel > 0){
            playerImage = forwardWalk[walkIndex_Y];
        }
    } else {
        walkIndex_Y = 0;
        if (find(backWalk.begin(), backWalk.end(), playerImage) != backWalk.end()){
            playerImage = backWalk[walkIndex_Y];
        }
        else if (find(forwardWalk.begin(), forwardWalk.end(), playerImage) != forwardWalk.end()){
            playerImage = forwardWalk[walkIndex_Y];
        }
    }
}

void Player::setPlayerVel(int xVel, int yVel) {
    this->xVel = xVel;
    this->yVel = yVel;
}

void Player::movePlayer() {
    playerPos.x += xVel;
    playerPos.y += yVel;
}

void Player::setPlayerPos(int x, int y) {
    playerPos.x = x;
    playerPos.y = y;
}

void Player::setSprinting() {
    playerSpeed = SPRINT_SPEED;
    walkTimeStep = 10;
}

void Player::setWalking() {
    playerSpeed = WALK_SPEED;
    walkTimeStep = 5;
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
        catchProbability = float(0.5 - pokemonToCatch->getHealthPercent()) + float(0.1 * (float(getTeamAverageLevel()) / float(pokemonToCatch->getLevel())));
        catchProbability += currPokeball->getCatchBooster() * 0.2;
        if (randomRange(outputNum) < catchProbability){
            addToPlayersPokemon(pokemonToCatch);
            removeFromPlayersPokeballs(currPokeball);
            catchingChancesCount = 0;
            return CAUGHT;
        }
        catchingChancesCount++;
    }
    else {
        removeFromPlayersPokeballs(currPokeball);

        //This is that if the player used their last pokeball the menu would reset
        bool outOfPokeballs = true;
        for (vector<Pokeball*> vectorOfPokeballs : playersPokeballs){
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

    //Sets player's current pokemon to the first pokemon gotten by the player
    if (playersPokemon.size() == 1){
        currPokemon = pokemon;
    }
}

//Function to remove Pokemon so the player can catch new ones, but didn't have enough time to impliment
/*
void Player::removeFromPlayersPokemon(Pokemon *pokemon) {
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
}*/

void Player::addToPlayersItems(Item *item) {
    for (vector<Item*> &vectorOfItems : playersItems){
        if (vectorOfItems[0]->getName() == item->getName()){
            if (vectorOfItems.size() < MAX_ITEMS) {
                vectorOfItems.push_back(item);
            } else {
                messageList.push_back("But you don't have enough room for it!");
            }
            return;
        }
    }

    playersItems.push_back({});
    playersItems[playersItems.size() - 1].push_back(item);
}

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

vector<Pokemon *> Player::getAllPokemon() {
    return playersPokemon;
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

bool Player::allPokemonHealthy() {
    for (Pokemon* pokemon : playersPokemon){
        if (pokemon->getHealth() == 0){
            return false;
        }
    }
    return true;
}

PlayerAction Player::displayBattleMenu(SDL_Surface *windowSurf) {
    switch (chosenAction){
        case NOT_CHOSEN:
            if (currPokemon->getHealth() > 0 && checkForClickAndDisplayButton({120, 650, 200, 100}, windowSurf, attackButtonIMG, attackButtonHoverIMG) == PRESSED){
                chosenAction = ATTACKING;
            }
            else if (currPokemon->getHealth() > 0 && checkForClickAndDisplayButton({370, 650, 200, 100}, windowSurf, itemsButtonIMG, itemsButtonHoverIMG) == PRESSED){
                chosenAction = USE_ITEM;
            }
            else if (currPokemon->getHealth() > 0 && checkForClickAndDisplayButton({620, 650, 200, 100}, windowSurf, catchButtonIMG, catchButtonHoverIMG) == PRESSED){
                chosenAction = CATCH;
            }
            else if (checkForClickAndDisplayButton({870, 650, 200, 100}, windowSurf, swapButtonIMG, swapButtonHoverIMG) == PRESSED || currPokemon->getHealth() <= 0){
                chosenAction = SWITCH_POKEMON;
            }
            else if (currPokemon->getHealth() > 0 && checkForClickAndDisplayButton({1120, 650, 200, 100}, windowSurf, runButtonIMG, runButtonHoverIMG) == PRESSED){
                chosenAction = RUN;
            }
            break;
        case ATTACKING:
            if (getCurrPokemon()->displayAndChooseMoves(windowSurf)){
                return chosenAction;
            }
            else if (checkForClickAndDisplayButton({1230, 745, 120, 50}, windowSurf, backButtonIMG, backButtonHoverIMG) == PRESSED){
                resetBattleMenu();
            }
            break;
        case USE_ITEM:
            if (!playersItems.empty()){
                playersItems[currItem][0]->displayDescription(windowSurf);

                int buttonHeight = 100;

                //Not if else statements because the other button images would dispensary if another one was pressed
                if (currItem < playersItems.size() - 1 && checkForClickAndDisplayButton({1280, 655, buttonHeight / 2, buttonHeight / 2}, windowSurf, rightButtonIMG, rightButtonHoverIMG) == PRESSED){
                    currItem++;
                }
                if (currItem > 0 && checkForClickAndDisplayButton({1110, 655, buttonHeight / 2, buttonHeight / 2}, windowSurf, leftButtonIMG, leftButtonHoverIMG) == PRESSED){
                    currItem--;
                }
                if (checkForClickAndDisplayButton({1170, 630, buttonHeight, buttonHeight}, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED){
                    messageList.push_back("You used " + playersItems[currItem][0]->getName());
                    return chosenAction;
                }
                if (checkForClickAndDisplayButton({1210, 740, 120, 50}, windowSurf, backButtonIMG, backButtonHoverIMG) == PRESSED){
                    resetBattleMenu();
                }

                int itemIMGDimensions = 90;
                SDL_Rect destRect = {1170 + (buttonHeight - itemIMGDimensions) / 2, 630 + (buttonHeight - itemIMGDimensions) / 2, itemIMGDimensions, itemIMGDimensions};
                playersItems[currItem][0]->displayItem(windowSurf, destRect);

                string text = to_string(playersItems[currItem].size());
                destRect.x = destRect.x + destRect.w - text.size() * SMALL_FONT_SIZE / FONT_PIXEL_HEIGHT_TO_WIDTH;
                destRect.y = destRect.y;
                textSurf = TTF_RenderText_Solid(smallFont, text.c_str(), textColor);
                SDL_BlitSurface(textSurf, nullptr, windowSurf, &destRect);
                SDL_FreeSurface(textSurf);
            } else {
                messageList.push_back("You don't have any items to use!");
                resetBattleMenu();
            }
            break;
        case CATCH:
            if (!canDoAction) {
                canDoAction = true;
                if (playersPokemon.size() >= MAX_POKEMON) {
                    canDoAction = false;
                    messageList.push_back("You can't carry any more pokemon...");
                    resetBattleMenu();
                } else {
                    if (playersPokeballs.empty()) {
                        canDoAction = false;
                        messageList.push_back("You don't have any pokeballs...");
                        resetBattleMenu();
                    }
                }
            } else {
                for (int i = playersPokeballs.size() - 1; i >= 0; i--){
                    if (!playersPokeballs[i].empty()) {
                        SDL_Rect displayPos = {1230 - 120 * i, 635, 100, 100};
                        ButtonState selectButton = checkForClickAndDisplayButton(displayPos, windowSurf, buttonIMG, buttonHoverIMG);
                        if (selectButton == PRESSED){
                            currPokeball = playersPokeballs[i][0];
                            canDoAction = false;
                            return CATCH;
                        }
                        else if (selectButton == HOVER){
                            playersPokeballs[i][0]->displayDescription(windowSurf);
                        }

                        string text = to_string(playersPokeballs[i].size());
                        int offset = 5;

                        displayPos.w -= offset * 2;
                        displayPos.h -= offset * 2;
                        displayPos.x += offset;
                        displayPos.y += offset;

                        playersPokeballs[i][0]->displayItem(windowSurf, displayPos);

                        displayPos.x = int(displayPos.x + displayPos.w - text.size() * SMALL_FONT_SIZE / FONT_PIXEL_HEIGHT_TO_WIDTH);
                        displayPos.y = displayPos.y - offset;

                        textSurf = TTF_RenderText_Solid(smallFont, text.c_str(), textColor);
                        SDL_BlitSurface(textSurf, nullptr, windowSurf, &displayPos);
                        SDL_FreeSurface(textSurf);
                    }
                }
                if (checkForClickAndDisplayButton({1210, 745, 120, 50}, windowSurf, backButtonIMG, backButtonHoverIMG) == PRESSED){
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
                    messageList.push_back("You don't have any other pokemon you can bring out!");
                    resetBattleMenu();
                }
            }
            else {
                int spacing = 0;
                for (int i = playersPokemon.size() - 1; i >= 0; i--) {
                    if (playersPokemon[i] != currPokemon && playersPokemon[i]->getHealth() > 0) {
                        SDL_Rect displayPos = {1150 - spacing, 630, 100, 100};
                        if (checkForClickAndDisplayButton(displayPos, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED) {
                            pokemonToSwapTo = playersPokemon[i];
                            pokemonToSwapTo->setImagePos(currPokemon->getImagePos().x, currPokemon->getImagePos().y);
                            pokemonToSwapTo->setInfoPos(currPokemon->getInfoPos().x, currPokemon->getInfoPos().y);
                            messageList.push_back(
                                    "You swapped out " + currPokemon->getName() + " and brought out " +
                                    pokemonToSwapTo->getName() + ".");
                            canDoAction = false;
                            return SWITCH_POKEMON;
                        }

                        int offset = 10;
                        displayPos = {displayPos.x + offset / 2, displayPos.y + offset / 2, displayPos.w - offset, displayPos.h - offset};
                        playersPokemon[i]->displayPokemonInfoButton(windowSurf, displayPos);
                        spacing += 250;
                    }
                }
                if (currPokemon->getHealth() > 0 && checkForClickAndDisplayButton({1260, 630, 90, 40}, windowSurf, backButtonIMG, backButtonHoverIMG) == PRESSED && currPokemon->getHealth() > 0) {
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

void Player::resetBattleMenu() {
    chosenAction = NOT_CHOSEN;
}

void Player::addToPlayersPokeballs(Pokeball* pokeball) {
    for (vector<Pokeball*> &vectorOfPokeballs : playersPokeballs){
        if (vectorOfPokeballs[0]->getName() == pokeball->getName()){
            if (vectorOfPokeballs.size() < MAX_POKEBALLS) {
                vectorOfPokeballs.push_back(pokeball);
            } else {
                messageList.push_back("But you don't have enough room for it!");
            }
            return;
        }
    }

    playersPokeballs.push_back({});
    playersPokeballs[playersPokeballs.size() - 1].push_back(pokeball);
}

void Player::removeFromPlayersPokeballs(Pokeball* pokeball) {
    for (vector<Pokeball*> &vectorOfPokeballs : playersPokeballs){
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

int Player::getTeamAverageLevel() {
    int total = 0;
    for (Pokemon* pokemon : playersPokemon){
        total += pokemon->getLevel();
    }

    return total / playersPokemon.size();
}

void Player::calculateTeamExperience(Pokemon* pokemonDefeated) {
    int experienceToAdd = pokemonDefeated->getExperience() + 10;
    messageList.push_back(currPokemon->getName() + " got " + to_string(experienceToAdd) + " EXP!");
    if (playersPokemon.size() > 1) {
        if (allPokemonHealthy()){
            messageList.push_back("Everyone else got " + to_string((experienceToAdd) / 2) + " EXP.");
        } else {
            messageList.push_back("Everyone else who didn't faint got " + to_string((experienceToAdd) / 2) + " EXP.");
        }
    }

    for (Pokemon* pokemon : playersPokemon){
        if (pokemon->getHealth() > 0){
            if (pokemon != currPokemon){
                pokemon->addToExperience((experienceToAdd) / 2);
            }
            else {
                pokemon->addToExperience(experienceToAdd);
            }
        }
    }

    checkForLevelUps();
}

void Player::checkForLevelUps() {
    for (Pokemon* pokemon : playersPokemon){
        if (pokemon->checkForLevelUp()){
            while (pokemon->checkForLevelUp()) {
                pokemon->levelUp();
            }
            messageList.push_back(pokemon->getName() + " leveled up to level " + to_string(pokemon->getLevel()) + "!");
            if (pokemon->getLevel() == pokemon->getMaxLevel()){
                messageList.push_back(pokemon->getName() + " has maxed out its level!");
            }
            messageList.push_back(pokemon->getName() + "'s health, attack, and defense power rose!");

            int moveCount = pokemon->getMoves().size();

            if (moveCount < pokemon->getMaxMoveAmount()) {
                random_device random;
                mt19937 outputNum(random());
                const float CHANCE_TO_LEARN_NEW_MOVE = 0.1;
                const int MAX_LEVEL_UPS_WITHOUT_NEW_MOVE = 5;

                if (randomChanceRange(outputNum) <= CHANCE_TO_LEARN_NEW_MOVE || pokemon->getLevelUpsWithoutNewMove() >= MAX_LEVEL_UPS_WITHOUT_NEW_MOVE) {
                    try {
                        pokemon->addRandomMove(typeid(*pokemon).name());
                        messageList.push_back(pokemon->getName() + " learned " + pokemon->getMoves()[moveCount]);
                        pokemon->setLevelUpsWithoutNewMove(0);
                    } catch (string &errorMessage) {
                        cout << errorMessage << endl;
                    }
                }
            }
        }
    }
}
