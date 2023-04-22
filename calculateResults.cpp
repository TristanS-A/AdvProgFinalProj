//
// Created by tropi on 4/20/2023.
//

#include "calculateResults.h"

void calculateResults(){

    //Creates output results file
    ofstream fout("Your_Results.txt");
    if (fout.is_open()) {

        vector<Pokemon *> vectorOfPokemon = player->getAllPokemon();
        int teamPower = 0;

        fout << "Your Results: \n\n" << "Total Pokemon caught: " << vectorOfPokemon.size() << "\n\n";

        for (Pokemon *pokemon: vectorOfPokemon) {
            fout << pokemon;
            teamPower = *pokemon + teamPower;
        }

        fout << "Total team power: " << teamPower << endl << endl << "Thanks For Playing!!";

        fout.close();
    }
}