#ifndef VECTOR2_H__
#define VECTOR2_H__

#include <SFML/System/Vector2.hpp>

/**
 * @brief Template class pour manipuler des vecteurs 2d
 */
template <class T>
class Vector2 : public sf::Vector2<T>
{
public:
	Vector2(const T& x = 0, const T& y = 0);
};

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;


template <class T>
Vector2<T>::Vector2(const T& x, const T& y) : sf::Vector2<T>(x, y)
{

}

#endif