#include "son.h"
#include "info.h"
#include <sstream>

Son::Son() : m_music(sf::Music()), m_stepTmr(0), m_trackNumber(0), m_footStep(0), m_soundStep(0)
{
	m_sndBuffers = new sf::SoundBuffer[Sons::SON_LAST];
	m_footSteps = new sf::SoundBuffer[Foots::FOOT_LAST * SOUND_FOOT_NUMBER + 1];
	m_sndChannels = new sf::Sound[Channel::CHANNEL_LAST];
	m_musicList = new std::string[Musics::MUSIC_LAST];
}


Son::~Son()
{
	delete [] m_sndBuffers;
	delete [] m_footSteps;
	delete [] m_sndChannels;
	delete [] m_musicList;
}

bool Son::LoadSounds()
{
	//Sons divers
	if (!m_sndBuffers[Sons::SON_CLICK].loadFromFile(SOUND_PATH "g_click.ogg"))
		return false;
	if (!m_sndBuffers[Sons::SON_JUMP1].loadFromFile(SOUND_PATH "g_jump1.ogg"))
		return false;
	if (!m_sndBuffers[Sons::SON_JUMP2].loadFromFile(SOUND_PATH "g_jump2.ogg"))
		return false;
	if (!m_sndBuffers[Sons::SON_FALLPAIN].loadFromFile(SOUND_PATH "g_fallpain.ogg"))
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

	if (!(LoadFootSteps(FOOT_CONCRETE, "concrete") &&
		LoadFootSteps(FOOT_DIRT, "dirt") &&
		LoadFootSteps(FOOT_GRASS, "grass") &&
		LoadFootSteps(FOOT_GRAVEL, "gravel") &&
		LoadFootSteps(FOOT_METAL, "metal") &&
		LoadFootSteps(FOOT_MUD, "mud") &&
		LoadFootSteps(FOOT_SLOSH, "slosh") &&
		LoadFootSteps(FOOT_SNOW, "snow") &&
		LoadFootSteps(FOOT_TILE, "tile") &&
		LoadFootSteps(FOOT_WADE, "wade") &&
		LoadFootSteps(FOOT_WOOD, "wood") ))
	{
		std::cout << "Erreur lors du chargement des sons de bruits de pas" << std::endl;
		return false;
	}


	//Musiques
	m_musicList[Musics::MUSIC_OVERWORLD1] = SOUND_PATH "m_overworld1.ogg";
	m_musicList[Musics::MUSIC_OVERWORLD2] = SOUND_PATH "m_overworld2.ogg";
	return true;
}

bool Son::PlayMusic()
{
	if (Info::Get().Options().GetOptMusic())
	{
		if (m_music.getStatus() != sf::SoundSource::Playing)
		{
			//			Ouverture  de (string  de l'enum  ou on est rendu / nombre de piste)
			if (!m_music.openFromFile(m_musicList[(Musics)(m_trackNumber % Musics::MUSIC_LAST)]))
				return false;

			m_music.play();
		}
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
	//Joue le son dans le canal par d�faut
	PlaySnd(snd, Channel::CHANNEL_DEFAULT);
	return true;
}

bool Son::PlaySnd(const Sons& snd, const Channel& channel, bool aSync)
{
	if (Info::Get().Options().GetOptSound())
	{
		//Joue le son dans le canal voulu et v�rifie si le son doit jouer en asynchrome
		if (aSync || m_sndChannels[channel].getStatus() != sf::SoundSource::Playing)
		{
			//Assigne le son (buffer) voulu au canal voulu et joue le son
			m_sndChannels[channel].setBuffer(m_sndBuffers[snd]);
			m_sndChannels[channel].play();
		}
	}
	return true;
}

bool Son::PlayStep(const BlockType type, float elapsedTime, float timeout, bool priority)
{
	//Conversion du BlockType en Foots
	Foots typePas = GetFootType(type);
	//V�rification que le bloc rencontr� est solide (ou a un son de d�finit)
	if (typePas != FOOT_AIR)
	{
		//Incr�mentation du timer
		m_stepTmr += elapsedTime;
		//V�rification que le timer soit rendu au temps minimal
		if (timeout <= m_stepTmr || priority)
		{
			//Assigne le buffer correspondant au type de pas
			m_sndChannels[CHANNEL_STEP].setBuffer(m_footSteps[typePas * 4 + (m_footStep % SOUND_FOOT_NUMBER)]);
			//Joue le son
			m_sndChannels[CHANNEL_STEP].play();
			//Incr�mente le pas afin d'obtenir des sons presque diff�rents � chaque appel
			m_footStep++;
			//Comme le son est jou�, on r�initialise le timer
			m_stepTmr = 0;
		}
	}
	return true;
}

void Son::TestSon()
{
	m_sndChannels[CHANNEL_DEFAULT].setBuffer(m_footSteps[m_soundStep % (Foots::FOOT_LAST * SOUND_FOOT_NUMBER + 1)]);
	m_sndChannels[CHANNEL_DEFAULT].play();
	m_soundStep++;
}

Son::Foots Son::GetFootType(BlockType type) const
{
	switch (type)
	{
	case BLOCK_TYPE::BTYPE_BRICK:
		return Son::FOOT_CONCRETE;
	case BLOCK_TYPE::BTYPE_GRASS:
		return Son::FOOT_GRASS;
	case BLOCK_TYPE::BTYPE_DIRT:
		return Son::FOOT_DIRT;
	case BLOCK_TYPE::BTYPE_SAND:
		return Son::FOOT_GRAVEL;
	default:
		return Son::FOOT_AIR;
	}
}

bool Son::LoadFootSteps(const Son::Foots type, const std::string filename)
{
	//Initialisation du stream pour les noms de fichier
	std::ostringstream ss;
	//Entr�e en boucle
	for (unsigned short i = 0; i < SOUND_FOOT_NUMBER; i++)
	{
		//Construit le nom de fichier complet avec les infos
		ss << SOUND_FOOT_PATH << filename << (i + 1) << ".ogg";
		//Charge le son gr�ce au nom de fichier construit
		if (!m_footSteps[type * 4 + i].loadFromFile(ss.str()))
			return false;
		//R�initialisation du stream
		ss.str("");
	}

}