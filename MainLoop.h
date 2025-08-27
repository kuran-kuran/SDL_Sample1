#ifndef MAINLOOP_HPP
#define MAINLOOP_HPP 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>

// VGAÉTÉCÉYÇÃâÊñ ÇçÏê¨
static const int WIDTH = 640;
static const int HEIGHT = 480;
static const unsigned int BUTTON_UP = 0x1;
static const unsigned int BUTTON_DOWN = 0x2;
static const unsigned int BUTTON_LEFT = 0x4;
static const unsigned int BUTTON_RIGHT = 0x8;

void MainLoop_Setup(void);
bool MainLoop_Loop(void);
void MainLoop_NoWaitLoop(void);
void MainLoop_Finalize(void);
void MainLoop_SetKey(unsigned int key);
void MainLoop_ResetKey(unsigned int key);

#endif
