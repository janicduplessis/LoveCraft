#include "son.h"


Son::Son() : m_stepTmr(0)
{
	m_sndBuffers = new sf::SoundBuffer[Son::Sons::SON_LAST];
	m_sndChannels = new sf::Sound[Son::Channel::CHANNEL_LAST];
}


Son::~Son()
{
}

bool Son::LoadSounds()
{
	if (!m_sndBuffers[Sons::MUSIC_OVERWORLD].loadFromFile(SOUND_PATH "m_overworld.wav"))
		return false;
	if (!m_sndBuffers[Sons::SON_FOOT1].loadFromFile(SOUND_FOOT_PATH "wood1.wav"))
		return false;
	if (!m_sndBuffers[Sons::SON_FOOT2].loadFromFile(SOUND_FOOT_PATH "wood2.wav"))
		return false;
	if (!m_sndBuffers[Sons::SON_CLICK].loadFromFile(SOUND_PATH "g_click.wav"))
		return false;
	if (!m_sndBuffers[Sons::SON_JUMP].loadFromFile(SOUND_PATH "g_jump2.wav"))
		return false;
	if (!m_sndBuffers[Sons::SON_BOLT].loadFromFile(SOUND_PATH "s_bolt.wav"))
		return false;
	if (!m_sndBuffers[Sons::SON_DEFEND].loadFromFile(SOUND_PATH "s_defend.wav"))
		return false;
	if (!m_sndBuffers[Sons::SON_FIRE].loadFromFile(SOUND_PATH "s_fire.wav"))
		return false;
	if (!m_sndBuffers[Sons::SON_FREEZE].loadFromFile(SOUND_PATH "s_freeze.wav"))
		return false;
	if (!m_sndBuffers[Sons::SON_HEAL1].loadFromFile(SOUND_PATH "s_heal1.wav"))
		return false;
	if (!m_sndBuffers[Sons::SON_HEAL2].loadFromFile(SOUND_PATH "s_heal2.wav"))
		return false;
	if (!m_sndBuffers[Sons::SON_POISON].loadFromFile(SOUND_PATH "s_poison.wav"))
		return false;
	if (!m_sndBuffers[Sons::SON_SHIELD].loadFromFile(SOUND_PATH "s_shield.wav"))
		return false;
	if (!m_sndBuffers[Sons::SON_SHOCK].loadFromFile(SOUND_PATH "s_shock.wav"))
		return false;
	if (!m_sndBuffers[Sons::SON_STORM].loadFromFile(SOUND_PATH "s_storm.wav"))
		return false;
	return true;
}

bool Son::PlayMusic()
{
	if (m_sndChannels[Sons::MUSIC_OVERWORLD].getStatus() != sf::SoundSource::Playing)
	{
		m_sndChannels[Sons::MUSIC_OVERWORLD].setVolume(100);
		m_sndChannels[Sons::MUSIC_OVERWORLD].setLoop(true);
		m_sndChannels[Sons::MUSIC_OVERWORLD].play();
	}
	return true;
}
bool Son::PlaySnd(const Sons& snd)
{
	PlaySnd(snd, Channel::CHANNEL_DEFAULT);
	return true;
}

bool Son::PlaySnd(const Sons& snd, const Channel& channel, bool aSync)
{
	if (aSync || m_sndChannels[channel].getStatus() != sf::SoundSource::Playing)
	{
		m_sndChannels[channel].setBuffer(m_sndBuffers[snd]);
		m_sndChannels[channel].play();
	}
	return true;
}