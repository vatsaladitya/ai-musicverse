#ifndef AMVAPP_H
#define AMVAPP_H

#include <iostream>
#include <string>
#include <SDL2/SDL.h>

class AMVApp
{
    public:
        bool active;
        AMVApp();
        void Init();
        virtual ~AMVApp();

        void Create(int width, int height, std::string title);
        void Close();

        SDL_Window* GetWindow();
        SDL_Renderer* GetRenderer();
        void GetWindowDimension(int* width, int* height);
        void SetMinimumDimension(int width, int height);
        void SetMaximumDimension(int width, int height);

        void ProcessEvents();

        void SetMouseCallback(void (*callback)(int button, int state, int x, int y));
        void SetMouseWheelCallback(void (*callback)(int yScroll, int x, int y));
        void SetMotionCallback(void (*callback)(int button, int state, int x, int y));
        void SetKeyCallback(void (*callback)(SDL_Scancode key, Uint16 modifier, int state));
        void SetDropFileCallback(void (*dropfilecallback)(std::string filename));
        void SetDisplayCallback(void (*callback)());

        void SetResizeCallback(void (*callback)(int width, int height));

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Event event;
        int windowW;
        int windowH;

        void (*mousecallback)(int button, int state, int x, int y);
        void (*mousewheelcallback)(int yScroll, int x, int y);
        void (*motioncallback)(int button, int state, int x, int y);
        void (*keycallback)(SDL_Scancode key, Uint16 modifier, int state);
        void (*dropfilecallback)(std::string filename);
        void (*displaycallback)();

        void (*resizecallback)(int width, int height);
};

#endif // AMVAPP_H
