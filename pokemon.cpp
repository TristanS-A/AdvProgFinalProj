//
// Created by tropi on 4/1/2023.
//

#include "pokemon.h"
#include "button.h"
#include "globalVariables.h"
#include <iostream>
#include <random>
#include <algorithm>

Pokemon::Pokemon(string name, int level, int healthOffset, SDL_Surface *pokeImage) {
    this->name = name;
    this->level = level;
    this->health = 20 + LEVEL_BOOST / 2 * level + healthOffset;
    this->maxHealth = health;
    this->pokeImage = pokeImage;
    pokeRect = {0, 0, this->pokeImage->w, this->pokeImage->h};
    infoDestination = {0, 0, 0, 0};
    baseAttackPower = 1 + ((level - 1.0) / LEVEL_BOOST);
    baseDefensePower = 1 + ((level - 1.0) / LEVEL_BOOST);
    experience = BASE_EXPERIENCE * (level - 1) * (level - 1);
    experienceUntilNextLevel = BASE_EXPERIENCE + experience * 3;
}

Pokemon::~Pokemon() {
    SDL_FreeSurface(pokeImage);
}

bool Pokemon::attack(Pokemon* pokemonToAttack) {
    pokemonToAttack->addToHealth(int(-movePower[currAttack] * baseAttackPower) / pokemonToAttack->getBaseDefense());
    return true;
}

bool Pokemon::displayAndChooseMoves(SDL_Surface* windowSurf) {
    /////////////////////////////////////Check over this with currAttack = -1 placement;
    int spacing = 0;
    currAttack = -1;
    for (int i = 0; i < moveNames.size(); i++) {
        SDL_Surface *textSurf;
        int buttonOffset = 50;
        int buttonLength = moveNames[i].size() * MEDIUM_FONT_SIZE / FONT_PIXEL_HEIGHT_TO_WIDTH + buttonOffset * 2;
        ButtonState movesButton = checkForClickAndDisplayButton({150 + spacing - buttonOffset, 650, buttonLength, int(buttonOffset * 1.5)}, windowSurf, buttonIMG, buttonHoverIMG);
        if (movesButton == PRESSED){
            currAttack = i;
        }
        else if (movesButton == HOVER){
            textSurf = TTF_RenderText_Solid(smallFont, moveDescriptions[i].c_str(), {255, 255, 255});
            SDL_Rect dest = {100, 750, 0, 0};
            SDL_BlitSurface(textSurf, nullptr, windowSurf, &dest);
        }

        textSurf = TTF_RenderText_Solid(mediumFont, moveNames[i].c_str(), {255, 255, 255});
        SDL_Rect dest = {150 + spacing, 650, 0, 0};
        SDL_BlitSurface(textSurf, nullptr, windowSurf, &dest);

        spacing += buttonLength + buttonOffset / 2;
    }

    if (currAttack >= 0 && currAttack < 4){
        messageList.push_back(name + " used " + moveNames[currAttack] + "!");
        return true;
    }
    else{
        return false;
    }

}

void Pokemon::pickRandomMove() {
    int movesCount = 0;
    random_device random;

    mt19937 outputNum(random());

    for (const string& moveName : moveNames){
        if (!moveName.empty()){
            movesCount++;
        }
    }

    uniform_real_distribution<double> randomRange(0, movesCount - 0.01);

    currAttack = static_cast<int>(randomRange(outputNum));

    messageList.push_back(name + " used " + moveNames[currAttack]);

}

void Pokemon::addToHealth(int amount) {
    //////////////////////////////////////////Add things like water lowers fire temp or something.
    health += amount;
    if (amount > 0) {
        if (health > maxHealth) {
            messageList.push_back(name + " restored " + to_string(amount - (health - maxHealth)) + " HP.");
            health = maxHealth;
        } else {
            messageList.push_back(name + " restored " + to_string(amount) + " HP.");
        }
    }
    else {
        if (health < 0) {
            messageList.push_back(name + " took " + to_string(-amount + health) + " damage.");
            health = 0;
        } else {
            messageList.push_back(name + " took " + to_string(-amount) + " damage.");
        }
    }
}

int Pokemon::getHealth() const {
    return health;
}

float Pokemon::getHealthPercent() const {
    return static_cast<float>(health) / static_cast<float>(maxHealth);
}

string Pokemon::getName() const {
    return name;
}

int Pokemon::getLevel() const {
    return level;
}

void Pokemon::displayPokemonAndInfo(SDL_Surface *windowSurf) {
    ////////////////////////////////////////////////Maybe also make an image for pokemon info
    SDL_BlitSurface(pokeImage, nullptr, windowSurf, &pokeRect);

    string pokeInfo = getName() + "    " + to_string(getHealth()) + "/" + to_string(maxHealth) + " HP";
    SDL_Surface *textSurf = TTF_RenderText_Solid(mediumFont, (pokeInfo).c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &infoDestination);

    ////////////////////////////////////////////////////Maybe clean this up
    pokeInfo = "Level: " + to_string(getLevel());
    int spacing = 50;
    SDL_Rect nextLine = {infoDestination.x, infoDestination.y + spacing, 0, 0};
    textSurf = TTF_RenderText_Solid(mediumFont, (pokeInfo).c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
}

void Pokemon::displayPokemonInfoButton(SDL_Surface* windowSurf, SDL_Rect destRect) {
    SDL_BlitScaled(pokeImage, nullptr, windowSurf, &destRect);

    int spacing = destRect.h;
    int centeredXPos = destRect.x + destRect.w / 2 - name.size() * SMALL_FONT_SIZE / 2 / FONT_PIXEL_HEIGHT_TO_WIDTH;
    SDL_Rect nextLine = {centeredXPos, destRect.y + spacing, 0, 0};

    SDL_Surface* textSurf = TTF_RenderText_Solid(smallFont, name.c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);

    spacing = destRect.h + SMALL_FONT_SIZE;
    string healthInfo = "Health: " + to_string(health) + "/" + to_string(maxHealth);
    centeredXPos = destRect.x + destRect.w / 2 - healthInfo.size() * SMALL_FONT_SIZE / 2 / FONT_PIXEL_HEIGHT_TO_WIDTH;
    nextLine = {centeredXPos, destRect.y + spacing, 0, 0};

    textSurf = TTF_RenderText_Solid(smallFont, healthInfo.c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
}

void Pokemon::setImagePos(int x, int y) {
    pokeRect = {x, y, 0, 0};
}

void Pokemon::setInfoPos(int x, int y) {
    infoDestination = {x, y, 0, 0};
}

SDL_Rect Pokemon::getImagePos() {
    return pokeRect;
}

SDL_Rect Pokemon::getInfoPos() {
    return infoDestination;
}

float Pokemon::getBaseDefense() const {
    return baseDefensePower;
}

int Pokemon::getExperience() const {
    return experience;
}

bool Pokemon::checkForLevelUp() const {
    if (experience >= experienceUntilNextLevel && level < MAX_LEVEL){
        return true;
    }

    return false;
}

void Pokemon::addToExperience(int amount) {
    experience += amount;
}

void Pokemon::levelUp() {
    level++;
    maxHealth += static_cast<int>(LEVEL_BOOST / 2);
    health = maxHealth;

    float scaler = 100;
    baseAttackPower += LEVEL_BOOST / scaler;
    baseDefensePower += LEVEL_BOOST / (scaler * 5);
    experienceUntilNextLevel = BASE_EXPERIENCE + (BASE_EXPERIENCE * (level - 1) * (level - 1)) * 3;
}

void Pokemon::addRandomMove(string typeID) {
    random_device random;

    mt19937 outputNum(random());
    uniform_real_distribution<double> randomChanceRange(0, amountOfMovesPerType - 0.001);
    int fileLine;
    bool noDuplicates = false;
    int count;
    char ch;
    string fullInfo;
    string temp;
    bool foundType;

    string tempName;
    int tempPower;
    string tempDescription;

    moveNames.push_back("");
    movePower.push_back(0);
    moveDescriptions.push_back("");

    fileLine = randomChanceRange(outputNum);

    int lineToLoopBackTo = fileLine;

    while (!noDuplicates) {
        fstream fin("moveInfo.txt");
        fullInfo = "";
        temp = "";
        ch = '\0';
        count = 0;
        tempName = "";
        tempPower = 0;
        tempDescription = "";
        foundType = false;
        noDuplicates = true;
        while (!fin.eof()) {
            if (foundType && count == fileLine) {
                if (fullInfo == "Name: ") {
                    if (ch != ';') {
                        tempName += ch;
                    } else {
                        cout << fileLine << endl;
                        fullInfo = "";
                    }
                }
                else if (fullInfo == "Power: "){
                    if (ch != ';') {
                        temp += ch;
                    } else {
                        tempPower = stoi(temp);
                        fullInfo = "";
                    }
                }
                else if (fullInfo == "Description: "){
                    if (ch != ';') {
                        tempDescription += ch;
                    } else {
                        break;
                    }
                } else {
                    if (ch && (ch != ' ' || !fullInfo.empty())) {
                        fullInfo += ch;
                    }
                }
            }

            if (ch == '\n') {
                if (fullInfo != "FireType\n" && fullInfo != "IceType\n" && fullInfo != "GrassType\n" && fullInfo != "WaterType\n") {
                    count++;
                }
                else {
                    count = 0;
                    if (fullInfo.substr(0, fullInfo.length() - 1) == typeID.substr(1, typeID.length())) {
                        foundType = true;
                    }
                    else {
                        foundType = false;
                    }
                }

                fullInfo = "";
            }

            fin >> noskipws >> ch;

            if (!foundType) {
                fullInfo += ch;
            }
        }

        fin.close();

        for (string name: moveNames) {
            if (name == tempName) {
                noDuplicates = false;
            }
        }

        if (!noDuplicates){
            if (fileLine == amountOfMovesPerType - 1){
                fileLine = 0;
            } else {
                fileLine++;
            }
            if (fileLine == lineToLoopBackTo){
                string errorMessage = "Could not find a new move to give " + name + " after looping through all moves of its type.";
                moveNames.erase(find(moveNames.begin(), moveNames.end(), moveNames[moveNames.size() - 1]));
                movePower.erase(find(movePower.begin(), movePower.end(), movePower[movePower.size() - 1]));
                moveDescriptions.erase(find(moveDescriptions.begin(), moveDescriptions.end(), moveDescriptions[moveDescriptions.size() - 1]));
                throw errorMessage;
            }
        }
    }

    moveNames[moveNames.size() - 1] = tempName;
    movePower[movePower.size() - 1] = tempPower;
    moveDescriptions[moveDescriptions.size() - 1] = tempDescription;
}

vector<string> Pokemon::getMoves() {
    return moveNames;
}
/////////////////Make these two const later because of lag stuff
int Pokemon::getMaxMoveAmount(){
    return MAX_MOVE_AMOUNT;
}

int Pokemon::getMaxLevel(){
    return MAX_LEVEL;
}

FireType::FireType(string name, int level, int healthOffset, SDL_Surface *pokeImage, int fireTemperature) : Pokemon(name, level, healthOffset, pokeImage){
    this->fireTemperature = fireTemperature;
    try {
        addRandomMove(typeid(*this).name());
        addRandomMove(typeid(*this).name());
        addRandomMove(typeid(*this).name());
        addRandomMove(typeid(*this).name());
    } catch (string &errorMessage) {
        cout << errorMessage << endl;
    }
}

bool FireType::attack(Pokemon* pokemonToAttack) {
    ////////////////////////////Add change to movePower[currAttack] based on heat here
    int temp = movePower[currAttack];
    movePower[currAttack] *= baseAttackPower * fireTemperature / 100;

    if (typeid(*pokemonToAttack) == typeid(GrassType)){
        movePower[currAttack] *= 1.5;
        messageList.push_back("It was super effective!");
    }
    else if (typeid(*pokemonToAttack) == typeid(WaterType)){
        movePower[currAttack] *= 0.5;
        messageList.push_back("It wasn't very effective...");
    }

    Pokemon::attack(pokemonToAttack);

    movePower[currAttack] = temp;
    currAttack = -1;
    ////////////////////////////////////////Add message that is was super effective or not here
    return true;
}

bool FireType::displayAndChooseMoves(SDL_Surface* windowSurf) {
    return Pokemon::displayAndChooseMoves(windowSurf);
}

void FireType::displayPokemonAndInfo(SDL_Surface *windowSurf) {
    Pokemon::displayPokemonAndInfo(windowSurf);

    ///////////////////////////////////////////////////Maybe redo this with global variables
    int spacingY = 50;
    int spacingX = 200 + to_string(level).size() * MEDIUM_FONT_SIZE;
    SDL_Rect nextLine = {infoDestination.x + spacingX, infoDestination.y + spacingY, 0, 0};
    SDL_Surface* textSurf = TTF_RenderText_Solid(mediumFont, ("Fire Temperature: " + to_string(fireTemperature)).c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
}

WaterType::WaterType(string name, int level, int healthOffset, SDL_Surface *pokeImage, float mineralValue) : Pokemon(name, level, healthOffset, pokeImage) {
    this->mineralValue = mineralValue;

    try {
        addRandomMove(typeid(*this).name());
        addRandomMove(typeid(*this).name());
        addRandomMove(typeid(*this).name());
        addRandomMove(typeid(*this).name());
    } catch (string &errorMessage) {
        cout << errorMessage << endl;
    }
}

bool WaterType::attack(Pokemon* pokemonToAttack) {
    if (moveNames[currAttack] != "Heal") {
        int temp = movePower[currAttack];
        movePower[currAttack] *= baseAttackPower;
        if (typeid(*pokemonToAttack) == typeid(FireType)){
            movePower[currAttack] *= 1.5;
            messageList.push_back("It was super effective!");
        }
        else if (typeid(*pokemonToAttack) == typeid(GrassType)){
            movePower[currAttack] *= 0.5;
            messageList.push_back("It wasn't very effective...");
        }

        Pokemon::attack(pokemonToAttack);
        movePower[currAttack] = temp;
    }
    else {
        addToHealth(movePower[currAttack] * mineralValue);
    }

    currAttack = -1;
    return true;
}

bool WaterType::displayAndChooseMoves(SDL_Surface* windowSurf) {
    return Pokemon::displayAndChooseMoves(windowSurf);
}

void WaterType::displayPokemonAndInfo(SDL_Surface *windowSurf) {
    Pokemon::displayPokemonAndInfo(windowSurf);

    ///////////////////////////////////////////////////Maybe redo this with global variables
    int spacingY = 50;
    int spacingX = 200 + to_string(level).size() * MEDIUM_FONT_SIZE;
    SDL_Rect nextLine = {infoDestination.x + spacingX, infoDestination.y + spacingY, 0, 0};
    SDL_Surface* textSurf = TTF_RenderText_Solid(mediumFont, ("Mineral Value: " + to_string(int(mineralValue)) + "." +
                                                                                  to_string(int(mineralValue * 10) % 10)).c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
}

GrassType::GrassType(string name, int level, int healthOffset, SDL_Surface *pokeImage, float waterEfficiency) : Pokemon(name, level, healthOffset, pokeImage) {
    this->waterEfficiency = waterEfficiency;
    percentDriedUp = 100;

    try {
        addRandomMove(typeid(*this).name());
        addRandomMove(typeid(*this).name());
        addRandomMove(typeid(*this).name());
        addRandomMove(typeid(*this).name());
    } catch (string &errorMessage) {
        cout << errorMessage << endl;
    }
}

bool GrassType::attack(Pokemon* pokemonToAttack) {
    if (moveNames[currAttack] != "Rehydrate") {
        if (percentDriedUp > 0) {
            int temp = movePower[currAttack];
            movePower[currAttack] *= baseAttackPower;
            if (typeid(*pokemonToAttack) == typeid(IceType)) {
                movePower[currAttack] *= 1.5;
                messageList.push_back("It was super effective!");
            } else if (typeid(*pokemonToAttack) == typeid(FireType)) {
                movePower[currAttack] *= 0.5;
                messageList.push_back("It wasn't very effective...");
            };

            Pokemon::attack(pokemonToAttack);

            movePower[currAttack] = temp;
            setDriedUpPercent(percentDriedUp - movePower[currAttack] * waterEfficiency);
        } else {
            messageList.push_back(name + " is all dried up and can't attack!");
        }
    }
    else {
        setDriedUpPercent(percentDriedUp + movePower[currAttack] * waterEfficiency);
    }

    currAttack = -1;
    return true;
}

void GrassType::setDriedUpPercent(int newPercent) {
    if (newPercent < 0){
        addToHealth(newPercent);
        messageList.push_back(name + " used too much water and overexerted itself!");
        percentDriedUp = 0;
    }
    else if (newPercent > 100){
        percentDriedUp = 100;
    } else {
        percentDriedUp = newPercent;
    }
}

bool GrassType::displayAndChooseMoves(SDL_Surface* windowSurf) {
    return Pokemon::displayAndChooseMoves(windowSurf);
}

void GrassType::displayPokemonAndInfo(SDL_Surface *windowSurf) {
    Pokemon::displayPokemonAndInfo(windowSurf);

    ///////////////////////////////////////////////////Maybe redo this with global variables
    int spacingY = 50;
    int spacingX = 200 + to_string(level).size() * MEDIUM_FONT_SIZE;
    SDL_Rect nextLine = {infoDestination.x + spacingX, infoDestination.y + spacingY, 0, 0};
    SDL_Surface* textSurf = TTF_RenderText_Solid(mediumFont, ("% Dried up: " + to_string(percentDriedUp)).c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
}

IceType::IceType(string name, int level, int healthOffset, SDL_Surface *pokeImage, float inchesOfIceDefense) : Pokemon(name, level, healthOffset, pokeImage) {
    this->inchesOfIceDefense = inchesOfIceDefense;

    try {
        addRandomMove(typeid(*this).name());
        addRandomMove(typeid(*this).name());
        addRandomMove(typeid(*this).name());
        addRandomMove(typeid(*this).name());
    } catch (string &errorMessage) {
        cout << errorMessage << endl;
    }
}

bool IceType::attack(Pokemon* pokemonToAttack) {
    int temp = movePower[currAttack];

    movePower[currAttack] *= baseAttackPower;
    if (typeid(*pokemonToAttack) == typeid(FireType)) {
        movePower[currAttack] *= 1.5;
        messageList.push_back("It was super effective!");
    } else if (typeid(*pokemonToAttack) == typeid(GrassType)) {
        movePower[currAttack] *= 0.5;
        messageList.push_back("It wasn't very effective...");
    };

    Pokemon::attack(pokemonToAttack);

    movePower[currAttack] = temp;
    return true;
}

bool IceType::displayAndChooseMoves(SDL_Surface* windowSurf) {
    return Pokemon::displayAndChooseMoves(windowSurf);
}

void IceType::displayPokemonAndInfo(SDL_Surface *windowSurf) {
    Pokemon::displayPokemonAndInfo(windowSurf);

    ///////////////////////////////////////////////////Maybe redo this with global variables
    int spacingY = 50;
    int spacingX = 200 + to_string(level).size() * MEDIUM_FONT_SIZE;
    SDL_Rect nextLine = {infoDestination.x + spacingX, infoDestination.y + spacingY, 0, 0};
    SDL_Surface* textSurf = TTF_RenderText_Solid(mediumFont, ("Ice Defense: " + to_string(int(inchesOfIceDefense)) + "." +
            to_string(int(inchesOfIceDefense * 10) % 10) + "``").c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
}

void IceType::addToHealth(int amount) {
    if (amount < 0) {
        amount /= inchesOfIceDefense + 1;
    }
    Pokemon::addToHealth(amount);
}
