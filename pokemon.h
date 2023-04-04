//
// Created by tropi on 4/1/2023.
//

#ifndef ADVPROGFINPROJ_POKEMON_H
#define ADVPROGFINPROJ_POKEMON_H

#include "SDL.h"
#include "SDL_ttf.h"
#include <string>
#include <vector>

using namespace std;

class Pokemon{
public:

    Pokemon(string name, int level, int health, SDL_Surface* pokeImage);

    virtual bool attack(Pokemon* pokemonToAttack, vector<string> &messages) = 0;

    virtual bool displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, vector<string> &messages);

    virtual void pickRandomMove(vector<string> &messages);

    virtual void addToHealth(int amount);

    int getHealth() const;

protected:
    string name;
    int health;
    int maxHealth;
    int level;
    SDL_Surface* pokeImage;
    SDL_Rect pokeRect;
    string moveNames[4] = {"attak", "double", "eat", "goooo"};
    int movePower[4] = {1, 5,  20, 50};
    int currAttack;
};

class FireType : public Pokemon{
public:
    FireType(string name, int level, int health, SDL_Surface *pokeImage, int fireTemperature);

    bool attack(Pokemon* pokemonToAttack, vector<string> &messages) override;

    bool displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, vector<string> &messages) override;
private:
    int fireTemperature;
};

class WaterType : public Pokemon{
public:
    WaterType(string name, int level, int health, SDL_Surface *pokeImage, int mineralValue);

    bool attack(Pokemon* pokemonToAttack, vector<string> &messages) override;

    void heal(int healAmount);

    bool displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, vector<string> &messages) override;
private:
    int mineralValue;
};

class GrassType : public Pokemon{
public:
    GrassType(string name, int level, int health, SDL_Surface *pokeImage, float waterEfficiency);

    bool attack(Pokemon* pokemonToAttack, vector<string> &messages) override;

    bool displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, vector<string> &messages) override;
private:
    float percentDriedUp;
    float waterEfficiency;
};

class IceType : public Pokemon{
public:
    IceType(string name, int level, int health, SDL_Surface *pokeImage, float inchesOfIceDefense);

    bool attack(Pokemon* pokemonToAttack, vector<string> &messages) override;

    bool displayAndChooseMoves(TTF_Font* font, SDL_Surface* windowSurf, vector<string> &messages) override;
private:
    float inchesOfIceDefense;
};




#endif //ADVPROGFINPROJ_POKEMON_H
