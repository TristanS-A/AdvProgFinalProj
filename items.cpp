//
// Created by tropi on 4/4/2023.
//

#include "items.h"
#include "globalVariables.h"

Item::Item(string name, SDL_Surface *itemImage, string description) {
    this->name = name;
    this->itemImage = itemImage;
    this->description = description;
}

Item::~Item() {
    SDL_FreeSurface(itemImage);
}

string Item::getName() {
    return name;
}

void Item::displayDescription(SDL_Surface* windowSurf) {
    SDL_Surface* textSurf = TTF_RenderText_Solid(largeFont, name.c_str(), {255, 255, 255});
    SDL_Rect dest = {100, 660, 0, 0};

    SDL_BlitSurface(textSurf, nullptr, windowSurf, &dest);
    textSurf = TTF_RenderText_Solid(mediumFont, description.c_str(), {255, 255, 255});
    dest = {100, 740, 0, 0};
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &dest);
}

void Item::displayItem(SDL_Surface* windowSurf, SDL_Rect destRect) {
    SDL_BlitSurface(itemImage, nullptr, windowSurf, &destRect);
}

HealthItem::HealthItem(string name, int amount, SDL_Surface *itemImage, string description) : Item(name, itemImage, description){
    this->amount = amount;
}

int HealthItem::getAmount() {
    return amount;
}

void HealthItem::use(Pokemon *pokemonToEffect) {
    pokemonToEffect->addToHealth(amount);
}

void HealthItem::displayItem(SDL_Surface *windowSurf, SDL_Rect destRect) {
    Item::displayItem(windowSurf, destRect);
}

BoostItem::BoostItem(string name, EffectType itemEffectType, float boostAmount, SDL_Surface *itemImage, string description) : Item(name, itemImage, description){
    this->itemEffectType = itemEffectType;
    this->boostAmount = boostAmount;
}

float BoostItem::getBoostAmount() {
    return  boostAmount;
}

EffectType BoostItem::getEffectType() {
    return itemEffectType;
}

void BoostItem::use(Pokemon* pokemonToEffect) {
    ///////////////////////////////////////////////Impliment boost functionality (perhaps with base multiplier variable on a pokemon
}

void BoostItem::displayItem(SDL_Surface *windowSurf, SDL_Rect destRect) {
    Item::displayItem(windowSurf, destRect);
}

Pokeball::Pokeball(string name, float catchBooster, SDL_Surface *itemImage, string description) : Item(name, itemImage, description){
    this->catchBooster = catchBooster;
}

float Pokeball::getCatchBooster() const {
    return catchBooster;
}

void Pokeball::use(Pokemon *pokemonToEffect) {
    return;
}

void Pokeball::displayItem(SDL_Surface *windowSurf, SDL_Rect destRect) {
    Item::displayItem(windowSurf, destRect);
}
