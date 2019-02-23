#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H
#pragma once

#include <cstdio>
#include <cstdlib>
#include "../ECS/System.h"
#include "../ECS/EntityManager.h"
#include "../Audio/fmod.h"
#include "../Audio/fmod_errors.h"
#include "../Utility/GameStateManager.h"

class Sound
{
	FMOD_BOOL	soundOn;		/* is sound on? */
	FMOD_BOOL	canPlaySound;	/* is it possible to play sound? */
	char*		currentSound;	/* currently played sound */

	/* FMOD-specific stuff */
	FMOD_RESULT		result;		/* allows error checking for FMOD functions */
	FMOD_SYSTEM		*system;	/* the system where the sound will be using */
	FMOD_SOUND		*fmodSound;	/* holding the actual sound */
	FMOD_CHANNEL	*channel;	/* the channel where the sound will be playing from */
public:
	Sound();
	/* Error-checking*/
	void fmodErrorCheck(FMOD_RESULT result);
	/* FMOD initialisation */
	void initialise();
	/* FMOD sound/channel/system creation */
	void createBGM(const char* filename);
	void create(const char* filename);
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
	/* Getter/Setter */
	FMOD_SYSTEM	*getSystem();
	FMOD_SOUND *getFMODSound();
};

class AudioSystem final : public BaseSystem // Add event listeners here
, public EventListener<Events::EV_PLAYER_COLLISION>
{
	Sound BGM;
	Sound SFX;
public:
	AudioSystem(EntityManager* entityManagerPtr);
	~AudioSystem();
	void Initialize();
	void receive(const Events::EV_PLAYER_COLLISION& eventData) override;
	void Update(float dt) override;
};


#endif