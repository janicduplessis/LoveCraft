#include "son.h"
#include "info.h"
#include <sstream>

float Son::m_stepTmr = 0;
uint8 Son::m_footStep = 0;
uint8 Son::m_trackNumber = 0;
string* Son::m_musicList = 0;
sf::Music Son::m_music = sf::Music();
sf::SoundBuffer* Son::m_footSteps = 0;
sf::SoundBuffer* Son::m_sndBuffers = 0;
sf::Sound* Son::m_sndChannels = 0;

Son::Son()
{
}


Son::~Son()
{
}

void Son::Init()
{
	assert(LoadSounds());
}

void Son::CheckExistence()
{
	if (!m_sndChannels)
		Init();
}

bool Son::LoadSounds()
{
	m_sndBuffers = new sf::SoundBuffer[SON_LAST];
	m_footSteps = new sf::SoundBuffer[FOOT_LAST * SOUND_FOOT_NUMBER + 1];
	m_sndChannels = new sf::Sound[CHANNEL_LAST];
	m_musicList = new string[MUSIC_LAST];

	//Sons divers
	if (!m_sndBuffers[SON_CLICK].loadFromFile(SOUND_PATH "g_click.ogg"))
		return false;
	if (!m_sndBuffers[SON_JUMP].loadFromFile(SOUND_PATH "g_jump.ogg"))
		return false;
	if (!m_sndBuffers[SON_FALLPAIN].loadFromFile(SOUND_PATH "g_fallpain.ogg"))
		return false;
	if (!m_sndBuffers[SON_DEATH].loadFromFile(SOUND_PATH "g_laugh.ogg"))
		return false;
	if (!m_sndBuffers[SON_LEVELUP].loadFromFile(SOUND_PATH "g_level.ogg"))
		return false;
	if (!m_sndBuffers[SON_BOLT].loadFromFile(SOUND_PATH "s_bolt.ogg"))
		return false;
	if (!m_sndBuffers[SON_DEFEND].loadFromFile(SOUND_PATH "s_defend.ogg"))
		return false;
	if (!m_sndBuffers[SON_FIRE].loadFromFile(SOUND_PATH "s_fire.ogg"))
		return false;
	if (!m_sndBuffers[SON_FREEZE].loadFromFile(SOUND_PATH "s_freeze.ogg"))
		return false;
	if (!m_sndBuffers[SON_HEAL1].loadFromFile(SOUND_PATH "s_heal1.ogg"))
		return false;
	if (!m_sndBuffers[SON_HEAL2].loadFromFile(SOUND_PATH "s_heal2.ogg"))
		return false;
	if (!m_sndBuffers[SON_POISON].loadFromFile(SOUND_PATH "s_poison.ogg"))
		return false;
	if (!m_sndBuffers[SON_SHIELD].loadFromFile(SOUND_PATH "s_shield.ogg"))
		return false;
	if (!m_sndBuffers[SON_SHOCK].loadFromFile(SOUND_PATH "s_shock.ogg"))
		return false;
	if (!m_sndBuffers[SON_STORM].loadFromFile(SOUND_PATH "s_storm.ogg"))
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
	m_musicList[MUSIC_MENU] = SOUND_PATH "m_menu.ogg";
	m_musicList[MUSIC_LOADING] = SOUND_PATH "m_loading.ogg";
	m_musicList[MUSIC_PLAY1] = SOUND_PATH "m_play1.ogg";
	return true;
}

void Son::PlayMusic()
{
	PlayMusic((Musics)m_trackNumber);
}
void Son::PlayMusic(Musics music)
{
	CheckExistence();

	static bool playmusic = true;
#ifdef OPTIONS_H_
	playmusic = Info::Get().Options().GetOptMusic();
#endif
	if (playmusic)
	{
		m_music.stop();
		if (m_trackNumber == MUSIC_LAST)
			m_trackNumber = 0;

		assert(m_music.openFromFile(m_musicList[music]));

		m_trackNumber = music;
		m_music.play();
	}
}
void Son::PlayNextTrack()
{
	PlayMusic((Musics)(m_trackNumber + 1));
}
void Son::PlaySnd(const Sons& snd)
{
	//Joue le son dans le canal par défaut
	PlaySnd(snd, Channel::CHANNEL_DEFAULT);
}

void Son::PlaySnd(const Sons& snd, const Channel& channel, bool aSync)
{
	CheckExistence();

	static bool playsound = true;
#ifdef OPTIONS_H_
	playsound = Info::Get().Options().GetOptSound();
#endif

	if (playsound)
	{
		//Joue le son dans le canal voulu et vérifie si le son doit jouer en asynchrome
		if (aSync || m_sndChannels[channel].getStatus() != sf::SoundSource::Playing)
		{
			//Assigne le son (buffer) voulu au canal voulu et joue le son
			m_sndChannels[channel].setBuffer(m_sndBuffers[snd]);
			m_sndChannels[channel].play();
		}
	}
}

void Son::PlayStep(const BlockType type, float elapsedTime, float speed, bool priority)
{
	CheckExistence();

	static bool playfoot = true;
#ifdef OPTIONS_H_
	playfoot = Info::Get().Options().GetOptSound();
#endif

	if (playfoot)
	{
		//Conversion du BlockType en Foots
		Foots typePas = GetFootType(type);
		//Vérification que le bloc rencontré est solide (ou a un son de définit)
		if (typePas != FOOT_AIR)
		{
			//Incrémentation du timer par la normalisation de la vitesse
			m_stepTmr += elapsedTime * (speed / 8);
			//Vérification que le timer soit rendu au temps minimal ou que le son ait priorité
			if (SOUND_FOOT_TIMEOUT <= m_stepTmr || priority)
			{
				//Assigne le buffer correspondant au type de pas
				m_sndChannels[CHANNEL_STEP].setBuffer(m_footSteps[typePas * 4 + (m_footStep % SOUND_FOOT_NUMBER)]);
				//Joue le son
				m_sndChannels[CHANNEL_STEP].play();
				//Incrémente le pas afin d'obtenir des sons presque différents à chaque appel
				m_footStep++;
				//Comme le son est joué, on réinitialise le timer
				m_stepTmr = 0;
			}
		}
	}
}

void Son::StopMusic()
{
	CheckExistence();
	m_music.stop();
}

Son::Foots Son::GetFootType(BlockType type)
{
	switch (type)
	{
	case BLOCK_TYPE::BTYPE_BRICK:
		return FOOT_CONCRETE;
	case BLOCK_TYPE::BTYPE_GRASS:
		return FOOT_GRASS;
	case BLOCK_TYPE::BTYPE_DIRT:
		return FOOT_DIRT;
	case BLOCK_TYPE::BTYPE_SAND:
		return FOOT_GRAVEL;
	case BLOCK_TYPE::BTYPE_ROCK:
		return FOOT_TILE;
	case BLOCK_TYPE::BTYPE_SNOW:
		return FOOT_SNOW;
	case BLOCK_TYPE::BTYPE_SWAMP:
		return FOOT_SLOSH;
	default:
		return FOOT_AIR;
	}
}

bool Son::LoadFootSteps(const Son::Foots type, const std::string filename)
{
	//Initialisation du stream pour les noms de fichier
	std::ostringstream ss;
	//Entrée en boucle
	for (uint8 i = 0; i < SOUND_FOOT_NUMBER; i++)
	{
		//Construit le nom de fichier complet avec les infos
		ss << SOUND_FOOT_PATH << filename << (i + 1) << ".ogg";
		if (!m_footSteps[type * 4 + i].loadFromFile(ss.str()))
			return false;
		//Réinitialisation du stream
		ss.str("");
	}
	return true;
}