//
// Created by tropi on 4/1/2023.
//

#include "pokemon.h"
#include <iostream>

Pokemon::Pokemon(string name, int level, int health, SDL_Surface *pokeImage) {
    this->name = name;
    this->level = level;
    this->health = health;
    this->pokeImage = pokeImage;}

bool Pokemon::attack(string &message) {
    message = "Wooo";
    currAttack = 0;
}

bool Pokemon::displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, bool mouseDown, string &message) {
    for (int i = 0; i < sizeof(moveNames) / sizeof(moveNames[0]); i++) {
        SDL_Surface *textSurf = TTF_RenderText_Solid(font, moveNames[i].c_str(), {255, 255, 255});
        SDL_Rect dest = {800, 500 + i * 90, 0, 0};
        SDL_BlitSurface(textSurf, nullptr, windowSurf, &dest);
    }

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    if (mouseDown) {
        if (mouseY > 500 && mouseY < 580 && mouseX > 800 && mouseX < 900) {
            cout << "1\n";
            currAttack = 1;
        } else if (mouseY > 590 && mouseY < 670 && mouseX > 800 && mouseX < 900) {
            cout << "2\n";
            currAttack = 2;
        } else if (mouseY > 680 && mouseY < 760 && mouseX > 800 && mouseX < 900) {
            cout << "3\n";
            currAttack = 3;
        } else if (mouseY > 770 && mouseY < 850 && mouseX > 800 && mouseX < 900) {
            cout << "4\n";
            currAttack = 4;
        }
    }

    if (currAttack > 0 && currAttack < 5){
        message = "Hello";
        return true;
    }
    else{
        message = "";
        return false;
    }

}

FireType::FireType(string name, int level, int health, SDL_Surface *pokeImage, int fireTemperature) : Pokemon(name, level, health, pokeImage){

}

bool FireType::attack(string &message) {
    Pokemon::attack(message);
    return true;
}

bool FireType::displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, bool mouseDown, string &message) {
    Pokemon::displayAndChooseMoves(font, windowSurf, mouseDown, message);
}

WaterType::WaterType(string name, int level, int health, SDL_Surface *pokeImage, int mineralValue) : Pokemon(name, level, health, pokeImage) {

}

bool WaterType::attack(string &message) {
    if (moveNames[currAttack] != "Heal") {
        Pokemon::attack(message);
    }
    else {
        heal(movePower[currAttack]);
    }
    return true;
}

void WaterType::heal(int healAmount) {

}

bool WaterType::displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, bool mouseDown, string &message) {
    Pokemon::displayAndChooseMoves(font, windowSurf, mouseDown, message);
}

GrassType::GrassType(string name, int level, int health, SDL_Surface *pokeImage, float waterEfficiency) : Pokemon(name, level, health, pokeImage) {

}

bool GrassType::attack(string &message) {
    Pokemon::attack(message);
    return true;
}

bool GrassType::displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, bool mouseDown, string &message) {
    Pokemon::displayAndChooseMoves(font, windowSurf, mouseDown, message);
}

IceType::IceType(string name, int level, int health, SDL_Surface *pokeImage, float inchesOfIceDefense) : Pokemon(name, level, health, pokeImage) {

}

bool IceType::attack(string &message) {
    Pokemon::attack(message);
    return true;
}

bool IceType::displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, bool mouseDown, string &message) {
    Pokemon::displayAndChooseMoves(font, windowSurf, mouseDown, message);
}
