// Created and edited by Tristan Schonfeldt-Aultman (tropi is also me on the other files)
//
// Certificate of authenticity:
//
/*
Author        : Tristan Schonfeldt-Aultman
Class         : CSI - 240 : Advanced Programming
Assignment    : Final Project
Date Assigned : 3/24/2023
Due Date      : Due-Date and Time as Tuesday 4/23/2023 by 11:59pm


Project Description :
This is my final project for the class. It is a game that mimics Pokemon. Read the README.txt for a more detailed '
description of how the game works and credits

Certification of Authenticity :

I certify that this is entirely my own work, except where I have given
fully-documented references to the work of others. I understand the
definition and consequences of plagiarism and acknowledge that the assessor
of this assignment may, for the purpose of assessing this assignment:
- Reproduce this assignment and provide a copy to another member of
academic staff; and/or
- Communicate a copy of this assignment to a plagiarism checking
service (which may then retain a copy of this assignment on its
database for the purpose of future plagiarism checking)
*/

#define SDL_MAIN_HANDLED true
#define _USE_MATH_DEFINES
#include <iostream>
#include <algorithm>
#include <random>
#include <filesystem>

#include "screenSizeChange.h"
#include "player.h"
#include "handlePlayerMovement.h"
#include "pokemon.h"
#include "items.h"
#include "globalVariables.h"
#include "showMessages.h"
#include "pokemonNameHandler.h"
#include "button.h"
#include "getRandomPokemon.h"
#include "calculateResults.h"
#include "freeingSDLStuff.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

using namespace std;

//Values for screen dimensions
#define SCREEN_WIDTH    1440
#define SCREEN_HEIGHT   810

//I know this is a massive main function, but I didn't have enough time to really clean it up well
int main(int argc, char* argv[]) {

    //--------------------------------------------------------\\
    //------------- SDL Set Up and File Checking -------------\\
    //--------------------------------------------------------\\

    // Unused argc, argv
    (void) argc;
    (void) argv;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        cout << "SDL could not be initialized!" << endl
             << "SDL_Error: " << SDL_GetError() << endl;
        return 0;
    }

    #if defined linux && SDL_VERSION_ATLEAST(2, 0, 8)
        // Disable compositor bypass
       if(!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0"))
       {
           cout << "SDL can not disable compositor bypass!" << endl;
           return 0;
       }
    #endif

   //For if there is an error creating SDL stuff
   bool error = false;

    // Create window
    SDL_Window *window = SDL_CreateWindow("Basic C++ SDL project",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

    //Test for if window wasn't created
    if (!window) {
        cout << "Window could not be created!" << endl
             << "SDL_Error: " << SDL_GetError() << endl;
        error = true;
    }

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cout << "Renderer could not be created!" << endl
             << "SDL_Error: " << SDL_GetError() << endl;
        error = true;
    }

    //Initializes sdl_image
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        cout << "SDL_Image could not be created!" << endl
             << "SDL_Error: " << SDL_GetError() << endl;
        error = true;
    }

    //Sets up SDL_Mixer Audio
    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 1000;

    //Initializes/Opens mixer audio
    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0){
        fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError());
        error = true;
    }

    //Initialize SDL_ttf
    if(TTF_Init() == -1)
    {
        error = true;
    }

    namespace fs = std::filesystem;

    //Checks for images file
    string filePath = "images";
    fs::path f{ filePath };

    if (!fs::exists(f)) {
        cout << "Could not find images file!\n";
        error = true;
    }

    //Checks for pokemon names file
    filePath = "nameList.txt";
    f = { filePath };

    if (!fs::exists(f)) {
        cout << "Could not find pokemon names list file!\n";
        error = true;
    }

    //Checks for pokemon moves file
    filePath = "moveInfo.txt";
    f = { filePath };

    if (!fs::exists(f)) {
        cout << "Could not find pokemon moves list file!\n";
        error = true;
    }

    //Checks for music file
    filePath = "music";
    f = { filePath };

    if (!fs::exists(f)) {
        cout << "Could not find music file!\n";
        error = true;
    }

    //Checks for fonts file
    filePath = "fonts";
    f = { filePath };

    if (!fs::exists(f)) {
        cout << "Could not find fonts file!\n";
        error = true;
    }

    //Checks for sound effects file
    filePath = "sounds";
    f = { filePath };

    if (!fs::exists(f)) {
        cout << "Could not find sounds file!\n";
        error = true;
    }

    //Handles if there was an error initializing something
    if (error) {

        if (renderer){

            // Destroy renderer
            SDL_DestroyRenderer(renderer);
        }

        if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) == 0){

            //Closes audio
            Mix_CloseAudio();
        }

        if (window){

            // Destroy window
            SDL_DestroyWindow(window);
        }

        if (TTF_Init() != -1){

            //Quit SDL_ttf
            TTF_Quit();
        }

        // Quit SDL
        SDL_Quit();

        return 0;
    }

    //---------------------------------------------------\\
    //------------- Variable initialization -------------\\
    //---------------------------------------------------\\

    //Initialize fonts
    smallFont = TTF_OpenFont("fonts/font.ttf", SMALL_FONT_SIZE);
    mediumFont = TTF_OpenFont("fonts/font.ttf", MEDIUM_FONT_SIZE);
    largeFont = TTF_OpenFont("fonts/font.ttf", LARGE_FONT_SIZE);


    //Check for if fonts were correctly opened
    if (!smallFont){
        cout << "Small font could not be opened\n";
    }
    if (!mediumFont){
        cout << "Medium font could not be opened\n";
    }
    if (!largeFont){
        cout << "Large font could not be opened\n";
    }

    //Looks for moveInfo file and calculates the amount of moves per pokemon type there are in the file
    fstream fin("moveInfo.txt");
    if (fin.is_open()) {
        char ch = '\0';
        string fullInfo;

        //Searches by char (to get '\n') and makes a full string of chars with fullInfo
        while (!fin.eof()) {
            fin >> noskipws >> ch;
            fullInfo += ch;

            if (ch == '\n') {

                //So the counting concludes after reaching the end of a type and doesn't stop before going through a type
                if (fullInfo != "FireType\n" && fullInfo != "IceType\n" && fullInfo != "GrassType\n" &&
                    fullInfo != "WaterType\n") {
                    amountOfMovesPerType++;
                } else if (amountOfMovesPerType > 0) {
                    break;
                }
                fullInfo = "";
            }
        }

        fin.close();
    } else {
        cout << "Could not find and open moveInfo.txt file.\n";
    }

    //Looks for nameList.txt file and calculates the amount of names per pokemon type there are in the file
    fin.open("nameList.txt");
    if (fin.is_open()) {
        char ch = '\0';
        string fullInfo;

        //Also looks by char and adds to fullInfo to make full string
        while (!fin.eof()) {
            fin >> noskipws >> ch;
            fullInfo += ch;

            if (ch == '\n') {

                //So the counting concludes after reaching the end of a type and doesn't stop before going through a type
                if (fullInfo != "FireType\n" && fullInfo != "IceType\n" && fullInfo != "GrassType\n" &&
                    fullInfo != "WaterType\n") {
                    amountOfNamesPerType++;
                } else if (amountOfNamesPerType > 0) {
                    break;
                }
                fullInfo = "";
            }
        }

        fin.close();
    } else {
        cout << "Could not find and open nameList.txt file.\n";
    }

    //The background rect with its position, height, and width;
    bgPos = {-background->w / 2 + SCREEN_WIDTH / 2, -background->h / 2 + SCREEN_HEIGHT / 2, background->w, background->h};

    //The curtain rect
    SDL_Rect curtainPos = {0, -SCREEN_HEIGHT, curtain->w, curtain->h};

    //To test if the curtain has dropped
    bool curtainHasDropped = false;

    //To test if the curtain is currently dropping
    bool curtainIsDropping = false;

    //The pokecenter rect
    SDL_Rect pokecenterPos = {bgPos.w / 2 - pokecenter->w / 2, bgPos.h / 2 - pokecenter->h / 2, pokecenter->w, pokecenter->h};

    //The rect for the textbox image
    SDL_Rect textboxPos = {0, SCREEN_HEIGHT - textboxIMG->h, 0, 0};

    //Creates the player
    player = new Player({SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2, 0, 0});

    //So that the player doesn't spawn directly under the pokecenter
    bgPos.y += player->getPlayerRect().h * 2;

    //This exists because SDL's blitting function changes the destination rect's position when blitting, if the
    // position is offscreen
    SDL_Rect placeHolderRect = {0, 0, 0, 0};

    //To quit the game
    bool quit = false;

    //Current time for time tracking
    Uint32 currTime;

    //Previous time for time tracking
    Uint32 prevTime = 0;

    //Creates surface that will be used for blitting
    SDL_Surface *windowSurf = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT,
                                             32, 0, 0, 0, 0);

    //Creates texture from windowSurf surface to be able to render windowSurf surface and the images blitted to it with
    // renderer
    SDL_Texture *windowTexture = SDL_CreateTextureFromSurface(renderer, windowSurf);

    //Adding starting items to players items
    player->addToPlayersItems(new HealthItem("Herb", 20, IMG_Load("images/items/herb.png"), "Heals 20 health points."));
    player->addToPlayersItems(new HealthItem("Herb", 20, IMG_Load("images/items/herb.png"), "Heals 20 health points."));
    player->addToPlayersItems(new HealthItem("Herb", 20, IMG_Load("images/items/herb.png"), "Heals 20 health points."));

    //The chance the player will encounter a wild pokemon
    float encounterChance = 0.15;

    //The succses the player will have in running away from a battle
    float runSuccessRate = 0.5;

    //The chance the player will get an item after winning a battle
    float randomItemChance = 0.3;

    //The amount of time in seconds that will be waited until checking if the player encounters a wild pokemon
    int encounterCheckTime = 1;

    //Pointer for a wild pokemon
    Pokemon* wildPokemon = nullptr;

    //For keeping track of the actions chosen in battles
    int chosenAction = 0;

    //To test when a player has attacked (This was going to be used for implementing animations)
    bool hasAttacked = false;

    //To test if the player had finished the animation of catching a pokemon (Did not get to implementing that animation)
    CatchingState catchState = ANIMATION_NOT_FINISHED;

    //To test if it is the players turn in a battle
    bool playersTurn = true;

    //To force the player to switch pokemon if their current one faints
    bool hadToForceSwitchPokemon = false;

    //To test if the battle has begun (FOr animation purposes)
    bool battleHasBegun = false;

    //To test if the battle is over (For animation purposes and calculating results)
    bool battleIsOver = true;

    //To test if the results are being calculated (For animation purposes)
    bool calculatingResults = false;

    //To check if the player has chosen a starter pokemon
    bool hasChoseStarterPokemon = false;

    //Array of starter pokemon
    Pokemon* listOfStarterPokemon[3] = {new FireType("Embi", 1, 5, 120),
                                        new WaterType("Splooshy", 1, 5, 1.5),
                                        new GrassType("Lakileaf", 1, 5, 0.5)};

    //To tell if the player is on the title screen or not
    bool onTitleScreen = true;

    //For keeping track of time with the encounter checking
    Uint32 currEncounterTime;
    Uint32 prevEncounterTime = 0;

    //Sets up random device for calculating random floats
    random_device random;

    //Output variable that goes in the random range to calculate random floats
    mt19937 outputNum(random());

    //Starting pokeballs the player has
    for (int i = 0; i < 5; i++) {
        player->addToPlayersPokeballs(new Pokeball("Pokeball", 1, IMG_Load("images/items/pokeball.png"),
                                                   "A normal Pokeball to catch pokemon"));
    }
    for (int i = 0; i < 2; i++) {
        player->addToPlayersPokeballs(new Pokeball("Great Ball", 1, IMG_Load("images/items/greatBall.png"),
                                                   "A better Pokeball with a higher catching chance"));
    }

    //To tell if the player is in a battle
    bool inBattle = false;

    //Sets mixer volume to max
    Mix_MasterVolume(MIX_MAX_VOLUME);

    //Loads music and sounds
    titleMusic = Mix_LoadMUS("music/titleTheme.wav");
    worldMusic = Mix_LoadMUS("music/worldTheme.wav");
    battleMusic = Mix_LoadMUS("music/battleMusic.wav");
    battleIntro = Mix_LoadWAV("sounds/introBattleSound.wav");
    buttonSound = Mix_LoadWAV("sounds/buttonSound.wav");
    pokecenterSound = Mix_LoadWAV("sounds/pokecenterSound.wav");
    critHit = Mix_LoadWAV("sounds/critHit.wav");
    weakHit = Mix_LoadWAV("sounds/enemyHit.wav");
    regularHit = Mix_LoadWAV("sounds/playerHit.wav");
    healSound = Mix_LoadWAV("sounds/healSound.wav");

    //Plays title music
    Mix_PlayMusic(titleMusic, -1);

    //-------------------------------------\\
    //------------- Game Loop -------------\\
    //-------------------------------------\\

    while (!quit) {

        //Gets console ticks for delta time calculations
        currTime = SDL_GetTicks();

        //Set to 60 FPS
        if (currTime > prevTime + 1000 / 60) {
            prevTime = currTime;

            //Clears surface that was blitted to
            SDL_FillRect(windowSurf, nullptr, 0x000000);

            //Gets events
            SDL_Event event;

            //Gets player input events
            SDL_PollEvent(&event);

            //Switch loop for events
            switch (event.type) {

                // User requests quit
                case SDL_QUIT:
                    quit = true;
                    break;

                //Presses mouse button
                case SDL_MOUSEBUTTONDOWN:
                    mouseDown = true;
                    break;

                //User lets go of the mouse button
                case SDL_MOUSEBUTTONUP:
                    mouseDown = false;
                    break;

                //User changes the screen size
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED){
                        //Handles the user changing the window size
                        screenSizeChange(window);
                    }
                    break;

                //User lets go of a key
                case SDL_KEYUP:

                    //Sets player speed back to walk speed (This is done here instead of an else statement after the
                    // sprinting check to not call this every frame that the user is not pressing the sprint button)
                    player->setWalking();
            }

            //The title screen and starter pokemon choosing screen
            if (onTitleScreen || !hasChoseStarterPokemon){
                SDL_Rect buttonPos;
                placeHolderRect = {0, 0, 0, 0};

                //Title screen
                if (onTitleScreen) {

                    //Displays title screen background and makes start button rect
                    SDL_BlitSurface(titleScreenImage, nullptr, windowSurf, &placeHolderRect);
                    buttonPos = {SCREEN_WIDTH / 2 - 150, 600, 300, 100};

                    //Start button
                    ButtonState startButton = checkForClickAndDisplayButton(buttonPos, windowSurf, startButtonIMG,
                                                                            startButtonHoverIMG);
                    if (startButton == PRESSED && ! curtainIsDropping) {

                        //Checks if the player has already chosen a starter pokemon (For if the player decides to go
                        // back to the main menu, but I didn't get to implementing that yet)
                        if (hasChoseStarterPokemon){
                            curtainIsDropping = true;
                        } else {
                            onTitleScreen = false;
                        }
                    }

                //Choosing starter pokemon screen
                } else if (!hasChoseStarterPokemon){

                    //Displays choosing starter pokemon background
                    SDL_BlitSurface(chooseScreen, nullptr, windowSurf, &placeHolderRect);

                    //Goes through each starter pokemon making a button and displaying the pokemon and info
                    for (int i = 0; i < 3; i++) {
                        buttonPos = {200 + i * 400, 400, 250, 250};
                        ButtonState starterPokemonButton = checkForClickAndDisplayButton(buttonPos, windowSurf, buttonIMG,
                                                                                buttonHoverIMG);
                        SDL_BlitSurface(listOfStarterPokemon[i]->getPokeImage(), nullptr, windowSurf, &buttonPos);

                        string text = listOfStarterPokemon[i]->getName();
                        SDL_Rect textPos = {int(buttonPos.x + buttonPos.w / 2 - text.size() / 2.0 * MEDIUM_FONT_SIZE / FONT_PIXEL_HEIGHT_TO_WIDTH), buttonPos.y + buttonPos.h, 0, 0};
                        textSurf = TTF_RenderText_Solid(mediumFont, text.c_str(), {0, 0, 0});
                        SDL_BlitSurface(textSurf, nullptr, windowSurf, &textPos);
                        SDL_FreeSurface(textSurf);

                        text = typeid(*listOfStarterPokemon[i]).name();

                        if (text.find("FireType") != string::npos){
                            text = "FireType";
                        } else if (text.find("GrassType") != string::npos){
                            text = "GrassType";
                        } else if (text.find("WaterType") != string::npos){
                            text = "WaterType";
                        } else if (text.find("IceType") != string::npos){
                            text = "IceType";
                        }

                        textPos = {int(buttonPos.x + buttonPos.w / 2 - text.size() / 2.0 * MEDIUM_FONT_SIZE / FONT_PIXEL_HEIGHT_TO_WIDTH), buttonPos.y + buttonPos.h + MEDIUM_FONT_SIZE, 0, 0};
                        textSurf = TTF_RenderText_Solid(mediumFont, text.c_str(), {0, 0, 0});
                        SDL_BlitSurface(textSurf, nullptr, windowSurf, &textPos);
                        SDL_FreeSurface(textSurf);

                        if (starterPokemonButton == PRESSED && !curtainIsDropping){

                            //Adds the starter pokemon chosen to the players pokemon and then starts transition to main game
                            player->addToPlayersPokemon(listOfStarterPokemon[i]);
                            curtainIsDropping = true;
                            Mix_FadeOutMusic(500);
                        }
                    }
                } else {
                    curtainIsDropping = true;
                    Mix_FadeOutMusic(500);
                }

                //Drops curtain
                if (curtainIsDropping && curtainPos.y != 0) {
                    curtainPos.y /= 1.2;
                    placeHolderRect = curtainPos;

                    //Displays curtain
                    SDL_BlitSurface(curtain, nullptr, windowSurf, &placeHolderRect);

                    //If curtain has dropped
                    if (curtainPos.y > -1) {
                        curtainPos.y = -2;
                        curtainHasDropped = true;
                        curtainIsDropping = false;
                        onTitleScreen = false;
                        hasChoseStarterPokemon = true;
                        if (listOfStarterPokemon[0] != nullptr){
                            for (Pokemon* &pokemon : listOfStarterPokemon){
                                if (pokemon != player->getCurrPokemon()) {
                                    delete pokemon;
                                }
                                pokemon = nullptr;
                            }
                        }
                        Mix_FadeInMusic(worldMusic, -1, 500);
                    }
                }
            }
            else { // Main Game
                if (!battleHasBegun || (battleIsOver && curtainHasDropped)) { // Determines if the player is not in a battle

                    //For disabling movement while a message is being displayed and while transitioning to and from a battle
                    if (!inBattle && messageList.empty()) {

                        //Handles movement inputs
                        handlePlayerMovement(player, bgPos, SCREEN_WIDTH, SCREEN_HEIGHT, keystates);

                        //Test if the player is not on the stone pathway (So they won't run into a wild pokemon otherwise)
                        if ((bgPos.x > topLeftQuadrant[0] && bgPos.y > topLeftQuadrant[1]) ||
                            (bgPos.x < topRightQuadrant[0] && bgPos.y > topRightQuadrant[1]) ||
                            (bgPos.x > bottomLeftQuadrant[0] && bgPos.y < bottomLeftQuadrant[1]) ||
                            (bgPos.x < bottomRightQuadrant[0] && bgPos.y < bottomRightQuadrant[1])) {

                            //Calculating time for encounters
                            currEncounterTime = SDL_GetTicks();
                            if (currEncounterTime > prevEncounterTime + encounterCheckTime * 1000) {
                                prevEncounterTime = currEncounterTime;

                                //Test for if the player encounters a pokemon
                                if (randomChanceRange(outputNum) <= encounterChance) {

                                    //Deletes wild pokemon in case it already points to another wild pokemon
                                    delete wildPokemon;

                                    //Creates a randomly generated pokemon based on what quadrant of the map the player is in
                                    wildPokemon = getRandomPokemon();

                                    //Starts battle
                                    inBattle = true;
                                    battleIsOver = false;
                                    curtainIsDropping = true;
                                    Mix_FadeOutMusic(0);
                                    Mix_PlayChannel(1, battleIntro, 0);
                                    Mix_FadeInMusic(battleMusic, -1, 2500);
                                }
                            }
                        }
                    }

                    //Displays world map, player, and pokecenter (Its here so that the transition/animation between
                    // battles and the title screen display the world while disabling player movement)
                    if (!curtainHasDropped || battleIsOver) {

                        placeHolderRect = bgPos;
                        SDL_BlitSurface(background, nullptr, windowSurf, &placeHolderRect);

                        player->displayPlayer(windowSurf);

                        //Test for if the pokecenter is off-screen and does not need to be displayed
                        if ((bgPos.x > -bgPos.w / 2 - pokecenterPos.w / 2 && bgPos.x < -bgPos.w / 2 + SCREEN_WIDTH +
                                                                                       pokecenterPos.w / 2) &&
                            (bgPos.y > -bgPos.h / 2 - pokecenterPos.h / 2 && bgPos.y < -bgPos.h
                                                                                       / 2 + SCREEN_HEIGHT +
                                                                                       pokecenterPos.h / 2)) {

                            //Sets pokecenter location based on background location
                            pokecenterPos.x = bgPos.x + background->w / 2 - pokecenterPos.w / 2;
                            pokecenterPos.y = bgPos.y + background->h / 2 - pokecenterPos.h / 2;

                            placeHolderRect = pokecenterPos;

                            SDL_BlitSurface(pokecenter, nullptr, windowSurf, &placeHolderRect);

                            //Test for if the player is close enough to interact with the pokecenter
                            if ((bgPos.x > -bgPos.w / 2 + SCREEN_WIDTH / 2 -
                                           pokecenterPos.w && bgPos.x < -bgPos.w / 2 + SCREEN_WIDTH / 2 +
                                                                        pokecenterPos.w) &&
                                (bgPos.y < -bgPos.h / 2 + SCREEN_HEIGHT / 2
                                           + pokecenterPos.h && bgPos.y > -bgPos.h / 2 + SCREEN_HEIGHT / 2 -
                                                                          pokecenterPos.h / 2)) {

                                //Test for interacting with the pokecenter
                                if ((keystates[SDL_SCANCODE_SPACE] && !noSkip) || !messageList.empty() ||
                                    (player->noOtherHealthyPokemon() && player->getCurrPokemon()->getHealth() == 0 &&
                                     !curtainHasDropped)) {
                                    if (messageList.empty()) {
                                        for (Pokemon *pokemon: player->getAllPokemon()) {
                                            pokemon->restore();
                                        }

                                        //noSkip is here so that the player doesn't interact with the pokecenter the next frame after leaving
                                        noSkip = true;
                                        Mix_FadeOutMusic(500);
                                        Mix_PlayChannel(1, pokecenterSound, 0);
                                        messageList.emplace_back("You healed up all your Pokemon!");
                                    } else {

                                        //Displays textbox and messages
                                        SDL_BlitSurface(textboxIMG, nullptr, windowSurf, &textboxPos);
                                        showMessages(messageList, windowSurf);

                                        //Test for if the message has been read and then fades back in music
                                        if (messageList.empty()){
                                            Mix_FadeOutMusic(0);
                                            Mix_FadeInMusicPos(worldMusic, -1, 1000, Mix_GetMusicPosition(worldMusic));
                                        }
                                    }
                                }
                                else {

                                    //To reset noSkip so the player can interact with the pokecenter again
                                    if (!keystates[SDL_SCANCODE_SPACE]) {
                                        noSkip = false;
                                    }

                                    //Displays space button notification while not using the pokecenter and while close enough
                                    placeHolderRect = {pokecenterPos.x + pokecenterPos.w / 2 - spaceButton->w / 2,
                                                       pokecenterPos.y - spaceButton->h, 0, 0};
                                    SDL_BlitSurface(spaceButton, nullptr, windowSurf, &placeHolderRect);
                                }
                            }
                        }
                    }
                }

                //Check for if the player is in a battle (Not an if else statement because transition would not work
                // and duplicate code would be needed)
                if (inBattle) {

                    //Displays battle background, battling pokemon and textbox
                    if (battleHasBegun || curtainHasDropped) {
                        placeHolderRect = {0, 0, 0, 0};
                        SDL_BlitSurface(currBattleBackground, nullptr, windowSurf, &placeHolderRect);
                        SDL_BlitSurface(textboxIMG, nullptr, windowSurf, &textboxPos);
                        player->getCurrPokemon()->displayPokemonAndInfo(windowSurf, true);

                        //Check for if the player catches the wild pokemon and wildPokemon gets set to null
                        if (wildPokemon != nullptr) {
                            wildPokemon->displayPokemonAndInfo(windowSurf, false);
                        }
                    }
                    //So that nothing can happen while a message is getting displayed (I know, this is a terribly
                    // massive wall of if statements, but I don't know too much about making turn based combat)
                    if (messageList.empty()) {
                        if (battleHasBegun && !battleIsOver) { //Test if the player is entering/exiting the battle (For animation)
                            if (playersTurn) { //Test for if it is the players turn
                                if (chosenAction != NOT_CHOSEN) { //Test if the player hasn't chosen a move yet
                                    if (chosenAction == ATTACKING) { // Test for if player is attacking
                                        if (!hasAttacked) {

                                            //Was originally going to make attacking animation here but didn't get to it
                                            hasAttacked = player->getCurrPokemon()->attack(wildPokemon);

                                            //Test for if animation(unimplemented) is done and player has attacked
                                            if (hasAttacked) {
                                                chosenAction = NOT_CHOSEN;
                                                hasAttacked = false;
                                                playersTurn = false;

                                                //Test for if wild pokemon faints from attack
                                                if (wildPokemon->getHealth() <= 0) {
                                                    battleIsOver = true;
                                                    playersTurn = true;
                                                    messageList.push_back(wildPokemon->getName() + " fainted!");

                                                    calculatingResults = true;
                                                }

                                                //Check for if player faints (from self inflicted damage)
                                                if (player->getCurrPokemon()->getHealth() <= 0) {
                                                    messageList.push_back(
                                                            player->getCurrPokemon()->getName() + " fainted!");
                                                    if (player->noOtherHealthyPokemon()) {
                                                        battleIsOver = true;
                                                        calculatingResults = true;
                                                    } else {
                                                        player->resetBattleMenu();
                                                        playersTurn = true;
                                                    }
                                                }
                                            }
                                        }
                                    } else if (chosenAction == USE_ITEM) { //Test for if player uses an item

                                        //Uses the currently selected item
                                        player->useItem();
                                        chosenAction = NOT_CHOSEN;
                                        playersTurn = false;
                                    } else if (chosenAction == CATCH) { //Test for if the player tries catching the wild pokemon
                                        catchState = player->tryCatchingPokemon(wildPokemon);

                                        //Check for if the catching animation finished (Didn't get to implementing)
                                        if (catchState != ANIMATION_NOT_FINISHED) {
                                            chosenAction = NOT_CHOSEN;

                                            //Check for if the player caught the pokemon or not
                                            if (catchState == CAUGHT) {
                                                messageList.push_back("You caught the " + wildPokemon->getName() + "!");
                                                wildPokemon = nullptr;
                                                battleIsOver = true;
                                            } else if (catchState == NOT_CAUGHT) {
                                                messageList.push_back("The " + wildPokemon->getName() + " escaped!");
                                                playersTurn = false;
                                            }
                                        }
                                    } else if (chosenAction == SWITCH_POKEMON) { //Test for if the player switches pokemon

                                        //Checks if the player was forced to switch pokemon (If the current pokemon has fainted)
                                        if (player->getCurrPokemon()->getHealth() == 0) {
                                            hadToForceSwitchPokemon = true;
                                        }

                                        //Check for if the pokemon have switched (For animating purposes, but didn't get
                                        // to implementing it)
                                        if (!player->switchingPokemon()) {
                                            chosenAction = NOT_CHOSEN;

                                            //Lets the player have another turn if they were forced to switch pokemon
                                            if (!hadToForceSwitchPokemon) {
                                                playersTurn = false;
                                            } else {
                                                hadToForceSwitchPokemon = false;
                                            }
                                        }
                                    } else if (chosenAction == RUN) { //Test for if the player chooses to run from the battle

                                        //Calculates running success
                                        if (randomChanceRange(outputNum) < runSuccessRate) {
                                            messageList.push_back("You got away.");
                                            battleIsOver = true;
                                        } else {
                                            messageList.push_back("You didn't away in time...");
                                        }
                                        chosenAction = NOT_CHOSEN;
                                        playersTurn = false;
                                        player->resetBattleMenu();
                                    }
                                } else {

                                    //Displays player battle menu when not chosen an action
                                    chosenAction = player->displayBattleMenu(windowSurf);
                                }
                            } else { //Enemy's turn
                                if (chosenAction != NOT_CHOSEN) { //If enemy hasn't chosen a move yet
                                    if (!hasAttacked) {

                                        //Same deal with the player attacking
                                        hasAttacked = wildPokemon->attack(player->getCurrPokemon());
                                        if (hasAttacked) {
                                            chosenAction = NOT_CHOSEN;
                                            hasAttacked = false;
                                            playersTurn = true;

                                            if (player->getCurrPokemon()->getHealth() <= 0) {
                                                messageList.push_back(
                                                        player->getCurrPokemon()->getName() + " fainted!");
                                                if (player->noOtherHealthyPokemon()) {
                                                    battleIsOver = true;
                                                    calculatingResults = true;
                                                } else {
                                                    player->resetBattleMenu();
                                                }
                                            }

                                            if (wildPokemon->getHealth() <= 0) {
                                                battleIsOver = true;
                                                playersTurn = true;
                                                calculatingResults = true;
                                                messageList.push_back(wildPokemon->getName() + " fainted!");
                                            }
                                        }
                                    }
                                } else {

                                    //Pokemon picks a rando move (Can only attack as of now)
                                    wildPokemon->pickRandomMove();
                                    chosenAction = ATTACKING;
                                }
                            }
                        } else if (!battleHasBegun) { //Test for if player is entering the battle

                            //Drops curtain to transition to battle
                            if (!curtainHasDropped && curtainPos.y != 0) {
                                curtainPos.y /= 1.2;
                                placeHolderRect = curtainPos;
                                SDL_BlitSurface(curtain, nullptr, windowSurf, &placeHolderRect);

                                //Tests if the curtain has dropped to set up the battle
                                if (curtainPos.y > -1) {
                                    curtainPos.y = -2;
                                    for (Pokemon *pokemon: player->getAllPokemon()) {
                                        pokemon->setImageFacingRight(true);
                                    }
                                    wildPokemon->setImageFacingRight(false);
                                    player->getCurrPokemon()->setImagePos(100, 360);
                                    player->getCurrPokemon()->setInfoPos(500, 490);
                                    wildPokemon->setImagePos(950, 100);
                                    wildPokemon->setInfoPos(100, 100);
                                    curtainHasDropped = true;
                                    messageList.emplace_back("You encountered a wild pokemon!");
                                    chosenAction = NOT_CHOSEN;
                                    player->resetBattleMenu();
                                    battleHasBegun = true;
                                    battleIsOver = false;
                                    curtainIsDropping = false;
                                }
                            }
                        } else { //Test for if the battle has ended
                            if (calculatingResults) { //Test for id calculating results (So that exit animation/transition doesnt play immediately

                                //Test for if the player has won (By testing if the current pokemon hasn't fainted or if
                                // the player still has more pokemon if the current pokemon both faints and defeats the
                                // wild pokemon)
                                if (player->getCurrPokemon()->getHealth() != 0 || !player->noOtherHealthyPokemon()) {
                                    player->calculateTeamExperience(wildPokemon);
                                    messageList.push_back("You won!");

                                    //Checks if player gets an item
                                    if (randomChanceRange(outputNum) < randomItemChance) {

                                        //Does the same check again to see if the player gets a better item
                                        if (randomChanceRange(outputNum) < randomItemChance) {
                                            messageList.push_back("You got a Super Herb!");
                                            player->addToPlayersItems(new HealthItem("Super Herb", 60, IMG_Load(
                                                                                             "images/items/superHerb.png"),
                                                                                     "Heals 60 health points."));
                                        } else {
                                            messageList.push_back("You got a Herb!");
                                            player->addToPlayersItems(
                                                    new HealthItem("Herb", 20, IMG_Load("images/items/herb.png"),
                                                                   "Heals 20 health points."));
                                        }
                                    }

                                    //Check for if the player gets a pokeball
                                    if (randomChanceRange(outputNum) < randomItemChance) {
                                        float chance = randomChanceRange(outputNum);

                                        //Check for if the player gets a better pokeball (Lower probobility for better pokeball)
                                        if (chance < randomItemChance / 2) {
                                            messageList.push_back("You got a Master Ball!");
                                            player->addToPlayersPokeballs(new Pokeball("Master Ball", 3, IMG_Load(
                                                                                               "images/items/masterBall.png"),
                                                                                       "A super Pokeball with a much higher catching chance"));
                                        } else if (chance < randomItemChance) {
                                            messageList.push_back("You got a Great Ball!");
                                            player->addToPlayersPokeballs(new Pokeball("Great Ball", 2, IMG_Load(
                                                                                               "images/items/greatBall.png"),
                                                                                       "A better Pokeball with a higher catching chance"));
                                        } else {
                                            messageList.push_back("You got a Pokeball!");
                                            player->addToPlayersPokeballs(
                                                    new Pokeball("Pokeball", 1, IMG_Load("images/items/pokeball.png"),
                                                                 "A normal Pokeball to catch pokemon"));
                                        }
                                    }
                                } else { //Check for if the player lost the battle
                                    messageList.push_back("You lost...");
                                    messageList.push_back("You quickly ran to the Pokecenter, carrying your Pokemon.");
                                    player->setPlayerPos(SCREEN_WIDTH / 2 - player->getPlayerRect().w / 2,
                                                         SCREEN_HEIGHT / 2 - player->getPlayerRect().h / 2);
                                    bgPos.x = -bgPos.w / 2 + SCREEN_WIDTH / 2;
                                    bgPos.y = -bgPos.h / 2 + SCREEN_HEIGHT / 2;
                                }
                                calculatingResults = false;
                            } else { //Check for if finished calculating results

                                //Fades out battle music
                                if (Mix_PlayingMusic()){
                                    Mix_FadeOutMusic(500);
                                }

                                //Drops curtain to tansition out of the battle
                                if (!curtainHasDropped && curtainPos.y != 0) {
                                    curtainPos.y /= 1.2;
                                    placeHolderRect = curtainPos;
                                    SDL_BlitSurface(curtain, nullptr, windowSurf, &placeHolderRect);

                                    //Check for if the curtain has been dropped
                                    if (curtainPos.y > -1) {
                                        curtainPos.y = -2;
                                        curtainHasDropped = true;
                                        inBattle = false;
                                        battleHasBegun = false;
                                        prevEncounterTime = SDL_GetTicks();
                                        Mix_FadeInMusic(worldMusic, -1, 500);
                                    }
                                }
                            }
                        }
                    } else {
                        //Displays battle messages
                        showMessages(messageList, windowSurf);
                    }
                }
            }

            //Check for raising the curtain (Done here so that there is less duplicate code)
            if (curtainHasDropped && curtainPos.y != -SCREEN_HEIGHT) {
                curtainPos.y /= 0.65;
                placeHolderRect = curtainPos;
                SDL_BlitSurface(curtain, nullptr, windowSurf, &placeHolderRect);

                //Check for if the curtain has risen
                if (curtainPos.y < -SCREEN_HEIGHT) {
                    curtainPos.y = -SCREEN_HEIGHT;
                    curtainHasDropped = false;
                }
            }

            //Updates windowTexture, so it can be rendered with new blit info
            SDL_UpdateTexture(windowTexture, nullptr, windowSurf->pixels, windowSurf->pitch);
        }

        // Initialize renderer color white for the background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

        // Clear screen
        SDL_RenderClear(renderer);

        //Prepares windowTexture to be rendered at the location of windowTextureSize and also resizes/soft scales it to the
        // dimensions of windowTextureSize
        SDL_RenderCopy(renderer, windowTexture, nullptr, &windowTextureSize);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    //Calculate and output results to results file
    calculateResults();

    //Frees surfaces
    SDL_FreeSurface(windowSurf);

    //Destroys textures
    SDL_DestroyTexture(windowTexture);

    //Deletes player
    delete player;
    player = nullptr;

    //Deletes wildPokemon
    delete wildPokemon;
    wildPokemon = nullptr;

    //Frees SDL surfaces and music/sound
    freeingSDLStuff();

    //Close the font that was used
    TTF_CloseFont(smallFont);
    TTF_CloseFont(mediumFont);
    TTF_CloseFont(largeFont);

    //Quit SDL_ttf
    TTF_Quit();

    // Destroy renderer
    SDL_DestroyRenderer(renderer);

    //Closes audio
    Mix_CloseAudio();

    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL
    SDL_Quit();

    return 0;
}