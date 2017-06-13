#pragma once

#include "inc/fmod.hpp"
#define g_pSoundManager cSoundManager::GetInstance()
#pragma comment(lib, "lib/fmodex_vc.lib")

#define SOUND_BUFFER 10
#define EXTRA_SOUND_CHANNEL 5

#define TOTAL_SOUND_CHANNEL (SOUND_BUFFER + EXTRA_SOUND_CHANNEL)

using namespace FMOD;

class cSoundManager
{
private:
	SINGLETONE(cSoundManager);
	typedef map<string, Sound**> arrSounds;
	typedef map<string, Sound**>::iterator arrSoundsIter;
	typedef map<string, string> arrSoundNames;
	typedef map<string, string>::iterator arrSoundNamesIter;

	arrSounds _mTotalSounds;
	arrSoundNames _mTotalSoundNames;

	System* _system;
	Sound** _sound;
	Channel** _channel;
	float _volume;

public:
	void Destroy();
	void update();

	void addSound(string keyName, string soundName, bool background, bool loop);
	void play(string keyName, float volume);
	void stop(string keyName);
	void pause(string keyName);
	void resume(string keyName);
	void next(string keyName, float volume);
	void prev(string keyName, float volume);
	void setTime(string keyName, DWORD32 time);
	int getTime(string keyName);
	int getMaxTime(string keyName);
	void setVolume(string keyName, float volume);
	const char* getName(string keyName);
};

