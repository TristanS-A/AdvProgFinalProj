//
// Created by tropi on 4/1/2023.
//

#include "pokemon.h"
#include "button.h"
#include "globalVariables.h"
#include <iostream>
#include <iomanip>
#include <random>

Pokemon::Pokemon(string name, int level, int health, SDL_Surface *pokeImage) {
    this->name = name;
    this->level = level;
    this->health = health;
    this->maxHealth = health;
    this->pokeImage = pokeImage;
    pokeRect = {0, 0, this->pokeImage->w, this->pokeImage->h};
    infoDestination = {0, 0, 0, 0};
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
    int spacing = 0;
    for (int i = 0; i < sizeof(moveNames) / sizeof(moveNames[0]); i++) {
        SDL_Surface *textSurf;
        int buttonLength = (moveNames[i].size() - 1) * 26;
        int buttonOffset = 50;
        ButtonState movesButton = checkForClickAndDisplayButton({150 + spacing - buttonOffset, 650, buttonLength + buttonOffset * 2, int(buttonOffset * 1.5)}, font, windowSurf, buttonIMG, buttonHoverIMG);
        if (movesButton == PRESSED){
            currAttack = i;
        }
        else if (movesButton == HOVER){
            textSurf = TTF_RenderText_Solid(font, moveDescriptions[i].c_str(), {255, 255, 255});
            SDL_Rect dest = {100, 750, 0, 0};
            SDL_BlitSurface(textSurf, nullptr, windowSurf, &dest);
        }

        textSurf = TTF_RenderText_Solid(font, moveNames[i].c_str(), {255, 255, 255});
        SDL_Rect dest = {150 + spacing, 650, 0, 0};
        SDL_BlitSurface(textSurf, nullptr, windowSurf, &dest);

        spacing += (moveNames[i].size() + 4) * 26;
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

void Pokemon::displayPokemonAndInfo(TTF_Font *font, SDL_Surface *windowSurf) {
    ////////////////////////////////////////////////Maybe also make an image for pokemon info
    SDL_BlitSurface(pokeImage, nullptr, windowSurf, &pokeRect);

    string pokeInfo = getName() + "    " + to_string(getHealth()) + "/" + to_string(maxHealth) + " HP";
    SDL_Surface *textSurf = TTF_RenderText_Solid(font, (pokeInfo).c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &infoDestination);

    ////////////////////////////////////////////////////Maybe clean this up
    pokeInfo = "Level: " + to_string(getLevel());
    int spacing = 50;
    SDL_Rect nextLine = {infoDestination.x, infoDestination.y + spacing, 0, 0};
    textSurf = TTF_RenderText_Solid(font, (pokeInfo).c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
}

void Pokemon::displayPokemonInfoButton(SDL_Surface* windowSurf, SDL_Rect destRect) {
    SDL_BlitScaled(pokeImage, nullptr, windowSurf, &destRect);
}

void Pokemon::setImagePos(int x, int y) {
    pokeRect = {x, y, 0, 0};
}

void Pokemon::setInfoPos(int x, int y) {
    infoDestination = {x, y, 0, 0};
}

SDL_Rect Pokemon::getImagePos() {
    return pokeRect;
}

SDL_Rect Pokemon::getInfoPos() {
    return infoDestination;
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

void FireType::displayPokemonAndInfo(TTF_Font *font, SDL_Surface *windowSurf) {
    Pokemon::displayPokemonAndInfo(font, windowSurf);

    ///////////////////////////////////////////////////Maybe redo this with global variables
    int spacingY = 50;
    int spacingX = 200 + to_string(level).size() * 26;
    SDL_Rect nextLine = {infoDestination.x + spacingX, infoDestination.y + spacingY, 0, 0};
    SDL_Surface* textSurf = TTF_RenderText_Solid(font, ("Fire Temperature: " + to_string(fireTemperature)).c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
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

void WaterType::displayPokemonAndInfo(TTF_Font *font, SDL_Surface *windowSurf) {
    Pokemon::displayPokemonAndInfo(font, windowSurf);

    ///////////////////////////////////////////////////Maybe redo this with global variables
    int spacingY = 50;
    int spacingX = 200 + to_string(level).size() * 26;
    SDL_Rect nextLine = {infoDestination.x + spacingX, infoDestination.y + spacingY, 0, 0};
    SDL_Surface* textSurf = TTF_RenderText_Solid(font, ("Healing Power: " + to_string(mineralValue)).c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
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

void GrassType::displayPokemonAndInfo(TTF_Font *font, SDL_Surface *windowSurf) {
    Pokemon::displayPokemonAndInfo(font, windowSurf);

    ///////////////////////////////////////////////////Maybe redo this with global variables
    int spacingY = 50;
    int spacingX = 200 + to_string(level).size() * 26;
    SDL_Rect nextLine = {infoDestination.x + spacingX, infoDestination.y + spacingY, 0, 0};
    SDL_Surface* textSurf = TTF_RenderText_Solid(font, ("% Dried up: " + to_string(percentDriedUp)).c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
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

void IceType::displayPokemonAndInfo(TTF_Font *font, SDL_Surface *windowSurf) {
    Pokemon::displayPokemonAndInfo(font, windowSurf);

    ///////////////////////////////////////////////////Maybe redo this with global variables
    int spacingY = 50;
    int spacingX = 200 + to_string(level).size() * 26;
    SDL_Rect nextLine = {infoDestination.x + spacingX, infoDestination.y + spacingY, 0, 0};
    SDL_Surface* textSurf = TTF_RenderText_Solid(font, ("Ice Defense: " + to_string(int(inchesOfIceDefense)) + "." +
            to_string(int(inchesOfIceDefense * 10) % 10) + "``").c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
}
