#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H
#pragma once


#include "../ECS/System.h"
#include "../ECS/EntityManager.h"
#include <fmod.h>
#include <fmod_errors.h>
#include "../Utility/GameStateManager.h"

class Sound
{
	FMOD_BOOL	soundOn;		/* is sound on? */
	FMOD_BOOL	canPlaySound;	/* is it possible to play sound? */
	char*			currentSound;	/* currently played sound */

	/* FMOD-specific stuff */
	FMOD_SYSTEM		*system;	/* the system where the sound will be using */
	FMOD_SOUND		*fmodSound;	/* holding the actual sound */
	FMOD_CHANNEL	*channel;	/* the channel where the sound will be playing from */
	FMOD_RESULT		result;		/* allows error checking for FMOD functions */
	char m_c_PlayCap	= 0;
	float m_f_Timer		= 0.0f;
public:
	float m_f_PlayTimer = 0.0f;
	bool m_b_IsPlaying = false;
	Sound();
	/* Error-checking*/
	void FmodErrorCheck(FMOD_RESULT result);
	/* FMOD initialisation */
	void Initialize();
	/* FMOD sound/channel/system creation */
	void CreateBGM(const char* filename);
	void Create(const char* filename);
	/* Playing */
	void Play(float volume = 0.33f);
	/* Update functions */
	void Update();
	/* Pauses the sound */
	void Pause(FMOD_BOOL pause);
	/* Unload sound */
	void Unload();
	/* Getter/setter functions */
	bool CheckPlaying();
	/* Unload from memory */
	void Release();
	/* Getter/Setter */
	FMOD_SYSTEM	*GetSystem();
	FMOD_SOUND *GetFmodSound();
};

class AudioSystem final : public BaseSystem // Add event listeners here
, public EventListener<Events::EV_PLAYER_COLLISION>
, public EventListener<Events::EV_GAME_STATE_CHANGED>
{
	Sound m_o_MainMenuMusic;
	Sound m_o_IntroBattleMusic;
	Sound m_o_BattleLoopMusic;
	Sound m_o_HitSound;
	Sound m_o_PowerUpSound;
	Sound m_o_ExplosionSound;

	//std::map<const char*, Sound> m_x_SoundMap;
public:
	AudioSystem(EntityManager* entityManagerPtr);
	~AudioSystem();
	void Initialize();
	void Receive(const Events::EV_PLAYER_COLLISION& eventData) override;
	void Receive(const Events::EV_GAME_STATE_CHANGED& eventData) override;
	void Update(float dt) override;
};


#endif