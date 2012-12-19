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
	if (!m_sndBuffers[Son::Sons::SON_FOOT1].loadFromFile(SOUND_PATH "foot1.wav"))
		return false;
	if (!m_sndBuffers[Son::Sons::SON_FOOT2].loadFromFile(SOUND_PATH "foot2.wav"))
		return false;
	if (!m_sndBuffers[Son::Sons::SON_CLICK].loadFromFile(SOUND_PATH "click.wav"))
		return false;
	if (!m_sndBuffers[Son::Sons::SON_JUMP].loadFromFile(SOUND_PATH "jump.wav"))
		return false;
	return true;
}

bool Son::PlayMusic()
{
	m_music.openFromFile(SOUND_PATH "overworld.ogg");
	m_music.setVolume(100);
	m_music.setLoop(true);
	m_music.play();
	return true;
}
bool Son::PlaySnd(const Sons& snd)
{
	PlaySnd(snd, Channel::CHANNEL_DEFAULT);
	return true;
}

bool Son::PlaySnd(const Sons& snd, const Channel& channel)
{
	if (m_sndChannels[channel].getStatus() != sf::SoundSource::Playing)
	{
		m_sndChannels[channel].setBuffer(m_sndBuffers[snd]);
		m_sndChannels[channel].play();
	}
	return true;
}