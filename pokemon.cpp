//
// Created by tropi on 4/1/2023.
//

#include "pokemon.h"
#include "button.h"
#include "globalVariables.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <filesystem>

Pokemon::Pokemon(string name, int level, int healthOffset) {
    this->name = name;
    this->level = level;
    this->health = 20 + LEVEL_BOOST / 2 * level + healthOffset;
    this->maxHealth = health;
    string filePath = "images/pokemon/"+ name + "Left.png";

    namespace fs = std::filesystem;
    fs::path f{ filePath };

    if (fs::exists(f)) {
        this->leftImage = IMG_Load(filePath.c_str());
        pokeRect = {0, 0, this->leftImage->w, this->leftImage->h};

    } else {
        cout << "Could not find left image for the name " + name << endl;
        this->leftImage = IMG_Load("images/m/png");
    }

    filePath = "images/pokemon/"+ name + "Right.png";

    if (fs::exists(f)) {
        this->rightImage = IMG_Load(filePath.c_str());
        pokeRect = {0, 0, this->rightImage->w, this->rightImage->h};

    } else {
        cout << "Could not find right image for the name " + name << endl;
        this->rightImage = IMG_Load("images/m/png");
    }

    pokeImage = leftImage;

    pokeRect = {0, 0, this->pokeImage->w, this->pokeImage->h};
    infoDestination = {0, 0, 0, 0};
    baseAttackPower = 1 + ((level - 1.0) / LEVEL_BOOST);
    baseDefensePower = 1 + ((level - 1.0) / LEVEL_BOOST);
    experience = BASE_EXPERIENCE * (level - 1) * (level - 1);
    experienceUntilNextLevel = BASE_EXPERIENCE + experience * 3;
    levelUpsWithoutNewMove = 0;
}

Pokemon::~Pokemon() {
    SDL_FreeSurface(leftImage);
    SDL_FreeSurface(rightImage);
    pokeImage = nullptr;
    //SDL_FreeSurface(pokeImage);
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
        int buttonOffset = 50;
        int buttonLength;
        int fontSize;

        if (moveNames[i].size() < 10) {
            fontSize = int(MEDIUM_FONT_SIZE / FONT_PIXEL_HEIGHT_TO_WIDTH);
            buttonLength = moveNames[i].size() * fontSize + buttonOffset * 2;
            textSurf = TTF_RenderText_Solid(mediumFont, moveNames[i].c_str(), {255, 255, 255});
        } else {
            fontSize = int(SMALL_FONT_SIZE / FONT_PIXEL_HEIGHT_TO_WIDTH);
            buttonLength = moveNames[i].size() * fontSize + buttonOffset * 2;
            textSurf = TTF_RenderText_Solid(smallFont, moveNames[i].c_str(), {255, 255, 255});
        }

        ButtonState movesButton = checkForClickAndDisplayButton({150 + spacing - buttonOffset, 650, buttonLength, int(buttonOffset * 1.5)}, windowSurf, buttonIMG, buttonHoverIMG);

        SDL_Rect dest = {150 + spacing, 655, 0, 0};
        SDL_BlitSurface(textSurf, nullptr, windowSurf, &dest);
        SDL_FreeSurface(textSurf);

        string powerDisplay = "Power: " + to_string(movePower[i]);

        textSurf = TTF_RenderText_Solid(smallFont, powerDisplay.c_str(), {255, 255, 255});
        dest = {150 + spacing + int(moveNames[i].length() * fontSize / 2) -
                int(powerDisplay.length() * SMALL_FONT_SIZE / FONT_PIXEL_HEIGHT_TO_WIDTH / 2.0), 650 + MEDIUM_FONT_SIZE + SMALL_FONT_SIZE / 2, 0, 0};
        SDL_BlitSurface(textSurf, nullptr, windowSurf, &dest);
        SDL_FreeSurface(textSurf);

        if (movesButton == PRESSED){
            currAttack = i;
        }
        else if (movesButton == HOVER){
            textSurf = TTF_RenderText_Solid(smallFont, moveDescriptions[i].c_str(), {255, 255, 255});
            dest = {100, 750, 0, 0};
            SDL_BlitSurface(textSurf, nullptr, windowSurf, &dest);
            SDL_FreeSurface(textSurf);
        }

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
    health += amount;
    if (amount > 0) {
        currSound = healSound;
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

    Mix_PlayChannel(1, currSound, 0);
}

void Pokemon::restore() {
    health = maxHealth;
    ////////////////Put sp full here if implemented
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

void Pokemon::displayPokemonAndInfo(SDL_Surface *windowSurf, bool isPlayersPokemon) {
    int spaceing = 30;

    SDL_Rect offset = {infoDestination.x - spaceing, infoDestination.y - spaceing, 0, 0};
    SDL_BlitSurface(pokeInfoBG, nullptr, windowSurf, &offset);
    SDL_BlitSurface(pokeImage, nullptr, windowSurf, &pokeRect);

    string pokeInfo = getName() + "    " + to_string(getHealth()) + "/" + to_string(maxHealth) + " HP";
    textSurf = TTF_RenderText_Solid(mediumFont, (pokeInfo).c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &infoDestination);
    SDL_FreeSurface(textSurf);

    ////////////////////////////////////////////////////Maybe clean this up
    pokeInfo = "Level: " + to_string(getLevel());
    spaceing = 50;
    offset = {infoDestination.x, infoDestination.y + spaceing, 0, 0};
    textSurf = TTF_RenderText_Solid(mediumFont, (pokeInfo).c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &offset);
    SDL_FreeSurface(textSurf);
}

void Pokemon::displayPokemonInfoButton(SDL_Surface* windowSurf, SDL_Rect destRect) {
    SDL_BlitScaled(pokeImage, nullptr, windowSurf, &destRect);

    int spacing = destRect.h;
    int centeredXPos = destRect.x + destRect.w / 2 - name.size() * SMALL_FONT_SIZE / 2 / FONT_PIXEL_HEIGHT_TO_WIDTH;
    SDL_Rect nextLine = {centeredXPos, destRect.y + spacing, 0, 0};

    textSurf = TTF_RenderText_Solid(smallFont, name.c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
    SDL_FreeSurface(textSurf);

    spacing = destRect.h + SMALL_FONT_SIZE;
    string healthInfo = "Health: " + to_string(health) + "/" + to_string(maxHealth);
    centeredXPos = destRect.x + destRect.w / 2 - healthInfo.size() * SMALL_FONT_SIZE / 2 / FONT_PIXEL_HEIGHT_TO_WIDTH;
    nextLine = {centeredXPos, destRect.y + spacing, 0, 0};

    textSurf = TTF_RenderText_Solid(smallFont, healthInfo.c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
    SDL_FreeSurface(textSurf);
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

SDL_Surface *Pokemon::getPokeImage() {
    return pokeImage;
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

    const float SCALER = 100;
    baseAttackPower += LEVEL_BOOST / SCALER;
    baseDefensePower += LEVEL_BOOST / (SCALER * 5);
    experienceUntilNextLevel = BASE_EXPERIENCE + (BASE_EXPERIENCE * (level - 1) * (level - 1)) * 3;

    levelUpsWithoutNewMove++;
}

void Pokemon::addRandomMove(string typeID) {
    random_device random;

    mt19937 outputNum(random());
    uniform_real_distribution<double> randomMoveRange(0, amountOfMovesPerType - 0.001);

    /////////////////////////////Maybe ask teacher about creating variables here and then adding values in loop instead of all in the loop
    int fileLine = int(randomMoveRange(outputNum));
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

    int lineToLoopBackTo = fileLine;

    while (!noDuplicates) {
        fstream fin("moveInfo.txt");
        if (fin.is_open()) {
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
                            fullInfo = "";
                        }
                    } else if (fullInfo == "Power: ") {
                        if (ch != ';') {
                            temp += ch;
                        } else {
                            tempPower = stoi(temp);
                            fullInfo = "";
                        }
                    } else if (fullInfo == "Description: ") {
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
                    if (fullInfo != "FireType\n" && fullInfo != "IceType\n" && fullInfo != "GrassType\n" &&
                        fullInfo != "WaterType\n") {
                        count++;
                    } else {
                        count = 0;
                        if (fullInfo.substr(0, fullInfo.length() - 1) == typeID.substr(1, typeID.length())) {
                            foundType = true;
                        } else {
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
        } else {
            string errorMessage = "Could not find and open moveInfo.txt file.\n";
            moveNames.erase(find(moveNames.begin(), moveNames.end(), moveNames[moveNames.size() - 1]));
            movePower.erase(find(movePower.begin(), movePower.end(), movePower[movePower.size() - 1]));
            moveDescriptions.erase(find(moveDescriptions.begin(), moveDescriptions.end(), moveDescriptions[moveDescriptions.size() - 1]));
            throw errorMessage;
        }

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
int Pokemon::getMaxMoveAmount() const{
    return MAX_MOVE_AMOUNT;
}

int Pokemon::getMaxLevel() const{
    return MAX_LEVEL;
}

int Pokemon::getLevelUpsWithoutNewMove() const{
    return levelUpsWithoutNewMove;
}

void Pokemon::setLevelUpsWithoutNewMove(int num) {
    levelUpsWithoutNewMove = num;
}

void Pokemon::addMoveByName(string moveName) {
    fstream fin("moveInfo.txt");
    if (fin.is_open()) {
        char ch;
        string fullInfo;
        string moveInfo;
        string tempPower;

        moveNames.push_back("");
        movePower.push_back(0);
        moveDescriptions.push_back("");

        while (!fin.eof()) {
            if (fullInfo == "Name: " + moveName + ";") {
                if (moveNames[moveNames.size() - 1].empty()) {
                    moveNames[moveNames.size() - 1] = moveName;
                    moveInfo = "";
                }
                if (moveInfo == "Power: ") {
                    if (ch != ';') {
                        tempPower += ch;
                    } else {
                        movePower[movePower.size() - 1] = stoi(tempPower);
                        moveInfo = "";
                    }
                } else if (moveInfo == "Description: ") {
                    if (ch != ';') {
                        moveDescriptions[moveDescriptions.size() - 1] += ch;
                    } else {
                        break;
                    }
                } else {
                    if ((ch != ' ' || !moveInfo.empty())) {
                        moveInfo += ch;
                    }
                }
            } else {
                if (ch && (ch != ' ' || !fullInfo.empty())) {
                    fullInfo += ch;
                }

                if (ch == '\n') {
                    fullInfo = "";
                }
            }

            fin >> noskipws >> ch;
        }

        fin.close();
    } else {
        string errorMessage = "Could not find and open moveInfo.txt file.\n";
        throw errorMessage;
    }

    if (moveNames[moveNames.size() - 1].empty()){
        string errorMessage = "Could not find the move, " + moveName + ".";
        moveNames.erase(find(moveNames.begin(), moveNames.end(), moveNames[moveNames.size() - 1]));
        movePower.erase(find(movePower.begin(), movePower.end(), movePower[movePower.size() - 1]));
        moveDescriptions.erase(find(moveDescriptions.begin(), moveDescriptions.end(), moveDescriptions[moveDescriptions.size() - 1]));
        throw errorMessage;
    }
}

void Pokemon::setImageFacingRight(bool isFacingLeft) {
    if (isFacingLeft){
        pokeImage = leftImage;
    } else {
        pokeImage = rightImage;
    }
}

ostream &operator<<(ostream &out, const Pokemon *pokemon) {
    out << "Name: " << pokemon->getName() << "\nLevel: " << pokemon->getLevel() << endl << endl;
    return out;
}

int Pokemon::operator+(int num) {
    return num + level;
}

FireType::FireType(string name, int level, int healthOffset, int fireTemperature) : Pokemon(name, level, healthOffset){
    this->fireTemperature = fireTemperature;
    try {
        for (int i = 0; i < (level + 10) / 5 && i < MAX_MOVE_AMOUNT; i++) {
            addRandomMove(typeid(*this).name());
        }
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
        currSound = critHit;
    }
    else if (typeid(*pokemonToAttack) == typeid(WaterType)){
        movePower[currAttack] *= 0.5;
        messageList.push_back("It wasn't very effective...");
        currSound = weakHit;
    } else {
        currSound = regularHit;
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

void FireType::displayPokemonAndInfo(SDL_Surface *windowSurf, bool isPlayersPokemon) {
    Pokemon::displayPokemonAndInfo(windowSurf, isPlayersPokemon);

    ///////////////////////////////////////////////////Maybe redo this with global variables
    int spacingY = 50;
    int spacingX = 200 + to_string(level).size() * MEDIUM_FONT_SIZE;
    SDL_Rect nextLine = {infoDestination.x + spacingX, infoDestination.y + spacingY, 0, 0};
    textSurf = TTF_RenderText_Solid(mediumFont, ("Fire Temperature: " + to_string(fireTemperature)).c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
    SDL_FreeSurface(textSurf);
}

WaterType::WaterType(string name, int level, int healthOffset, float mineralValue) : Pokemon(name, level, healthOffset) {
    this->mineralValue = mineralValue;

    try {
        addMoveByName("Heal");
        for (int i = 1; i < (level + 10) / 5 && i < MAX_MOVE_AMOUNT; i++) {
            addRandomMove(typeid(*this).name());
        }
        if (moveNames.size() == 2){
            if (moveNames[1] == "Ultra Heal") {
                while (moveNames[1] == "Ultra Heal") {
                    moveNames.erase(find(moveNames.begin(), moveNames.end(), moveNames[1]));
                    movePower.erase(find(movePower.begin(), movePower.end(), movePower[1]));
                    moveDescriptions.erase(find(moveDescriptions.begin(), moveDescriptions.end(), moveDescriptions[1]));
                    addRandomMove(typeid(*this).name());
                }
            }
        }
    } catch (string &errorMessage) {
        cout << errorMessage << endl;
    }
}

bool WaterType::attack(Pokemon* pokemonToAttack) {
    if (moveNames[currAttack] != "Heal" && moveNames[currAttack] != "Ultra Heal") {
        int temp = movePower[currAttack];
        movePower[currAttack] *= baseAttackPower;
        if (typeid(*pokemonToAttack) == typeid(FireType)){
            movePower[currAttack] *= 1.5;
            messageList.push_back("It was super effective!");
            currSound = critHit;
        }
        else if (typeid(*pokemonToAttack) == typeid(GrassType)){
            movePower[currAttack] *= 0.5;
            messageList.push_back("It wasn't very effective...");
            currSound = weakHit;
        } else {
            currSound = regularHit;
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

void WaterType::displayPokemonAndInfo(SDL_Surface *windowSurf, bool isPlayersPokemon) {
    Pokemon::displayPokemonAndInfo(windowSurf, isPlayersPokemon);

    ///////////////////////////////////////////////////Maybe redo this with global variables
    int spacingY = 50;
    int spacingX = 200 + to_string(level).size() * MEDIUM_FONT_SIZE;
    SDL_Rect nextLine = {infoDestination.x + spacingX, infoDestination.y + spacingY, 0, 0};
    textSurf = TTF_RenderText_Solid(mediumFont, ("Mineral Value: " + to_string(int(mineralValue)) + "." +
                                                                                  to_string(int(mineralValue * 10) % 10)).c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
    SDL_FreeSurface(textSurf);
}

void WaterType::pickRandomMove() {
    Pokemon::pickRandomMove();

    int loopCheck = 0;
    if (health == maxHealth){
        while (moveNames[currAttack] == "Heal" || moveNames[currAttack] == "Ultra Heal") {
            if (currAttack == moveNames.size() - 1) {
                currAttack = 0;
            } else {
                currAttack++;
            }
            loopCheck++;
            if (loopCheck > MAX_MOVE_AMOUNT){
                break;
            }
        }
        messageList[messageList.size() - 1] = name + " used " + moveNames[currAttack];
    }
}

GrassType::GrassType(string name, int level, int healthOffset, float waterEfficiency) : Pokemon(name, level, healthOffset) {
    this->waterEfficiency = waterEfficiency;
    percentDriedUp = 0;
    baseWaterEfficiency = 3;

    try {
        addMoveByName("Rehydrate");
        for (int i = 1; i < (level + 10) / 5 && i < MAX_MOVE_AMOUNT; i++) {
            addRandomMove(typeid(*this).name());
        }
    } catch (string &errorMessage) {
        cout << errorMessage << endl;
    }
}

bool GrassType::attack(Pokemon* pokemonToAttack) {
    if (moveNames[currAttack] != "Rehydrate") {
        if (percentDriedUp < 100) {
            int temp = movePower[currAttack];
            movePower[currAttack] *= baseAttackPower;
            if (typeid(*pokemonToAttack) == typeid(IceType)) {
                movePower[currAttack] *= 1.5;
                messageList.push_back("It was super effective!");
                currSound = critHit;
            } else if (typeid(*pokemonToAttack) == typeid(FireType)) {
                movePower[currAttack] *= 0.5;
                messageList.push_back("It wasn't very effective...");
                currSound = weakHit;
            } else {
                currSound = regularHit;
            }

            Pokemon::attack(pokemonToAttack);

            movePower[currAttack] = temp;
            setDriedUpPercent(percentDriedUp + (1 + movePower[currAttack] * waterEfficiency) * baseWaterEfficiency);
        } else {
            messageList.push_back(name + " is all dried up and can't attack!");
        }
    }
    else {
        currSound = healSound;
        setDriedUpPercent(percentDriedUp - movePower[currAttack]);
    }

    currAttack = -1;
    return true;
}

void GrassType::setDriedUpPercent(int newPercent) {
    if (newPercent > 100) {
        messageList.push_back(name + " used too much water and overexerted itself!");
        addToHealth(100 - newPercent);
        percentDriedUp = 100;
    } else {
        if (newPercent < 0) {
            newPercent = 0;
        }
        if (newPercent - percentDriedUp <= 0) {
            Mix_PlayChannel(1, currSound, 0);
            messageList.push_back(
                    name + " restored " + to_string(percentDriedUp - newPercent) + " percent of its body water.");
        }

        percentDriedUp = newPercent;
    }
}

bool GrassType::displayAndChooseMoves(SDL_Surface* windowSurf) {
    return Pokemon::displayAndChooseMoves(windowSurf);
}

void GrassType::displayPokemonAndInfo(SDL_Surface *windowSurf, bool isPlayersPokemon) {
    Pokemon::displayPokemonAndInfo(windowSurf, isPlayersPokemon);

    ///////////////////////////////////////////////////Maybe redo this with global variables
    int spacingY = SMALL_FONT_SIZE + 20;
    int spacingX = 200 + to_string(level).size() * SMALL_FONT_SIZE;
    SDL_Rect nextLine = {infoDestination.x + spacingX, infoDestination.y + spacingY, 0, 0};
    textSurf = TTF_RenderText_Solid(smallFont, ("% Dried up: " + to_string(percentDriedUp)).c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
    SDL_FreeSurface(textSurf);

    nextLine = {nextLine.x, nextLine.y + SMALL_FONT_SIZE, 0, 0};
    float waterEfficiencyFlip = 1 - waterEfficiency;
    textSurf = TTF_RenderText_Solid(smallFont, ("Water Efficiency: " + to_string(int(waterEfficiencyFlip * 10) % 10) +
                                to_string(int(waterEfficiencyFlip * 100) % 10) + "%").c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
    SDL_FreeSurface(textSurf);
}

void GrassType::levelUp() {
    Pokemon::levelUp();

    const float SCALER = 500;
    baseWaterEfficiency -= LEVEL_BOOST / SCALER;
}

void GrassType::pickRandomMove() {
    if (percentDriedUp != 100) {
        Pokemon::pickRandomMove();
        if (percentDriedUp == 0 && moveNames[currAttack] == "Rehydrate"){
            if (currAttack == moveNames.size() - 1){
                currAttack = 0;
            } else {
                currAttack++;
            }
            messageList[messageList.size() - 1] = name + " used " + moveNames[currAttack];
        }
    } else {
        for (int i = 0; i < moveNames.size() - 1; i++) {
            if (moveNames[i] == "Rehydrate") {
                currAttack = i;
                messageList.push_back(name + " used " + moveNames[currAttack]);
            }
        }
    }
}

void GrassType::restore() {
    Pokemon::restore();
    percentDriedUp = 0;
}

IceType::IceType(string name, int level, int healthOffset, float inchesOfIceDefense) : Pokemon(name, level, healthOffset) {
    this->inchesOfIceDefense = inchesOfIceDefense;

    try {
        for (int i = 0; i < (level + 10) / 5 && i < MAX_MOVE_AMOUNT; i++) {
            addRandomMove(typeid(*this).name());
        }
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
        currSound = critHit;
    } else if (typeid(*pokemonToAttack) == typeid(GrassType)) {
        movePower[currAttack] *= 0.5;
        messageList.push_back("It wasn't very effective...");
        currSound = weakHit;
    } else {
        currSound = regularHit;
    }

    Pokemon::attack(pokemonToAttack);

    movePower[currAttack] = temp;
    return true;
}

bool IceType::displayAndChooseMoves(SDL_Surface* windowSurf) {
    return Pokemon::displayAndChooseMoves(windowSurf);
}

void IceType::displayPokemonAndInfo(SDL_Surface *windowSurf, bool isPlayersPokemon) {
    Pokemon::displayPokemonAndInfo(windowSurf, isPlayersPokemon);

    ///////////////////////////////////////////////////Maybe redo this with global variables
    int spacingY = 50;
    int spacingX = 200 + to_string(level).size() * MEDIUM_FONT_SIZE;
    SDL_Rect nextLine = {infoDestination.x + spacingX, infoDestination.y + spacingY, 0, 0};
    textSurf = TTF_RenderText_Solid(mediumFont, ("Ice Defense: " + to_string(int(inchesOfIceDefense)) + "." +
            to_string(int(inchesOfIceDefense * 10) % 10) + "``").c_str(), {255, 255, 255});
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &nextLine);
    SDL_FreeSurface(textSurf);
}

void IceType::addToHealth(int amount) {
    if (amount < 0) {
        amount /= inchesOfIceDefense + 1;
    }
    Pokemon::addToHealth(amount);
}
