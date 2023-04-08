#define SDL_MAIN_HANDLED true
#define _USE_MATH_DEFINES
#include <iostream>
#include <algorithm>
#include <random>

using namespace std;

#include "screenSizeChange.h"
#include "player.h"
#include "handlePlayerMovement.h"
#include "pokemon.h"
#include "items.h"
#include "globalVariables.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#define SCREEN_WIDTH    1440
#define SCREEN_HEIGHT   810

void showMessages(string messageToSend, SDL_Surface* windowSurf){
    SDL_Surface *textSurf = TTF_RenderText_Solid(mediumFont, messageToSend.c_str(), {255, 255, 255});
    SDL_Rect dest = {100, 650, 0, 0};
    SDL_BlitSurface(textSurf, nullptr, windowSurf, &dest);
}
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

    //TODO: Pressing:
    //      Make attacks also do things like heal
    //      Apply type bonuses like more damage for fire types
    //      Calculate and give experience to players pokemon
    //
    //TODO: Later
    //      Add pokecenter to heal all pokemon and go when all the player's pokemon faint
    //      Make Art for buttons, pokemon, and battle areas.
    //      Add music and sound effects.

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


    //Sets up key presses
    const Uint8 *keystates = SDL_GetKeyboardState(nullptr);

    SDL_Surface* background = IMG_Load("images/background.png");
    SDL_Rect bgPos = {-background->w / 2 + SCREEN_WIDTH / 2, -background->h / 2 + SCREEN_HEIGHT / 2, background->w, background->h};

    SDL_Surface* textboxIMG = IMG_Load("images/textbox.png");
    SDL_Rect textboxPos = {0, SCREEN_HEIGHT - textboxIMG->h, 0, 0};

    Player* player = new Player({SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2, 0, 0});
    Pokemon* p1 = new FireType("Poki", 1, 100, IMG_Load("images/p.png"), 100);
    Pokemon* p2 = new FireType("Poki2", 1, 100, IMG_Load("images/p.png"), 100);
    Pokemon* p3 = new FireType("Poki3", 1, 100, IMG_Load("images/p.png"), 100);
    Pokemon* p4 = new FireType("Poki4", 1, 100, IMG_Load("images/p.png"), 100);
    Pokemon* p5 = new FireType("Poki5", 1, 100, IMG_Load("images/p.png"), 100);
    player->addToPlayersPokemon(p1);
    player->addToPlayersPokemon(p2);
    player->addToPlayersPokemon(p3);
    player->addToPlayersPokemon(p4);
    player->addToPlayersPokemon(p5);

    //This exists because SDL's blitting function changes the destination rect's position when blitting, if the
    // position is offscreen
    SDL_Rect placeHolderRect = {0, 0, 0, 0};

    //Surface to display text
    SDL_Surface *textSurf;

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

    Pokemon* wildPokemon = nullptr;

    Item* collectedItem = new HealthItem("Herb", 10, IMG_Load("images/m.png"), "Heals 10 HP and i guess does stuff.");
    Item* collectedItem2 = new HealthItem("Herb2", 10, IMG_Load("images/m.png"), "Heals 10 HPP.");
    Item* collectedItem3 = new HealthItem("Herb3", 10, IMG_Load("images/m.png"), "Heals 10 HPPP.");
    player->addToPlayersItems(collectedItem);
    player->addToPlayersItems(collectedItem2);
    player->addToPlayersItems(collectedItem3);
    collectedItem = nullptr;

    float encounterChance = 1.0;

    float runSuccessRate = 0.5;

    int encounterCheckTime = 1;

    int chosenAction = 0;

    bool hasAttacked = false;

    CatchingState catchState = ANIMATION_NOT_FINISHED;

    bool playersTurn = true;

    bool battleHasBegun = false;

    bool battleIsOver = false;

    bool noSkip = true;

    vector<string> messages;

    int currMessage = 0;

    Uint32 currEncounterTime;
    Uint32 prevEncounterTime = 0;

    random_device random;

    mt19937 outputNum(random());
    uniform_real_distribution<double> randomRange(0, 1.0);

    Pokeball* p = new Pokeball("Pokeball", 1, IMG_Load("images/m.png"), "A normal Pokebal to catch pokemon");
    Pokeball* pe = new Pokeball("Pokeball 2", 1, IMG_Load("images/m.png"), "More likely to successfully catch a Pokmeon");
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
            //if (typeid(*player) == typeid(Player)){

            //    cout << typeid(*player).name() << endl;
            //}

            if (!inBattle){
                //Handles movement inputs
                handlePlayerMovement(player, bgPos, SCREEN_WIDTH, SCREEN_HEIGHT, keystates);

                placeHolderRect = bgPos;
                SDL_BlitSurface(background, nullptr, windowSurf, &placeHolderRect);

                player->displayPlayer(windowSurf);

                currEncounterTime = SDL_GetTicks();
                if (currEncounterTime > prevEncounterTime + encounterCheckTime * 1000){
                    prevEncounterTime = currEncounterTime;
                    if (randomRange(outputNum) <= encounterChance) {
                        inBattle = true;

                        //Deletes in case wild pokemon points to an existing pokemon
                        delete wildPokemon;

                        wildPokemon = new IceType("Wild Lad", 1, 100, IMG_Load("images/p.png"), 1.5);
                    }
                }
            }
            else {
                SDL_BlitSurface(textboxIMG, nullptr, windowSurf, &textboxPos);
                player->getCurrPokemon()->displayPokemonAndInfo(windowSurf);
                //Check for if the player catches the wild pokemon and wildPokemon gets set to null
                if (wildPokemon != nullptr) {
                    wildPokemon->displayPokemonAndInfo(windowSurf);
                }
                if (messages.empty()) {
                    if (battleHasBegun && !battleIsOver) {
                        if (playersTurn) {
                            if (chosenAction != NOT_CHOSEN) {
                                if (chosenAction == ATTACKING) {
                                    if (!hasAttacked) {
                                        hasAttacked = player->getCurrPokemon()->attack(wildPokemon, messages);
                                        if (hasAttacked) {
                                            chosenAction = NOT_CHOSEN;
                                            hasAttacked = false;
                                            playersTurn = false;

                                            if (wildPokemon->getHealth() <= 0) {
                                                battleIsOver = true;
                                                playersTurn = true;
                                                ///////////////////////////////////////////Calculate experience
                                                messages.emplace_back("You won!");
                                            }
                                        }
                                    }
                                }
                                else if (chosenAction == USE_ITEM){
                                    //////////////////////////////////////////Make animation for using item
                                    player->useItem();
                                    chosenAction = NOT_CHOSEN;
                                    playersTurn = false;
                                }
                                else if (chosenAction == CATCH){
                                    //////////////////////////////////////Make set rect pos func for pokemon to move off and on screen
                                    /////////////////////////////////////////Also make animation for catching pokemon
                                    catchState = player->tryCatchingPokemon(wildPokemon);

                                    if (catchState != ANIMATION_NOT_FINISHED) {
                                        chosenAction = NOT_CHOSEN;
                                        if (catchState == CAUGHT) {
                                            messages.push_back("You caught the " + wildPokemon->getName() + "!");
                                            wildPokemon = nullptr;
                                            battleIsOver = true;
                                        } else if (catchState == NOT_CAUGHT) {
                                            messages.push_back("The " + wildPokemon->getName() + " escaped!");
                                            playersTurn = false;
                                        }
                                    }
                                }
                                else if (chosenAction == SWITCH_POKEMON){
                                    if (!player->switchingPokemon()){
                                        chosenAction = NOT_CHOSEN;
                                        playersTurn = false;
                                    }
                                }
                                else if (chosenAction == RUN){
                                    if (randomRange(outputNum) < runSuccessRate){
                                        messages.push_back("You got away.");
                                        battleIsOver = true;
                                    }
                                    else {
                                        messages.push_back("You didn't away in time...");
                                    }
                                    chosenAction = NOT_CHOSEN;
                                    playersTurn = false;
                                    //////////////////////////////////////Put this somewhere else maybe if menu is reset every turn
                                    player->resetBattleMenu();
                                }
                            } else {
                                chosenAction = player->displayBattleMenu(windowSurf, messages);

                                ////////////////////////////////////////////////Reset menu or no?
//                                if (chosenAction != NOT_CHOSEN){
//                                    player->resetBattleMenu();
//                                }
                            }
                        } else {
                            if (chosenAction != NOT_CHOSEN) {
                                if (!hasAttacked) {
                                    hasAttacked = wildPokemon->attack(player->getCurrPokemon(), messages);
                                    if (hasAttacked) {
                                        chosenAction = NOT_CHOSEN;
                                        hasAttacked = false;
                                        playersTurn = true;

                                        if (player->getCurrPokemon()->getHealth() <= 0){
                                            messages.push_back(player->getCurrPokemon()->getName() + " fainted!");
                                            if (player->noOtherHealthyPokemon()) {
                                                battleIsOver = true;
                                                //////////////////////////////////////////////Calculate experience
                                                messages.push_back("You lost...");
                                            }
                                            else {
                                                player->resetBattleMenu();
                                            }
                                        }
                                    }
                                }
                            } else {
                                ///////////////////////////////////////////Impliment random chance to run or other
                                wildPokemon->pickRandomMove(messages);
                                chosenAction = ATTACKING;
                            }
                        }
                    } else if (!battleHasBegun) {
                        player->getCurrPokemon()->setImagePos(100, 500);
                        player->getCurrPokemon()->setInfoPos(300, 500);
                        wildPokemon->setImagePos(1100, 100);
                        wildPokemon->setInfoPos(400, 100);
                        ///////////////////////////////////////////////////////Play enter battle animation
                        messages.emplace_back("You encountered a wild pokemon!");
                        ///////////////////////////////Check all NOT_CHOSEN things to see if this one here is necessary
                        chosenAction = NOT_CHOSEN;
                        player->resetBattleMenu();
                        battleHasBegun = true;
                        battleIsOver = false;
                    } else {
                        ////////////////////////////////////////////////////////Play exit battle animation
                        inBattle = false;
                        battleHasBegun = false;
                    }
                }
                else {
                    if (currMessage < messages.size()) {
                        showMessages(messages[currMessage], windowSurf);

                        if (keystates[SDL_SCANCODE_N] && !noSkip) {
                            currMessage++;
                            noSkip = true;
                        } else if (!keystates[SDL_SCANCODE_N]) {
                            noSkip = false;
                        }
                    }
                    else {
                        noSkip = true;
                        currMessage = 0;
                        messages.clear();
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

    //////////////////////////////////////Investigate exit code not zero because of this sometimes
    ////////////////////////////////////////////Also check for other surfaces that don't get freed like the item surfs
    //Frees surfaces
    SDL_FreeSurface(windowSurf);
    SDL_FreeSurface(textSurf);

    //Destroys textures
    SDL_DestroyTexture(windowTexture);

    //Deletes player
    delete player;

    //Checks if wildPokemon is still pointing to a Pokemon and deletes it
    if (wildPokemon != nullptr){
        delete wildPokemon;
    }

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