//
// Created by tropi on 4/1/2023.
//

#include "pokemon.h"
#include "button.h"
#include <iostream>
#include <random>

Pokemon::Pokemon(string name, int level, int health, SDL_Surface *pokeImage) {
    this->name = name;
    this->level = level;
    this->health = health;
    this->maxHealth = health;
    this->pokeImage = pokeImage;
    pokeRect = {0, 0, this->pokeImage->w, this->pokeImage->h};
    currAttack = -1;
}

Pokemon::~Pokemon() {
    SDL_FreeSurface(pokeImage);
}

bool Pokemon::attack(Pokemon* pokemonToAttack, vector<string> &messages) {
    pokemonToAttack->addToHealth(-movePower[currAttack]);
    messages.push_back("Damage delt: " + to_string(movePower[currAttack]));
    currAttack = -1;
    return true;
}

bool Pokemon::displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, vector<string> &messages) {
    for (int i = 0; i < sizeof(moveNames) / sizeof(moveNames[0]); i++) {
        SDL_Surface *textSurf = TTF_RenderText_Solid(font, moveNames[i].c_str(), {255, 255, 255});
        SDL_Rect dest = {800, 500 + i * 90, 0, 0};
        SDL_BlitSurface(textSurf, nullptr, windowSurf, &dest);
    }

    if (checkForClickAndDisplayButton({800, 500, 90, 90}, font, windowSurf)) {
        currAttack = 0;
    } else if (checkForClickAndDisplayButton({800, 580, 90, 90}, font, windowSurf)) {
        currAttack = 1;
    } else if (checkForClickAndDisplayButton({800, 670, 90, 90}, font, windowSurf)) {
        currAttack = 2;
    } else if (checkForClickAndDisplayButton({800, 760, 90, 90}, font, windowSurf)) {
        currAttack = 3;
    }

    if (currAttack >= 0 && currAttack < 4){
        messages.push_back(to_string(currAttack + 1));
        return true;
    }
    else{
        return false;
    }

}

void Pokemon::pickRandomMove(vector<string> &messages) {
    int movesCount = 0;
    random_device random;

    mt19937 outputNum(random());

    for (const string& moveName : moveNames){
        if (!moveName.empty()){
            movesCount++;
        }
    }

    uniform_real_distribution<double> randomRange(0, movesCount - 0.01);

    currAttack = static_cast<int>(randomRange(outputNum));

    messages.push_back(to_string(currAttack + 1));

}

void Pokemon::addToHealth(int amount) {
    health += amount;
    if (health > maxHealth){
        health = maxHealth;
    }
    else if (health < 0){
        health = 0;
    }
}

int Pokemon::getHealth() const {
    return health;
}

float Pokemon::getHealthPercent() const {
    return static_cast<float>(health) / static_cast<float>(maxHealth);
}

string Pokemon::getName() const {
    return name;
}

int Pokemon::getLevel() const {
    return level;
}

void Pokemon::displayPokemonInfoButton(SDL_Surface* windowSurf, SDL_Rect destRect) {
    SDL_BlitSurface(pokeImage, nullptr, windowSurf, &destRect);
}

FireType::FireType(string name, int level, int health, SDL_Surface *pokeImage, int fireTemperature) : Pokemon(name, level, health, pokeImage){
    this->fireTemperature = fireTemperature;
}

bool FireType::attack(Pokemon* pokemonToAttack, vector<string> &messages) {
    ////////////////////////////Add change to movePower[currAttack] based on heat here
    Pokemon::attack(pokemonToAttack, messages);
    ////////////////////////////////////////Add message that is was super effective or not here
    return true;
}

bool FireType::displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, vector<string> &messages) {
    return Pokemon::displayAndChooseMoves(font, windowSurf, messages);
}

WaterType::WaterType(string name, int level, int health, SDL_Surface *pokeImage, int mineralValue) : Pokemon(name, level, health, pokeImage) {
    this->mineralValue = mineralValue;
}

bool WaterType::attack(Pokemon* pokemonToAttack, vector<string> &messages) {
    if (moveNames[currAttack] != "Heal") {
        Pokemon::attack(pokemonToAttack, messages);
    }
    else {
        heal(movePower[currAttack]);
    }
    return true;
}

void WaterType::heal(int healAmount) {

}

bool WaterType::displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, vector<string> &messages) {
    return Pokemon::displayAndChooseMoves(font, windowSurf, messages);
}

GrassType::GrassType(string name, int level, int health, SDL_Surface *pokeImage, float waterEfficiency) : Pokemon(name, level, health, pokeImage) {
    this->waterEfficiency = waterEfficiency;
    percentDriedUp = 100;
}

bool GrassType::attack(Pokemon* pokemonToAttack, vector<string> &messages) {
    Pokemon::attack(pokemonToAttack, messages);
    return true;
}

bool GrassType::displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, vector<string> &messages) {
    return Pokemon::displayAndChooseMoves(font, windowSurf, messages);
}

IceType::IceType(string name, int level, int health, SDL_Surface *pokeImage, float inchesOfIceDefense) : Pokemon(name, level, health, pokeImage) {
    this->inchesOfIceDefense = inchesOfIceDefense;
}

bool IceType::attack(Pokemon* pokemonToAttack, vector<string> &messages) {
    Pokemon::attack(pokemonToAttack, messages);
    return true;
}

bool IceType::displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, vector<string> &messages) {
    return Pokemon::displayAndChooseMoves(font, windowSurf, messages);
}
