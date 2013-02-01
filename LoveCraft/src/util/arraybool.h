#ifndef ARRAYBOOL_H__
#define ARRAYBOOL_H__

#include "util/array.h"

template <>
class Array<bool>
{
public:
	Array(unsigned int size, const bool& valeur = bool());
	Array(const Array<bool>& array);
	~Array();

	unsigned int Size() const;
	void Reset(const bool& valeur);
	bool Get(unsigned int position) const;
	void Set(unsigned int position, const bool& valeur);

	void Resize(unsigned int size);

private:
	unsigned int m_size;
	unsigned char* m_data;

	unsigned int TailleReelle(unsigned int taille);
};


inline Array<bool>::Array(unsigned int size, const bool& valeur) : m_size(size)
{
	m_data = new unsigned char[TailleReelle(size)];
	Reset(valeur);
}

inline Array<bool>::Array(const Array<bool>& array) : m_size(array.m_size)
{
	m_data = new unsigned char[TailleReelle(m_size)];
	for (unsigned int i = 0; i < TailleReelle(m_size); i++)
	{
		m_data[i] = array.m_data[i];
	}
}

inline Array<bool>::~Array()
{
	if (m_data)
		delete [] m_data;
}

inline unsigned int Array<bool>::Size() const
{
	return m_size;
}

inline void Array<bool>::Reset(const bool& valeur)
{
	for (unsigned int i = 0; i < TailleReelle(m_size); i++)
	{
		m_data[i] = valeur ? 0xff : 0x0;
	}
}

inline bool Array<bool>::Get(unsigned int position) const
{
	if (position >= m_size)
		return false;
	unsigned int positionOctet = position / 8;
	unsigned int positionBit = position % 8;
	return (m_data[positionOctet] & (1 << positionBit)) != 0;
}

inline void Array<bool>::Set(unsigned int position, const bool& valeur)
{
	assert(position < m_size);
	unsigned int positionOctet = position / 8;
	unsigned int positionBit = position % 8;
	if (valeur)
		m_data[positionOctet] |= (1 << positionBit);
	else m_data[positionOctet] &= ~(1 << positionBit);
}

inline void Array<bool>::Resize(unsigned int size)
{
	if (size == m_size)
		return;

	unsigned char* arr = m_data;
	m_data = new unsigned char[TailleReelle(size)];
	for (unsigned int i = 0; i < TailleReelle(std::min(size, m_size)); i++)
		m_data[i] = arr[i];
	m_size = size;
	delete [] arr;
}

inline unsigned int Array<bool>::TailleReelle(unsigned int size)
{
	unsigned int taille = size / 8;
	if (size % 8 != 0)
		taille++;
	return taille;
}

#endif

