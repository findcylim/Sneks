#include "AudioSystem.h"

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

void Sound::Initialize()
{
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
	if (m_c_PlayCap < 5)
	{
		soundOn = true;

		FMOD_System_PlaySound(system, fmodSound, 0, false, &channel);
		m_b_IsPlaying = true;
		FmodErrorCheck(result);
		FMOD_Channel_SetVolume(channel, volume);
		//FMOD_Channel_SetPaused(channel, false);
		++m_c_PlayCap;
		m_f_PlayTimer = 0;
	}
	else
	{
		m_c_PlayCap = m_c_PlayCap;
	}
}

void Sound::Update()
{
	FMOD_System_Update(system);
	FmodErrorCheck(result);
	float dt = static_cast<float>(AEFrameRateControllerGetFrameTime());
	m_f_Timer += dt;
	if (m_f_Timer > 1.0f)
	{
		m_f_Timer	= 0.0f;
		m_c_PlayCap = 0;
	}
	if (m_b_IsPlaying)
	{
		m_f_PlayTimer += dt;
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

AudioSystem::AudioSystem(EntityManager* entityManagerPtr) :
BaseSystem(entityManagerPtr)
{
	m_o_MainMenuMusic.Initialize();
	m_o_MainMenuMusic.CreateBGM("../Resources/Sounds/MainMenu-Loop.mp3");
	m_o_MainMenuMusic.Play(0.4f);

	m_o_IntroBattleMusic.Initialize();
	m_o_IntroBattleMusic.Create("../Resources/Sounds/BattleMusic-Intro.mp3");

	m_o_BattleLoopMusic.Initialize();
	m_o_BattleLoopMusic.CreateBGM("../Resources/Sounds/BattleMusic-Loop.wav");

	m_o_HitSound.Initialize();
	m_o_HitSound.Create("../Resources/Sounds/hitsound.wav");

	m_o_PowerUpSound.Initialize();
	m_o_PowerUpSound.Create("../Resources/Sounds/powerup.wav");

	m_o_ExplosionSound.Initialize();
	m_o_ExplosionSound.Create("../Resources/Sounds/explosion.wav");

}

AudioSystem::~AudioSystem()
{
	m_po_EventManagerPtr->RemoveListener<Events::EV_PLAYER_COLLISION>(this);
	m_po_EventManagerPtr->RemoveListener<Events::EV_GAME_STATE_CHANGED>(this);
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
}

void AudioSystem::Receive(const Events::EV_PLAYER_COLLISION& eventData)
{
	if (eventData.object1->IsGroup(kCollGroupMouse) && eventData.object2->IsGroup(kCollGroupUIButton))
	{
		
	}
	else if (eventData.object2->IsGroup(kCollGroupPowerUp))
	{
		m_o_PowerUpSound.Play();
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
		m_o_HitSound.Play();
	}
}

void AudioSystem::Receive(const Events::EV_GAME_STATE_CHANGED & eventData)
{
	if (eventData.changedToState == kStateGame && eventData.changedFromState == kStateHelpMenu) // Change this to countdown
	{
		m_o_MainMenuMusic.Pause(true);
		m_o_IntroBattleMusic.Play();
	}
	else if (eventData.changedToState == kStateMainMenu &&
			(eventData.changedFromState == kStatePause || eventData.changedFromState == kStateWinScreen))
	{
		m_o_IntroBattleMusic.Pause(true);
		m_o_BattleLoopMusic.Pause(true);
		m_o_MainMenuMusic.Play(0.4f);
	}
}

void AudioSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	m_o_MainMenuMusic.Update();
	m_o_IntroBattleMusic.Update();
	if (m_o_IntroBattleMusic.m_f_PlayTimer > 10.0f)
	{
		m_o_IntroBattleMusic.Pause(true);
		m_o_IntroBattleMusic.m_f_PlayTimer = 0.0f;
		m_o_BattleLoopMusic.Play();
	}
	m_o_BattleLoopMusic.Update();
	m_o_HitSound.Update();
	m_o_PowerUpSound.Update();
	m_o_ExplosionSound.Update();
}

