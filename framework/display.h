#include <stdbool.h>
#include <SDL2/SDL.h>

int WINDOW_WIDTH, WINDOW_HEIGHT;

int initDisplay(char*, int, int);
void displayClear(void);
void displayPresent(void);
void destroyDisplay(void);
void displayFrameStart(void);
void displayFrameEnd(void);
void displayUpdateTime(void);
void displayApplyResolution(void);
int displayGetWindowWidth(void);
int displayGetWindowHeight(void);
void displayResizeWindow(void);
bool displayGetFullscreen(void);
void displaySetFullscreen(void);
void displaySetWindowed(void);
void displaySetWindowBorder(bool);
void displaySetWindowWidth(int);
void displaySetWindowHeight(int);
void displaySetRenderWidth(int);
void displaySetRenderHeight(int);
void diplaySetWindowFlag(int);
SDL_Window* displayGetWindow(void);
SDL_Surface* displayGetWindowSurface(void);
SDL_Renderer* displayGetRenderer(void);
double displayGetAccumulator(void);
double displayGetTimestep(void);
double displayGetAlpha(void);
int displayGetRenderWidth(void);
int displayGetRenderHeight(void);
