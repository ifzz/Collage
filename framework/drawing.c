#include <SDL2/SDL.h>


void drawPixel(Uint32 *pixels, int x, int y, int w, Uint32 pixel){
    pixels[(y * w) + x] = pixel;
}

SDL_Texture* textureGenerateTile(SDL_Renderer *renderer,
		SDL_Texture *sourceTexture,
		int width, int height) {
	int sourceTextureWidth, sourceTextureHeight;
	SDL_Texture *targetTexture = SDL_CreateTexture(renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_TARGET,
			width,
			height);

	SDL_SetRenderTarget(renderer, targetTexture); 
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_QueryTexture(sourceTexture, NULL, NULL, &sourceTextureWidth,
			&sourceTextureHeight);
	SDL_SetTextureBlendMode(sourceTexture, SDL_BLENDMODE_BLEND);
	SDL_Rect srcRect = {0, 0, sourceTextureWidth, sourceTextureHeight};

	for (int w = 0; w < width; w += sourceTextureWidth) {
		for (int h = 0; h < height; h += sourceTextureHeight) {
			SDL_Rect dstRect = {w, h, sourceTextureWidth, sourceTextureHeight};
			SDL_RenderCopy(renderer, sourceTexture, &srcRect, &dstRect);
		}
	}

	SDL_SetRenderTarget(renderer, NULL); 
	
	return targetTexture;
}
