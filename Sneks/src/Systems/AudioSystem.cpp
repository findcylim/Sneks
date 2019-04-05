#include "AudioSystem.h"
#include "../Utility/GameStateManager.h"

Sound::Sound()
{
	soundOn = false;		/* is sound on? */
	canPlaySound = true;	/* is it possible to play sound? */
	currentSound = 0;		/* currently played sound */
	result = FMOD_OK;
	system = 0;
	fmodSound = 0;
	channel = 0;
}

void Sound::FmodErrorCheck(FMOD_RESULT resultCheck)
{
	/* Throw an error if FMOD finds something wrong */
	if (resultCheck != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", resultCheck, FMOD_ErrorString(resultCheck));
		exit(-1);
	}
}

void Sound::Initialize(char counterCap,float playTimer)
{
	m_c_PlayCap = counterCap;
	m_f_PlayTimer = playTimer;
	FMOD_System_Create(&system);
	FmodErrorCheck(result);
	FMOD_System_Init(system, 64, FMOD_INIT_NORMAL, 0);
	FmodErrorCheck(result);
}

void Sound::Create(const char* filename)
{
	FMOD_System_CreateSound(system, filename, FMOD_LOOP_OFF | FMOD_CREATESAMPLE, 0, &fmodSound);
	FmodErrorCheck(result);
	FMOD_System_PlaySound(system, fmodSound, 0, true, &channel);
	FmodErrorCheck(result);
}

void Sound::CreateBGM(const char*filename)
{
	FMOD_Sound_SetMusicChannelVolume(fmodSound, 0, 0);
	FmodErrorCheck(result);
	FMOD_System_CreateStream(system, filename, FMOD_LOOP_NORMAL, 0, &fmodSound);
	FmodErrorCheck(result);
	FMOD_System_PlaySound(system, fmodSound, 0, true, &channel);
	FmodErrorCheck(result);
}

void Sound::Play(float volume)
{
	if (m_c_PlayCounter < m_c_PlayCap)
	{
		soundOn = true;

		FMOD_System_PlaySound(system, fmodSound, 0, false, &channel);
		m_b_IsPlaying = true;
		FmodErrorCheck(result);
		FMOD_Channel_SetVolume(channel, volume);
		//FMOD_Channel_SetPaused(channel, false);
		++m_c_PlayCounter;
		m_f_Timer = 0;
	}
}

void Sound::Update()
{
	FMOD_System_Update(system);
	FmodErrorCheck(result);
	float dt = static_cast<float>(AEFrameRateControllerGetFrameTime());
	//m_f_Timer += dt;
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

bool Sound::CheckPlaying()
{
	FMOD_Channel_IsPlaying(channel, &soundOn);
	FmodErrorCheck(result);
	return soundOn;
}

void Sound::Release()
{
	FMOD_Sound_Release(fmodSound);
	FmodErrorCheck(result);
	FMOD_System_Release(system);
	FmodErrorCheck(result);
}

FMOD_SYSTEM	*Sound::GetSystem()
{
	return system;
}

FMOD_SOUND* Sound::GetFmodSound()
{
	return fmodSound;
}

void Sound::ResetSoundCounter()
{
	m_f_Timer = 0;
	m_c_PlayCounter = 0;
}

AudioSystem::AudioSystem()
{
	m_o_MainMenuMusic.Initialize(1,112.0f);
	m_o_MainMenuMusic.CreateBGM("../Resources/Sounds/MainMenu-Loop.mp3");
	m_o_MainMenuMusic.Play(0.3f);

	m_o_IntroBattleMusic.Initialize(1,10.0f);
	m_o_IntroBattleMusic.Create("../Resources/Sounds/BattleMusic-Intro.mp3");

	m_o_BattleLoopMusic.Initialize(1, 64.0f);
	m_o_BattleLoopMusic.CreateBGM("../Resources/Sounds/BattleMusic-Loop.wav");

	m_o_HitSound.Initialize(5,1.0f);
	m_o_HitSound.Create("../Resources/Sounds/hitsound.wav");

	/*m_o_PowerUpSound.Initialize();
	m_o_PowerUpSound.Create("../Resources/Sounds/powerup.wav");
*/
	m_o_ExplosionSound.Initialize(2,2.0f);
	m_o_ExplosionSound.Create("../Resources/Sounds/explosion.wav");

	m_o_StarModeMusic.Initialize(1,5.0f);
	m_o_HealthPickup.Initialize(2,1.0f);
	m_o_SpeedPickup.Initialize(2,1.0f);

	m_o_StarModeMusic.Create("../Resources/Sounds/StarMode-Music.wav");
	m_o_HealthPickup.Create("../Resources/Sounds/Health-SFX.wav");
	m_o_SpeedPickup.Create("../Resources/Sounds/SpeedUp-SFX.wav");
}

AudioSystem::~AudioSystem()
{
	m_po_EventManagerPtr->RemoveListener<Events::EV_PLAYER_COLLISION>(this);
	m_po_EventManagerPtr->RemoveListener<Events::EV_GAME_STATE_CHANGED>(this);
	m_po_EventManagerPtr->RemoveListener<Events::EV_POWERUP_PICKUP_HEALTH>(this);
	m_po_EventManagerPtr->RemoveListener<Events::EV_POWERUP_PICKUP_SPEED>(this);
	m_po_EventManagerPtr->RemoveListener<Events::EV_POWERUP_PICKUP_STARMODE>(this);
	if (m_o_MainMenuMusic.GetSystem() != NULL)
		m_o_MainMenuMusic.Release();
	if (m_o_HitSound.GetSystem() != NULL)
		m_o_HitSound.Release();
	if (m_o_PowerUpSound.GetSystem() != NULL)
		m_o_PowerUpSound.Release();
	if (m_o_ExplosionSound.GetSystem() != NULL)
		m_o_ExplosionSound.Release();
}

void AudioSystem::Initialize()
{
	m_po_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this,this);
	m_po_EventManagerPtr->AddListener<Events::EV_GAME_STATE_CHANGED>(this, this);
	m_po_EventManagerPtr->AddListener<Events::EV_POWERUP_PICKUP_HEALTH>(this, this);
	m_po_EventManagerPtr->AddListener<Events::EV_POWERUP_PICKUP_SPEED>(this, this);
	m_po_EventManagerPtr->AddListener<Events::EV_POWERUP_PICKUP_STARMODE>(this, this);
}

void AudioSystem::Receive(const Events::EV_PLAYER_COLLISION& eventData)
{
	if (eventData.object1->IsGroup(kCollGroupMouse) && eventData.object2->IsGroup(kCollGroupUIButton))
	{
		
	}
	else if (eventData.object2->IsGroup(kCollGroupPowerUp))
	{
		//m_o_PowerUpSound.Play();
	}
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
		if(GameStateManager::ReturnCurrentState()==kStateGame)
			m_o_HitSound.Play();
	}
}

void AudioSystem::Receive(const Events::EV_GAME_STATE_CHANGED & eventData)
{
	if (eventData.changedToState == kStateGame && eventData.changedFromState == kStateHelpMenu) // Change this to countdown
	{
		m_o_MainMenuMusic.Pause(true);
		m_o_IntroBattleMusic.ResetSoundCounter();
		m_o_BattleLoopMusic.ResetSoundCounter();
		m_o_StarModeMusic.ResetSoundCounter();
		m_o_IntroBattleMusic.Play(0.1f);
	}
	else if (eventData.changedToState == kStateMainMenu &&
			(eventData.changedFromState == kStatePause || eventData.changedFromState == kStateWinScreen))
	{
		m_o_IntroBattleMusic.Pause(true);
		m_o_BattleLoopMusic.Pause(true);
		m_o_StarModeMusic.Pause(true);
		m_o_MainMenuMusic.Pause(false);
	}
}

void AudioSystem::Receive(const Events::EV_POWERUP_PICKUP_HEALTH & eventData)
{
	UNREFERENCED_PARAMETER(eventData);
	m_o_HealthPickup.Play(0.4f);
}

void AudioSystem::Receive(const Events::EV_POWERUP_PICKUP_SPEED & eventData)
{
	UNREFERENCED_PARAMETER(eventData);
	m_o_SpeedPickup.Play(0.4f);
}

void AudioSystem::Receive(const Events::EV_POWERUP_PICKUP_STARMODE & eventData)
{
	UNREFERENCED_PARAMETER(eventData);
	m_o_StarModeMusic.Play(0.15f);
	m_o_BattleLoopMusic.Pause(true);
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
	}
	m_o_IntroBattleMusic.Update();
	m_o_BattleLoopMusic.Update();
	m_o_HitSound.Update();
	//m_o_PowerUpSound.Update();
	m_o_ExplosionSound.Update();


	if (m_o_StarModeMusic.m_f_Timer > m_o_StarModeMusic.m_f_PlayTimer)
	{
		m_o_BattleLoopMusic.Pause(false);
		m_o_StarModeMusic.Pause(true);
	}
	m_o_StarModeMusic.Update();
	m_o_HealthPickup.Update();
	m_o_SpeedPickup.Update();
}