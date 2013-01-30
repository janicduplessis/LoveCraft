#ifndef STRUCTURE_H__
#define STRUCTURE_H__



class Structure
{
public:
	Structure();
	virtual ~Structure();
protected:
	bool m_initialized;
	void Init();
};

#endif