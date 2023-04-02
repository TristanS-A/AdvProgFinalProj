//
// Created by tropi on 4/1/2023.
//

#ifndef ADVPROGFINPROJ_POKEMON_H
#define ADVPROGFINPROJ_POKEMON_H

#include "SDL.h"
#include "string"

using namespace std;

class Pokemon{
public:
    void attack(string name, int damage);

    void displayMoves(string* moveNames, int movePower);
protected:
    string name;
    int health;
    int level;
    SDL_Rect pokeRect;
    SDL_Surface* pokeImage;
    string moveNames[4];
    int movePower[4];
    int currAttack;
};

class FireType : public Pokemon{
public:
    FireType(string name, SDL_Surface* pokeImage, int health);

    void fireAttack(string name, int damage);

    void displayMoves(string* moveNames, int movePower);
private:
    int fireTemperature;
};

class WaterType : public Pokemon{
public:
    WaterType(string name, SDL_Surface* pokeImage, int health);

    void waterType(string name, int damage);

    void heal(string name, int healAmount);

    void displayMoves(string* moveNames, int movePower);
private:
    int mineralValue;
};

class GrassType : public Pokemon{
public:
    GrassType(string name, SDL_Surface* pokeImage, int health);

    void grassAttack(string name, int damage);

    void displayMoves(string* moveNames, int movePower);
private:
    float percentDriedUp;
};

class IceType : public Pokemon{
public:
    IceType(string name, SDL_Surface* pokeImage, int health);

    void iceAttack(string name, int damage);

    void displayMoves(string* moveNames, int movePower);
private:
    float inchesOfIceDefense;
};




#endif //ADVPROGFINPROJ_POKEMON_H
