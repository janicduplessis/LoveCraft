#ifndef OBJECT_H__
#define OBJECT_H__

#include <string>

class Object
{
public:
	Object();
	~Object();
	std::string GetName();
protected:
	std::string m_name;
};

#endif