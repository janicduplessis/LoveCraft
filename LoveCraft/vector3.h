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
};

typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;


template <class T>
Vector3<T>::Vector3(const T& x, const T& y, const T& z) : sf::Vector3<T>(x, y, z)
{
	
}

#endif
