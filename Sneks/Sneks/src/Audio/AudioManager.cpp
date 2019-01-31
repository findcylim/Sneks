#include "AudioManager.h"

struct Audio
{
	FMOD_BOOL	soundOn;		/* is sound on? */
	FMOD_BOOL	canPlaySound;	/* is it possible to play sound? */
	char*		currentSound;	/* currently played sound */

	/* FMOD-specific stuff */
	FMOD_RESULT		result;		/* allows error checking for FMOD functions */
	FMOD_SYSTEM		*system;	/* the system where the sound will be using */
	FMOD_SOUND		*fmodSound;	/* holding the actual sound */
	FMOD_CHANNEL	*channel;	/* the channel where the sound will be playing from */
};

Sound::Sound()
{
	*FX =
	{
		false,		/* is sound on? */
		true,		/* is it possible to play sound? */
		0,			/* currently played sound */
		FMOD_OK,
		0,
		0,
		0
	};

}

void Sound::fmodErrorCheck(FMOD_RESULT result)
{
	/* Throw an error if FMOD finds something wrong */
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

void Sound::initialise()
{
	FMOD_System_Create(&FX->system);
	fmodErrorCheck(FX->result);
	FMOD_System_Init(FX->system, 64, FMOD_INIT_NORMAL, 0);
	fmodErrorCheck(FX->result);
}

void Sound::create(char* filename)
{
	FMOD_System_CreateSound(FX->system, filename, FMOD_LOOP_OFF | FMOD_CREATESAMPLE, 0, &FX->fmodSound);
	fmodErrorCheck(FX->result);
	FMOD_System_PlaySound(FX->system, FX->fmodSound, 0, true, &FX->channel);
	fmodErrorCheck(FX->result);
}

void Sound::createBGM(char* filename)
{
	FMOD_System_CreateStream(FX->system, filename, FMOD_LOOP_NORMAL, 0, &FX->fmodSound);
	fmodErrorCheck(FX->result);
	FMOD_System_PlaySound(FX->system, FX->fmodSound, 0, true, &FX->channel);
	fmodErrorCheck(FX->result);
}

void Sound::play()
{
	FX->soundOn = true;
	FMOD_System_PlaySound(FX->system, FX->fmodSound, 0, false, &FX->channel);
	fmodErrorCheck(FX->result);
}

void Sound::update()
{
	FMOD_System_Update(FX->system);
	fmodErrorCheck(FX->result);
}

void Sound::pause(FMOD_BOOL pause)
{
	FMOD_Channel_SetPaused(FX->channel, pause);
	fmodErrorCheck(FX->result);
}

void Sound::unload()
{
	FX->currentSound = 0;
	FX->soundOn = false;
	FMOD_Sound_Release(FX->fmodSound);
	fmodErrorCheck(FX->result);
}

bool Sound::checkPlaying()
{
	FMOD_Channel_IsPlaying(FX->channel, &FX->soundOn);
	fmodErrorCheck(FX->result);
	return FX->soundOn;
}

void Sound::release()
{
	FMOD_Sound_Release(FX->fmodSound);
	fmodErrorCheck(FX->result);
	FMOD_System_Release(FX->system);
	fmodErrorCheck(FX->result);
}

Sound BGM;
Sound SFX;

void Audio_Init()
{
	BGM.initialise();
	SFX.initialise();
}

void Audio_Cleanup()
{
	/* We only release the system if it's not NULL */
	if (BGM.FX->system != NULL)
		BGM.release();
	if (SFX.FX->system != NULL)
		SFX.release();
}