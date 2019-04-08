/* Start Header****************************************************************/
/*!
\file AudioSystem.cpp
\author Primary Author : Javier Foo, javier.foo
\par email: javier.foo\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains code that listens to events and react by playing 
sounds.

\par Contribution : CY     - 13.07% (Adding of sounds)
\par Contribution : Javier - 70.59% (Sound & Audio System structure)
\par Contribution : Adam   - 16.34% (State and Audio (Event)Management)

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "AudioSystem.h"
#include "../Utility/GameStateManager.h"
#include "Menus/ConfirmationScreenSystem.h"

/***********************************************************
				 Stream Functions
***********************************************************/

/* Constructor */
Stream::Stream(): system{0}, result{FMOD_OK}
{
}

/* Error check */
void Stream::FmodErrorCheck(FMOD_RESULT resultCheck)
{
	/* Throw an error if FMOD finds something wrong */
	if (resultCheck != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", resultCheck, FMOD_ErrorString(resultCheck));
		exit(-1);
	}
}

/* General System Functions */
void Stream::Initialize()
{
	FMOD_System_Create(&system);
	FmodErrorCheck(result);
	FMOD_System_Init(system, 64, FMOD_INIT_NORMAL, 0);
	FmodErrorCheck(result);
}

void Stream::Release()
{
	FMOD_System_Release(system);
	FmodErrorCheck(result);
}

/***********************************************************
				 Sound Functions
***********************************************************/

/* Constructor */
Sound::Sound() : soundOn{ false }, canPlaySound{ true }, currentSound{ 0 }, result{ FMOD_OK }, 
system{ 0 }, fmodSound{ 0 }, channel{ 0 }
{
}

/* Error check */
void Sound::FmodErrorCheck(FMOD_RESULT resultCheck)
{
	/* Throw an error if FMOD finds something wrong */
	if (resultCheck != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", resultCheck, FMOD_ErrorString(resultCheck));
		exit(-1);
	}
}

/* Sound Creation */

/* For SFX */
void Sound::Create(const char* filename, char counterCap, float playTimer, Stream* audioPtr)
{
	system = audioPtr->system;
	m_c_PlayCap = counterCap;
	m_f_PlayTimer = playTimer;
	FMOD_System_CreateSound(system, filename, FMOD_LOOP_OFF | FMOD_CREATESTREAM, 0, &fmodSound);
	FmodErrorCheck(result);
	FMOD_System_PlaySound(system, fmodSound, 0, true, &channel);
	FmodErrorCheck(result);
}

/* For BGM */
void Sound::CreateBGM(const char*filename, char counterCap, float playTimer, Stream* audioPtr)
{
	system = audioPtr->system;
	m_c_PlayCap = counterCap;
	m_f_PlayTimer = playTimer;
	FMOD_System_CreateStream(system, filename, FMOD_LOOP_NORMAL | FMOD_CREATESTREAM, 0, &fmodSound);
	FmodErrorCheck(result);
	FMOD_Sound_SetMusicChannelVolume(fmodSound, 0, 0);
	FmodErrorCheck(result);
	FMOD_System_PlaySound(system, fmodSound, 0, true, &channel);
	FmodErrorCheck(result);
}

/* General Audio Functions */

void Sound::Play(float volume)
{
	if (m_c_PlayCounter < m_c_PlayCap)
	{
		soundOn = true;
		FMOD_System_PlaySound(system, fmodSound, 0, false, &channel);
		m_b_IsPlaying = true;
		FmodErrorCheck(result);
		FMOD_Channel_SetVolume(channel, volume);
		++m_c_PlayCounter;
		m_f_Timer = 0;
	}
}

void Sound::Update()
{
	FMOD_System_Update(system);
	FmodErrorCheck(result);
	float dt = static_cast<float>(AEFrameRateControllerGetFrameTime());
	if (m_f_Timer > m_f_PlayTimer)
	{
		m_f_Timer	= 0.0f;
		m_c_PlayCounter = 0;
	}
	if (m_b_IsPlaying)
	{
		m_f_Timer += dt;
	}
}

void Sound::Pause(FMOD_BOOL pause)
{
	FMOD_Channel_SetPaused(channel, pause);
	m_b_IsPlaying = false;
	FmodErrorCheck(result);
}

void Sound::Unload()
{
	currentSound = 0;
	soundOn = false;
	FMOD_Sound_Release(fmodSound);
	FmodErrorCheck(result);
}

void Sound::Release()
{
	FMOD_Sound_Release(fmodSound);
	FmodErrorCheck(result);
}

/* Getters/Setters */
FMOD_SYSTEM	*Sound::GetSystem()
{
	return system;
}

FMOD_SOUND* Sound::GetFmodSound()
{
	return fmodSound;
}

float Sound::GetVolume()
{
	float volume;
	FMOD_Channel_GetVolume(channel, &volume);
	return volume;
}

void Sound::SetVolume(float volume)
{
	FMOD_Channel_SetVolume(channel, volume);
}

void Sound::ResetSoundCounter()
{
	m_f_Timer = 0;
	m_c_PlayCounter = 0;
}

bool Sound::CheckPlaying()
{
	FMOD_Channel_IsPlaying(channel, &soundOn);
	FmodErrorCheck(result);
	return soundOn;
}

/***********************************************************
                 Audio System Functions 
***********************************************************/

/* Constructor */
AudioSystem::AudioSystem(GameStateManager* gameStateManager): m_po_GameStateManager{gameStateManager}
{
	BGM.Initialize(); // create BGM channel
	SFX.Initialize(); // create SFX channel

	/* Load up BGMs */
	m_o_MainMenuMusic.CreateBGM("../Resources/Sounds/MainMenu-Loop.mp3", 1, 112.0f, &BGM);
	m_o_MainMenuMusic.Play(0.3f);

	m_o_IntroBattleMusic.Create("../Resources/Sounds/BattleMusic-Intro.mp3", 1, 10.0f, &BGM);

	m_o_BattleLoopMusic.CreateBGM("../Resources/Sounds/BattleMusic-Loop.wav", 1, 64.0f, &BGM);

	/* Load Up SFX */
	m_o_HitSound.Create("../Resources/Sounds/hitsound.wav", 5, 1.0f, &SFX);

	m_o_ExplosionSound.Create("../Resources/Sounds/explosion.wav", 2, 2.0f, &SFX);

	m_o_SpeedSpecialSound.Create("../Resources/Sounds/SpeedUp-SFX.wav", 2, 1.0f, &SFX);
	m_o_FlipSpecialSound.Create("../Resources/Sounds/SpeedUp-SFX.wav", 2, 1.0f, &SFX);

	m_o_StarModeMusic.Create("../Resources/Sounds/StarMode-Music.wav", 1, 5.0f, &SFX);
	m_o_HealthPickup.Create("../Resources/Sounds/Health-SFX.wav", 2, 1.0f, &SFX);
	m_o_SpeedPickup.Create("../Resources/Sounds/SpringBoost-SFX.wav", 2, 4.0f, &SFX);
}

/* Destructor */
AudioSystem::~AudioSystem()
{
	m_po_EventManagerPtr->RemoveListener<Events::EV_PLAYER_COLLISION>(this);
	m_po_EventManagerPtr->RemoveListener<Events::EV_GAME_STATE_CHANGED>(this);
	m_po_EventManagerPtr->RemoveListener<Events::EV_POWERUP_PICKUP_HEALTH>(this);
	m_po_EventManagerPtr->RemoveListener<Events::EV_POWERUP_PICKUP_SPEED>(this);
	m_po_EventManagerPtr->RemoveListener<Events::EV_POWERUP_PICKUP_STARMODE>(this);
	m_po_EventManagerPtr->RemoveListener<Events::EV_SPECIAL_SKILL_BOOST>(this);
	m_po_EventManagerPtr->RemoveListener<Events::EV_SPECIAL_SKILL_FLIP>(this);
	m_po_EventManagerPtr->RemoveListener<Events::EV_PAUSED_GAME>(this);

	if (m_o_MainMenuMusic.GetSystem() != NULL)
		m_o_MainMenuMusic.Release();
	if (m_o_HitSound.GetSystem() != NULL)
		m_o_HitSound.Release();
	if (m_o_ExplosionSound.GetSystem() != NULL)
		m_o_ExplosionSound.Release();

	BGM.Release();
	SFX.Release();
}

void AudioSystem::Initialize()
{
	m_po_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this,this);
	m_po_EventManagerPtr->AddListener<Events::EV_GAME_STATE_CHANGED>(this, this);
	m_po_EventManagerPtr->AddListener<Events::EV_POWERUP_PICKUP_HEALTH>(this, this);
	m_po_EventManagerPtr->AddListener<Events::EV_POWERUP_PICKUP_SPEED>(this, this);
	m_po_EventManagerPtr->AddListener<Events::EV_POWERUP_PICKUP_STARMODE>(this, this);
	m_po_EventManagerPtr->AddListener<Events::EV_SPECIAL_SKILL_BOOST>(this, this);
	m_po_EventManagerPtr->AddListener<Events::EV_SPECIAL_SKILL_FLIP>(this, this);
	m_po_EventManagerPtr->AddListener<Events::EV_PAUSED_GAME>(this, this);
}

void AudioSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	m_o_MainMenuMusic.Update();
	if (m_o_IntroBattleMusic.m_f_Timer > m_o_IntroBattleMusic.m_f_PlayTimer)
	{
		m_o_IntroBattleMusic.Pause(true);
		m_o_IntroBattleMusic.m_f_Timer = 0.0f;
		m_o_BattleLoopMusic.Play(0.1f);
		if (muted)
			FMOD_Channel_SetVolume(m_o_BattleLoopMusic.channel, 0.0f);
	}
	m_o_IntroBattleMusic.Update();
	m_o_BattleLoopMusic.Update();
	m_o_HitSound.Update();
	m_o_ExplosionSound.Update();

	if (m_o_StarModeMusic.m_f_Timer > m_o_StarModeMusic.m_f_PlayTimer && !muted)
	{
		m_o_BattleLoopMusic.Pause(false);
		m_o_StarModeMusic.Pause(true);
	}
	m_o_StarModeMusic.Update();
	m_o_HealthPickup.Update();
	m_o_SpeedPickup.Update();
	m_o_SpeedSpecialSound.Update();
	m_o_FlipSpecialSound.Update();
}

/* Collision Event */
void AudioSystem::Receive(const Events::EV_PLAYER_COLLISION& eventData)
{
	if (!muted)
	{
		if (eventData.object1->IsGroup(kCollGroupMouse) && eventData.object2->IsGroup(kCollGroupUIButton))
		{
		}
		// Snek on Snek collision
		else if (eventData.object1->IsGroup(kCollGroupSnek1Head) &&
			eventData.object2->IsGroup(kCollGroupSnek2Body) ||
			eventData.object1->IsGroup(kCollGroupSnek2Head) &&
			eventData.object2->IsGroup(kCollGroupSnek1Body) ||
			eventData.object1->IsGroup(kCollGroupSnek1Head) &&
			eventData.object2->IsGroup(kCollGroupSnek2Head))
		{
			m_o_ExplosionSound.Play();
		}
		else
		{
			if (GameStateManager::ReturnCurrentState() == kStateGame)
				m_o_HitSound.Play();
		}
	}
}

/* Game State Change Event */
void AudioSystem::Receive(const Events::EV_GAME_STATE_CHANGED & eventData)
{
	// Change to Battle Music
	if (eventData.changedToState == kStateCountdown)
	{
		m_o_MainMenuMusic.Pause(true);
		m_o_IntroBattleMusic.Pause(true);
		m_o_BattleLoopMusic.Pause(true);
		m_o_IntroBattleMusic.ResetSoundCounter();
		m_o_BattleLoopMusic.ResetSoundCounter();
		m_o_StarModeMusic.ResetSoundCounter();
		m_o_IntroBattleMusic.Play(0.1f);
		if(muted)
			FMOD_Channel_SetVolume(m_o_IntroBattleMusic.channel, 0.0f);
	}
	// Change to Main Menu Music
	else if (eventData.changedToState == kStateMainMenu &&
			(eventData.changedFromState == kStatePause || 
			 eventData.changedFromState == kStateWinScreen || 
			 (eventData.changedFromState == kStateConfirmationScreen &&
			  ConfirmationScreenSystem::m_e_PrevState == kStatePause)))
	{
		m_o_IntroBattleMusic.Pause(true);
		m_o_BattleLoopMusic.Pause(true);
		m_o_StarModeMusic.Pause(true);
		m_o_MainMenuMusic.Pause(false);
	}
}

/* Health Powerup Pickup */
void AudioSystem::Receive(const Events::EV_POWERUP_PICKUP_HEALTH & eventData)
{
	UNREFERENCED_PARAMETER(eventData);
	if (!muted)
		m_o_HealthPickup.Play(0.25f);
}

/* Speed Powerup Pickup */
void AudioSystem::Receive(const Events::EV_POWERUP_PICKUP_SPEED & eventData)
{
	UNREFERENCED_PARAMETER(eventData);
	if (!muted)
		m_o_SpeedPickup.Play(0.4f);
}

/* Star Powerup Pickup */
void AudioSystem::Receive(const Events::EV_POWERUP_PICKUP_STARMODE & eventData)
{
	UNREFERENCED_PARAMETER(eventData);

	if (!muted)
	{
		m_o_StarModeMusic.Play(0.15f);
		m_o_BattleLoopMusic.Pause(true);
	}
}

/* Boost Special Skill */
void AudioSystem::Receive(const Events::EV_SPECIAL_SKILL_BOOST & eventData)
{
	UNREFERENCED_PARAMETER(eventData);
	if (!muted)
	{
		m_o_SpeedSpecialSound.Play(0.4f);
	}
}

/* Flip Special Skill */
void AudioSystem::Receive(const Events::EV_SPECIAL_SKILL_FLIP & eventData)
{
	UNREFERENCED_PARAMETER(eventData);
	if (!muted)
	{
		m_o_FlipSpecialSound.Play(0.4f);
	}
}

/* Game Paused */
void AudioSystem::Receive(const Events::EV_PAUSED_GAME & eventData)
{
	if (eventData.pauseCheck)
	{
		
		m_o_IntroBattleMusic.SetVolume(m_o_IntroBattleMusic.GetVolume()*0.2f);
		m_o_BattleLoopMusic.SetVolume(m_o_BattleLoopMusic.GetVolume()*0.2f);
		m_o_StarModeMusic.SetVolume(m_o_StarModeMusic.GetVolume()*0.2f);
	}
	else
	{
		m_o_IntroBattleMusic.SetVolume(m_o_IntroBattleMusic.GetVolume()*5.0f);
		m_o_BattleLoopMusic.SetVolume(m_o_BattleLoopMusic.GetVolume()*5.0f);
		m_o_StarModeMusic.SetVolume(m_o_StarModeMusic.GetVolume()*5.0f);
	}
}

/* Mute/Unmute */
void AudioSystem::ToggleMute()
{
	muted = !muted;

	/* Mute currently playing BGM */
	if (muted)
	{
		FMOD_Channel_SetVolume(m_o_MainMenuMusic.channel, 0.0f);
		FMOD_Channel_SetVolume(m_o_IntroBattleMusic.channel, 0.0f);
		FMOD_Channel_SetVolume(m_o_BattleLoopMusic.channel, 0.0f);
		FMOD_Channel_SetVolume(m_o_StarModeMusic.channel, 0.0f);
	}
	/* Unmute currently playing BGM */
	else
	{
		FMOD_Channel_SetVolume(m_o_MainMenuMusic.channel, 0.3f);
		FMOD_Channel_SetVolume(m_o_IntroBattleMusic.channel, 0.1f);
		FMOD_Channel_SetVolume(m_o_BattleLoopMusic.channel, 0.1f);
		FMOD_Channel_SetVolume(m_o_StarModeMusic.channel, 0.15f);
	}
}