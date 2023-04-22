//
// Created by tropi on 4/4/2023.
//

#ifndef ADVPROGFINPROJ_ITEMS_H
#define ADVPROGFINPROJ_ITEMS_H

#include "pokemon.h"
#include <string>
using namespace std;

//The type of item that would boost defence or offensive power, but didn't get around to implementing it
enum EffectType {OFFENSE, DEFENSE};

//Class for items the player would use in battle (Wanted to also be able to use outside of battle but didn't get to it)
class Item{
public:

    Item(string name, SDL_Surface* itemImage, string desciption);

    virtual ~Item();

    string getName();

    void displayDescription(SDL_Surface* windowSurf);

    virtual void use(Pokemon* pokemonToEffect) = 0;

    virtual void displayItem(SDL_Surface* windowSurf, SDL_Rect destRect);
private:
    string name;
    SDL_Surface* itemImage;
    string description;
};

//Derived class for a healing item the player can use to heal Pokemon
class HealthItem : public Item{
public:
    HealthItem(string name, int amount, SDL_Surface* itemImage, string description);

    //int getAmount(); //Not used but thought it might be useful in the future

    void use(Pokemon* pokemonToEffect) override;

    void displayItem(SDL_Surface* windowSurf, SDL_Rect destRect) override;
private:
    int amount;
};

//Class for a booster item (implemented since I didn't have the time to implement it)
// (Was also going to make items that gave effects to the other Pokemon)
/*
class BoostItem : public Item{
public:
    BoostItem(string name, EffectType itemEffectType, float boostAmount, SDL_Surface* itemImage, string description);

    float getBoostAmount();

    EffectType getEffectType();

    void use(Pokemon* pokemonToEffect) override;

    void displayItem(SDL_Surface* windowSurf, SDL_Rect destRect) override;
private:
    float boostAmount;
    EffectType itemEffectType;

};
 */

//Class for pokeballs that the player can use to catch pokemon
class Pokeball : public Item{
public:
    Pokeball(string name, float catchBooster, SDL_Surface* itemImage, string description);

    float getCatchBooster() const;

    void use(Pokemon* pokemonToEffect) override;

    void displayItem(SDL_Surface* windowSurf, SDL_Rect destRect) override;
private:
    float catchBooster;
};

#endif //ADVPROGFINPROJ_ITEMS_H
