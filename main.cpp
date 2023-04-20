#define SDL_MAIN_HANDLED true
#define _USE_MATH_DEFINES
#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>

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

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

using namespace std;

#define SCREEN_WIDTH    1440
#define SCREEN_HEIGHT   810

int main(int argc, char* argv[]) {

    //--------------------------------------\\
    //------------- SDL Set Up -------------\\
    //--------------------------------------\\

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
    int audio_buffers = 4096;

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

    //TODO: Need to
    //      Write new moves in the move txt file and more names in the names file ( Check chain reaction and ulra heal des and names) (Also check ultra heal implimentation)
    //      Refactoring
    //      Comment everything and review (balancing and boolean logic for battle animations (like the booleans logic to blit the player when entering a battle))
    //      Going over grade requirements (like deleting dynamics)
    //
    //TODO: Maybe
    //      Add music and sound effects.
    //      Add sp so that big moves cannot be used every turn maybe.
    //      Make death animation (make pokemon go offscreen)
    //      Make Attack animation
    //      Make healing animations


    //Initialize fonts
    smallFont = TTF_OpenFont("fonts/font.ttf", SMALL_FONT_SIZE);
    mediumFont = TTF_OpenFont("fonts/font.ttf", MEDIUM_FONT_SIZE);
    largeFont = TTF_OpenFont("fonts/font.ttf", LARGE_FONT_SIZE);

    //Check for if font was correctly opened
    if (!smallFont){
        cout << "Small font could not be opened\n";
    }
    if (!mediumFont){
        cout << "Medium font could not be opened\n";
    }
    if (!largeFont){
        cout << "Large font could not be opened\n";
    }

    fstream fin("moveInfo.txt");
    if (fin.is_open()) {
        char ch;
        string fullInfo;
        while (!fin.eof()) {
            fin >> noskipws >> ch;
            fullInfo += ch;
            if (ch == '\n') {
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

    fin.open("nameList.txt");
    if (fin.is_open()) {
        char ch;
        string fullInfo;
        while (!fin.eof()) {
            fin >> noskipws >> ch;
            fullInfo += ch;
            if (ch == '\n') {
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

    bgPos = {-background->w / 2 + SCREEN_WIDTH / 2, -background->h / 2 + SCREEN_HEIGHT / 2, background->w, background->h};

    SDL_Surface* curtain = IMG_Load("images/curtain.png");
    SDL_Rect curtainPos = {0, -SCREEN_HEIGHT, curtain->w, curtain->h};

    bool curtainHasDropped = false;

    SDL_Surface* pokecenter = IMG_Load("images/p.png");
    SDL_Rect pokecenterPos = {bgPos.w / 2 - 50, bgPos.h / 2 - 50, 100, 100};

    SDL_Surface* spaceButton = IMG_Load("images/spaceButton.png");

    SDL_Surface* textboxIMG = IMG_Load("images/textbox.png");
    SDL_Rect textboxPos = {0, SCREEN_HEIGHT - textboxIMG->h, 0, 0};

    player = new Player({SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2, 0, 0});

    //So that the player doesn't spawn directly under the pokecenter
    bgPos.y += player->getPlayerRect().h * 2;

    //This exists because SDL's blitting function changes the destination rect's position when blitting, if the
    // position is offscreen
    SDL_Rect placeHolderRect = {0, 0, 0, 0};

    //Text color
    SDL_Color textColor = {255, 255, 255};

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

    //Creates Rect for windowSurf surface so that its dimensions can be rescaled and repositioned
    SDL_Rect windowTextureSize = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    player->addToPlayersItems(new HealthItem("Herb", 20, IMG_Load("images/items/herb.png"), "Heals 20 health points."));
    player->addToPlayersItems(new HealthItem("Herb", 20, IMG_Load("images/items/herb.png"), "Heals 20 health points."));
    player->addToPlayersItems(new HealthItem("Herb", 20, IMG_Load("images/items/herb.png"), "Heals 20 health points."));

    float encounterChance = 1;

    float runSuccessRate = 0.5;

    float randomItemChance = 0.3;

    int encounterCheckTime = 1;

    //Pointer for a wild pokemon
    Pokemon* wildPokemon = nullptr;

    int chosenAction = 0;

    bool hasAttacked = false;

    CatchingState catchState = ANIMATION_NOT_FINISHED;

    bool playersTurn = true;

    bool hadToForceSwitchPokemon;

    bool battleHasBegun = false;

    bool battleIsOver = false;

    bool calculatingResults = false;

    bool hasChoseStarterPokemon = false;

    Pokemon* listOfStarterPokemon[3] = {new FireType("Embi", 1, 5, 120),
                                        new WaterType("Splooshy", 1, 5, 1.5),
                                        new GrassType("Lakileaf", 1, 5, 0.5)};

    bool onTitleScreen = true;

    SDL_Surface* titleScreenImage = IMG_Load("images/titleScreenImage.png");
    SDL_Surface* startButtonIMG = IMG_Load("images/startButton.png");
    SDL_Surface* startButtonHoverIMG = IMG_Load("images/startButtonHover.png");

    SDL_Surface* chooseScreen = IMG_Load("images/chooseScreen.png");

    Uint32 currEncounterTime;
    Uint32 prevEncounterTime = 0;

    random_device random;

    mt19937 outputNum(random());

    Pokeball* p = new Pokeball("Master Ball", 3, IMG_Load("images/items/masterBall.png"),
    "A super Pokeball with a much higher catching chance");
    Pokeball* pe = new Pokeball("Great Ball", 1, IMG_Load("images/items/greatBall.png"),
                                "A better Pokeball with a higher catching chance");
    player->addToPlayersPokeballs(p);
    player->addToPlayersPokeballs(pe);

    bool inBattle = false;

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
                        screenSizeChange(windowTextureSize, window);
                    }
                    break;

                //User lets go of a key
                case SDL_KEYUP:

                    //Sets player speed back to walk speed (This is done here instead of an else statement after the
                    // sprinting check to not call this every frame that the user is not pressing the sprint button)
                    player->setWalking();
            }

            if (onTitleScreen || !hasChoseStarterPokemon){
                SDL_Rect buttonPos;
                placeHolderRect = {0, 0, 0, 0};
                if (onTitleScreen) {
                    SDL_BlitSurface(titleScreenImage, nullptr, windowSurf, &placeHolderRect);
                    buttonPos = {SCREEN_WIDTH / 2 - 150, 600, 300, 100};
                    ButtonState startButton = checkForClickAndDisplayButton(buttonPos, windowSurf, startButtonIMG,
                                                                            startButtonHoverIMG);
                    if (startButton == PRESSED) {
                        onTitleScreen = false;
                    }
                } else if (!hasChoseStarterPokemon){
                    SDL_BlitSurface(chooseScreen, nullptr, windowSurf, &placeHolderRect);
                    for (int i = 0; i < 3; i++) {
                        buttonPos = {200 + i * 400, 400, 250, 250};
                        ButtonState starterPokemonButton = checkForClickAndDisplayButton(buttonPos, windowSurf, buttonIMG,
                                                                                buttonHoverIMG);
                        SDL_BlitSurface(listOfStarterPokemon[i]->getPokeImage(), nullptr, windowSurf, &buttonPos);

                        string text = listOfStarterPokemon[i]->getName();
                        SDL_Rect textPos = {int(buttonPos.x + buttonPos.w / 2 - text.size() / 2.0 * MEDIUM_FONT_SIZE / FONT_PIXEL_HEIGHT_TO_WIDTH), buttonPos.y + buttonPos.h, 0, 0};
                        SDL_Surface* textSurf = TTF_RenderText_Solid(mediumFont, text.c_str(), {0, 0, 0});
                        SDL_BlitSurface(textSurf, nullptr, windowSurf, &textPos);

                        text = typeid(*listOfStarterPokemon[i]).name();
                        text = text.substr(1, text.size());
                        textPos = {int(buttonPos.x + buttonPos.w / 2 - text.size() / 2.0 * MEDIUM_FONT_SIZE / FONT_PIXEL_HEIGHT_TO_WIDTH), buttonPos.y + buttonPos.h + MEDIUM_FONT_SIZE, 0, 0};
                        textSurf = TTF_RenderText_Solid(mediumFont, text.c_str(), {0, 0, 0});
                        SDL_BlitSurface(textSurf, nullptr, windowSurf, &textPos);

                        if (starterPokemonButton == PRESSED){
                            player->addToPlayersPokemon(listOfStarterPokemon[i]);
                            for (Pokemon* &pokemon : listOfStarterPokemon){
                                if (pokemon != listOfStarterPokemon[i]) {
                                    delete pokemon;
                                }
                                pokemon = nullptr;
                            }
                            hasChoseStarterPokemon = true;
                            break;
                        }
                    }
                }
            } else {
                if (!battleHasBegun || (battleIsOver && curtainHasDropped)) {

                    if (!inBattle && messageList.empty()) {

                        //Handles movement inputs
                        handlePlayerMovement(player, bgPos, SCREEN_WIDTH, SCREEN_HEIGHT, keystates);

                        if ((bgPos.x > topLeftQuadrant[0] && bgPos.y > topLeftQuadrant[1]) ||
                            (bgPos.x < topRightQuadrant[0] && bgPos.y > topRightQuadrant[1]) ||
                            (bgPos.x > bottomLeftQuadrant[0] && bgPos.y < bottomLeftQuadrant[1]) ||
                            (bgPos.x < bottomRightQuadrant[0] && bgPos.y < bottomRightQuadrant[1])) {

                            currEncounterTime = SDL_GetTicks();
                            if (currEncounterTime > prevEncounterTime + encounterCheckTime * 1000) {
                                prevEncounterTime = currEncounterTime;
                                if (randomChanceRange(outputNum) <= encounterChance) {

                                    delete wildPokemon;
                                    wildPokemon = nullptr;

                                    wildPokemon = getRandomPokemon();

                                    inBattle = true;
                                    battleIsOver = false;
                                }
                            }
                        }
                    }
                    if (!curtainHasDropped || battleIsOver) {

                        placeHolderRect = bgPos;
                        SDL_BlitSurface(background, nullptr, windowSurf, &placeHolderRect);

                        player->displayPlayer(windowSurf);

                        if ((bgPos.x > -bgPos.w / 2 - pokecenterPos.w / 2 && bgPos.x < -bgPos.w / 2 + SCREEN_WIDTH +
                                                                                       pokecenterPos.w / 2) &&
                            (bgPos.y > -bgPos.h / 2 - pokecenterPos.h / 2 && bgPos.y < -bgPos.h
                                                                                       / 2 + SCREEN_HEIGHT +
                                                                                       pokecenterPos.h / 2)) {

                            pokecenterPos.x = bgPos.x + background->w / 2 - pokecenterPos.w / 2;
                            pokecenterPos.y = bgPos.y + background->h / 2 - pokecenterPos.h / 2;

                            placeHolderRect = pokecenterPos;

                            SDL_BlitSurface(pokecenter, nullptr, windowSurf, &placeHolderRect);
                            if ((bgPos.x > -bgPos.w / 2 + SCREEN_WIDTH / 2 -
                                           pokecenterPos.w && bgPos.x < -bgPos.w / 2 + SCREEN_WIDTH / 2 +
                                                                        pokecenterPos.w) &&
                                (bgPos.y < -bgPos.h / 2 + SCREEN_HEIGHT / 2
                                           + pokecenterPos.h && bgPos.y > -bgPos.h / 2 + SCREEN_HEIGHT / 2 -
                                                                          pokecenterPos.h)) {
                                if (keystates[SDL_SCANCODE_SPACE] || !messageList.empty() ||
                                    (player->noOtherHealthyPokemon() && player->getCurrPokemon()->getHealth() == 0 &&
                                     !curtainHasDropped)) {
                                    if (messageList.empty()) {
                                        for (Pokemon *pokemon: player->getAllPokemon()) {
                                            pokemon->restore();
                                        }
                                        messageList.emplace_back("You healed up all your Pokemon!");
                                    } else {
                                        SDL_BlitSurface(textboxIMG, nullptr, windowSurf, &textboxPos);
                                        showMessages(messageList, windowSurf);
                                    }
                                } else {
                                    placeHolderRect = {pokecenterPos.x + pokecenterPos.w / 2 - spaceButton->w / 2,
                                                       pokecenterPos.y - spaceButton->h, 0, 0};
                                    SDL_BlitSurface(spaceButton, nullptr, windowSurf, &placeHolderRect);
                                }
                            }
                        }
                    }
                }
                if (inBattle) {
                    if ((battleHasBegun || curtainHasDropped) && (!battleIsOver || !curtainHasDropped)) {
                        placeHolderRect = {0, 0, 0, 0};
                        SDL_BlitSurface(currBattleBackground, nullptr, windowSurf, &placeHolderRect);
                        SDL_BlitSurface(textboxIMG, nullptr, windowSurf, &textboxPos);
                        player->getCurrPokemon()->displayPokemonAndInfo(windowSurf, true);

                        //Check for if the player catches the wild pokemon and wildPokemon gets set to null
                        if (wildPokemon != nullptr) {
                            wildPokemon->displayPokemonAndInfo(windowSurf, false);
                        }
                    }
                    if (messageList.empty()) {
                        if (battleHasBegun && !battleIsOver) {
                            if (playersTurn) {
                                if (chosenAction != NOT_CHOSEN) {
                                    if (chosenAction == ATTACKING) {
                                        if (!hasAttacked) {
                                            hasAttacked = player->getCurrPokemon()->attack(wildPokemon);
                                            if (hasAttacked) {
                                                chosenAction = NOT_CHOSEN;
                                                hasAttacked = false;
                                                playersTurn = false;

                                                ////////////////Maybe change the position of this since this is used twice whiel battle is happening. Also the folowing bit too
                                                if (wildPokemon->getHealth() <= 0) {
                                                    battleIsOver = true;
                                                    playersTurn = true;
                                                    messageList.push_back(wildPokemon->getName() + " fainted!");

                                                    calculatingResults = true;
                                                }

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
                                    } else if (chosenAction == USE_ITEM) {
                                        //////////////////////////////////////////Make animation for using item
                                        player->useItem();
                                        chosenAction = NOT_CHOSEN;
                                        playersTurn = false;
                                    } else if (chosenAction == CATCH) {
                                        //////////////////////////////////////Make set rect pos func for pokemon to move off and on screen
                                        /////////////////////////////////////////Also make animation for catching pokemon
                                        catchState = player->tryCatchingPokemon(wildPokemon);

                                        if (catchState != ANIMATION_NOT_FINISHED) {
                                            chosenAction = NOT_CHOSEN;
                                            if (catchState == CAUGHT) {
                                                messageList.push_back("You caught the " + wildPokemon->getName() + "!");
                                                wildPokemon = nullptr;
                                                battleIsOver = true;
                                            } else if (catchState == NOT_CAUGHT) {
                                                messageList.push_back("The " + wildPokemon->getName() + " escaped!");
                                                playersTurn = false;
                                            }
                                        }
                                    } else if (chosenAction == SWITCH_POKEMON) {

                                        if (player->getCurrPokemon()->getHealth() == 0) {
                                            hadToForceSwitchPokemon = true;
                                        }

                                        if (!player->switchingPokemon()) {
                                            chosenAction = NOT_CHOSEN;
                                            if (!hadToForceSwitchPokemon) {
                                                playersTurn = false;
                                            } else {
                                                hadToForceSwitchPokemon = false;
                                            }
                                        }
                                    } else if (chosenAction == RUN) {
                                        if (randomChanceRange(outputNum) < runSuccessRate) {
                                            messageList.push_back("You got away.");
                                            battleIsOver = true;
                                        } else {
                                            messageList.push_back("You didn't away in time...");
                                        }
                                        chosenAction = NOT_CHOSEN;
                                        playersTurn = false;
                                        //////////////////////////////////////Put this somewhere else maybe if menu is reset every turn
                                        player->resetBattleMenu();
                                    }
                                } else {
                                    chosenAction = player->displayBattleMenu(windowSurf);

                                    ////////////////////////////////////////////////Reset menu or no?
//                                if (chosenAction != NOT_CHOSEN){
//                                    player->resetBattleMenu();
//                                }
                                }
                            } else {
                                if (chosenAction != NOT_CHOSEN) {
                                    if (!hasAttacked) {
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
                                    ///////////////////////////////////////////Impliment random chance to run or other
                                    wildPokemon->pickRandomMove();
                                    chosenAction = ATTACKING;
                                }
                            }
                        } else if (!battleHasBegun) {
                            if (!curtainHasDropped && curtainPos.y != 0) {
                                curtainPos.y /= 1.2;
                                placeHolderRect = curtainPos;
                                SDL_BlitSurface(curtain, nullptr, windowSurf, &placeHolderRect);

                                if (curtainPos.y > -1) {
                                    curtainPos.y = -2;
                                    for (Pokemon *pokemon: player->getAllPokemon()) {
                                        pokemon->setImageFacingRight(true);
                                    }
                                    wildPokemon->setImageFacingRight(false);
                                    player->getCurrPokemon()->setImagePos(100, 370);
                                    player->getCurrPokemon()->setInfoPos(500, 490);
                                    wildPokemon->setImagePos(950, 100);
                                    wildPokemon->setInfoPos(100, 100);
                                    curtainHasDropped = true;
                                }
                            } else if (curtainHasDropped && curtainPos.y != -SCREEN_HEIGHT) {
                                curtainPos.y /= 0.65;
                                placeHolderRect = curtainPos;
                                SDL_BlitSurface(curtain, nullptr, windowSurf, &placeHolderRect);

                                if (curtainPos.y < -SCREEN_HEIGHT) {
                                    curtainPos.y = -SCREEN_HEIGHT;
                                    curtainHasDropped = false;
                                    ///////////////////////////////////////////////////////Play enter battle animation (done-ish)
                                    messageList.emplace_back("You encountered a wild pokemon!");
                                    ///////////////////////////////Check all NOT_CHOSEN things to see if this one here is necessary
                                    chosenAction = NOT_CHOSEN;
                                    player->resetBattleMenu();
                                    battleHasBegun = true;
                                    battleIsOver = false;
                                }
                            }
                        } else {
                            if (calculatingResults) {

                                if (player->getCurrPokemon()->getHealth() != 0) {
                                    player->calculateTeamExperience(wildPokemon);
                                    messageList.push_back("You won!");

                                    if (randomChanceRange(outputNum) < randomItemChance) {
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
                                    if (randomChanceRange(outputNum) < randomItemChance) {
                                        float chance = randomChanceRange(outputNum);
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
                                } else {
                                    messageList.push_back("You lost...");
                                    messageList.push_back("You quickly ran to the Pokecenter, carrying your Pokemon.");
                                    player->setPlayerPos(SCREEN_WIDTH / 2 - player->getPlayerRect().w / 2,
                                                         SCREEN_HEIGHT / 2 - player->getPlayerRect().h / 2);
                                    bgPos.x = -bgPos.w / 2 + SCREEN_WIDTH / 2;
                                    bgPos.y = -bgPos.h / 2 + SCREEN_HEIGHT / 2;
                                }
                                calculatingResults = false;
                            } else {
                                if (!curtainHasDropped && curtainPos.y != 0) {
                                    curtainPos.y /= 1.2;
                                    placeHolderRect = curtainPos;
                                    SDL_BlitSurface(curtain, nullptr, windowSurf, &placeHolderRect);
                                    if (curtainPos.y > -1) {
                                        curtainPos.y = -2;
                                        curtainHasDropped = true;
                                    }
                                } else if (curtainHasDropped && curtainPos.y != -SCREEN_HEIGHT) {
                                    curtainPos.y /= 0.65;
                                    placeHolderRect = curtainPos;
                                    SDL_BlitSurface(curtain, nullptr, windowSurf, &placeHolderRect);
                                    if (curtainPos.y < -SCREEN_HEIGHT) {
                                        curtainPos.y = -SCREEN_HEIGHT;
                                        inBattle = false;
                                        battleHasBegun = false;
                                        curtainHasDropped = false;
                                        prevEncounterTime = SDL_GetTicks();
                                    }
                                }
                            }
                        }
                    } else {
                        showMessages(messageList, windowSurf);
                    }
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

    ////////////////////////////////////////////Also check for other surfaces that don't get freed like the item surfs and textbox surf
    //Frees surfaces
    SDL_FreeSurface(windowSurf);

    //Destroys textures
    SDL_DestroyTexture(windowTexture);

    //Deletes player
    delete player;

    delete wildPokemon;

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