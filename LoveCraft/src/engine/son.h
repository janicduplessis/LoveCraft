#ifndef SON_H_
#define SON_H_

#include "define.h"

class Son
{
public:
	enum Sons
	{
		MUSIC_OVERWORLD,
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
	Son();
	~Son();
	bool LoadSounds();
	bool PlayMusic();
	bool PlaySnd(const Sons& snd);
	bool PlaySnd(const Sons& snd, const Channel& channel, bool aSync = true);
private:
	unsigned short m_stepTmr;

	sf::SoundBuffer* m_sndBuffers;
	sf::Sound* m_sndChannels;
};
#endif
