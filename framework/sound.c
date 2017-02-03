#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <SDL2/SDL_mixer.h>
#include <assert.h>
#include "../ecs/entities.h"
#include "strings.h"
#include "list.h"


linkedList_t *SOUND_MANAGER = NULL;
bool PERFORM_SOUND_FILE_CHECK = false;

typedef struct soundManagerEntry {
	char *filename, *name;
	Mix_Chunk *sound;
} soundManagerEntry;

typedef struct soundCallbackManager {
	unsigned int entityId;
	World *world;
	void (*callback)(World*, unsigned int);
	char *soundName;
	int loops;
	bool active;
} soundCallbackManager;

int SOUND_CHANNELS = 25;
double GLOBAL_VOLUME = .75;
soundCallbackManager SOUND_CALLBACK_MANAGER[25];
Mix_Music *CURRENT_MUSIC_TRACK = NULL;


void setGlobalVolume(float volume) {
	GLOBAL_VOLUME = volume;
}

void addSound(char *filename, char *name) {
	soundManagerEntry *soundEntry = malloc(sizeof(soundManagerEntry));

	copyText(&soundEntry->filename, filename);
	copyText(&soundEntry->name, name);

	soundEntry->sound = Mix_LoadWAV(filename);

	addListItem(SOUND_MANAGER, soundEntry);

	printf("Added sound: %s\n", name);
}

Mix_Chunk* getSoundWithName(char *name) {
	listItem_t *soundList = SOUND_MANAGER->head;

	if (soundList == NULL)
		return NULL;

	while (soundList) {
		soundManagerEntry *item = soundList->item;

		if (!strcmp(item->name, name)) {
			return item->sound;
		}

		soundList = soundList->next;
	}

	if (PERFORM_SOUND_FILE_CHECK)
		printf("Sound not found: %s\n", name);

	return NULL;
}

soundManagerEntry* getSoundStructWithName(char *name) {
	listItem_t *soundList = SOUND_MANAGER->head;

	if (soundList == NULL)
		return NULL;

	while (soundList) {
		soundManagerEntry *item = soundList->item;

		if (!strcmp(item->name, name))
			return item;

		soundList = soundList->next;
	}

	return NULL;
}

void loadSound(char *filename, char *name) {
	Mix_Chunk *sound = getSoundWithName(name);

	if (sound != NULL) {
		printf("ERROR: Sound already loaded: %s\n", name);

		return;
	}

	addSound(filename, name);
}

int playMusic(char *filename) {
	Mix_Music *music = Mix_LoadMUS(filename);

	if (CURRENT_MUSIC_TRACK)
		Mix_FreeMusic(CURRENT_MUSIC_TRACK);

	if (!music) {
		printf("ERROR: Could not play music: %s\n", Mix_GetError());

		return 0;
	}

	Mix_VolumeMusic(round(MIX_MAX_VOLUME * (GLOBAL_VOLUME * .9)));
	/*Mix_PlayMusic(music, -1);*/

	printf("Playing song: %s\n", filename);

	CURRENT_MUSIC_TRACK = music;

	return 1;
}

void stopAllSounds() {
	for (int c = 0; c < SOUND_CHANNELS; ++ c) {
		soundCallbackManager *sCM = &SOUND_CALLBACK_MANAGER[c];

		if (!sCM->active)
			continue;

		sCM->active = false;
		Mix_HaltChannel(c);
	}

	Mix_HaltChannel(-1);
}

void stopMusic() {
	Mix_HaltMusic();
}

int playSound(char *name) {
	//return 0;
	int soundChannel = Mix_PlayChannel(-1, getSoundWithName(name), 0);

	if (soundChannel == -1) {
		//printf("Error playing sound: No free channels\n");

		//printf("%s\n", Mix_GetError());
		return -1;
	} else {
		Mix_Volume(soundChannel, round(MIX_MAX_VOLUME * GLOBAL_VOLUME));
	}

	return soundChannel;
}

int playSoundWithVolume(char *name, double volume) {
	int soundChannel = playSound(name);

	if (soundChannel == -1)
		return -1;

	Mix_Volume(soundChannel, round(MIX_MAX_VOLUME * (volume * GLOBAL_VOLUME)));

	return soundChannel;
}

int playSoundWithLoops(char *name, int loops) {
	int soundChannel = Mix_PlayChannel(-1, getSoundWithName(name), loops);

	if (soundChannel == -1) {
		printf("Error playing sound: No free channels\n");

		return -1;
	}

	Mix_Volume(soundChannel, MIX_MAX_VOLUME * GLOBAL_VOLUME);

	return soundChannel;
}

void playSoundWithCallback(char *name, int loops, World *world, unsigned int entityId, void (*callback)(World*, unsigned int)) {
	int soundChannel = playSoundWithLoops(name, loops);

	if (soundChannel == -1) {
		printf("\tCallback not set trying to play: %s\n", name);

		return;
	}

	soundCallbackManager *sCM = &SOUND_CALLBACK_MANAGER[soundChannel];

	if (sCM->active) {
		printf("Error playing sound with callback: Callback already set\n");

		return;
	}

	sCM->active = true;
	sCM->callback = callback;
	sCM->entityId = entityId;
	sCM->world = world;
	sCM->soundName = getSoundStructWithName(name)->name;
	sCM->loops = loops;

	if (loops >= 0)
		sCM->loops += 1;
}

void stopSoundPlayedByEntity(char *name, World *world, unsigned int entityId) {
	for (int c = 0; c < SOUND_CHANNELS; ++ c) {
		soundCallbackManager *sCM = &SOUND_CALLBACK_MANAGER[c];

		if (!sCM->active)
			continue;

		if (sCM->entityId != entityId)
			continue;

		if (!strcmp(sCM->soundName, name)) {
			sCM->active = false;
			Mix_HaltChannel(c);

			return;
		}
	}
}

bool isSoundPlayedByEntity(char *name, World *world, unsigned int entityId) {
	for (int c = 0; c < SOUND_CHANNELS; ++ c) {
		soundCallbackManager *sCM = &SOUND_CALLBACK_MANAGER[c];

		if (!sCM->active)
			continue;

		if (sCM->entityId != entityId)
			continue;

		if (!strcmp(sCM->soundName, name)) {
			return true;
		}
	}

	return false;
}

void soundCallbackManagerCallback(int channel) {
	assert(channel > -1);

	soundCallbackManager *sCM = &SOUND_CALLBACK_MANAGER[channel];

	if (!sCM->active)
		return;

	if (sCM->callback)
		sCM->callback(sCM->world, sCM->entityId);

	if (sCM->loops == -1) {
		printf("Infinite: %i\n", sCM->loops);

		return;
	}

	if (sCM->loops)
		-- sCM->loops;

	if (!sCM->loops)
		sCM->active = false;
}

void deleteSoundFromList(void *sound) {
	Mix_FreeChunk(sound);
}

void startSound() {
	int flags = MIX_INIT_MP3;

	SOUND_MANAGER = createLinkedList(&deleteSoundFromList);

	if ((Mix_Init(flags) & flags) != flags) {
		printf("Mix_Init: %s\n", Mix_GetError());
	}

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}

	Mix_AllocateChannels(SOUND_CHANNELS);
	Mix_ChannelFinished(soundCallbackManagerCallback);

	loadSound("data/sound/player_spawn.wav", "player spawn");
	loadSound("data/sound/player_spawn_end.wav", "player spawn end");
	loadSound("data/sound/hit.wav", "hit");
	loadSound("data/sound/hit_2.wav", "hit 2");
	loadSound("data/sound/mine.wav", "mine");
	loadSound("data/sound/jetpack.wav", "jetpack");
	loadSound("data/sound/jetpack_full.wav", "jetpack full");
	loadSound("data/sound/shoot.wav", "weak shot");
	loadSound("data/sound/shoot_2.wav", "shot");
	loadSound("data/sound/storm.wav", "storm");
	loadSound("data/sound/voice_blood.wav", "voice taunt");
	loadSound("data/sound/voice_blood_2.wav", "voice taunt 2");
	loadSound("data/sound/voice_burn.wav", "voice taunt 3");
	loadSound("data/sound/voice_crushed.wav", "voice taunt 4");
	loadSound("data/sound/voice_death_is_quick.wav", "voice taunt 5");
	loadSound("data/sound/voice_fuck_you.wav", "voice taunt 6");
	loadSound("data/sound/voice_gunning.wav", "voice taunt 7");
	loadSound("data/sound/voice_more_death.wav", "voice taunt 8");
	loadSound("data/sound/voice_youre_done.wav", "voice taunt 9");
	loadSound("data/sound/teleport_out.wav", "teleport out");
	loadSound("data/sound/em_intro.wav", "game intro 1");
	loadSound("data/sound/em_outtro.wav", "game intro 2");
	loadSound("data/sound/em_intro_2.wav", "game intro 3");
	loadSound("data/sound/em_intro_3.wav", "game intro 4");
	loadSound("data/sound/em_intro_4.wav", "game intro 5");
	loadSound("data/sound/robot_death.wav", "death");
	loadSound("data/sound/human_death_1.wav", "human death 1");
	loadSound("data/sound/human_death_2.wav", "human death 2");
	loadSound("data/sound/human_death_3.wav", "human death 3");
	loadSound("data/sound/human_death_4_long.wav", "human death 4");
	loadSound("data/sound/human_death_5_long.wav", "human death 5");
	loadSound("data/sound/human_death_6.wav", "human death 6");
	loadSound("data/sound/human_death_7.wav", "human death 7");
	loadSound("data/sound/explosion_1.wav", "explosion 1");
	loadSound("data/sound/explosion_2.wav", "explosion 2");
	loadSound("data/sound/explosion_far_away.wav", "explosion far");
	loadSound("data/sound/deflector_shield_hit.wav", "deflector shield hit");
	loadSound("data/sound/deflector_shield_idle.wav", "deflector shield idle");
	loadSound("data/sound/ship_lower.wav", "ship lower");
	loadSound("data/sound/missile_hit.wav", "missile hit");
	loadSound("data/sound/missile_kill.wav", "missile kill");
	loadSound("data/sound/rocket_launch.wav", "rocket launch");
	loadSound("data/sound/rocket_launch_2.wav", "rocket launch 2");
	loadSound("data/sound/land.wav", "player land");
	loadSound("data/sound/mortar_volley.wav", "mortar volley");
	loadSound("data/sound/shoot_missile.wav", "missile shoot");
	loadSound("data/sound/shoot_shotgun.wav", "shotgun shoot");
	loadSound("data/sound/surgical_laser.wav", "surgical laser fire");
	loadSound("data/sound/surgical_laser_hit.wav", "surgical laser hit");
	loadSound("data/sound/lightning.wav", "lightning");
	loadSound("data/sound/lightning_far.wav", "lightning far");
	loadSound("data/sound/gravity_well.wav", "gravity well");
	loadSound("data/sound/sniper_missile.wav", "sniper missile");
	loadSound("data/sound/fire.wav", "fire");
	loadSound("data/sound/warning.wav", "warning");
	loadSound("data/sound/ambient_career_menu_idle.wav", "career menu idle");
	loadSound("data/sound/ambient_career_menu_load.wav", "career menu load");
	loadSound("data/sound/menu_error.wav", "career menu error");
	loadSound("data/sound/menu_select_1.wav", "career menu select 1");
	loadSound("data/sound/menu_back_1.wav", "career menu back 1");
	loadSound("data/sound/game_boot.wav", "boot jingle");
	loadSound("data/sound/talk_1.wav", "talk 1");
	loadSound("data/sound/barrier_build.wav", "barrier build");

	//printf("Volume set to: %i\n", 10);
	//Mix_Volume(-1, 10);

	PERFORM_SOUND_FILE_CHECK = false;
}

void shutdownSound() {
	listItem_t *soundList = SOUND_MANAGER->head;

	Mix_HaltChannel(-1);

	while (soundList) {
		soundManagerEntry *item = soundList->item;

		if (item->sound)
			Mix_FreeChunk(item->sound);

		item->sound = NULL;
		soundList = soundList->next;
	}

	if (CURRENT_MUSIC_TRACK)
		Mix_FreeMusic(CURRENT_MUSIC_TRACK);
	//

	//while(Mix_Init(0))
	//

	Mix_CloseAudio();
	Mix_Quit();
}
