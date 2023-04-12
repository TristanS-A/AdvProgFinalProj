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
        catchProbability = float(0.5 - pokemonToCatch->getHealthPercent()) + float(0.1 * (float(getTeamAverageLevel()) / float(pokemonToCatch->getLevel())));
        cout << catchProbability << endl;
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

PlayerAction Player::displayBattleMenu(SDL_Surface *windowSurf) {
    ///////////////////////////////////////////////////////////Maybe add random text for how the currPokemon is doing
    switch (chosenAction){
        case NOT_CHOSEN:
            if (currPokemon->getHealth() > 0 && checkForClickAndDisplayButton({120, 650, 200, 100}, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED){
                chosenAction = ATTACKING;
            }
            else if (currPokemon->getHealth() > 0 && checkForClickAndDisplayButton({370, 650, 200, 100}, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED){
                chosenAction = USE_ITEM;
            }
            else if (currPokemon->getHealth() > 0 && checkForClickAndDisplayButton({620, 650, 200, 100}, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED){
                chosenAction = CATCH;
            }
            else if (checkForClickAndDisplayButton({870, 650, 200, 100}, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED || currPokemon->getHealth() <= 0){
                chosenAction = SWITCH_POKEMON;
            }
            else if (currPokemon->getHealth() > 0 && checkForClickAndDisplayButton({1120, 650, 200, 100}, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED){
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
            //////////////////////////////////////////////////////Display item descriptions
            if (!playersItems.empty()){
                cout << currItem << endl;
                playersItems[currItem][0]->displayDescription(windowSurf);

                //Not if else statements because the other button images would dispensary if another one was pressed
                if (currItem < playersItems.size() - 1 && checkForClickAndDisplayButton({1280, 655, 50, 50}, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED){
                    currItem++;
                }
                if (currItem > 0 && checkForClickAndDisplayButton({1110, 655, 50, 50}, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED){
                    currItem--;
                }
                if (checkForClickAndDisplayButton({1170, 630, 100, 100}, windowSurf, buttonIMG, buttonHoverIMG) == PRESSED){
                    messageList.push_back("You used " + playersItems[currItem][0]->getName());
                    return chosenAction;
                }
                if (checkForClickAndDisplayButton({1210, 740, 120, 50}, windowSurf, backButtonIMG, backButtonHoverIMG) == PRESSED){
                    resetBattleMenu();
                }
                playersItems[currItem][0]->displayItem(windowSurf, {1170, 630, 0, 0});
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
                        playersPokeballs[i][0]->displayItem(windowSurf, displayPos);
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

int Player::getTeamAverageLevel() {
    int total = 0;
    for (Pokemon* pokemon : playersPokemon){
        total += pokemon->getLevel();
    }

    return total / playersPokemon.size();
}

void Player::calculateTeamExperience(Pokemon* pokemonDefeated) {
    /////////////////////////////Check if currPokemon hurts itself and faints
    int experienceToAdd = pokemonDefeated->getExperience() + 10;
    messageList.push_back(currPokemon->getName() + " got " + to_string(experienceToAdd) + " EXP!");
    if (playersPokemon.size() > 1) {
        messageList.push_back("Everyone else got " + to_string((experienceToAdd) / 2) + " EXP.");
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
                uniform_real_distribution<double> randomChanceRange(0, 1);
                const float CHANCE_TO_LEARN_NEW_MOVE = 1;

                if (randomChanceRange(outputNum) <= CHANCE_TO_LEARN_NEW_MOVE) {
                    try {
                        pokemon->addRandomMove(typeid(*pokemon).name());
                        messageList.push_back(pokemon->getName() + " learned " + pokemon->getMoves()[moveCount]);
                    } catch (string &errorMessage) {
                        cout << errorMessage << endl;
                    }
                }
            }
        }
    }
}
