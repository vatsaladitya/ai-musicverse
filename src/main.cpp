#include "private.h"
#include "AMVApp.h"
// #include "button_dark.h"
// #include "button_light.h"

bool light_mode = true;
bool fullscreen = false;

int wnd_width = 800, wnd_height = 600;

// Component coordinates in source image
// They remain same throughout the program
SDL_Rect play_s       = {CBWIDTH * 0, 800, 50, 50};
SDL_Rect pause_s      = {CBWIDTH * 1, 800, 50, 50};
SDL_Rect stop_s       = {CBWIDTH * 2, 800, 50, 50};
SDL_Rect playlist_s   = {CBWIDTH * 3, 800, 50, 50};
SDL_Rect volume_s     = {CBWIDTH * 4, 800, 50, 50};
SDL_Rect fullscreen_s = {CBWIDTH * 5, 800, 50, 50};
SDL_Rect background_s = {0, 0, 800, 800};
SDL_Rect logo_s       = {800, 0, 200, 200};

// Component coordinates for destination buttons
// They change with window size
SDL_Rect play_b       = {(CBWIDTH * 0) + (20 * 1), wnd_height - 75, 50, 50};
SDL_Rect pause_b      = {(CBWIDTH * 1) + (20 * 2), wnd_height - 75, 50, 50};
SDL_Rect stop_b       = {(CBWIDTH * 2) + (20 * 3), wnd_height - 75, 50, 50};
SDL_Rect playlist_b   = {(CBWIDTH * 3) + (20 * 4), wnd_height - 75, 50, 50};
SDL_Rect fullscreen_b = {(CBWIDTH * 4) + (20 * 5), wnd_height - 75, 50, 50};
SDL_Rect volume_b     = {wnd_width - CBWIDTH - 20, wnd_height - 75, 50, 50};
SDL_Rect background_b = {0, 0, wnd_width, wnd_height - 100};
SDL_Rect logo_b       = {wnd_width/2 - 100, wnd_height/2 - 100, 200, 200};
SDL_Rect seek_b;

int globalVolume = 50;
bool globalVolumeChanged = false;
bool muted = false;
bool player_ready = false;
std::string mediafilename;

bool got_filename = false;
bool play_start = false;
bool play_end = false;

struct highlightButton
{
    bool play;
    bool pause;
    bool stop;
    bool playlist;
    bool fullscreen;
    bool volume;
};

struct highlightButton highlightableButton;

float position = 0, length = 0;
int second = 0, minute = 0, hour = 0;

SDL_Texture *texture_light = nullptr, *texture_dark = nullptr, *logo_light = nullptr, *logo_dark = nullptr;
SDL_Texture *texture_current = nullptr;
AMVApp app;

SDL_AudioSpec spec;
Mix_Music *music;

static void printString (int x, int y, int height, const char *fmt, ...)
{
    if(height > 0) TTF_SetFontSize(font, height);
    else TTF_SetFontSize(font, 15);

    char buf[1024];
    va_list args;

    va_start(args, fmt);
    vsnprintf (buf, sizeof(buf), fmt, args);
    va_end(args);

    SDL_Rect tmp;
    SDL_Surface* textSurface = nullptr;
    SDL_Texture* textTexture = nullptr;
    SDL_SetSurfaceBlendMode(textSurface, SDL_BLENDMODE_BLEND);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color transparent = {0, 0, 0, 1};

    textSurface = TTF_RenderText_Blended(font, buf, (light_mode?black:white));
    textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    SDL_SetTextureBlendMode(textTexture, SDL_BLENDMODE_BLEND);
    tmp = {x, y, textSurface->w, textSurface->h};

    SDL_RenderCopy(gRenderer, textTexture, nullptr, &tmp);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void wheel(int yScroll, int x, int y)
{
    globalVolume += yScroll;
    if(globalVolume > 100) globalVolume = 100;
    if(globalVolume < 0) globalVolume = 0;
    if(!muted) Mix_VolumeMusic(globalVolume);
}

void mouse(int button, int state, int x, int y)
{
    if(button == SDL_BUTTON_LEFT && state == SDL_PRESSED)
    {
        if(highlightableButton.play && got_filename)
        {
            if(Mix_PausedMusic()) Mix_ResumeMusic();
            else Mix_PlayMusic(music, 0);
        };

        if(highlightableButton.pause) Mix_PauseMusic();
        if(highlightableButton.stop) Mix_CloseAudio();

        // TODO: Add playlist feature
        //if(highlightableButton.playlist);

        if(highlightableButton.fullscreen)
        {
            fullscreen = fullscreen ? false:true;
            if(fullscreen) SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
            else SDL_SetWindowFullscreen(gWindow, 0);
        }

        if(highlightableButton.volume)
        {
            if(muted)
                Mix_VolumeMusic(globalVolume);
            else
                Mix_VolumeMusic(0);

            muted = !muted;
        }

        if(inside(seek_b, x, y))
        {
            std::cout << "hit\n";
            position = (x - play_b.x) * length / (volume_b.x + volume_b.w - play_b.x);
            std::cout << position << std::endl;
            Mix_SetMusicPosition(position);
        }
    }
}

void motion(int button, int state, int x, int y)
{
    if((y > wnd_height - 75) && (y < (wnd_height - 75 + CBWIDTH)))
    {
        if(inside(play_b, x, y))
            highlightableButton.play = true;
        else
            highlightableButton.play = false;

        if(inside(pause_b, x, y))
            highlightableButton.pause = true;
        else
            highlightableButton.pause = false;

        if(inside(stop_b, x, y))
            highlightableButton.stop = true;
        else
            highlightableButton.stop = false;

        if(inside(playlist_b, x, y))
            highlightableButton.playlist = true;
        else
            highlightableButton.playlist = false;

        if(inside(fullscreen_b, x, y))
            highlightableButton.fullscreen = true;
        else
            highlightableButton.fullscreen = false;

        if(inside(volume_b, x, y))
            highlightableButton.volume = true;
        else
            highlightableButton.volume = false;
    }
    else
        highlightableButton = {false, false, false, false, false, false};
}

void key (SDL_Scancode key, Uint16 modifier, int state)
{
    switch(key)
    {
    case SDL_SCANCODE_F:
        fullscreen = fullscreen ? false:true;
        if(fullscreen) SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
        else SDL_SetWindowFullscreen(gWindow, 0);
        break;

    case SDL_SCANCODE_D:
        light_mode = (light_mode ? false : true);
        texture_current = light_mode ? texture_light : texture_dark;
        if(light_mode) SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 255);
        else SDL_SetRenderDrawColor(gRenderer, 125, 0, 255, 255);
        break;

    case SDL_SCANCODE_SPACE:
        if(Mix_PlayingMusic()) Mix_PauseMusic();
        if(Mix_PausedMusic()) Mix_ResumeMusic();
        break;

    case SDL_SCANCODE_ESCAPE:
    case SDL_SCANCODE_Q:
        app.Close();
        break;

    case SDL_SCANCODE_UP:
        globalVolume += 5;
        if(globalVolume > 100) globalVolume = 100;
        if(!muted) Mix_VolumeMusic(globalVolume);
        break;

    case SDL_SCANCODE_DOWN:
        globalVolume -= 5;
        if(globalVolume < 0) globalVolume = 0;
        if(!muted) Mix_VolumeMusic(globalVolume);
        break;

    case SDL_SCANCODE_LEFT:
        position -= 5;
        Mix_SetMusicPosition(position);
        break;

    case SDL_SCANCODE_RIGHT:
        position += 5;
        Mix_SetMusicPosition(position);
        break;

    default :
        break;
    }
}

void resize(int width, int height)
{
    wnd_width = width;
    wnd_height = height;

    play_b       = {(CBWIDTH * 0) + (20 * 1), wnd_height - 75, 50, 50};
    pause_b      = {(CBWIDTH * 1) + (20 * 2), wnd_height - 75, 50, 50};
    stop_b       = {(CBWIDTH * 2) + (20 * 3), wnd_height - 75, 50, 50};
    playlist_b   = {(CBWIDTH * 3) + (20 * 4), wnd_height - 75, 50, 50};
    fullscreen_b = {(CBWIDTH * 4) + (20 * 5), wnd_height - 75, 50, 50};
    volume_b     = {wnd_width - CBWIDTH - 20, wnd_height - 75, 50, 50};
    background_b = {0, 0, wnd_width, wnd_height - 100};
    logo_b       = {wnd_width/2 - 100, wnd_height/2 - 100, 200, 200};
    seek_b       = {play_b.x, play_b.y - 20, volume_b.x + volume_b.w - play_b.x, 15};
}

void displayTime()
{
    if(!Mix_PlayingMusic()) return;

    // TODO: Setup playback time
    position = static_cast<float>(Mix_GetMusicPosition(music));
    int iPosition = static_cast<int>(position);

    int s = iPosition % 60;
    int m = (iPosition / 60) % 60;
    int h = iPosition / 3600;

    float w = static_cast<float>(volume_b.x + volume_b.w - play_b.x) * position / length;
    SDL_Rect rc = {play_b.x, play_b.y - 20, static_cast<int>(w), 15};

    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(gRenderer, &r, &g, &b, &a);
    (light_mode ? SDL_SetRenderDrawColor(gRenderer, 0, 200, 255, 255) : SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255));
    SDL_RenderFillRect(gRenderer, &rc);
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(gRenderer, &seek_b);
    SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
    printString(play_b.x, play_b.y - 50, 20, "%d:%d:%d/%d:%d:%d", h, m, s, hour, minute, second);
}

void disp()
{
    SDL_RenderClear(gRenderer);

    // Render background and logo
    SDL_RenderCopy(gRenderer, texture_current, &background_s, &background_b);
    SDL_RenderCopy(gRenderer, texture_current, &logo_s, &logo_b);

    // Render control buttons
    SDL_RenderCopy(gRenderer, texture_current, &play_s, &play_b);
    SDL_RenderCopy(gRenderer, texture_current, &pause_s, &pause_b);
    SDL_RenderCopy(gRenderer, texture_current, &stop_s, &stop_b);
    SDL_RenderCopy(gRenderer, texture_current, &playlist_s, &playlist_b);
    SDL_RenderCopy(gRenderer, texture_current, &fullscreen_s, &fullscreen_b);
    SDL_RenderCopy(gRenderer, texture_current, &volume_s, &volume_b);

    if(muted)
    {
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(gRenderer, &r, &g, &b, &a);
        (light_mode ? SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255) : SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255));
        SDL_RenderDrawLine(gRenderer, volume_b.x, volume_b.y + volume_b.h, volume_b.x + volume_b.w, volume_b.y);
        SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
    }


    if(highlightableButton.play) highlightCtrlButton(play_b.x, play_b.y);
    if(highlightableButton.pause) highlightCtrlButton(pause_b.x, pause_b.y);
    if(highlightableButton.stop) highlightCtrlButton(stop_b.x, stop_b.y);
    if(highlightableButton.playlist) highlightCtrlButton(playlist_b.x, playlist_b.y);
    if(highlightableButton.fullscreen) highlightCtrlButton(fullscreen_b.x, fullscreen_b.y);
    if(highlightableButton.volume) highlightCtrlButton(volume_b.x, volume_b.y);

    printString(volume_b.x, volume_b.y + volume_b.h, 15, "%d%%", globalVolume);
    displayTime();

    SDL_RenderPresent(gRenderer);
}

void loadTexture()
{
    HMODULE hModule = GetModuleHandle(NULL);
    HRSRC hRes = FindResourceA(hModule, MAKEINTRESOURCEA(2), "PNG");
    HGLOBAL hGlobal = LoadResource(hModule, hRes);
    void* data = LockResource(hGlobal);
    DWORD sz = SizeofResource(hModule, hRes);
    SDL_RWops* rw = SDL_RWFromMem(data, sz);
    texture_light = IMG_LoadTexture_RW(gRenderer, rw, 0);

    hRes = FindResourceA(hModule, MAKEINTRESOURCEA(3), "PNG");
    hGlobal = LoadResource(hModule, hRes);
    data = LockResource(hGlobal);
    sz = SizeofResource(hModule, hRes);
    rw = SDL_RWFromMem(data, sz);

    texture_dark = IMG_LoadTexture_RW(gRenderer, rw, 0);

    texture_current = texture_light;

    if(light_mode) SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 255);
    else SDL_SetRenderDrawColor(gRenderer, 125, 0, 255, 255);

    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BlendMode::SDL_BLENDMODE_BLEND);
}

void loadFont()
{
    font = TTF_OpenFont("C:\\Windows\\Fonts\\SegoeUI.ttf", 15);
    TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
    TTF_SetFontDirection(font, TTF_DIRECTION_LTR);
}

void loadMusic()
{
    if(!got_filename) return;
    spec.channels = MIX_DEFAULT_CHANNELS;
    spec.format = MIX_DEFAULT_FORMAT;
    spec.freq = MIX_DEFAULT_FREQUENCY;

    Mix_OpenAudio(spec.freq, spec.format, spec.channels, 0);
    Mix_VolumeMusic(globalVolume);
    muted = false;
    music = Mix_LoadMUS(mediafilename.c_str());
    length = static_cast<float>(Mix_MusicDuration(music));
    int iLength = static_cast<int>(length);

    hour = iLength / 3600;
    minute = (iLength / 60) % 60;
    second = iLength % 60;

    std::cout << "Length: " << length << std::endl
              << "Hours: " << hour << std::endl
              << "Minutes: " << minute << std::endl
              << "Seconds: " << second << std::endl;

    std::string title = "AI MUSICVERSE - ";
    title += mediafilename;
    SDL_SetWindowTitle(gWindow, title.c_str());
    Mix_PlayMusic(music, 0);
}

void dropfile(std::string file)
{
    std::cout << file << std::endl;
    got_filename = true;
    mediafilename = file;
    loadMusic();
}


int main (int argc, char* argv[])
{
    std::string temp, file_extension;
    if(argv[1])
    {
        temp = argv[1];
        int dotPos = static_cast<int>(temp.rfind("."));
        file_extension = std::string(temp, dotPos, temp.length() - dotPos);

        if(file_extension == ".mp3" || file_extension == ".wav" || file_extension == ".ogg")
        {
            mediafilename = argv[1];
            std::cout << "Media file           : " << mediafilename << std::endl;
            std::cout << "Media file extension : " << file_extension << std::endl;
            got_filename = true;
        }
        else
        {
            ShowError("Cannot open the input file. File format or extension not recognized!");
        }
    }

    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

    app.Create(wnd_width, wnd_height, "AI MUSICVERSE");
    app.SetDisplayCallback(disp);
    app.SetKeyCallback(key);
    app.SetMouseCallback(mouse);
    app.SetMotionCallback(motion);
    app.SetMouseWheelCallback(wheel);
    app.SetResizeCallback(resize);
    app.SetMinimumDimension(600, 400);
    app.SetDropFileCallback(dropfile);

    gWindow = app.GetWindow();
    gRenderer = app.GetRenderer();

    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

    loadFont();
    loadMusic();
    loadTexture();

    resize(wnd_width, wnd_height);

    while(app.active)
    {
        app.ProcessEvents();
    }

    return 0;
}

