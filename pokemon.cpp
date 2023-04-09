//
// Created by tropi on 4/1/2023.
//

#include "pokemon.h"
#include "button.h"
#include "globalVariables.h"
#include <iostream>
#include <iomanip>
#include <random>

Pokemon::Pokemon(string name, int level, int healthOffset, SDL_Surface *pokeImage) {
    this->name = name;
    this->level = level;
    this->health = 20 + 10 * level + healthOffset;
    this->maxHealth = health;
    this->pokeImage = pokeImage;
    pokeRect = {0, 0, this->pokeImage->w, this->pokeImage->h};
    infoDestination = {0, 0, 0, 0};
    baseAttackPower = 1 + ((level - 1) / 10.0);
    baseDefensePower = 1 + ((level - 1) / 10.0);
    experience = 50 * (level - 1) * (level - 1);
    experienceUntilNextLevel = 50 * (level) * (level);
}

Pokemon::~Pokemon() {
    SDL_FreeSurface(pokeImage);
}

bool Pokemon::attack(Pokemon* pokemonToAttack) {
    pokemonToAttack->addToHealth(-movePower[currAttack] / pokemonToAttack->getBaseDefense());
    return true;
}

bool Pokemon::displayAndChooseMoves(SDL_Surface* windowSurf) {
    int spacing = 0;
    for (int i = 0; i < sizeof(moveNames) / sizeof(moveNames[0]); i++) {
        SDL_Surface *textSurf;
        int buttonOffset = 50;
        int buttonLength = (moveNames[i].size()) * MEDIUM_FONT_SIZE / FONT_PIXEL_HEIGHT_TO_WIDTH + buttonOffset * 2;
        ButtonState movesButton = checkForClickAndDisplayButton({150 + spacing - buttonOffset, 650, buttonLength, int(buttonOffset * 1.5)}, windowSurf, buttonIMG, buttonHoverIMG);
        if (movesButton == PRESSED){
            currAttack = i;
        }
        else if (movesButton == HOVER){
            textSurf = TTF_RenderText_Solid(smallFont, moveDescriptions[i].c_str(), {255, 255, 255});
            SDL_Rect dest = {100, 750, 0, 0};
            SDL_BlitSurface(textSurf, nullptr, windowSurf, &dest);
        }

        textSurf = TTF_RenderText_Solid(mediumFont, moveNames[i].c_str(), {255, 255, 255});
        SDL_Rect dest = {150 + spacing, 650, 0, 0};
        SDL_BlitSurface(textSurf, nullptr, windowSurf, &dest);

        spacing += buttonLength + buttonOffset / 2;
    }

    if (currAttack >= 0 && currAttack < 4){
        messageList.push_back(name + " used " + moveNames[currAttack] + "!");
        return true;
    }
    else{
        return false;
    }

}

void Pokemon::pickRandomMove() {
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

    messageList.push_back(name + " used " + moveNames[currAttack]);

}

void Pokemon::addToHealth(int amount) {
    //////////////////////////////////////////Add things like water lowers fire temp or something.
    health += amount;
    if (amount > 0) {
        if (health > maxHealth) {
            messageList.push_back(name + " restored " + to_string(amount - (health - maxHealth)) + " HP.");
            health = maxHealth;
        } else {
            messageList.push_back(name + " restored " + to_string(amount) + " HP.");
        }
    }
    else {
        if (health < 0) {
            messageList.push_back(name + " took " + to_string(-amount + health) + " damage.");
            health = 0;
        } else {
            messageList.push_back(name + " took " + to_string(-amount) + " damage.");
        }
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

void Pokemon::displayPokemonAndInfo(SDL_Surface *windowSurf) {
    ////////////////////////////////////////////////Maybe also make an image for pokemon info
    SDL_BlitSurface(pokeImage, nullptr, windowSurf, &pokeRect);

    string pokeInfo = getName() + "    " + to_string(getHealth()) + "/" + to_string(maxHealth) + " HP";
    SDL_Surface *textSurf = TTF_RenderText_Solid(mediumFont, (pokeInfo).c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &infoDestination);

    ////////////////////////////////////////////////////Maybe clean this up
    pokeInfo = "Level: " + to_string(getLevel());
    int spacing = 50;
    SDL_Rect nextLine = {infoDestination.x, infoDestination.y + spacing, 0, 0};
    textSurf = TTF_RenderText_Solid(mediumFont, (pokeInfo).c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
}

void Pokemon::displayPokemonInfoButton(SDL_Surface* windowSurf, SDL_Rect destRect) {
    SDL_BlitScaled(pokeImage, nullptr, windowSurf, &destRect);

    int spacing = destRect.h;
    int centeredXPos = destRect.x + destRect.w / 2 - name.size() * SMALL_FONT_SIZE / 2 / FONT_PIXEL_HEIGHT_TO_WIDTH;
    SDL_Rect nextLine = {centeredXPos, destRect.y + spacing, 0, 0};

    SDL_Surface* textSurf = TTF_RenderText_Solid(smallFont, name.c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);

    spacing = destRect.h + SMALL_FONT_SIZE;
    string healthInfo = "Health: " + to_string(health) + "/" + to_string(maxHealth);
    centeredXPos = destRect.x + destRect.w / 2 - healthInfo.size() * SMALL_FONT_SIZE / 2 / FONT_PIXEL_HEIGHT_TO_WIDTH;
    nextLine = {centeredXPos, destRect.y + spacing, 0, 0};

    textSurf = TTF_RenderText_Solid(smallFont, healthInfo.c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
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

float Pokemon::getBaseDefense() {
    return baseDefensePower;
}

FireType::FireType(string name, int level, int healthOffset, SDL_Surface *pokeImage, int fireTemperature) : Pokemon(name, level, healthOffset, pokeImage){
    this->fireTemperature = fireTemperature;
}

bool FireType::attack(Pokemon* pokemonToAttack) {
    ////////////////////////////Add change to movePower[currAttack] based on heat here
    int temp = movePower[currAttack];
    movePower[currAttack] *= baseAttackPower * fireTemperature / 100;

    if (typeid(*pokemonToAttack) == typeid(GrassType)){
        movePower[currAttack] *= 1.5;
        messageList.push_back("It was super effective!");
    }
    else if (typeid(*pokemonToAttack) == typeid(WaterType)){
        movePower[currAttack] *= 0.5;
        messageList.push_back("It wasn't very effective...");
    }

    Pokemon::attack(pokemonToAttack);

    movePower[currAttack] = temp;
    currAttack = -1;
    ////////////////////////////////////////Add message that is was super effective or not here
    return true;
}

bool FireType::displayAndChooseMoves(SDL_Surface* windowSurf) {
    return Pokemon::displayAndChooseMoves(windowSurf);
}

void FireType::displayPokemonAndInfo(SDL_Surface *windowSurf) {
    Pokemon::displayPokemonAndInfo(windowSurf);

    ///////////////////////////////////////////////////Maybe redo this with global variables
    int spacingY = 50;
    int spacingX = 200 + to_string(level).size() * MEDIUM_FONT_SIZE;
    SDL_Rect nextLine = {infoDestination.x + spacingX, infoDestination.y + spacingY, 0, 0};
    SDL_Surface* textSurf = TTF_RenderText_Solid(mediumFont, ("Fire Temperature: " + to_string(fireTemperature)).c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
}

WaterType::WaterType(string name, int level, int healthOffset, SDL_Surface *pokeImage, float mineralValue) : Pokemon(name, level, healthOffset, pokeImage) {
    this->mineralValue = mineralValue;
}

bool WaterType::attack(Pokemon* pokemonToAttack) {
    if (moveNames[currAttack] != "Heal") {
        int temp = movePower[currAttack];
        movePower[currAttack] *= baseAttackPower;
        if (typeid(*pokemonToAttack) == typeid(FireType)){
            movePower[currAttack] *= 1.5;
            messageList.push_back("It was super effective!");
        }
        else if (typeid(*pokemonToAttack) == typeid(GrassType)){
            movePower[currAttack] *= 0.5;
            messageList.push_back("It wasn't very effective...");
        }

        Pokemon::attack(pokemonToAttack);
        movePower[currAttack] = temp;
    }
    else {
        addToHealth(movePower[currAttack] * mineralValue / 20.0);
    }

    currAttack = -1;
    return true;
}

bool WaterType::displayAndChooseMoves(SDL_Surface* windowSurf) {
    return Pokemon::displayAndChooseMoves(windowSurf);
}

void WaterType::displayPokemonAndInfo(SDL_Surface *windowSurf) {
    Pokemon::displayPokemonAndInfo(windowSurf);

    ///////////////////////////////////////////////////Maybe redo this with global variables
    int spacingY = 50;
    int spacingX = 200 + to_string(level).size() * MEDIUM_FONT_SIZE;
    SDL_Rect nextLine = {infoDestination.x + spacingX, infoDestination.y + spacingY, 0, 0};
    SDL_Surface* textSurf = TTF_RenderText_Solid(mediumFont, ("Mineral Value: " + to_string(mineralValue)).c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
}

GrassType::GrassType(string name, int level, int healthOffset, SDL_Surface *pokeImage, int waterEfficiency) : Pokemon(name, level, healthOffset, pokeImage) {
    this->waterEfficiency = waterEfficiency;
    percentDriedUp = 100;
}

bool GrassType::attack(Pokemon* pokemonToAttack) {
    if (moveNames[currAttack] != "Rehydrate") {
        if (percentDriedUp > 0) {
            int temp = movePower[currAttack];
            movePower[currAttack] *= baseAttackPower;
            if (typeid(*pokemonToAttack) == typeid(IceType)) {
                movePower[currAttack] *= 1.5;
                messageList.push_back("It was super effective!");
            } else if (typeid(*pokemonToAttack) == typeid(FireType)) {
                movePower[currAttack] *= 0.5;
                messageList.push_back("It wasn't very effective...");
            };

            Pokemon::attack(pokemonToAttack);

            movePower[currAttack] = temp;
            setDriedUpPercent(percentDriedUp - movePower[currAttack] * waterEfficiency);
        } else {
            messageList.push_back(name + " is all dried up and can't attack!");
        }
    }
    else {
        setDriedUpPercent(percentDriedUp + movePower[currAttack] * waterEfficiency);
    }

    currAttack = -1;
    return true;
}

void GrassType::setDriedUpPercent(int newPercent) {
    if (newPercent < 0){
        addToHealth(newPercent);
        messageList.push_back(name + " used too much water and overexerted itself!");
        percentDriedUp = 0;
    }
    else if (newPercent > 100){
        percentDriedUp = 100;
    } else {
        percentDriedUp = newPercent;
    }
}

bool GrassType::displayAndChooseMoves(SDL_Surface* windowSurf) {
    return Pokemon::displayAndChooseMoves(windowSurf);
}

void GrassType::displayPokemonAndInfo(SDL_Surface *windowSurf) {
    Pokemon::displayPokemonAndInfo(windowSurf);

    ///////////////////////////////////////////////////Maybe redo this with global variables
    int spacingY = 50;
    int spacingX = 200 + to_string(level).size() * MEDIUM_FONT_SIZE;
    SDL_Rect nextLine = {infoDestination.x + spacingX, infoDestination.y + spacingY, 0, 0};
    SDL_Surface* textSurf = TTF_RenderText_Solid(mediumFont, ("% Dried up: " + to_string(percentDriedUp)).c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
}

IceType::IceType(string name, int level, int healthOffset, SDL_Surface *pokeImage, float inchesOfIceDefense) : Pokemon(name, level, healthOffset, pokeImage) {
    this->inchesOfIceDefense = inchesOfIceDefense;
}

bool IceType::attack(Pokemon* pokemonToAttack) {
    int temp = movePower[currAttack];

    movePower[currAttack] *= baseAttackPower;
    if (typeid(*pokemonToAttack) == typeid(FireType)) {
        movePower[currAttack] *= 1.5;
        messageList.push_back("It was super effective!");
    } else if (typeid(*pokemonToAttack) == typeid(GrassType)) {
        movePower[currAttack] *= 0.5;
        messageList.push_back("It wasn't very effective...");
    };

    Pokemon::attack(pokemonToAttack);

    movePower[currAttack] = temp;
    return true;
}

bool IceType::displayAndChooseMoves(SDL_Surface* windowSurf) {
    return Pokemon::displayAndChooseMoves(windowSurf);
}

void IceType::displayPokemonAndInfo(SDL_Surface *windowSurf) {
    Pokemon::displayPokemonAndInfo(windowSurf);

    ///////////////////////////////////////////////////Maybe redo this with global variables
    int spacingY = 50;
    int spacingX = 200 + to_string(level).size() * MEDIUM_FONT_SIZE;
    SDL_Rect nextLine = {infoDestination.x + spacingX, infoDestination.y + spacingY, 0, 0};
    SDL_Surface* textSurf = TTF_RenderText_Solid(mediumFont, ("Ice Defense: " + to_string(int(inchesOfIceDefense)) + "." +
            to_string(int(inchesOfIceDefense * 10) % 10) + "``").c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
}

void IceType::addToHealth(int amount) {
    if (amount < 0) {
        amount /= inchesOfIceDefense + 1;
    }
    Pokemon::addToHealth(amount);
}
