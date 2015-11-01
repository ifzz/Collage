#include <stdbool.h>
#include <SDL2/SDL.h>

#define WINDOW_TITLE "testWorlds"

int WINDOW_WIDTH, WINDOW_HEIGHT;

int initDisplay(void);
void displayClear(void);
void displayPresent(void);
void destroyDisplay(void);
void displayFrameStart(void);
void displayFrameEnd(void);
void displayUpdateTime(void);
void displayApplyResolution(void);
int displayGetWindowWidth(void);
int displayGetWindowHeight(void);
bool displayGetFullscreen(void);
void displaySetFullscreen(void);
void displaySetWindowed(void);
void displaySetWindowWidth(int);
void displaySetWindowHeight(int);
void displaySetRenderWidth(int);
void displaySetRenderHeight(int);

SDL_Window* displayGetWindow(void);
SDL_Surface* displayGetWindowSurface(void);
SDL_Renderer* displayGetRenderer(void);
double displayGetAccumulator(void);
double displayGetTimestep(void);
double displayGetAlpha(void);
int displayGetRenderWidth(void);
int displayGetRenderHeight(void);
