#ifndef ARRAY_3D_H__
#define ARRAY_3D_H__

#include <cassert>
#include "vector3.h"

/**
 * @brief Template class tableau 3 dimentions
 */
template <class T>
class Array3d
{
    public:
    Array3d(uint32 x, uint32 y, uint32 z);
    ~Array3d();
    Array3d(const Array3d& array);

	Array3d& operator=(const Array3d& array);

    void Set(uint32 x, uint32 y, uint32 z, const T& value);
    T Get(uint32 x, uint32 y, uint32 z) const;

    void Reset(const T& value);

    private:
        Vector3<uint32> m_size;
        T* m_values;
};

template <class T>
Array3d<T>::Array3d(uint32 x, uint32 y, uint32 z) : m_size(x, y, z)
{
	m_values = new T[m_size.x * m_size.y * m_size.z];
}

template <class T>
Array3d<T>::Array3d(const Array3d& array)
{
	m_size = array.m_size;

	m_values = new T[m_size.x * m_size.y * m_size.z];
	for(unsigned int i = 0; i < m_size.x * m_size.y * m_size.z; ++i)
		m_values[i] = array.m_values[i];
}

template <class T>
Array3d<T>::~Array3d()
{
	delete [] m_values;
}

template <class T>
Array3d<T>& Array3d<T>::operator=( const Array3d& array )
{
	m_size = array.m_size;
	m_values = new T[m_size.x * m_size.y * m_size.z];
	for(unsigned int i = 0; i < m_size.x * m_size.y * m_size.z; ++i)
		m_values[i] = array.m_values[i];
	return *this;
}

template <class T>
void Array3d<T>::Set(uint32 x, uint32 y, uint32 z, const T& value)
{
	assert(x < m_size.x && y < m_size.y && z < m_size.z);
	m_values[x + (z * m_size.x) + (y * m_size.z * m_size.x)] = value;
}

template <class T>
T Array3d<T>::Get(uint32 x, uint32 y, uint32 z) const
{
	assert(x < m_size.x && y < m_size.y && z < m_size.z);
	return m_values[x + (z * m_size.x) + (y * m_size.z * m_size.x)];
}

template <class T>
void Array3d<T>::Reset(const T& value)
{
	for(uint32 i = 0; i < m_size.x * m_size.y * m_size.z; ++i)
		m_values[i] = value;
}

#endif // BLOCKARRAY3D_H__
