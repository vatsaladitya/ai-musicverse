# AI Musicverse
A simple music player written in C++ and OpenGL.
This is a FREEWARE PROGRAM. Use it however you want.

## Keyboard and mouse controls:

### General controls
1. Skip forward by 5 seconds: Right arrow key
2. Skip backward by 5 seconds: Left arrow key
3. Volume up: Up arrow key
4. Volume down: Down arrow key
5. Play/Pause : Space key

### Special controls
1. To change Volume using mouse whell, first move the mouse over the volume button
2. To switch between light and dark modes: press 'D' key

** The Playlist button doesn't work **

## Bugs and drawbacks
1. You cannot drag and drop file in the player
2. The player doesn't have menu bar
3. You cannot skip the music using mouse
4. The interface is primitive
5. It uses OpenGL for drawing and may be memory intensive
6. USE IT AT YOUR OWN RISK

With regards,
Vatsaladiya

## For developers
### Dependencies
1. OpenGL v1.1
2. Glu
3. Freeglut (http://freeglut.sourceforge.net/)
2. IrrKlang Library (dll files are included along with the source code. Credit: IrrKlang Team https://www.ambiera.com/irrklang/)
3. libdrawtext (https://github.com/jtsiomb/libdrawtext)

This project was built in Code::Blocks IDE with MinGW compilers and it's project settings may need to be tweaked if you are building it on your machine with different specs. Moreover, this project is focused on Windows only. Paths and some libraries are Windows specific. So, you can't directly build it on Linux without tweaking it a bit.
