#ifndef VECTOR_3_H__
#define VECTOR_3_H__

#include "define.h"
#include <iostream>
#include <SFML/Network.hpp>

/**
* @brief Template class pour manipuler des vecteurs 3d
*/
template <class T>
class Vector3
{
public:
	Vector3();
	Vector3(const T& t);
	Vector3(const T& nx, const T& ny, const T& nz);
	~Vector3();

	T Lenght() const;
	void Normalise();
	void Zero();

	T Dot(const Vector3<T>& v) const;
	Vector3<T> Cross(const Vector3<T>& v) const;

	Vector3<T> operator+(const Vector3<T>& v) const;
	Vector3<T> operator-(const Vector3<T>& v) const;
	Vector3<T> operator-() const;

	Vector3<T> operator+(const T& t) const;
	Vector3<T> operator-(const T& t) const;
	Vector3<T> operator*(const T& t) const;
	Vector3<T> operator/(const T& t) const;

	Vector3<T>& operator=(const Vector3<T>& v);
	Vector3<T>& operator+=(const Vector3<T>& v);
	Vector3<T>& operator-=(const Vector3<T>& v);
	Vector3<T>& operator+=(const T& t);
	Vector3<T>& operator-=(const T& t);
	Vector3<T>& operator*=(const T& t);
	Vector3<T>& operator/=(const T& t);

	bool operator==(const Vector3<T>& v) const;
	bool operator!=(const Vector3<T>& v) const;

	//sf::Packet& operator <<(sf::Packet& packet, const Vector3<T>& vector);
	//sf::Packet& operator >>(sf::Packet& packet, Vector3<T>& vector);

	void Afficher() const;

public:
	T x;
	T y;
	T z;
};

typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;


template <class T>
Vector3<T>::Vector3() : x(0), y(0), z(0) {}

template <class T>
Vector3<T>::Vector3(const T& t) : x(t), y(t), z(t) {}

template <class T>
Vector3<T>::Vector3(const T& nx, const T& ny, const T& nz) : x(nx), y(ny), z(nz) {}

template <class T>
Vector3<T> Vector3<T>::operator+(const T& t) const
{
	return Vector3<T>(x + t, y + t, z + t);
}

template <class T>
Vector3<T> Vector3<T>::operator-(const T& t) const
{
	return Vector3<T>(x - t, y - t, z - t);
}

template <class T>
Vector3<T> Vector3<T>::operator/( const T& t ) const
{
	return Vector3<T>(x / t, y / t, z / t);
}

template <class T>
Vector3<T> Vector3<T>::operator*( const T& t ) const
{
	return Vector3<T>(x * t, y * t, z * t);
}

template <class T>
Vector3<T> Vector3<T>::operator-() const
{
	return Vector3<T>(-x, -y, -z);
}

template <class T>
Vector3<T> Vector3<T>::operator-( const Vector3<T>& v ) const
{
	return Vector3<T>(x - v.y, y - v.y, z - v.z);
}

template <class T>
Vector3<T> Vector3<T>::operator+(const Vector3<T>& v) const
{
	return Vector3<T>(x + v.x, y + v.y, z + v.z);
}

template <class T>
Vector3<T>& Vector3<T>::operator=( const Vector3<T>& v )
{
	x = v.x;
	y = v.y;
	z = v.z;

	return *this;
}

template <class T>
Vector3<T>& Vector3<T>::operator/=( const T& t )
{
	return (*this = *this / t);
}

template <class T>
Vector3<T>& Vector3<T>::operator*=( const T& t )
{
	return (*this = *this * t);
}

template <class T>
Vector3<T>& Vector3<T>::operator-=( const T& t )
{
	return (*this = *this - t);
}

template <class T>
Vector3<T>& Vector3<T>::operator+=( const T& t )
{
	return (*this = *this + t);
}

template <class T>
Vector3<T>& Vector3<T>::operator-=( const Vector3<T>& v )
{
	return (*this = *this - v);
}

template <class T>
Vector3<T>& Vector3<T>::operator+=( const Vector3<T>& v )
{
	return (*this = *this + v);
}

template <class T>
bool Vector3<T>::operator!=( const Vector3<T>& v ) const
{
	return !(*this == v);
}

template <class T>
bool Vector3<T>::operator==( const Vector3<T>& v ) const
{
	return (abs(x - v.x) < FLT_EPSILON && abs(y - v.y) < FLT_EPSILON && abs(z - v.z) < FLT_EPSILON);
}

template <class T>
Vector3<T> Vector3<T>::Cross( const Vector3<T>& v ) const
{
	return Vector3<T>(
		y * v.z - v.y * z,
		z * v.x - v.z * x,
		x * v.y - v.x * y );
}

template <class T>
T Vector3<T>::Dot( const Vector3<T>& v ) const
{
	return x * v.x + y * v.y + z * v.z;
}

template <class T>
void Vector3<T>::Zero()
{
	x = y = z = 0;
}

template <class T>
void Vector3<T>::Normalise()
{
	T len = Lenght();
	if(len == 0)
		return;
	x /= len;
	y /= len;
	z /= len;
}

template <class T>
T Vector3<T>::Lenght() const
{
	return sqrt(x*x + y*y + z*z);
}

//template <class T>
//sf::Packet& Vector3<T>::operator <<(sf::Packet& packet, const Vector3<T>& vector)
//{
//	return packet << vector.x << vector.y << vector.z;
//}
//
//template <class T>
//sf::Packet& Vector3<T>::operator >>(sf::Packet& packet, Vector3<T>& vector)
//{
//	return packet >> vector.x << vector.y << vector.z;
//}

template <class T>
void Vector3<T>::Afficher() const
{
	std::cout << "( " << x << ", " << y << ", " << z << " )" << std::endl;
}

template <class T>
Vector3<T>::~Vector3() {}

template <class T>
std::ostream& operator<<(std::ostream& out, const Vector3<T>& vec)
{
	out << "( " << vec.x << ", " << vec.y << ", " << vec.z << " )";
	return out;
}

template <class T>
T Vec3Lenght(T x, T y, T z) {
	return sqrt(x * x + y * y + z * z);
}

#endif
