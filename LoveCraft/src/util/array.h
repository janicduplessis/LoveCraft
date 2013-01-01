#ifndef ARRAY_H__
#define ARRAY_H__

#include <cassert>
#include <algorithm>

template <class T>
class Array
{
public:
	Array(unsigned int size, const T& valeur = T());
	Array(const Array& array);
	~Array();

	unsigned int Size() const;
	void Reset(const T& valeur);
	T Get(unsigned int position) const;
	void Set(unsigned int position, const T& valeur);

	void Resize(unsigned int size);

private:
	unsigned int m_size;
	T* m_data;
};

template <class T>
Array<T>::Array(unsigned int size, const T& valeur) : m_size(size)
{
	m_data = new T[size];
	Reset(valeur);
}

template <class T>
Array<T>::Array(const Array& array) : m_size(array.m_size)
{
	m_data = new T[m_size];
	for (unsigned int i = 0; i < m_size; i++)
	{
		m_data[i] = array.m_data[i];
	}
}

template <class T>
Array<T>::~Array()
{
	if (m_data)
		delete [] m_data;
}

template <class T>
unsigned int Array<T>::Size() const
{
	return m_size;
}

template <class T>
void Array<T>::Reset(const T& valeur)
{
	for (unsigned int i = 0; i < m_size; i++)
	{
		m_data[i] = valeur;
	}
}

template <class T>
T Array<T>::Get(unsigned int position) const
{
	assert(position < m_size);
	return m_data[position];
}

template <class T>
void Array<T>::Set(unsigned int position, const T& valeur)
{
	assert(position < m_size);
	m_data[position] = valeur;
}

template <class T>
void Array<T>::Resize(unsigned int size)
{
	T* arr = m_data;
	m_data = new T[size];
	for (int i = 0; i < std::min(size, m_size); i++)
		m_data[i] = arr[i];
	m_size = size;
	delete [] arr;
}

#endif