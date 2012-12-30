#include "son.h"


Son::Son() : m_music(sf::Music()), m_stepTmr(0), m_trackNumber(0)
{
	m_sndBuffers = new sf::SoundBuffer[Sons::SON_LAST];
	m_sndChannels = new sf::Sound[Channel::CHANNEL_LAST];
	m_musicList = new std::string[Musics::MUSIC_LAST];
}


Son::~Son()
{
	delete [] m_sndBuffers;
	delete [] m_sndChannels;
	delete [] m_musicList;
}

bool Son::LoadSounds()
{
	//Sons
	if (!m_sndBuffers[Sons::SON_FOOT1].loadFromFile(SOUND_FOOT_PATH "wood1.ogg"))
		return false;
	if (!m_sndBuffers[Sons::SON_FOOT2].loadFromFile(SOUND_FOOT_PATH "wood2.ogg"))
		return false;
	if (!m_sndBuffers[Sons::SON_CLICK].loadFromFile(SOUND_PATH "g_click.ogg"))
		return false;
	if (!m_sndBuffers[Sons::SON_JUMP].loadFromFile(SOUND_PATH "g_jump2.ogg"))
		return false;
	if (!m_sndBuffers[Sons::SON_BOLT].loadFromFile(SOUND_PATH "s_bolt.ogg"))
		return false;
	if (!m_sndBuffers[Sons::SON_DEFEND].loadFromFile(SOUND_PATH "s_defend.ogg"))
		return false;
	if (!m_sndBuffers[Sons::SON_FIRE].loadFromFile(SOUND_PATH "s_fire.ogg"))
		return false;
	if (!m_sndBuffers[Sons::SON_FREEZE].loadFromFile(SOUND_PATH "s_freeze.ogg"))
		return false;
	if (!m_sndBuffers[Sons::SON_HEAL1].loadFromFile(SOUND_PATH "s_heal1.ogg"))
		return false;
	if (!m_sndBuffers[Sons::SON_HEAL2].loadFromFile(SOUND_PATH "s_heal2.ogg"))
		return false;
	if (!m_sndBuffers[Sons::SON_POISON].loadFromFile(SOUND_PATH "s_poison.ogg"))
		return false;
	if (!m_sndBuffers[Sons::SON_SHIELD].loadFromFile(SOUND_PATH "s_shield.ogg"))
		return false;
	if (!m_sndBuffers[Sons::SON_SHOCK].loadFromFile(SOUND_PATH "s_shock.ogg"))
		return false;
	if (!m_sndBuffers[Sons::SON_STORM].loadFromFile(SOUND_PATH "s_storm.ogg"))
		return false;

	//Musiques
	m_musicList[Musics::MUSIC_OVERWORLD1] = SOUND_PATH "m_overworld1.ogg";
	m_musicList[Musics::MUSIC_OVERWORLD2] = SOUND_PATH "m_overworld2.ogg";
	return true;
}

bool Son::PlayMusic()
{
	if (m_music.getStatus() != sf::SoundSource::Playing)
	{
		//			Ouverture  de (string  de l'enum  ou on est rendu / nombre de piste)
		if (!m_music.openFromFile(m_musicList[(Musics)(m_trackNumber % Musics::MUSIC_LAST)]))
			return false;

		m_music.play();
	}
	return true;
}
void Son::PlayNextTrack()
{
	m_music.stop();
	m_trackNumber++;
	PlayMusic();
}
bool Son::PlaySnd(const Sons& snd)
{
	//Joue le son dans le canal par défaut
	PlaySnd(snd, Channel::CHANNEL_DEFAULT);
	return true;
}

bool Son::PlaySnd(const Sons& snd, const Channel& channel, bool aSync)
{
	//Joue le son dans le canal voulu et vérifie si le son doit jouer en asynchrome
	if (aSync || m_sndChannels[channel].getStatus() != sf::SoundSource::Playing)
	{
		//Assigne le son (buffer) voulu au canal voulu et joue le son
		m_sndChannels[channel].setBuffer(m_sndBuffers[snd]);
		m_sndChannels[channel].play();
	}
	return true;
}

bool Son::PlayStep()
{
	return true;
}