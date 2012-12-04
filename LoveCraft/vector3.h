#ifndef VECTOR_3_H__
#define VECTOR_3_H__

#include "define.h"

/**
 * @brief Template class pour manipuler des vecteurs 3d
 */
template <class T>
class Vector3 : public sf::Vector3<T>
{
public:
    Vector3(const T& x = 0, const T& y = 0, const T& z = 0);

	Vector3<T> Cross(const Vector3<T>& v) const;
	T Dot(const Vector3<T>& v) const;
};

typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;


template <class T>
Vector3<T>::Vector3(const T& x, const T& y, const T& z) : sf::Vector3<T>(x, y, z)
{
	
}

template <class T>
Vector3<T> Vector3<T>::Cross(const Vector3<T>& v) const
{
	return Vector3<T>(this->y * v.z - v.y * this->z,
		this->z * v.x - v.z * this->x,
		this->x * v.y - v.x * this->y);
}

template <class T>
T Vector3<T>::Dot(const Vector3<T>& v) const
{
	return this->x * v.x + this->y * v.y + this->z * v.z;
}

#endif
