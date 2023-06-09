//
// Created by tropi on 4/4/2023.
//

#include "globalVariables.h"
#include "SDL_image.h"

//Dimensions for the screen
extern const int SCREEN_WIDTH = 1440;
extern const int SCREEN_HEIGHT = 810;

//Creates Rect for windowSurf surface so that its dimensions can be rescaled and repositioned
SDL_Rect windowTextureSize = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

//Small font for game
TTF_Font *smallFont;

//Medium font for game
TTF_Font *mediumFont;

//Large font for game
TTF_Font *largeFont;

//Pixel sizes for fonts
const int SMALL_FONT_SIZE = 20;
const int MEDIUM_FONT_SIZE = 28;
const int LARGE_FONT_SIZE = 50;

//To convert the pixel size of the font to width instead of hight (I think)
const float FONT_PIXEL_HEIGHT_TO_WIDTH = 1.3;

uniform_real_distribution<double> randomChanceRange(0, 0.9999);
uniform_real_distribution<double> randomLevelRange(-5.0, 5.0);

//To keep track of if the mouse is pressed
bool mouseDown = false;

//To keep track of if the mouse button is being held down
bool mouseHeldDown = false;

//So that holding down a key will not skip through messages
bool noSkip = false;

//Sets up key presses
const Uint8 *keystates{SDL_GetKeyboardState(nullptr)};

//Quadrants of the map
int topLeftQuadrant[2] = {-935, -422};
int topRightQuadrant[2] = {-1230, -422};
int bottomLeftQuadrant[2] = {-935, -742};
int bottomRightQuadrant[2] = {-1230, -742};

//Images for the battle backgrounds in each quadrant
SDL_Surface* battleBackground_Forest = IMG_Load("images/battleBG_Grass.png");
SDL_Surface* battleBackground_Swamp = IMG_Load("images/battleBG_Swamp.png");
SDL_Surface* battleBackground_Snow = IMG_Load("images/battleBG_Snow.png");
SDL_Surface* battleBackground_Fire = IMG_Load("images/battleBG_Fire.png");

//The current battle background
SDL_Surface* currBattleBackground;

//Image for the bakcground
SDL_Surface* background = IMG_Load("images/background.png");

//Position and size of the background
SDL_Rect bgPos;

//Pointer for the player
Player* player;

//To send messages in battle;
vector<string> messageList;

//Text color
SDL_Color textColor = {255, 255, 255};

//The amount of moves per pokemon type (in move txt file)
int amountOfMovesPerType = 0;

//The amount of names per pokemon type (in name txt file)
int amountOfNamesPerType = 0;

//Various button images
SDL_Surface* buttonIMG = IMG_Load("images/button.png");
SDL_Surface* buttonHoverIMG = IMG_Load("images/buttonHover.png");
SDL_Surface* backButtonIMG = IMG_Load("images/backButton.png");
SDL_Surface* backButtonHoverIMG = IMG_Load("images/backButtonHover.png");
SDL_Surface* attackButtonIMG = IMG_Load("images/attackButton.png");
SDL_Surface* attackButtonHoverIMG = IMG_Load("images/attackButtonHover.png");
SDL_Surface* itemsButtonIMG = IMG_Load("images/itemsButton.png");
SDL_Surface* itemsButtonHoverIMG = IMG_Load("images/itemsButtonHover.png");
SDL_Surface* catchButtonIMG = IMG_Load("images/catchButton.png");
SDL_Surface* catchButtonHoverIMG = IMG_Load("images/catchButtonHover.png");
SDL_Surface* swapButtonIMG = IMG_Load("images/swapButton.png");
SDL_Surface* swapButtonHoverIMG = IMG_Load("images/swapButtonHover.png");
SDL_Surface* runButtonIMG = IMG_Load("images/runButton.png");
SDL_Surface* runButtonHoverIMG = IMG_Load("images/runButtonHover.png");
SDL_Surface* leftButtonIMG = IMG_Load("images/leftButton.png");
SDL_Surface* leftButtonHoverIMG = IMG_Load("images/leftButtonHover.png");
SDL_Surface* rightButtonIMG = IMG_Load("images/rightButton.png");
SDL_Surface* rightButtonHoverIMG = IMG_Load("images/rightButtonHover.png");
SDL_Surface* startButtonIMG = IMG_Load("images/startButton.png");
SDL_Surface* spaceButton = IMG_Load("images/spaceButton.png");

//Various background or other images
SDL_Surface* pokeInfoBG = IMG_Load("images/pokeInfoBG.png");
SDL_Surface* titleScreenImage = IMG_Load("images/titleScreenImage.png");
SDL_Surface* startButtonHoverIMG = IMG_Load("images/startButtonHover.png");
SDL_Surface* chooseScreen = IMG_Load("images/chooseScreen.png");
SDL_Surface* curtain = IMG_Load("images/curtain.png");
SDL_Surface* pokecenter = IMG_Load("images/pokecenteIMG.png");
SDL_Surface* textboxIMG = IMG_Load("images/textbox.png");

//Surface for displaying text
SDL_Surface* textSurf;

//Music
Mix_Music* titleMusic;
Mix_Music* worldMusic;
Mix_Music* battleMusic;

//Sound effects
Mix_Chunk* currSound;
Mix_Chunk* battleIntro;
Mix_Chunk* critHit;
Mix_Chunk* weakHit;
Mix_Chunk* regularHit;
Mix_Chunk* buttonSound;
Mix_Chunk* pokecenterSound;
Mix_Chunk* healSound;