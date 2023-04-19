//
// Created by tropi on 4/19/2023.
//

#include "getRandomPokemon.h"

Pokemon* getRandomPokemon(){
    string randomName;

    random_device random;

    mt19937 outputNum(random());

    float memberOffset = randomChanceRange(outputNum);

    int wildPokemonLevel = static_cast<int>(player->getTeamAverageLevel() +
                                            randomLevelRange(outputNum));
    if (wildPokemonLevel <= 0) {
        wildPokemonLevel = 1;
    }

    if (bgPos.x > topLeftQuadrant[0] && bgPos.y > topLeftQuadrant[1]) {
        currBattleBackground = battleBackground_Snow;
        try {
            randomName = getRandomName(typeid(IceType).name());
        } catch (string message){
            cout << message << endl;
        }
        return new IceType(randomName,
                                  wildPokemonLevel,
                                  int(randomLevelRange(outputNum)),
                                  0.5 + memberOffset);
    } else if (bgPos.x < topRightQuadrant[0] && bgPos.y > topRightQuadrant[1]) {
        currBattleBackground = battleBackground_Forest;

        if (memberOffset > 0.9) {
            memberOffset = 0.9;
        } else if (memberOffset < 0.1) {
            memberOffset = 0.1;
        }

        try {
            randomName = getRandomName(typeid(GrassType).name());
        } catch (string message){
            cout << message << endl;
        }

        return new GrassType(randomName,
                                    wildPokemonLevel,
                                    int(randomLevelRange(outputNum)),
                                    1 - memberOffset);
    } else if (bgPos.x > bottomLeftQuadrant[0] && bgPos.y < bottomLeftQuadrant[1]) {
        currBattleBackground = battleBackground_Swamp;
        try {
            randomName = getRandomName(typeid(WaterType).name());
        } catch (string message){
            cout << message << endl;
        }
        return new WaterType(randomName,
                                    wildPokemonLevel,
                                    int(randomLevelRange(outputNum)),
                                    1.0 + memberOffset);
    } else if (bgPos.x < bottomRightQuadrant[0] && bgPos.y < bottomRightQuadrant[1]) {
        currBattleBackground = battleBackground_Fire;

        memberOffset *= 100;

        try {
            randomName = getRandomName(typeid(FireType).name());
        } catch (string message){
            cout << message << endl;
        }
        return new FireType(randomName,
                                   wildPokemonLevel,
                                   int(randomLevelRange(outputNum)),
                                   100 + memberOffset);
    }
    return nullptr;
}