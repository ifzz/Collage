#include <stdbool.h>
#include "SDL2/SDL_mixer.h"
#include "../ecs/entities.h"

void setGlobalVolume(float);
void startSound(void);
void shutdownSound(void);
Mix_Chunk *getSoundWithName(char*);
int playMusic(char*);
void stopMusic(void);
void stopAllSounds(void);
int playSound(char*);
int playSoundWithVolume(char*, double);
int playSoundWithLoops(char*, int);
int playSoundWithCallback(char*, int, World*, unsigned int, void (*callback)(World*, unsigned int));
bool isSoundPlayedByEntity(char*, World*, unsigned int);
void stopSoundPlayedByEntity(char*, World*, unsigned int);

