//
// Created by tropi on 4/4/2023.
//

#ifndef ADVPROGFINPROJ_ITEMS_H
#define ADVPROGFINPROJ_ITEMS_H

#include <string>
using namespace std;

enum EffectType {OFFENSE, DEFENSE};

class Item{
public:
    string getName();
private:
    string name;
};

class HealthItem{
public:
    HealthItem(int amount);

    int getAmount();
private:
    int amount;
};

class BoostItem{
public:
    BoostItem(EffectType itemEffectType, float boostAmount);

    float getBoostAmount();

    EffectType getEffectType();
private:
    float boostAmount;
    EffectType itemEffectType;

};

////////////////////////////////////////////Passably an effect nullified item

#endif //ADVPROGFINPROJ_ITEMS_H
