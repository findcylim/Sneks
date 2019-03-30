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
		FmodErrorCheck(result);
		FMOD_Channel_SetVolume(channel, volume);
		//FMOD_Channel_SetPaused(channel, false);
		++m_c_PlayCap;
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
	m_f_Timer += static_cast<float>(AEFrameRateControllerGetFrameTime());
	if (m_f_Timer > 1.0f)
	{
		m_f_Timer	= 0.0f;
		m_c_PlayCap = 0;
	}
}

void Sound::Pause(FMOD_BOOL pause)
{
	FMOD_Channel_SetPaused(channel, pause);
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
	m_o_BackgroundMusic.Initialize();
	m_o_BackgroundMusic.CreateBGM("../Resources/Sounds/main_menu.wav");
	m_o_BackgroundMusic.Play(0.05f);

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
	if (m_o_BackgroundMusic.GetSystem() != NULL)
		m_o_BackgroundMusic.Release();
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

void AudioSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	m_o_BackgroundMusic.Update();
	m_o_HitSound.Update();
	m_o_PowerUpSound.Update();
	m_o_ExplosionSound.Update();
}