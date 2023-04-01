#define SDL_MAIN_HANDLED true
#define _USE_MATH_DEFINES
#include <iostream>
#include <algorithm>

using namespace std;

#include "screenSizeChange.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

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

    //Font for game
    TTF_Font *font = TTF_OpenFont("fonts/font.ttf", 28);

    //Check for if font was correctly opened
    if (!font){
        cout << "Font could not be opened\n";
    }

    //Sets up key presses
    const Uint8 *keystates = SDL_GetKeyboardState(nullptr);

    SDL_Surface* player = IMG_Load("images/p.png");
    SDL_Rect playerPos = {SCREEN_WIDTH / 2 - player->w / 2,SCREEN_HEIGHT / 2 - player->h / 2, 0, 0};

    const int walkSpeed = 5;
    const int sprintSpeed = 10;
    int playerSpeed;

    SDL_Surface* background = IMG_Load("images/background.png");
    SDL_Rect bgPos = {-background->w / 2 + SCREEN_WIDTH / 2, -background->h / 2 + SCREEN_HEIGHT / 2, 0, 0};

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
                    cout << "Mouse button down\n";
                    break;

                //User lets go of the mouse button
                case SDL_MOUSEBUTTONUP:
                    cout << "Mouse button up\n";
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
                    playerSpeed = walkSpeed;
            }

            //Handles walking inputs
            if (keystates[SDL_SCANCODE_A]) {

                //Moves the background to the right while it's not at its left border, and while the player is at the
                // x-axis center of the screen (So moving the opposite direction moves the player and not the background)
                if (bgPos.x < 0 && playerPos.x == SCREEN_WIDTH / 2 - player->w / 2) {
                    bgPos.x += playerSpeed;

                    //Check for if the background goes past its left border
                    if (bgPos.x > 0){
                        bgPos.x = 0;
                    }
                }
                else {

                    //Moves player instead of the background while the background is at its left border
                    if (playerPos.x > 0) {
                        playerPos.x -= playerSpeed;

                        //Check for if the player moves past the left window border (Offscreen left)
                        if (playerPos.x < 0){
                            playerPos.x = 0;
                        }

                        //Check for if the player moves past the x-axis center of the screen (To transition to moving
                        // the background instead of the player)
                        else if (playerPos.x < SCREEN_WIDTH / 2 - player->w / 2 && bgPos.x != 0){
                            playerPos.x = SCREEN_WIDTH / 2 - player->w / 2;
                        }
                    }
                }
            }
            if ((keystates[SDL_SCANCODE_D])) {

                //Moves the background to the left while it's not at its right border, and while the player is at the
                // x-axis center of the screen (So moving the opposite direction moves the player and not the background)
                if (bgPos.x > -background->w + SCREEN_WIDTH && playerPos.x == SCREEN_WIDTH / 2 - player->w / 2) {
                    bgPos.x -= playerSpeed;

                    //Check for if the background goes past its right border
                    if (bgPos.x < -background->w + SCREEN_WIDTH) {
                        bgPos.x = -background->w + SCREEN_WIDTH;
                    }
                }
                else {

                    //Moves player instead of the background while the background is at its right border
                    if (playerPos.x < SCREEN_WIDTH - player->w) {
                        playerPos.x += playerSpeed;

                        //Check for if the player moves past the right window border (Offscreen right)
                        if (playerPos.x > SCREEN_WIDTH - player->w){
                            playerPos.x = SCREEN_WIDTH - player->w;
                        }

                        //Check for if the player moves past the x-axis center of the screen (To transition to moving
                        // the background instead of the player)
                        else if (playerPos.x > SCREEN_WIDTH / 2 - player->w / 2 && bgPos.x != -background->w + SCREEN_WIDTH){
                            playerPos.x = SCREEN_WIDTH / 2 - player->w / 2;
                        }
                    }
                }
            }
            if (keystates[SDL_SCANCODE_W]) {

                //Moves the background down while it's not at its top border, and while the player is at the y-axis
                // center of the screen (So moving the opposite direction moves the player and not the background)
                if (bgPos.y <  0 && playerPos.y == SCREEN_HEIGHT / 2 - player->h / 2) {
                    bgPos.y += playerSpeed;

                    //Check for if the background goes past its top border
                    if (bgPos.y > 0) {
                        bgPos.y = 0;
                    }
                }
                else {

                    //Moves player instead of the background while the background is at its top border
                    if (playerPos.y > 0) {
                        playerPos.y -= playerSpeed;

                        //Check for if the player moves past the top window border (Offscreen top)
                        if (playerPos.y < 0){
                            playerPos.y = 0;
                        }

                            //Check for if the player moves past the y-axis center of the screen (To transition to moving
                            // the background instead of the player)
                        else if (playerPos.y < SCREEN_HEIGHT / 2 - player->h / 2 && bgPos.y != 0){
                            playerPos.y = SCREEN_HEIGHT / 2 - player->h / 2;
                        }
                    }
                }
            }
            if ((keystates[SDL_SCANCODE_S])) {

                //Moves the background up while it's not at its bottom border, and while the player is at the y-axis
                // center of the screen (So moving the opposite direction moves the player and not the background)
                if (bgPos.y > -background->h + SCREEN_HEIGHT && playerPos.y == SCREEN_HEIGHT / 2 - player->h / 2) {
                    bgPos.y -= playerSpeed;

                    //Check for if the background goes past its bottom border
                    if (bgPos.y < -background->h + SCREEN_HEIGHT) {
                        bgPos.y = -background->h + SCREEN_HEIGHT;
                    }
                }
                else {

                    //Moves player instead of the background while the background is at its bottom border
                    if (playerPos.y < SCREEN_HEIGHT - player->w) {
                        playerPos.y += playerSpeed;

                        //Check for if the player moves past the bottom window border (Offscreen bottom)
                        if (playerPos.y > SCREEN_HEIGHT - player->w){
                            playerPos.y = SCREEN_HEIGHT - player->w;
                        }

                        //Check for if the player moves past the y-axis center of the screen (To transition to moving
                        // the background instead of the player)
                        else if (playerPos.y > SCREEN_HEIGHT / 2 - player->h / 2 && bgPos.y != -background->h + SCREEN_HEIGHT){
                            playerPos.y = SCREEN_HEIGHT / 2 - player->h / 2;
                        }
                    }
                }
            }

            //Gets input for sprinting and sets player speed to sprinting speed
            if (keystates[SDL_SCANCODE_LSHIFT]){
                playerSpeed = sprintSpeed;
            }

            placeHolderRect = bgPos;
            SDL_BlitSurface(background, nullptr, windowSurf, &placeHolderRect);
            SDL_BlitSurface(player, nullptr, windowSurf, &playerPos);

            SDL_Rect dest = {10, 10, 1000, 1000};
            textSurf = TTF_RenderText_Solid(font, "Woooooo!!", textColor);
            SDL_BlitSurface(textSurf, nullptr, windowSurf, &dest);

            //Updates windowTexture, so it can be rendered with new blit info
            SDL_UpdateTexture(windowTexture, nullptr, windowSurf->pixels, windowSurf->pitch);
        }

        // Initialize renderer color white for the background
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Clear screen
        SDL_RenderClear(renderer);

        //Prepares windowTexture to be rendered at the location of windowTextureSize and also resizes/soft scales it to the
        // dimensions of windowTextureSize
        SDL_RenderCopy(renderer, windowTexture, nullptr, &windowTextureSize);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    //Frees surfaces
    SDL_FreeSurface(windowSurf);
    SDL_FreeSurface(textSurf);

    //Close the font that was used
    TTF_CloseFont(font);

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