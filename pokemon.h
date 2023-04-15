//
// Created by tropi on 4/1/2023.
//

#ifndef ADVPROGFINPROJ_POKEMON_H
#define ADVPROGFINPROJ_POKEMON_H

#include "SDL.h"
#include "SDL_ttf.h"
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Pokemon{
public:

    Pokemon(string name, int level, int healthOffset, SDL_Surface* pokeImage);

    virtual ~Pokemon();

    virtual bool attack(Pokemon* pokemonToAttack) = 0;

    virtual void displayPokemonAndInfo(SDL_Surface* windowSurf);

    virtual void displayPokemonInfoButton(SDL_Surface* windowSurf, SDL_Rect destRect);

    virtual bool displayAndChooseMoves(SDL_Surface* windowSurf);

    virtual void pickRandomMove();

    virtual void addToHealth(int amount);

    int getHealth() const;

    float getHealthPercent() const;

    string getName() const;

    int getLevel() const;

    int getExperience() const;

    bool checkForLevelUp() const;

    void addToExperience(int amount);

    virtual void levelUp();

    void setImagePos(int x, int y);

    void setInfoPos(int x, int y);

    SDL_Rect getImagePos();

    SDL_Rect getInfoPos();

    float getBaseDefense() const;

    void addRandomMove(string typeID);

    void addMoveByName(string moveName);

    vector<string> getMoves();

    int getMaxMoveAmount() const;

    int getMaxLevel() const;

    int getLevelUpsWithoutNewMove() const;

    void setLevelUpsWithoutNewMove(int num);

    /////////////////////////////Review protected and stuff
protected:
    string name;
    int health;
    int maxHealth;
    int level;
    int experience;
    int experienceUntilNextLevel;
    const float LEVEL_BOOST = 10.0;
    const int BASE_EXPERIENCE = 50;
    const int MAX_LEVEL = 100;
    int levelUpsWithoutNewMove;
    float baseAttackPower;
    float baseDefensePower;
    SDL_Surface* pokeImage;
    SDL_Rect pokeRect;
    ////////////////////////////////////////////////////Maybe make a limit to how many times you can use a move
    vector<string> moveNames;
    vector<int> movePower;
    vector<string> moveDescriptions;
    const int MAX_MOVE_AMOUNT = 4;
    SDL_Rect infoDestination;
    int currAttack;
};

class FireType : public Pokemon{
public:
    FireType(string name, int level, int healthOffset, SDL_Surface *pokeImage, int fireTemperature);

    bool attack(Pokemon* pokemonToAttack) override;

    void displayPokemonAndInfo(SDL_Surface* windowSurf) override;

    bool displayAndChooseMoves(SDL_Surface* windowSurf) override;
private:
    int fireTemperature;
};

class WaterType : public Pokemon{
public:
    WaterType(string name, int level, int healthOffset, SDL_Surface *pokeImage, float mineralValue);

    bool attack(Pokemon* pokemonToAttack) override;

    void pickRandomMove() override;

    void displayPokemonAndInfo(SDL_Surface* windowSurf) override;

    bool displayAndChooseMoves(SDL_Surface* windowSurf) override;
private:
    float mineralValue;
};

class GrassType : public Pokemon{
public:
    GrassType(string name, int level, int healthOffset, SDL_Surface *pokeImage, float waterEfficiency);

    bool attack(Pokemon* pokemonToAttack) override;

    void pickRandomMove() override;

    void setDriedUpPercent(int newPercent);

    void displayPokemonAndInfo(SDL_Surface* windowSurf) override;

    bool displayAndChooseMoves(SDL_Surface* windowSurf) override;

    void levelUp() override;
private:
    int percentDriedUp;
    float waterEfficiency;
    float baseWaterEfficiency;
};

class IceType : public Pokemon{
public:
    IceType(string name, int level, int healthOffset, SDL_Surface *pokeImage, float inchesOfIceDefense);

    bool attack(Pokemon* pokemonToAttack) override;

    void addToHealth(int amount) override;

    void displayPokemonAndInfo(SDL_Surface* windowSurf) override;

    bool displayAndChooseMoves(SDL_Surface* windowSurf) override;
private:
    float inchesOfIceDefense;
};




#endif //ADVPROGFINPROJ_POKEMON_H
