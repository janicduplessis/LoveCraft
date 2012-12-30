#ifndef SON_H_
#define SON_H_

#include "define.h"
#include "blockinfo.h"

class Son
{
public:
	enum Sons
	{
		SON_CLICK,
		SON_JUMP1,
		SON_JUMP2,
		SON_FALLPAIN,
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
	enum Foots
	{
		FOOT_CONCRETE,
		FOOT_DIRT,
		FOOT_GRASS,
		FOOT_METAL,
		FOOT_GRAVEL,
		FOOT_MUD,
		FOOT_SLOSH,
		FOOT_SNOW,
		FOOT_TILE,
		FOOT_WADE,
		FOOT_WOOD,
		FOOT_LAST,
		FOOT_AIR
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
	* Constructeur par défaut
	*/
	Son();
	/**
	* Destructeur par défaut
	*/
	~Son();
	/**
	* Charge les sons en mémoire et les noms de fichier des musiques
	* @return Succès
	*/
	bool LoadSounds();
	/**
	* Fait jouer la musique au numéro de piste du champ m_trackNumber
	* @return Succès
	*/
	bool PlayMusic();
	/**
	* Arrête la musique en cours, incrémente la numéro de piste et refait jouer la musique
	*/
	void PlayNextTrack();
	/**
	* Fait jouer le son demandé dans le canal par défaut
	* 
	* @param snd		Le son à jouer - Voir Enum:Sons
	* @return Succès
	*/
	bool PlaySnd(const Sons& snd);
	/**
	* Fait jouer le son demandé dans le canal demandé
	* 
	* @param snd		Le son à jouer - Voir Enum:Sons
	* @param channel	Le canal dans lequel sera joué le son - Voir Enum:Channel
	* @param aSync		Spécifie si le son ne peut être joué que si aucun autre son
	* 					n'occupe le canal
	* @return Succès
	*/
	bool PlaySnd(const Sons& snd, const Channel& channel, bool aSync = true);
	/**
	* Fait jouer les sons de bruits de pas en alternance selon le type de bloc
	* 
	* @param type			Le type de bloc sur lequel est le joueur
	* @param elapsedTime	Le nombre de temps écoulé depuis le dernier frame
	* @param timeout		Le nombre de secondes qui doivent être écoulées avant de
	*						jouer le prochain son
	* @param priority		Joue le son quelque soit le temps depuis le dernier son
	* 
	* @note		Vérifier que les sons sont activés avant d'appeler cette méthode
	* 			pour éviter de surcharger inutilement la mémoire.
	*/
	bool PlayStep(const BlockType type, float elapsedTime, float timeout, bool priority = false);
	void TestSon();
private:
	Foots GetFootType(BlockType type) const;
	bool LoadFootSteps(const Foots type, const std::string filename);

	float m_stepTmr;
	unsigned short m_footStep;

	unsigned short m_trackNumber;

	std::string* m_musicList;
	sf::Music m_music;

	sf::SoundBuffer* m_footSteps;

	sf::SoundBuffer* m_sndBuffers;
	sf::Sound* m_sndChannels;

	//Tests
	unsigned short m_soundStep;
};
#endif
