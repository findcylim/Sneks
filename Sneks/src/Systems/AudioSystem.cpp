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

void Sound::fmodErrorCheck(FMOD_RESULT resultCheck)
{
	/* Throw an error if FMOD finds something wrong */
	if (resultCheck != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", resultCheck, FMOD_ErrorString(resultCheck));
		exit(-1);
	}
}

void Sound::initialise()
{
	FMOD_System_Create(&system);
	fmodErrorCheck(result);
	FMOD_System_Init(system, 64, FMOD_INIT_NORMAL, 0);
	fmodErrorCheck(result);
}

void Sound::create(const char* filename)
{
	FMOD_System_CreateSound(system, filename, FMOD_LOOP_OFF | FMOD_CREATESAMPLE, 0, &fmodSound);
	fmodErrorCheck(result);
	FMOD_System_PlaySound(system, fmodSound, 0, true, &channel);
	fmodErrorCheck(result);
}

void Sound::createBGM(const char* filename)
{
	FMOD_System_CreateStream(system, filename, FMOD_LOOP_NORMAL, 0, &fmodSound);
	fmodErrorCheck(result);
	FMOD_System_PlaySound(system, fmodSound, 0, true, &channel);
	fmodErrorCheck(result);
}

void Sound::play()
{
	if (m_c_PlayCap < 5)
	{
		soundOn = true;
		FMOD_System_PlaySound(system, fmodSound, 0, false, &channel);
		fmodErrorCheck(result);
		++m_c_PlayCap;
	}
	else
	{
		m_c_PlayCap = m_c_PlayCap;
	}
}

void Sound::update()
{
	FMOD_System_Update(system);
	fmodErrorCheck(result);
	m_f_Timer += static_cast<float>(AEFrameRateControllerGetFrameTime());
	if (m_f_Timer > 1.0f)
	{
		m_f_Timer	= 0.0f;
		m_c_PlayCap = 0;
	}
}

void Sound::pause(FMOD_BOOL pause)
{
	FMOD_Channel_SetPaused(channel, pause);
	fmodErrorCheck(result);
}

void Sound::unload()
{
	currentSound = 0;
	soundOn = false;
	FMOD_Sound_Release(fmodSound);
	fmodErrorCheck(result);
}

bool Sound::checkPlaying()
{
	FMOD_Channel_IsPlaying(channel, &soundOn);
	fmodErrorCheck(result);
	return soundOn;
}

void Sound::release()
{
	FMOD_Sound_Release(fmodSound);
	fmodErrorCheck(result);
	FMOD_System_Release(system);
	fmodErrorCheck(result);
}

FMOD_SYSTEM	*Sound::getSystem()
{
	return system;
}

FMOD_SOUND* Sound::getFMODSound()
{
	return fmodSound;
}

AudioSystem::AudioSystem(EntityManager* entityManagerPtr) :
BaseSystem(entityManagerPtr)
{
	BGM.initialise();
	SFX.initialise();
	BGM.createBGM("../Resources/main_menu.wav");
	
	BGM.play();
	SFX.create("../Resources/hitsound.wav");
}

AudioSystem::~AudioSystem()
{
	m_po_EventManagerPtr->RemoveListener<Events::EV_PLAYER_COLLISION>(this);
	if (BGM.getSystem() != NULL)
		BGM.release();
	if (SFX.getSystem() != NULL)
		SFX.release();
}

void AudioSystem::Initialize()
{
	m_po_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this,this);
}

void AudioSystem::Receive(const Events::EV_PLAYER_COLLISION& eventData)
{
	UNREFERENCED_PARAMETER(eventData);
	if (eventData._object1->m_i_CollisionGroupVec[0] == kCollGroupMouse && eventData._object2->m_i_CollisionGroupVec[0] == kCollGroupUIButton)
	{
		
	}
	else
	{
		SFX.play();
	}
}

void AudioSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	BGM.update();
	SFX.update();
}