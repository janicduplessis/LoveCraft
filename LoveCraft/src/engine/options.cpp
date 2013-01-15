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
bool Option::GetOptInfos() const
{
	return m_showInfo;
}

void Option::SetOptMusic(const bool value)
{
	m_music = value;
	if (m_music)
		Info::Get().Sound().PlayNextTrack();
	else Info::Get().Sound().StopMusic();
}

void Option::SetOptSound(const bool value)
{
	m_sound = value;
}
void Option::SetOptInfos(const bool value)
{
	m_showInfo = value;
}

bool Option::GetOptDebug() const
{
	return m_debug;
}
void Option::SetOptDebug(const bool value)
{
	m_debug = value;
}