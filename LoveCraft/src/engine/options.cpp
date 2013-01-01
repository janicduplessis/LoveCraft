#include "options.h"
#include "info.h"

Option::Option() : m_music(OPTION_MUSIC_STARTING_VALUE), m_sound(OPTION_SOUND_STARTING_VALUE), 
	m_showInfo(OPTION_INFO_STARTING_VALUE)
{
}


Option::~Option()
{
}

bool Option::GetOptMusic() const
{
	return m_music;
}

bool Option::GetOptSound() const
{
	return m_sound;
}

void Option::SetOptMusic(const bool value)
{
	m_music = value;
	if (m_music)
		Info::Get().Sound().PlayNextTrack();
}

void Option::SetOptSound(const bool value)
{
	m_sound = value;
}