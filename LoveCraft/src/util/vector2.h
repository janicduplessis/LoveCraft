#ifndef VECTOR2_H__
#define VECTOR2_H__

#include <iostream>

/**
* @brief Template class pour manipuler des vecteurs 2d
*/
template <class T>
class Vector2
{
public:
	Vector2();
	Vector2(const T& t);
	Vector2(const T& nx, const T& ny);
	~Vector2();

	T Lenght() const;
	Vector2<T> Normalise();
	Vector2<T> Zero();
	Vector2<T> Abs();

	Vector2<T> operator+(const Vector2<T>& v) const;
	Vector2<T> operator-(const Vector2<T>& v) const;
	Vector2<T> operator-() const;

	Vector2<T> operator+(const T& t) const;
	Vector2<T> operator-(const T& t) const;
	Vector2<T> operator*(const T& t) const;
	Vector2<T> operator/(const T& t) const;

	Vector2<T>& operator=(const Vector2<T>& v);
	Vector2<T>& operator+=(const Vector2<T>& v);
	Vector2<T>& operator-=(const Vector2<T>& v);
	Vector2<T>& operator+=(const T& t);
	Vector2<T>& operator-=(const T& t);
	Vector2<T>& operator*=(const T& t);
	Vector2<T>& operator/=(const T& t);

	bool operator==(const Vector2<T>& v) const;
	bool operator!=(const Vector2<T>& v) const;

	void Afficher() const;

public:
	T x;
	T y;
};

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;

template <class T>
Vector2<T>::Vector2() : x(0), y(0) {}

template <class T>
Vector2<T>::Vector2(const T& t) : x(t), y(t) {}

template <class T>
Vector2<T>::Vector2(const T& nx, const T& ny) : x(nx), y(ny) {}

template <class T>
Vector2<T>::~Vector2() {}

template <class T>
Vector2<T> Vector2<T>::operator+(const T& t) const
{
	return Vector2<T>(x + t, y + t);
}

template <class T>
Vector2<T> Vector2<T>::operator-(const T& t) const
{
	return Vector2<T>(x - t, y - t);
}

template <class T>
Vector2<T> Vector2<T>::operator/( const T& t ) const
{
	return Vector2<T>(x / t, y / t);
}

template <class T>
Vector2<T> Vector2<T>::operator*( const T& t ) const
{
	return Vector2<T>(x * t, y * t);
}

template <class T>
Vector2<T> Vector2<T>::operator-() const
{
	return Vector2<T>(-x, -y);
}

template <class T>
Vector2<T> Vector2<T>::operator-( const Vector2<T>& v ) const
{
	return Vector2<T>(x - v.y, y - v.y);
}

template <class T>
Vector2<T> Vector2<T>::operator+(const Vector2<T>& v) const
{
	return Vector2<T>(x + v.x, y + v.y);
}

template <class T>
Vector2<T>& Vector2<T>::operator=( const Vector2<T>& v )
{
	x = v.x;
	y = v.y;

	return *this;
}

template <class T>
Vector2<T>& Vector2<T>::operator/=( const T& t )
{
	return (*this = *this / t);
}

template <class T>
Vector2<T>& Vector2<T>::operator*=( const T& t )
{
	return (*this = *this * t);
}

template <class T>
Vector2<T>& Vector2<T>::operator-=( const T& t )
{
	return (*this = *this - t);
}

template <class T>
Vector2<T>& Vector2<T>::operator+=( const T& t )
{
	return (*this = *this + t);
}

template <class T>
Vector2<T>& Vector2<T>::operator-=( const Vector2<T>& v )
{
	return (*this = *this - v);
}

template <class T>
Vector2<T>& Vector2<T>::operator+=( const Vector2<T>& v )
{
	return (*this = *this + v);
}

template <class T>
bool Vector2<T>::operator!=( const Vector2<T>& v ) const
{
	return !(*this == v);
}

template <class T>
bool Vector2<T>::operator==( const Vector2<T>& v ) const
{
	return (abs(x - v.x) < FLT_EPSILON && abs(y - v.y) < FLT_EPSILON);
}

template <class T>
Vector2<T> Vector2<T>::Zero()
{
	x = y = 0;
	return *this;
}

template <class T>
Vector2<T> Vector2<T>::Normalise()
{
	T len = Lenght();
	if(len != 0) {
		x /= len;
		y /= len;
	}
	return *this;
}

template <class T>
T Vector2<T>::Lenght() const
{
	return sqrt(x*x + y*y);
}

template <class T>
Vector2<T> Vector2<T>::Abs()
{
	x = abs(x);
	y = abs(y);
	return *this;
}

template <class T>
void Vector2<T>::Afficher() const
{
	std::cout << "( " << x << ", " << y << " )" << std::endl;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const Vector2<T>& vec)
{
	out << "( " << vec.x << ", " << vec.y << " )";
	return out;
}

template <class T>
T Vec2Lenght(T x, T y) {
	return sqrt(x * x + y * y);
}

#endif