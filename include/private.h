#ifndef __PRIVATE_H__
#define __PRIVATE_H__

#ifndef __cplusplus
#error A C++ compiler is needed to process
#endif

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

#define CBPLAY             0x0000
#define CBPAUSE            0x0001
#define CBSTOP             0x0002
#define CBPLAYLIST         0x0003
#define CBVOLUME           0x0004
#define CBFULLSCREEN       0X0005

#define CBWIDTH 50
#define CBHEIGHT 50

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Event gEvent;

TTF_Font* font = nullptr;

SDL_Rect SDLRect(int x, int y, int w, int h)
{
    SDL_Rect tmp = {x, y, w, h};
    return tmp;
}

bool inside(const SDL_Rect& rect, int x, int y)
{
    if(x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h) return true;
    return false;
}

bool inside(const SDL_FRect& rect, int x, int y)
{
    if(x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h) return true;
    return false;
}

void highlightCtrlButton(int x, int y)
{
    Uint8 r, g, b, a;
    SDL_Rect rect = {x, y, CBWIDTH, CBHEIGHT};
    SDL_GetRenderDrawColor(gRenderer, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 150);
    SDL_RenderFillRect(gRenderer, &rect);
    SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
}

void ShowError(std::string text)
{
    #ifdef _WIN32
        MessageBoxA(NULL, text.c_str(), "Error", MB_ICONERROR | MB_OK);
    #endif // _WIN32

    std::cerr << "AI MUSICVERSE Error:: " << text << std::endl;
}

#endif
