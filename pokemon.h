//
// Created by tropi on 4/1/2023.
//

#ifndef ADVPROGFINPROJ_POKEMON_H
#define ADVPROGFINPROJ_POKEMON_H

#include "SDL.h"
#include "SDL_ttf.h"
#include "string"

using namespace std;

class Pokemon{
public:

    Pokemon(string name, int level, int health, SDL_Surface* pokeImage);

    virtual bool attack(string &message) = 0;

    virtual bool displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, bool mouseDown, string &message);
protected:
    string name;
    int health;
    int level;
    SDL_Rect pokeRect;
    SDL_Surface* pokeImage;
    string moveNames[4] {"attak", "double", "eat", "goooo"};
    int movePower[4];
    int currAttack;
};

class FireType : public Pokemon{
public:
    FireType(string name, int level, int health, SDL_Surface *pokeImage, int fireTemperature);

    bool attack(string &message) override;

    bool displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, bool mouseDown, string &message) override;
private:
    int fireTemperature;
};

class WaterType : public Pokemon{
public:
    WaterType(string name, int level, int health, SDL_Surface *pokeImage, int mineralValue);

    bool attack(string &message) override;

    void heal(int healAmount);

    bool displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, bool mouseDown, string &message) override;
private:
    int mineralValue;
};

class GrassType : public Pokemon{
public:
    GrassType(string name, int level, int health, SDL_Surface *pokeImage, float waterEfficiency);

    bool attack(string &message) override;

    bool displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, bool mouseDown, string &message) override;
private:
    float percentDriedUp;
    float waterEfficiency;
};

class IceType : public Pokemon{
public:
    IceType(string name, int level, int health, SDL_Surface *pokeImage, float inchesOfIceDefense);

    bool attack(string &message) override;

    bool displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, bool mouseDown, string &message) override;
private:
    float inchesOfIceDefense;
};




#endif //ADVPROGFINPROJ_POKEMON_H
