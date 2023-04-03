//
// Created by tropi on 4/1/2023.
//

#include "pokemon.h"
#include <iostream>
#include <random>

Pokemon::Pokemon(string name, int level, int health, SDL_Surface *pokeImage) {
    this->name = name;
    this->level = level;
    this->health = health;
    this->pokeImage = pokeImage;
    pokeRect = {0, 0, this->pokeImage->w, this->pokeImage->h};
    currAttack = -1;
}

bool Pokemon::attack(Pokemon* pokemonToAttack, vector<string> &messages) {
    pokemonToAttack->dealDamage(movePower[currAttack]);
    messages.push_back("Damage delt: " + to_string(movePower[currAttack]));
    currAttack = -1;
    return true;
}

bool Pokemon::displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, bool mouseDown, vector<string> &messages) {
    for (int i = 0; i < sizeof(moveNames) / sizeof(moveNames[0]); i++) {
        SDL_Surface *textSurf = TTF_RenderText_Solid(font, moveNames[i].c_str(), {255, 255, 255});
        SDL_Rect dest = {800, 500 + i * 90, 0, 0};
        SDL_BlitSurface(textSurf, nullptr, windowSurf, &dest);
    }

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    if (mouseDown) {
        if (mouseY > 500 && mouseY < 580 && mouseX > 800 && mouseX < 900) {
            currAttack = 0;
        } else if (mouseY > 590 && mouseY < 670 && mouseX > 800 && mouseX < 900) {
            currAttack = 1;
        } else if (mouseY > 680 && mouseY < 760 && mouseX > 800 && mouseX < 900) {
            currAttack = 2;
        } else if (mouseY > 770 && mouseY < 850 && mouseX > 800 && mouseX < 900) {
            currAttack = 3;
        }
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

    mt19937 mt(random());

    for (const string& moveName : moveNames){
        if (!moveName.empty()){
            movesCount++;
        }
    }

    uniform_real_distribution<double> dist(0, movesCount - 1.01);

    currAttack = static_cast<int>(dist(mt));

    messages.push_back(to_string(currAttack + 1));

}

void Pokemon::dealDamage(int damage) {
    health -= damage;
}

int Pokemon::getHealth() const {
    return  health;
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

bool FireType::displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, bool mouseDown, vector<string> &messages) {
    return Pokemon::displayAndChooseMoves(font, windowSurf, mouseDown, messages);
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

bool WaterType::displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, bool mouseDown, vector<string> &messages) {
    return Pokemon::displayAndChooseMoves(font, windowSurf, mouseDown, messages);
}

GrassType::GrassType(string name, int level, int health, SDL_Surface *pokeImage, float waterEfficiency) : Pokemon(name, level, health, pokeImage) {
    this->waterEfficiency = waterEfficiency;
    percentDriedUp = 100;
}

bool GrassType::attack(Pokemon* pokemonToAttack, vector<string> &messages) {
    Pokemon::attack(pokemonToAttack, messages);
    return true;
}

bool GrassType::displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, bool mouseDown, vector<string> &messages) {
    return Pokemon::displayAndChooseMoves(font, windowSurf, mouseDown, messages);
}

IceType::IceType(string name, int level, int health, SDL_Surface *pokeImage, float inchesOfIceDefense) : Pokemon(name, level, health, pokeImage) {
    this->inchesOfIceDefense = inchesOfIceDefense;
}

bool IceType::attack(Pokemon* pokemonToAttack, vector<string> &messages) {
    Pokemon::attack(pokemonToAttack, messages);
    return true;
}

bool IceType::displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, bool mouseDown, vector<string> &messages) {
    return Pokemon::displayAndChooseMoves(font, windowSurf, mouseDown, messages);
}
