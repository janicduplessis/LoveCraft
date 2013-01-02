#ifndef OPTIONS_H_
#define OPTIONS_H_



class Option
{
public:
	Option();
	~Option();
	/**
	* Obtient la valeur indiquant si la musique est active
	*/
	bool GetOptMusic() const;
	/**
	* Obtient la valeur indiquant si le son est actif
	*/
	bool GetOptSound() const;
	/**
	* Définit la valeur indiquant si la musique est active
	*/
	void SetOptMusic(const bool value);
	/**
	* Définit la valeur indiquant si le son est actif
	*/
	void SetOptSound(const bool value);
private:
	bool m_music;
	bool m_sound;
	bool m_showInfo;
};

#endif