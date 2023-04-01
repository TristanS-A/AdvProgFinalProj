//
// Created by tropi on 4/1/2023.
//

#include "handlePlayerMovement.h"

void handlePlayerMovement(Player* player, SDL_Rect &bgRect, const int SCREEN_WIDTH, const int SCREEN_HEIGHT, const Uint8* keystates){
    if (keystates[SDL_SCANCODE_A]) {

        //Moves the background to the right while it's not at its left border, and while the player is at the
        // x-axis center of the screen (So moving the opposite direction moves the player and not the background)
        if (bgRect.x < 0 && player->getPlayerRect().x == SCREEN_WIDTH / 2 - player->getPlayerRect().w / 2) {
            bgRect.x += player->getPlayerSpeed();

            //Check for if the background goes past its left border
            if (bgRect.x > 0){
                bgRect.x = 0;
            }
        }
        else {

            //Moves player instead of the background while the background is at its left border
            if (player->getPlayerRect().x > 0) {
                player->movePlayer(-player->getPlayerSpeed(), 0);

                //Check for if the player moves past the left window border (Offscreen left)
                if (player->getPlayerRect().x < 0){
                    player->setPlayerPos(0, player->getPlayerRect().y);
                }

                    //Check for if the player moves past the x-axis center of the screen (To transition to moving
                    // the background instead of the player)
                else if (player->getPlayerRect().x < SCREEN_WIDTH / 2 - player->getPlayerRect().w / 2 && bgRect.x != 0){
                    player->setPlayerPos(SCREEN_WIDTH / 2 - player->getPlayerRect().w / 2, player->getPlayerRect().y);
                }
            }
        }
    }
    if ((keystates[SDL_SCANCODE_D])) {

        //Moves the background to the left while it's not at its right border, and while the player is at the
        // x-axis center of the screen (So moving the opposite direction moves the player and not the background)
        if (bgRect.x > -bgRect.w + SCREEN_WIDTH && player->getPlayerRect().x == SCREEN_WIDTH / 2 - player->getPlayerRect().w / 2) {
            bgRect.x -= player->getPlayerSpeed();

            //Check for if the background goes past its right border
            if (bgRect.x < -bgRect.w + SCREEN_WIDTH) {
                bgRect.x = -bgRect.w + SCREEN_WIDTH;
            }
        }
        else {

            //Moves player instead of the background while the background is at its right border
            if (player->getPlayerRect().x < SCREEN_WIDTH - player->getPlayerRect().w) {
                player->movePlayer(player->getPlayerSpeed(), 0);

                //Check for if the player moves past the right window border (Offscreen right)
                if (player->getPlayerRect().x > SCREEN_WIDTH - player->getPlayerRect().w){
                    player->setPlayerPos(SCREEN_WIDTH - player->getPlayerRect().w, player->getPlayerRect().y);
                }

                    //Check for if the player moves past the x-axis center of the screen (To transition to moving
                    // the background instead of the player)
                else if (player->getPlayerRect().x > SCREEN_WIDTH / 2 - player->getPlayerRect().w / 2 && bgRect.x != -bgRect.w + SCREEN_WIDTH){
                    player->setPlayerPos(SCREEN_WIDTH / 2 - player->getPlayerRect().w / 2, player->getPlayerRect().y);
                }
            }
        }
    }
    if (keystates[SDL_SCANCODE_W]) {

        //Moves the background down while it's not at its top border, and while the player is at the y-axis
        // center of the screen (So moving the opposite direction moves the player and not the background)
        if (bgRect.y <  0 && player->getPlayerRect().y == SCREEN_HEIGHT / 2 - player->getPlayerRect().h / 2) {
            bgRect.y += player->getPlayerSpeed();

            //Check for if the background goes past its top border
            if (bgRect.y > 0) {
                bgRect.y = 0;
            }
        }
        else {

            //Moves player instead of the background while the background is at its top border
            if (player->getPlayerRect().y > 0) {
                player->movePlayer(0, -player->getPlayerSpeed());

                //Check for if the player moves past the top window border (Offscreen top)
                if (player->getPlayerRect().y < 0){
                    player->setPlayerPos(player->getPlayerRect().x, 0);
                }

                    //Check for if the player moves past the y-axis center of the screen (To transition to moving
                    // the background instead of the player)
                else if (player->getPlayerRect().y < SCREEN_HEIGHT / 2 - player->getPlayerRect().h / 2 && bgRect.y != 0){
                    player->setPlayerPos(player->getPlayerRect().x, SCREEN_HEIGHT / 2 - player->getPlayerRect().h / 2);
                }
            }
        }
    }
    if ((keystates[SDL_SCANCODE_S])) {

        //Moves the background up while it's not at its bottom border, and while the player is at the y-axis
        // center of the screen (So moving the opposite direction moves the player and not the background)
        if (bgRect.y > -bgRect.h + SCREEN_HEIGHT && player->getPlayerRect().y == SCREEN_HEIGHT / 2 - player->getPlayerRect().h / 2) {
            bgRect.y -= player->getPlayerSpeed();

            //Check for if the background goes past its bottom border
            if (bgRect.y < -bgRect.h + SCREEN_HEIGHT) {
                bgRect.y = -bgRect.h + SCREEN_HEIGHT;
            }
        }
        else {

            //Moves player instead of the background while the background is at its bottom border
            if (player->getPlayerRect().y < SCREEN_HEIGHT - player->getPlayerRect().w) {
                player->movePlayer(0, player->getPlayerSpeed());

                //Check for if the player moves past the bottom window border (Offscreen bottom)
                if (player->getPlayerRect().y > SCREEN_HEIGHT - player->getPlayerRect().w){
                    player->setPlayerPos(player->getPlayerRect().x, SCREEN_HEIGHT - player->getPlayerRect().w);
                }

                    //Check for if the player moves past the y-axis center of the screen (To transition to moving
                    // the background instead of the player)
                else if (player->getPlayerRect().y > SCREEN_HEIGHT / 2 - player->getPlayerRect().h / 2 && bgRect.y != -bgRect.h + SCREEN_HEIGHT){
                    player->setPlayerPos(player->getPlayerRect().x, SCREEN_HEIGHT / 2 - player->getPlayerRect().h / 2);
                }
            }
        }
    }

    //Gets input for sprinting and sets player speed to sprinting speed
    if (keystates[SDL_SCANCODE_LSHIFT]){
        player->setSprinting();
    }
}