//
// Created by tristan.schonfeldt-a on 4/17/2023.
//

#include <fstream>
#include <random>
#include "pokemonNameHandler.h"
#include "globalVariables.h"
#include <iostream>

string getRandomName(string typeID) {
    random_device random;

    mt19937 outputNum(random());
    uniform_real_distribution<double> randomChanceRange(0, amountOfNamesPerType - 0.001);

    /////////////////////////////Maybe ask teacher about creating variables here and then adding values in loop instead of all in the loop
    int fileLine;
    int count = 0;
    char ch;
    string fullInfo;
    string name;
    bool foundType = false;

    fileLine = randomChanceRange(outputNum);

    fstream fin("nameList.txt");
    if (fin.is_open()) {
        while (!fin.eof()) {
            if (foundType && count == fileLine) {
                if (ch != ';') {
                    name += ch;
                } else {
                    return name;
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
        }
    else {
        string errorMessage = "Could not find and open moveInfo.txt file.\n";
        throw errorMessage;
    }

    if (name.empty()){
        string errorMessage = "Could not find a name after looking through all names of its type.";
        throw errorMessage;
    }
}