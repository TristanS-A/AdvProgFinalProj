//
// Created by tropi on 4/16/2023.
//

#include "showMessages.h"

void showMessages(vector<string> &messagesToSend, SDL_Surface* windowSurf){
    if (!messageList.empty()) {
        textSurf = TTF_RenderText_Solid(mediumFont, messagesToSend[0].c_str(), {255, 255, 255});
        SDL_Rect dest = {100, 650, 0, 0};
        SDL_BlitSurface(textSurf, nullptr, windowSurf, &dest);
        SDL_FreeSurface(textSurf);

        if (keystates[SDL_SCANCODE_N] && !noSkip) {
            messagesToSend.erase(find(messagesToSend.begin(), messagesToSend.end(), messagesToSend[0]));
            noSkip = true;
        } else if (!keystates[SDL_SCANCODE_N]) {
            noSkip = false;
        }
    }
    else {
        noSkip = true;
        messageList.clear();
    }
}