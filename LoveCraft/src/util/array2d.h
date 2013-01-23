#ifndef ARRAY2D_H__
#define ARRAY2D_H__

#include <cassert>
#include <vector>
#include "util/vector2.h"

/**
* @brief Template class tableau 2 dimentions
*/
template <class T>
class Array2d
{
public:
	Array2d(uint32 x, uint32 y);
	Array2d(const Array2d& array);
	~Array2d();

	void Set(uint32 x, uint32 y, const T& value);
	T& Get(uint32 x, uint32 y);

	/**
	* Assigne une valeur a tout le tableau
	* @param value		valeur assignée
	*/
	void Reset(const T& value);

	Vector2i Size();

private:
	T* m_values;
	Vector2i m_size;
};

template <class T>
Vector2i Array2d<T>::Size()
{
	return m_size;
}

template <class T>
Array2d<T>::Array2d(uint32 x, uint32 y) : m_size(x, y)
{
	m_values = new T[m_size.x * m_size.y];
}

template <class T>
Array2d<T>::Array2d(const Array2d& array)
{
	m_size = array.m_size;

	m_values = new T[m_size.x * m_size.y];
	for(unsigned int i = 0; i < m_size.x * m_size.y; ++i)
		m_values[i] = array.m_values[i];
}


template <class T>
Array2d<T>::~Array2d()
{
	delete [] m_values;
}

template <class T>
void Array2d<T>::Set(uint32 x, uint32 y, const T& value)
{
	assert(x < m_size.x && y < m_size.y);
	m_values[x + (y * m_size.x)] = value;
}

template <class T>
T& Array2d<T>::Get(uint32 x, uint32 y)
{
	assert(x < m_size.x && y < m_size.y);
	return m_values[x + (y * m_size.x)];
}

template <class T>
void Array2d<T>::Reset(const T& value)
{
	for(uint32 i = 0; i < m_size.x * m_size.y; ++i)
		m_values[i] = value;
}


#endif // ARRAY2D_H__