#include "AMVApp.h"


AMVApp::AMVApp()
{
    Init();
}

void AMVApp::Init()
{
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    active = true;
}

AMVApp::~AMVApp()
{
    Close();
}

void AMVApp::Create(int width, int height, std::string title)
{
    SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, &window, &renderer);
    SDL_SetWindowTitle(window, title.c_str());
    windowW = width;
    windowH = height;
}

void AMVApp::Close()
{
    active = false;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

SDL_Window* AMVApp::GetWindow()
{
    return window;
}
SDL_Renderer* AMVApp::GetRenderer()
{
    return renderer;
}

void AMVApp::GetWindowDimension(int* width, int* height)
{
    *width = windowW;
    *height = windowH;
}

void AMVApp::SetMinimumDimension(int width, int height)
{
    SDL_SetWindowMinimumSize(window, width, height);
}
void AMVApp::SetMaximumDimension(int width, int height)
{
    SDL_SetWindowMaximumSize(window, width, height);
}

void AMVApp::ProcessEvents()
{
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {

        case SDL_QUIT:
            Close();
            break;

        case SDL_MOUSEBUTTONDOWN:
            mousecallback(event.button.button, event.button.state, event.button.x, event.button.y);
            break;

        case SDL_MOUSEWHEEL:
            mousewheelcallback(event.wheel.y, event.wheel.mouseX, event.wheel.mouseY);
            break;

        case SDL_MOUSEMOTION:
            motioncallback(event.motion.type, event.motion.state, event.button.x, event.button.y);
            break;

        case SDL_KEYUP:
            keycallback(event.key.keysym.scancode, event.key.keysym.mod, event.key.state);
            break;

        case SDL_DROPFILE:
            dropfilecallback(event.drop.file);
            break;

        default:
            break;
        }

        if(event.window.event == SDL_WINDOWEVENT_RESIZED) resizecallback(event.window.data1, event.window.data2);
    }

    displaycallback();
}

void AMVApp::SetMouseCallback(void (*callback)(int button, int state, int x, int y))
{
    mousecallback = callback;
}
void AMVApp::SetMouseWheelCallback(void (*callback)(int yScroll, int x, int y))
{
    mousewheelcallback = callback;
}
void AMVApp::SetMotionCallback(void (*callback)(int button, int state, int x, int y))
{
    motioncallback = callback;
}
void AMVApp::SetKeyCallback(void (*callback)(SDL_Scancode key, Uint16 modifier, int state))
{
    keycallback = callback;
}
void AMVApp::SetDropFileCallback(void (*callback)(std::string filename))
{
    dropfilecallback = callback;
}
void AMVApp::SetDisplayCallback(void (*callback)())
{
    displaycallback = callback;
}
void AMVApp::SetResizeCallback(void (*callback)(int width, int height))
{
    resizecallback = callback;
}
