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
		CHANNEL_LAST
	};
	Son();
	~Son();
	bool LoadSounds();
	bool PlayMusic();
	bool PlaySnd(const Sons& snd);
	bool PlaySnd(const Sons& snd, const Channel& channel);
private:
	unsigned short m_stepTmr;

	sf::Music m_music;
	sf::SoundBuffer* m_sndBuffers;
	sf::Sound* m_sndChannels;
};
#endif
