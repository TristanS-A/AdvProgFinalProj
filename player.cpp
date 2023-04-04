//
// Created by tropi on 4/1/2023.
//
#include "player.h"
#include "SDL.h"
#include "SDL_image.h"
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

void Player::addToPlayersPokemon(Pokemon* pokemon) {
    playersPokemon.push_back(pokemon);
}

void Player::removeFromPlayersPokemon(Pokemon *pokemon) {
    playersPokemon.erase(find(playersPokemon.begin(), playersPokemon.end(), pokemon));
}

Pokemon *Player::getCurrPokemon() {
    return playersPokemon[currPokemon];
}

playerAction Player::displayBattleMenu(TTF_Font *font, SDL_Surface *windowSurf, bool mouseDown, vector<string> &messages) {
    switch (chosenAction){
        case NOT_CHOSEN:
            if (mouseDown){
                chosenAction = ATTACKING;
            }
            break;
        case ATTACKING:
            if (this->getCurrPokemon()->displayAndChooseMoves(font, windowSurf, mouseDown,
                                                             messages)){
                return chosenAction;
            }
            break;
        case USE_ITEM:
            break;
        case CATCH:
            break;
        case RUN:
            break;
    }

    return NOT_CHOSEN;
}

void Player::resetBattleMenu() {
    chosenAction = NOT_CHOSEN;
}
