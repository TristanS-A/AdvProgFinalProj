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

    virtual ~Pokemon();

    virtual bool attack(Pokemon* pokemonToAttack, vector<string> &messages) = 0;

    virtual void displayPokemonAndInfo(SDL_Surface* windowSurf);

    virtual void displayPokemonInfoButton(SDL_Surface* windowSurf, SDL_Rect destRect);

    virtual bool displayAndChooseMoves(SDL_Surface* windowSurf, vector<string> &messages);

    virtual void pickRandomMove(vector<string> &messages);

    virtual void addToHealth(int amount);

    int getHealth() const;

    float getHealthPercent() const;

    string getName() const;

    int getLevel() const;

    void setImagePos(int x, int y);

    void setInfoPos(int x, int y);

    SDL_Rect getImagePos();

    SDL_Rect getInfoPos();

    /////////////////////////////Review protected and stuff
protected:
    string name;
    int health;
    int maxHealth;
    int level;
    SDL_Surface* pokeImage;
    SDL_Rect pokeRect;
    ////////////////////////////////////////////////////Maybe make a limit to how many times you can use a move
    string moveNames[4] = {"Spark", "Explosion", "Fire Fang", "Giga Impact"};
    int movePower[4] = {1, 5,  20, 50};
    string moveDescriptions[4] = {"Does an attack.", "Does like double the attack 1.", "A move where the pokemon tries to eat the other pokemon.", "Slams the enemie with all its might (Recoil Damage)."};
    SDL_Rect infoDestination;
    int currAttack;
};

class FireType : public Pokemon{
public:
    FireType(string name, int level, int health, SDL_Surface *pokeImage, int fireTemperature);

    bool attack(Pokemon* pokemonToAttack, vector<string> &messages) override;

    void displayPokemonAndInfo(SDL_Surface* windowSurf) override;

    bool displayAndChooseMoves(SDL_Surface* windowSurf, vector<string> &messages) override;
private:
    int fireTemperature;
};

class WaterType : public Pokemon{
public:
    WaterType(string name, int level, int health, SDL_Surface *pokeImage, int mineralValue);

    bool attack(Pokemon* pokemonToAttack, vector<string> &messages) override;

    void heal(int healAmount);

    void displayPokemonAndInfo(SDL_Surface* windowSurf) override;

    bool displayAndChooseMoves(SDL_Surface* windowSurf, vector<string> &messages) override;
private:
    int mineralValue;
};

class GrassType : public Pokemon{
public:
    GrassType(string name, int level, int health, SDL_Surface *pokeImage, float waterEfficiency);

    bool attack(Pokemon* pokemonToAttack, vector<string> &messages) override;

    void displayPokemonAndInfo(SDL_Surface* windowSurf) override;

    bool displayAndChooseMoves(SDL_Surface* windowSurf, vector<string> &messages) override;
private:
    float percentDriedUp;
    float waterEfficiency;
};

class IceType : public Pokemon{
public:
    IceType(string name, int level, int health, SDL_Surface *pokeImage, float inchesOfIceDefense);

    bool attack(Pokemon* pokemonToAttack, vector<string> &messages) override;

    void displayPokemonAndInfo(SDL_Surface* windowSurf) override;

    bool displayAndChooseMoves(SDL_Surface* windowSurf, vector<string> &messages) override;
private:
    float inchesOfIceDefense;
};




#endif //ADVPROGFINPROJ_POKEMON_H
