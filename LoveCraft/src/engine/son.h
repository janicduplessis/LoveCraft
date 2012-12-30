#ifndef SON_H_
#define SON_H_

#include "define.h"

class Son
{
public:
	enum Sons
	{
		SON_FOOT1,
		SON_FOOT2,
		SON_CLICK,
		SON_JUMP,
		SON_BOLT,
		SON_FIRE,
		SON_SHOCK,
		SON_POISON,
		SON_HEAL1,
		SON_HEAL2,
		SON_DEFEND,
		SON_STORM,
		SON_FREEZE,
		SON_SHIELD,
		SON_LAST
	};
	enum Musics
	{
		MUSIC_OVERWORLD1,
		MUSIC_OVERWORLD2,
		MUSIC_LAST
	};
	enum Channel
	{
		CHANNEL_DEFAULT,
		CHANNEL_CONSTANT,
		CHANNEL_STEP,
		CHANNEL_INTERFACE,
		CHANNEL_SPELL,
		CHANNEL_PARTICLES,
		CHANNEL_PLAYER,
		CHANNEL_MUSIC,
		CHANNEL_LAST
	};
	/**
	* Constructeur par d�faut
	*/
	Son();
	/**
	* Destructeur par d�faut
	*/
	~Son();
	/**
	* Charge les sons en m�moire et les noms de fichier des musiques
	* @return Succ�s
	*/
	bool LoadSounds();
	/**
	* Fait jouer la musique au num�ro de piste du champ m_trackNumber
	* @return Succ�s
	*/
	bool PlayMusic();
	/**
	* Arr�te la musique en cours, incr�mente la num�ro de piste et refait jouer la musique
	*/
	void PlayNextTrack();
	/**
	* Fait jouer le son demand� dans le canal par d�faut
	* @return Succ�s
	*/
	bool PlaySnd(const Sons& snd);
	/**
	* Fait jouer le son demand� dans le canal demand�
	* @param aSync		Sp�cifie si le son ne peut �tre jou� que si aucun autre son
	* 					n'occupe le canal
	* @return Succ�s
	*/
	bool PlaySnd(const Sons& snd, const Channel& channel, bool aSync = true);
	/**
	* Fait jouer les sons de bruits de pas en alternance selon le type de bloc
	*/
	bool PlayStep();
private:
	unsigned short m_stepTmr;
	unsigned short m_trackNumber;

	std::string* m_musicList;
	sf::Music m_music;

	sf::SoundBuffer* m_sndBuffers;
	sf::Sound* m_sndChannels;
};
#endif
