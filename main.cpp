#define FREEGLUT_STATIC
#define GLUT_DISABLE_ATEXIT_HACK
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG

bool light_mode = true;

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <IMG/SOIL.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <fstream>
#include "private.h"
#include <conio.h>
#include <windows.h>
#include <string>
#include <irrKlang.h>
#include <drawtext.h>
#include <commctrl.h>

#include <objidl.h>
#include <gdiplus.h>

using namespace std;
using namespace Gdiplus;
using namespace irrklang;

#define CBWIDTH 50
#define CBHEIGHT 50

int mainWindow;
HWND hwnd, progressBar;

quad play      = {CBWIDTH * 0, 800, 50, 50};
quad pause     = {CBWIDTH * 1, 800, 50, 50};
quad stop      = {CBWIDTH * 2, 800, 50, 50};
quad playlist  = {CBWIDTH * 3, 800, 50, 50};
quad volume    = {CBWIDTH * 4, 800, 50, 50};

float globalVolume = 0.50f;
bool globalVolumeChanged = false;

irrklang::ISound *snd;
irrklang::ISoundEngine *engine;
bool player_ready = false;
char *mediafilename;


bool windowGot = false;
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

highlightButton highlightableButton;
quad background = {0, 0, 800, 800};

int wnd_width, wnd_height;
int position = 0, length = 0;
int second = 0, minute = 0, hour = 0;

GLuint texture_light, texture_dark, logo_light, logo_dark;

struct dtx_font* font;

static void printString (int x, int y, int height, const char *fmt, ...)
{
    char buf[256];
    va_list args;

    va_start(args, fmt);
#if defined(WIN32) && !defined(__CYGWIN__)
    (void) _vsnprintf (buf, sizeof(buf), fmt, args);
#else
    (void) vsnprintf (buf, sizeof(buf), fmt, args);
#endif
    va_end(args);

    dtx_position(p2cx(x), p2cy(y + height));
    dtx_string(buf);
}

void wheel(int wheel, int direction, int x, int y)
{
    if((y > wnd_height - 75) && (y < (wnd_height - 75 + CBWIDTH)))
        if((x > ((wnd_width - (CBWIDTH) - (20 *1))) && (x < (wnd_width - (CBWIDTH) - (20 *1) + CBWIDTH))))
        {
            globalVolume += (direction / 100.0f);
            if(globalVolume > 1.0f) globalVolume = 1.0f;
            if(globalVolume < 0.0f) globalVolume = 0.0f;
            globalVolumeChanged = true;
            std::cout << "Global Volume : " << globalVolume << "   Mouse Wheel : " << direction << " / " << (direction / 10.0f) << endl;

        }

}

void mouse(int msbutton, int state, int x, int y)
{
    if(msbutton == 0 && state == 1)
    {
        if((y > wnd_height - 75) && (y < (wnd_height - 75 + CBWIDTH)))
        {
            if((x > ((CBWIDTH * 0) + (20 * 1))) && (x < ((CBWIDTH * 0) + (20 * 1) + CBWIDTH)))
            {
                if(snd && snd->getIsPaused()) snd->setIsPaused(false);
                if(play_end)
                {
                    second = 0;
                    minute = 0;
                    hour = 0;
                    snd = engine->play2D("./example.mp3", false, false, true);
                    play_start = true;
                    play_end = false;
                }
            }/* Play */

            if((x > ((CBWIDTH * 1) + (20 * 2))) && (x < ((CBWIDTH * 1) + (20 * 2) + CBWIDTH))) if(snd && !snd->getIsPaused()) snd->setIsPaused(true);/* Pause */
            if((x > ((CBWIDTH * 2) + (20 * 3))) && (x < ((CBWIDTH * 2) + (20 * 3) + CBWIDTH))) if(snd) snd->stop(); /* Stop */
            if((x > ((CBWIDTH * 3) + (20 * 4))) && (x < ((CBWIDTH * 3) + (20 * 4) + CBWIDTH))) ;/* Playlist */
            if((x > ((CBWIDTH * 4) + (20 * 5))) && (x < ((CBWIDTH * 4) + (20 * 5) + CBWIDTH))) glutFullScreenToggle();/* Fullscreen */
        }
    }
    glutPostRedisplay();
}

void motion(int x, int y)
{
    if((y > wnd_height - 75) && (y < (wnd_height - 75 + CBWIDTH)))
    {
        if((x > ((CBWIDTH * 0) + (20 * 1))) && (x < ((CBWIDTH * 0) + (20 * 1) + CBWIDTH)))
            highlightableButton.play = true;
        else
            highlightableButton.play = false;

        if((x > ((CBWIDTH * 1) + (20 * 2))) && (x < ((CBWIDTH * 1) + (20 * 2) + CBWIDTH)))
            highlightableButton.pause = true;
        else
            highlightableButton.pause = false;

        if((x > ((CBWIDTH * 2) + (20 * 3))) && (x < ((CBWIDTH * 2) + (20 * 3) + CBWIDTH)))
            highlightableButton.stop = true;
        else
            highlightableButton.stop = false;

        if((x > ((CBWIDTH * 3) + (20 * 4))) && (x < ((CBWIDTH * 3) + (20 * 4) + CBWIDTH)))
            highlightableButton.playlist = true;
        else
            highlightableButton.playlist = false;

        if((x > ((CBWIDTH * 4) + (20 * 5))) && (x < ((CBWIDTH * 4) + (20 * 5) + CBWIDTH)))
            highlightableButton.fullscreen = true;
        else
            highlightableButton.fullscreen = false;

        if((x > ((wnd_width - (CBWIDTH) - (20 *1))) && (x < (wnd_width - (CBWIDTH) - (20 *1) + CBWIDTH))))
            highlightableButton.volume = true;
        else
            highlightableButton.volume = false;
    }
    else
        highlightableButton = {false, false, false, false, false, false};
    glutPostRedisplay();
}

void special(int k, int x, int y)
{
    switch(k)
    {
    case GLUT_KEY_UP:
        globalVolume += (1 / 100.0f);
            if(globalVolume > 1.0f) globalVolume = 1.0f;
            if(globalVolume < 0.0f) globalVolume = 0.0f;
            globalVolumeChanged = true;
            std::cout << "Global Volume : " << globalVolume << endl;
        break;

    case GLUT_KEY_DOWN:
        globalVolume -= (1 / 100.0f);
            if(globalVolume > 1.0f) globalVolume = 1.0f;
            if(globalVolume < 0.0f) globalVolume = 0.0f;
            globalVolumeChanged = true;
            std::cout << "Global Volume : " << globalVolume << endl;
        break;

    case GLUT_KEY_LEFT:
        if(play_start)
        {
            position -= 5000;
            snd->setPlayPosition((ik_u32)position);
        }
        break;

    case GLUT_KEY_RIGHT:
        if(play_start)
        {
            position += 5000;
            snd->setPlayPosition((ik_u32)position);
        }
        break;

    default:
        break;
    }
    glutPostRedisplay();
}

void key (unsigned char k, int x, int y)
{
    switch(k)
    {
    case 'd':
    case 'D':
        light_mode = (light_mode ? false : true);
        break;

    case ' ':
        if(snd->isFinished()) break;
        else if (snd->getIsPaused()) snd->setIsPaused(false);
        else if (!snd->getIsPaused()) snd->setIsPaused(true);
        break;

    case 27:
    case 'q':
    case 'Q':
        glutLeaveMainLoop;
        break;
    default :
        break;
    }

    glutPostRedisplay();
}


void displayTime()
{
    if(!play_start || play_end) return;
    ik_u32 time = snd->getPlayPosition();
    int tmp = time / 1000;
    hour = tmp / 3600;
    tmp = tmp % 3600;
    minute = tmp / 60;
    second = tmp % 60;

    time = snd->getPlayLength();
    tmp = time / 1000;
    int phour = tmp / 3600;
    tmp = tmp % 3600;
    int pminute = tmp / 60;
    int psecond = tmp % 60;

    printString(20, wnd_height - 20, 15, "%sElapsed time: %d:%d:%d    Total time: %d:%d:%d", (snd->getIsPaused() ? "PAUSED    " : ""), hour, minute, second, phour, pminute, psecond);
}

void disp()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, (light_mode ? texture_light : texture_dark));
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);

    // Background
    glPushMatrix();
    glBegin(GL_QUADS);
    aiTexCoord2f(0,   0);
    aiVertex2f(0,0);
    aiTexCoord2f(800, 0);
    aiVertex2f(wnd_width, 0);
    aiTexCoord2f(800, 800);
    aiVertex2f(wnd_width, wnd_height - 100);
    aiTexCoord2f(0,   800);
    aiVertex2f(0, wnd_height - 100);
    glEnd();
    glPopMatrix();

    // Logo
    float lx1, lx2, ly1, ly2;
    lx1 = (wnd_width / 2) - 100;
    lx2 = (wnd_width / 2) + 100;
    ly1 = (wnd_height / 2) - 100;
    ly2 = (wnd_height / 2) + 100;

    glBegin(GL_QUADS);
    aiTexCoord2f(800,   0);
    aiVertex2f(lx1,ly1);
    aiTexCoord2f(1000, 0);
    aiVertex2f(lx2, ly1);
    aiTexCoord2f(1000, 200);
    aiVertex2f(lx2, ly2);
    aiTexCoord2f(800,   200);
    aiVertex2f(lx1, ly2);
    glEnd();

    // Ribbon
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    if(light_mode)
        glColor3f(0.0f, 0.9f, 0.9f);
    else
        glColor3f(0.2f, 0.2f, 0.2f);
    aiVertex2f(0, wnd_height);
    aiVertex2f(wnd_width, wnd_height);
    aiVertex2f(wnd_width, wnd_height - 100);
    aiVertex2f(0, wnd_height - 100);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();

    // Music slider
    if(play_start && !play_end)
    {
        glPushMatrix();
        (light_mode ? glColor3f(0.0f, 0.5f, 1.0f) : glColor3f(1.0f, 1.0f, 1.0f));
        glBegin(GL_QUADS);
        aiVertex2f(30.0f, wnd_height - 100);
        aiVertex2f(30 + ((wnd_width - 60) * position/length), wnd_height - 100);
        aiVertex2f(30 + ((wnd_width - 60) * position/length), wnd_height - 80);
        aiVertex2f(30.0f, wnd_height - 80);
        glEnd();
        glColor3f(1.0f, 1.0f, 1.0f);
        glPopMatrix();
    }

    // Control buttons
    ctrlButton(CBPLAY, (CBWIDTH * 0) + (20 * 1), wnd_height - 75);
    ctrlButton(CBPAUSE, (CBWIDTH * 1) + (20 * 2), wnd_height - 75);
    ctrlButton(CBSTOP, (CBWIDTH * 2) + (20 * 3), wnd_height - 75);
    ctrlButton(CBPLAYLIST, (CBWIDTH * 3) + (20 * 4), wnd_height - 75);
    ctrlButton(CBFULLSCREEN, (CBWIDTH * 4) + (20 * 5), wnd_height - 75);
    ctrlButton(CBVOLUME, wnd_width - (CBWIDTH) - (20 *1), wnd_height - 75);

    // Button highlighting
    glDisable(GL_TEXTURE_2D);

    if(highlightableButton.play)       highlightCtrlButton((CBWIDTH * 0) + (20 * 1), wnd_height - 75);
    if(highlightableButton.pause)      highlightCtrlButton((CBWIDTH * 1) + (20 * 2), wnd_height - 75);
    if(highlightableButton.stop)       highlightCtrlButton((CBWIDTH * 2) + (20 * 3), wnd_height - 75);
    if(highlightableButton.playlist)   highlightCtrlButton((CBWIDTH * 3) + (20 * 4), wnd_height - 75);
    if(highlightableButton.fullscreen) highlightCtrlButton((CBWIDTH * 4) + (20 * 5), wnd_height - 75);
    if(highlightableButton.volume)     highlightCtrlButton(wnd_width - (CBWIDTH) - (20 *1), wnd_height - 75);

    glEnable(GL_TEXTURE_2D);

    // Play time
    if(play_start && !play_end)
    {
        glPushMatrix();
        (light_mode ? glColor3f(0.0f, 0.0f, 0.0f) : glColor3f(1.0f, 1.0f, 1.0f));
        displayTime();
        glPopMatrix();
    }

    // Volume
    glPushMatrix();
    (light_mode ? glColor3f(0.0f, 0.0f, 0.0f) : glColor3f(1.0f, 1.0f, 1.0f));
    printString(wnd_width - (CBWIDTH) - (20 *1), wnd_height - 20, 15, "%.6g", round((globalVolume * 100)));
    glPopMatrix();
    glutSwapBuffers();
}

void idle()
{
    cout << endl << 1;
    wnd_width = glutGet(GLUT_WINDOW_WIDTH);
    wnd_height = glutGet(GLUT_WINDOW_HEIGHT);
    if(play_start) position = snd->getPlayPosition();

    glutPostRedisplay();
    player_ready = true;

    if(player_ready && got_filename && !play_start)
    {
        engine = irrklang::createIrrKlangDevice();
        if(engine) cout << "success" << endl;
        snd = engine->play2D(mediafilename, false, false, true);
        if(snd) snd->setVolume(0.5f);
        if(snd) cout << "success" << endl;
        play_start = true;
        length = snd->getPlayLength();
    }

    if(snd && snd->isFinished()) play_end = true;

    if(snd && globalVolumeChanged)
    {
        snd->setVolume(globalVolume);
        globalVolumeChanged = false;
    }

}

int loadTexture()
{
    int n = 4;

    texture_light = SOIL_load_OGL_texture("buttons\\light\\resource.png", 4, texture_light, SOIL_FLAG_DDS_LOAD_DIRECT | SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_INVERT_Y);
    texture_dark  = SOIL_load_OGL_texture("buttons\\dark\\resource.png", 4, texture_dark, SOIL_FLAG_DDS_LOAD_DIRECT | SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_INVERT_Y);


    cout << "---------------------------------\n";
    cout << "SOIL image load result           |\n";
    cout << "---------------------------------\n";
    cout << "Image 1  : " << (texture_light ? "Successful" : "Unsuccessful") << "\n";
    cout << "Image 2  : " << (texture_dark  ? "Successful" : "Unsuccessful") << "\n";
    cout << "Channels : " << n << " " << ((n == 4) ? "@RGBA" : ((n == 3) ? "@RGB" : "@R|G|B")) << "\n";
    cout << "---------------------------------+\n";

}

void reshape(int x, int y)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, x, y);
    glOrtho(-x/2, x/2, -y/2, y/2, -1, 1);

    glutPostRedisplay();
}

int main (int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    mainWindow = glutCreateWindow("AI MUSICVERSE");
    string temp, file_extension;

    if(argv[1])
    {
        temp = argv[1];
        file_extension = string(temp, temp.length() - 4, 4);
        if(file_extension == ".mp3" || file_extension == ".wav" || file_extension == ".ogg")
        {
            mediafilename = argv[1];
            cout << "Media file           : " << mediafilename << endl;
            cout << "Media file extension : " << file_extension << endl;
            got_filename = true;
        }
        else
        {
            MessageBox(NULL, "Cannot open the input file. File format or extension not recognized!", "Error", MB_ICONERROR | MB_OK);
        }
    }
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA);
    glAlphaFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenTextures(1, &texture_light);
    glGenTextures(1, &texture_dark);
    glGenTextures(1, &logo_light);
    glGenTextures(1, &logo_dark);

    loadTexture();

    /*-----------------------[ light texture ]-----------------------*/
    glBindTexture(GL_TEXTURE_2D, texture_light);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /*-----------------------[ dark texture ]------------------------*/
    glBindTexture(GL_TEXTURE_2D, texture_dark);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /*-----------------------[ light logo ]-----------------------*/
    glBindTexture(GL_TEXTURE_2D, logo_light);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /*-----------------------[ dark logo ]------------------------*/
    glBindTexture(GL_TEXTURE_2D, logo_dark);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /*---------------------------------------------------------------*/

    glBindTexture(GL_TEXTURE_2D, 0);

    glutPassiveMotionFunc(motion);
    glutMouseFunc(mouse);
    glutMouseWheelFunc(wheel);
    glutKeyboardFunc(key);
    glutDisplayFunc(disp);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);

    if(!(font = dtx_open_font("C:/Windows/Fonts/segoeui.ttf", 15))) cout << "!Font";
    dtx_use_font(font, 15);

    glutMainLoop();

    snd->drop();
    engine->drop();

    return 0;
}

