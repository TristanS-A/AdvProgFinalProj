//
// Created by tropi on 4/4/2023.
//

#ifndef ADVPROGFINPROJ_ITEMS_H
#define ADVPROGFINPROJ_ITEMS_H

#include "pokemon.h"
#include <string>
using namespace std;

enum EffectType {OFFENSE, DEFENSE};

class Item{
public:

    Item(string name, SDL_Surface* itemImage);

    virtual ~Item();

    string getName();

    virtual void use(Pokemon* pokemonToEffect) = 0;

    virtual void displayItem(SDL_Surface* windowSurf, SDL_Rect destRect);
private:
    string name;
    SDL_Surface* itemImage;
};

class HealthItem : public Item{
public:
    HealthItem(string name, int amount, SDL_Surface* itemImage);

    int getAmount();

    void use(Pokemon* pokemonToEffect) override;

    void displayItem(SDL_Surface* windowSurf, SDL_Rect destRect) override;
private:
    int amount;
};

class BoostItem : public Item{
public:
    BoostItem(string name, EffectType itemEffectType, float boostAmount, SDL_Surface* itemImage);

    float getBoostAmount();

    EffectType getEffectType();

    void use(Pokemon* pokemonToEffect) override;

    void displayItem(SDL_Surface* windowSurf, SDL_Rect destRect) override;
private:
    float boostAmount;
    EffectType itemEffectType;

};

class Pokeball : public Item{
public:
    Pokeball(string name, float catchBooster, SDL_Surface* itemImage);

    float getCatchBooster() const;

    void use(Pokemon* pokemonToEffect) override;

    void displayItem(SDL_Surface* windowSurf, SDL_Rect destRect) override;
private:
    float catchBooster;
};

////////////////////////////////////////////Passably an effect nullified item

#endif //ADVPROGFINPROJ_ITEMS_H
