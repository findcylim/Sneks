#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H
#pragma once

#include <cstdio>
#include <cstdlib>
#include "fmod.h"
#include "fmod_errors.h"

struct Audio;

class Sound
{
public:
	Audio* FX;

	Sound();
	/* Error-checking*/
	void fmodErrorCheck(FMOD_RESULT result);
	/* FMOD initialisation */
	void initialise();
	/* FMOD sound/channel/system creation */
	void createBGM(char* filename);
	void create(char* filename);
	/* Playing */
	void play();
	/* Update functions */
	void update();
	/* Pauses the sound */
	void pause(FMOD_BOOL pause);
	/* Unload sound */
	void unload();
	/* Getter/setter functions */
	bool checkPlaying();
	/* Unload from memory */
	void release();
};


/* Initialisation */
void Audio_Init();
/* Cleanup */
void Audio_Cleanup();

#endif