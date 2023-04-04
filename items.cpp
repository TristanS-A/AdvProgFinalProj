//
// Created by tropi on 4/4/2023.
//

#include "items.h"


string Item::getName() {
    return name;
}

HealthItem::HealthItem(int amount) {
    this->amount = amount;
}

int HealthItem::getAmount() {
    return amount;
}

BoostItem::BoostItem(EffectType itemEffectType, float boostAmount) {
    this->itemEffectType = itemEffectType;
    this->boostAmount = boostAmount;
}

float BoostItem::getBoostAmount() {
    return  boostAmount;
}

EffectType BoostItem::getEffectType() {
    return itemEffectType;
}
