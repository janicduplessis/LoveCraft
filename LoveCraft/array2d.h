#ifndef ARRAY2D_H__
#define ARRAY2D_H__

#include <cassert>
#include "vector2.h"

/**
 * @brief Template class tableau 2 dimentions
 */
template <class T>
class Array2d
{
public:
	Array2d(uint32 x, uint32 y);
	~Array2d();
	Array2d(const Array2d& array);

	void Set(uint32 x, uint32 y, const T& value);
	T Get(uint32 x, uint32 y) const;

	/**
	 * Assigne une valeur a tout le tableau
	 * @param value		valeur assignée
	 */
	void Reset(const T& value);

private:
	Vector2<uint32> m_size;
	T* m_values;
};

template <class T>
Array2d<T>::Array2d(uint32 x, uint32 y) : m_size(x, y)
{
	m_values = new T[m_size.x * m_size.y];
}

template <class T>
Array2d<T>::~Array2d()
{
	delete [] m_values;
}

template <class T>
Array2d<T>::Array2d(const Array2d& array)
{
	m_size = array.m_size;

	m_values = new T[m_size.x * m_size.y];
	for(int i = 0; i < m_size.x * m_size.y; ++i)
		m_values[i] = array.m_values[i];
}

template <class T>
void Array2d<T>::Set(uint32 x, uint32 y, const T& value)
{
	// valide le size
	assert(x < m_size.x && y < m_size.y);
	// assigne la valeur
	m_values[x + (y * m_size.x)] = value;
}

template <class T>
T Array2d<T>::Get(uint32 x, uint32 y) const
{
	// valide le size
	assert(x < m_size.x && y < m_size.y);
	// retourne la valeur
	return m_values[x + (y * m_size.x)];
}

template <class T>
void Array2d<T>::Reset(const T& value)
{
	for(int i = 0; i < m_size.x * m_size.y; ++i)
		m_values[i] = value;
}

#endif // ARRAY2D_H__