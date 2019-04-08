/* Start Header****************************************************************/
/*!
\file AudioSystem.h
\author Primary Author : Javier Foo, javier.foo
\par email: javier.foo\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : Javier - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H
#pragma once


#include "../ECS/System.h"
#include "../ECS/EntityManager.h"
#include <fmod.h>
#include <fmod_errors.h>
#include "../Utility/GameStateManager.h"

struct Stream
{
	FMOD_SYSTEM		*system;	/* the system where the sound will be using */
	FMOD_RESULT		result;		/* allows error checking for FMOD functions */
	/* Constructor */
	Stream();
	/* Error-checking*/
	void FmodErrorCheck(FMOD_RESULT result);
	/* FMOD initialisation */
	void Initialize();
	/* Release from system */
	void Release();
};

struct Sound
{
	FMOD_BOOL	soundOn;		/* is sound on? */
	FMOD_BOOL	canPlaySound;	/* is it possible to play sound? */
	char*		currentSound;	/* currently played sound */

	/* FMOD-specific */
	FMOD_SYSTEM		*system;	/* the system where the sound will be using */
	FMOD_RESULT		result;		/* allows error checking for FMOD functions */
	FMOD_SOUND		*fmodSound;	/* holding the actual sound */
	FMOD_CHANNEL	*channel;	/* the channel where the sound will be playing from */
public:
	/* Sound Statistics */
	float m_f_Timer = 0.0f;
	char m_c_PlayCounter = 0;
	float m_f_PlayTimer = 0.0f;
	char m_c_PlayCap = 0;
	bool m_b_IsPlaying = false;

	/* Constructor */
	Sound();
	/* Error-checking*/
	void FmodErrorCheck(FMOD_RESULT result);

	/* FMOD sound/channel/system creation */
	void CreateBGM(const char* filename, char counterCap, float playTimer, Stream* audioPtr);
	void Create(const char* filename, char counterCap, float playTimer, Stream* audioPtr);

	/* General Audio Functions */
	/* Play the sound */
	void Play(float volume = 0.33f);
	/* Update functions */
	void Update();
	/* Pauses the sound */
	void Pause(FMOD_BOOL pause);
	/* Unload sound */
	void Unload();
	/* Unload from memory */
	void Release();

	/* Getter/Setter */
	FMOD_SYSTEM	*GetSystem();
	FMOD_SOUND *GetFmodSound();
	float GetVolume();
	void SetVolume(float volume);
	void ResetSoundCounter();
	bool CheckPlaying();
};

class AudioSystem final : public BaseSystem // Add event listeners here
, public EventListener<Events::EV_PLAYER_COLLISION>
, public EventListener<Events::EV_GAME_STATE_CHANGED>
, public EventListener<Events::EV_POWERUP_PICKUP_HEALTH>
, public EventListener<Events::EV_POWERUP_PICKUP_SPEED>
, public EventListener<Events::EV_POWERUP_PICKUP_STARMODE>
, public EventListener<Events::EV_SPECIAL_SKILL_BOOST>
, public EventListener<Events::EV_SPECIAL_SKILL_FLIP>
, public EventListener<Events::EV_PAUSED_GAME>
{
	Stream BGM;
	Stream SFX;
	Sound m_o_MainMenuMusic;
	Sound m_o_IntroBattleMusic;
	Sound m_o_StarModeMusic;
	Sound m_o_HealthPickup;
	Sound m_o_SpeedPickup;
	Sound m_o_BattleLoopMusic;
	Sound m_o_HitSound;
	Sound m_o_ExplosionSound;
	Sound m_o_SpeedSpecialSound;
	Sound m_o_FlipSpecialSound;

	GameStateManager* m_po_GameStateManager;
	bool muted = false;
public:
	/* Constructor/Destructor */
	AudioSystem(GameStateManager* gameStateManager);
	~AudioSystem();

	/* Event Receivers */
	void Receive(const Events::EV_PLAYER_COLLISION& eventData) override;
	void Receive(const Events::EV_GAME_STATE_CHANGED& eventData) override;
	void Receive(const Events::EV_POWERUP_PICKUP_HEALTH& eventData) override;
	void Receive(const Events::EV_POWERUP_PICKUP_SPEED& eventData) override;
	void Receive(const Events::EV_POWERUP_PICKUP_STARMODE& eventData) override;
	void Receive(const Events::EV_SPECIAL_SKILL_BOOST& eventData) override;
	void Receive(const Events::EV_SPECIAL_SKILL_FLIP& eventData) override;
	void Receive(const Events::EV_PAUSED_GAME& eventData) override;

	/* General System Functions */
	void Initialize();
	void Update(float dt) override;

	/* Mute/Unmute */
	void ToggleMute();
};


#endif