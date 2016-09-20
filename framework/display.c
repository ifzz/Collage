#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "numbers.h"
#include "display.h"
#include "drawing.h"


SDL_Window *WINDOW = NULL;
SDL_Renderer *RENDERER = NULL;
int IMAGE_INIT_FLAGS = IMG_INIT_JPG | IMG_INIT_PNG;
int RENDER_WIDTH = 512;
int RENDER_HEIGHT = 448;
int WINDOW_WIDTH = 1024;
int WINDOW_HEIGHT = 768;
float FRAMES = 0;
float FRAME_TIMER = 0.;
float FPS_CAP = -1; //1000 / 400.;
bool FULLSCREEN = false;


int initDisplay(char *windowTitle, int windowWidth, int windowHeight) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER)
			!= 0) {
		printf("Couldn't init SDL.\n");
		
		return 0;
	}
	
	WINDOW_WIDTH = windowWidth;
	WINDOW_HEIGHT = windowHeight;
	WINDOW = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT,
			SDL_WINDOW_SHOWN);
	RENDERER = SDL_CreateRenderer(WINDOW, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	FRAME_TIMER = SDL_GetTicks();
	
	if (displayGetFullscreen())
		SDL_SetWindowFullscreen(WINDOW, SDL_WINDOW_FULLSCREEN);
	
	SDL_SetRenderDrawBlendMode(RENDERER, SDL_BLENDMODE_BLEND);
	
	if (WINDOW == NULL){
		printf("Couldn't create a window.\n");
		
		SDL_Quit();
		
		return 0;
	}
	
	if (RENDERER == NULL){
		printf("Couldn't create a renderer.\n");
		
		SDL_DestroyWindow(WINDOW);
		SDL_Quit();
		
		return 0;
	}
	
	if (IMG_Init(IMAGE_INIT_FLAGS) != IMAGE_INIT_FLAGS) {
		printf("Couldn't init SDL_image's JPG or PNG support.\n");
		
		SDL_Quit();
		
		return 0;
	}
	
	if (TTF_Init() != 0) {
		printf("Couldn't init SDL_ttf.\n");
		
		SDL_Quit();
		
		return 0;
	}
	
	startFonts();
	displayClear();
	
	return 1;
}

void displayClear() {
	SDL_RenderClear(RENDERER);
}

float getCameraZoom() {
	return 1.;
}

void displayPresent() {
	SDL_Rect displayRect;

	int viewportWidth = RENDER_WIDTH;//getViewportWidth();
	int viewportHeight = RENDER_HEIGHT;//getViewportHeight();
	double renderWidthScale = WINDOW_WIDTH / (double)viewportWidth;
	double renderHeightScale = WINDOW_HEIGHT / (double)viewportHeight;

	displayRect.w = viewportWidth;
	displayRect.h = viewportHeight;
	displayRect.x = 0;
	displayRect.y = 0;

	SDL_SetRenderDrawColor(RENDERER, 15, 10, 10, 255);
	SDL_RenderSetViewport(RENDERER, &displayRect);
	/*SDL_RenderSetScale(RENDERER, getCameraZoom() * renderWidthScale,*/
			/*getCameraZoom() * renderHeightScale);*/
	
	SDL_RenderSetScale(RENDERER, renderWidthScale, renderHeightScale);

	SDL_RenderPresent(RENDERER);

	/*++ FRAMES;*/

	/*if (FPS_CAP > -1) {*/
		/*float ticks = (SDL_GetTicks() - FRAME_TIMER) / 1000.;*/

		/*if (ticks < FPS_CAP) {*/
			/*SDL_Delay(FPS_CAP - ticks);*/
		/*}*/
	/*}*/

	/*if (SDL_GetTicks() - FRAME_TIMER >= 1000.) {*/
		/*printf("FPS=%f\n", FRAMES);*/

		/*FRAMES = 0;*/
		/*FRAME_TIMER = SDL_GetTicks();*/
	/*}*/
}

int displayGetWindowWidth() {
	return WINDOW_WIDTH;
}

int displayGetWindowHeight() {
	return WINDOW_HEIGHT;
}

void displaySetWindowWidth(int width) {
	WINDOW_WIDTH = width;
}

void displaySetWindowHeight(int height) {
	WINDOW_HEIGHT = height;
}

void displaySetRenderWidth(int width) {
	RENDER_WIDTH = width;
}

void displaySetRenderHeight(int height) {
	RENDER_HEIGHT = height;
}

bool displayGetFullscreen() {
	return FULLSCREEN;
}

void displaySetFullscreen() {
	FULLSCREEN = true;
}

void displaySetWindowed() {
	FULLSCREEN = false;
}

void displayApplyResolution() {
	SDL_DisplayMode displayMode;
	
	displayMode.format = SDL_PIXELFORMAT_ARGB8888;
	displayMode.w = WINDOW_WIDTH;
	displayMode.h = WINDOW_HEIGHT;
	displayMode.refresh_rate = 0;
	displayMode.driverdata = NULL;
	
	double ratio = displayMode.w / (double) displayMode.h;
	
	if (ratio >= 1.7) { // 16:9
		RENDER_WIDTH = 1280;
		RENDER_HEIGHT = 720;
	} else if (ratio >= 1.6) { // 16:10
		RENDER_WIDTH = 1280;
		RENDER_HEIGHT = 800;
	} else {
		RENDER_WIDTH = 1280;
		RENDER_HEIGHT = 960;
	}
	
	SDL_SetWindowDisplayMode(WINDOW, &displayMode);
	SDL_SetWindowSize(WINDOW, displayMode.w, displayMode.h);
	
	if (FULLSCREEN)
		SDL_SetWindowFullscreen(WINDOW, SDL_WINDOW_FULLSCREEN);
	else
		SDL_SetWindowFullscreen(WINDOW, 0);
}

void displayFrameStart() {
}

void displayFrameEnd() {
}

void displayUpdateTime() {
}

void destroyDisplay() {
	SDL_Quit();
}

int displayGetRenderWidth() {
	return RENDER_WIDTH;
}

int displayGetRenderHeight() {
	return RENDER_HEIGHT;
}

SDL_Window* displayGetWindow() {
	return WINDOW;
}

SDL_Surface* displayGetWindowSurface() {
	printf("**FATAL** Don't use this...\n");
	
	return SDL_GetWindowSurface(WINDOW);
}

SDL_Renderer* displayGetRenderer() {
	return RENDERER;
}

