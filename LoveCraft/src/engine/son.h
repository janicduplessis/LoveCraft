#ifndef SON_H_
#define SON_H_

#include "define.h"
#include "blockinfo.h"

class Son
{
public:
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
	
	static void PlayMusic();
	static void PlayMusic(Musics music);
	/**
	* Arrête la musique en cours, incrémente la numéro de piste et refait jouer la musique
	*/
	static void PlayNextTrack();
	/**
	* Fait jouer le son demandé dans le canal par défaut
	* 
	* @param snd		Le son à jouer - Voir Enum:Sons
	* @return Succès
	*/
	static void PlaySnd(const Sons& snd);
	/**
	* Fait jouer le son demandé dans le canal demandé
	* 
	* @param snd		Le son à jouer - Voir Enum:Sons
	* @param channel	Le canal dans lequel sera joué le son - Voir Enum:Channel
	* @param aSync		Spécifie si le son ne peut être joué que si aucun autre son
	* 					n'occupe le canal
	* @return Succès
	*/
	static void PlaySnd(const Sons& snd, const Channel& channel, bool aSync = true);
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
	static void PlayStep(const BlockType type, float elapsedTime, float speed, bool priority = false);
	/**
	* Arrête la musique en cours
	*/
	static void StopMusic();
private:
	virtual void EmptyFunction() = 0;
	static void Init();
	static void CheckExistence();
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
	static bool LoadSounds();
	static Foots GetFootType(BlockType type);
	static bool LoadFootSteps(const Foots type, const string filename);

	static float m_stepTmr;
	static uint8 m_footStep;

	static uint8 m_trackNumber;

	static string* m_musicList;
	static sf::Music m_music;

	static sf::SoundBuffer* m_footSteps;
	static sf::SoundBuffer* m_sndBuffers;
	static sf::Sound* m_sndChannels;

	//Tests
	static uint8 m_soundStep;
};
#endif
