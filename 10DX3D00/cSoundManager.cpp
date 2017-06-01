#include "stdafx.h"
#include "cSoundManager.h"


cSoundManager::cSoundManager()
	:_system(NULL)
	, _sound(NULL)
	, _channel(NULL)
{
	System_Create(&_system);
	_system->init(TOTAL_SOUND_CHANNEL, FMOD_INIT_NORMAL, 0);
	_sound = new Sound*[TOTAL_SOUND_CHANNEL];
	_channel = new Channel*[TOTAL_SOUND_CHANNEL];
	ZeroMemory(_sound, sizeof(Sound*) * (TOTAL_SOUND_CHANNEL));
	ZeroMemory(_channel, sizeof(Channel*) * (TOTAL_SOUND_CHANNEL));
}


cSoundManager::~cSoundManager()
{
}


void cSoundManager::Destroy()
{
	if (_channel != NULL || _sound != NULL)
	{
		for (int i = 0; i < TOTAL_SOUND_CHANNEL; ++i)
		{
			if (_channel != NULL)
				_channel[i]->stop();
			if (_sound != NULL)
				_sound[i]->release();
		}
	}

	SAFE_DELETE_ARRAY(_channel);
	SAFE_DELETE_ARRAY(_sound);

	if (_system != NULL)
	{
		_system->release();
		_system->close();
	}

	_mTotalSounds.clear();
}

void cSoundManager::update()
{
	_system->update();
}

void cSoundManager::addSound(string keyName, string soundName, bool background, bool loop)
{
	if (_mTotalSounds.end() == _mTotalSounds.find(keyName)) 
	{
		if (loop)
		{
			if (background)
			{
				_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL,
					0, &_sound[_mTotalSounds.size()]);
			}
			else
			{
				_system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL,
					0, &_sound[_mTotalSounds.size()]);
			}
		}
		else
			_system->createSound(soundName.c_str(), FMOD_DEFAULT,
				0, &_sound[_mTotalSounds.size()]);

		_mTotalSounds.insert(make_pair(keyName, &_sound[_mTotalSounds.size()]));
		_mTotalSoundNames.insert(make_pair(keyName, soundName));

	}
	
}

void cSoundManager::play(string keyName, float volume)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;
	bool isPlay = true;
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			if (_channel[count]->isPlaying(&isPlay))
			{
				_system->playSound(FMOD_CHANNEL_FREE, _sound[count], false, &_channel[count]);
				_channel[count]->setVolume(volume);
				break;
			}

			else if (!_channel[count]->getPaused(&isPlay)) resume(keyName);
		}
		

	}


}

void cSoundManager::stop(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->stop();
			break;
		}
	}
}

void cSoundManager::pause(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(true);
			break;
		}
	}
}

void cSoundManager::resume(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(false);
			break;
		}
	}
}

void cSoundManager::next(string keyName, float volume)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->stop();
			if (count + 1 == _mTotalSounds.size())
			{
				iter = _mTotalSounds.begin();
				play(iter->first, volume);
				break;
			}
			else
			{
				++iter;
				play(iter->first, volume);
				break;
			}
		}
	}
}

void cSoundManager::prev(string keyName, float volume)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->stop();
			if (count == 0)
			{
				iter = _mTotalSounds.end();
				--iter;
				play(iter->first, volume);
				break;
			}
			else
			{
				--iter;
				play(iter->first, volume);
				break;
			}
		}
	}
}

void cSoundManager::setTime(string keyName, DWORD32 time)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPosition(time * 1000, FMOD_TIMEUNIT_MS);
			break;
		}
	}
}

int cSoundManager::getTime(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;
	DWORD32 time = 0;
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->getPosition(&time, FMOD_TIMEUNIT_MS);
			return time;
		}
	}
	return NULL;

}

int cSoundManager::getMaxTime(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;
	DWORD32 maxTime = 0;
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_sound[count]->getLength(&maxTime, FMOD_TIMEUNIT_MS);
			return maxTime;
		}
	}
	return NULL;
}

void cSoundManager::setVolume(string keyName, float volume)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setVolume(volume);
		}
	}
	
}

const char * cSoundManager::getName(string keyName)
{
	arrSoundNamesIter iter = _mTotalSoundNames.begin();
	int count = 0;
	const char* soundName;
	for (iter; iter != _mTotalSoundNames.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			soundName = iter->second.c_str();
			return soundName;
		}
	}
	return NULL;
}
