#ifndef MATRIX4_H__
#define MATRIX4_H__

#include <string>
#include <sstream>
#include <ostream>

#include <Assimp/types.h>
#include "tool.h"
#include <cassert>

#include "vector3.h"

struct PersProjInfo
{
	float FOV;
	float Width; 
	float Height;
	float zNear;
	float zFar;
};

template <class T>
class Matrix4
{
public:
	typedef T Type;

public:
	static const Matrix4<T> ZERO;
	static const Matrix4<T> IDENTITY;

public:
	Matrix4()
	{        
	}

	// constructor from Assimp matrix
	Matrix4(const aiMatrix4x4& AssimpMatrix)
	{
		m[0][0] = AssimpMatrix.a1; m[0][1] = AssimpMatrix.a2; m[0][2] = AssimpMatrix.a3; m[0][3] = AssimpMatrix.a4;
		m[1][0] = AssimpMatrix.b1; m[1][1] = AssimpMatrix.b2; m[1][2] = AssimpMatrix.b3; m[1][3] = AssimpMatrix.b4;
		m[2][0] = AssimpMatrix.c1; m[2][1] = AssimpMatrix.c2; m[2][2] = AssimpMatrix.c3; m[2][3] = AssimpMatrix.c4;
		m[3][0] = AssimpMatrix.d1; m[3][1] = AssimpMatrix.d2; m[3][2] = AssimpMatrix.d3; m[3][3] = AssimpMatrix.d4;
	}

	Matrix4(const aiMatrix3x3& AssimpMatrix)
	{
		m[0][0] = AssimpMatrix.a1; m[0][1] = AssimpMatrix.a2; m[0][2] = AssimpMatrix.a3; m[0][3] = 0.0f;
		m[1][0] = AssimpMatrix.b1; m[1][1] = AssimpMatrix.b2; m[1][2] = AssimpMatrix.b3; m[1][3] = 0.0f;
		m[2][0] = AssimpMatrix.c1; m[2][1] = AssimpMatrix.c2; m[2][2] = AssimpMatrix.c3; m[2][3] = 0.0f;
		m[3][0] = 0.0f           ; m[3][1] = 0.0f           ; m[3][2] = 0.0f           ; m[3][3] = 1.0f;
	}   

	Matrix4(float a00, float a01, float a02, float a03,
		float a10, float a11, float a12, float a13,
		float a20, float a21, float a22, float a23,
		float a30, float a31, float a32, float a33)
	{
		m[0][0] = a00; m[0][1] = a01; m[0][2] = a02; m[0][3] = a03;
		m[1][0] = a10; m[1][1] = a11; m[1][2] = a12; m[1][3] = a13;
		m[2][0] = a20; m[2][1] = a21; m[2][2] = a22; m[2][3] = a23;
		m[3][0] = a30; m[3][1] = a31; m[3][2] = a32; m[3][3] = a33;        
	}

	void SetZero()
	{
		ZERO_MEM(m);
	}


	Matrix4<T> Transpose() const
	{
		Matrix4<T> n;

		for (unsigned int i = 0 ; i < 4 ; i++) {
			for (unsigned int j = 0 ; j < 4 ; j++) {
				n.m[i][j] = m[j][i];
			}
		}

		return n;
	}


	inline void InitIdentity()
	{
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	}

	inline Matrix4<T> operator*(const Matrix4<T>& Right) const
	{
		Matrix4f Ret;

		for (unsigned int i = 0 ; i < 4 ; i++) {
			for (unsigned int j = 0 ; j < 4 ; j++) {
				Ret.m[i][j] = m[i][0] * Right.m[0][j] +
					m[i][1] * Right.m[1][j] +
					m[i][2] * Right.m[2][j] +
					m[i][3] * Right.m[3][j];
			}
		}

		return Ret;
	}

	Vector3f operator*(const Vector3f& v) const
	{
		Vector3f r;

		r.x = m[0][0]* v.x + m[0][1]* v.y + m[0][2]* v.z + m[0][3]* 1;
		r.y = m[1][0]* v.x + m[1][1]* v.y + m[1][2]* v.z + m[1][3]* 1;
		r.z = m[2][0]* v.x + m[2][1]* v.y + m[2][2]* v.z + m[2][3]* 1;

		return r;
	}

	void Print() const
	{
		for (int i = 0 ; i < 4 ; i++) {
			printf("%f %f %f %f\n", m[i][0], m[i][1], m[i][2], m[i][3]);
		}       
	}

	void InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
	void InitRotateTransform(float RotateX, float RotateY, float RotateZ);
	void InitTranslationTransform(float x, float y, float z);
	void InitCameraTransform(const Vector3f& Target, const Vector3f& Up);
	void InitPersProjTransform(const PersProjInfo& p);

	T* GetInternalValues();
	const T* GetInternalValues() const;

public:
	T m[4][4];
};

typedef Matrix4<int> Matrix4i;
typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;

template <class T>
const Matrix4<T> Matrix4<T>::ZERO = Matrix4<T>(0);

template <class T>
const Matrix4<T> Matrix4<T>::IDENTITY = Matrix4<T>(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1);

template <class T>
void Matrix4<T>::InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ)
{
	m[0][0] = ScaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
	m[1][0] = 0.0f;   m[1][1] = ScaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
	m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = ScaleZ; m[2][3] = 0.0f;
	m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
}

template <class T>
void Matrix4<T>::InitRotateTransform(float RotateX, float RotateY, float RotateZ)
{
	Matrix4f rx, ry, rz;

	const float x = ToRadian(RotateX);
	const float y = ToRadian(RotateY);
	const float z = ToRadian(RotateZ);

	rx.m[0][0] = 1.0f; rx.m[0][1] = 0.0f   ; rx.m[0][2] = 0.0f    ; rx.m[0][3] = 0.0f;
	rx.m[1][0] = 0.0f; rx.m[1][1] = cosf(x); rx.m[1][2] = -sinf(x); rx.m[1][3] = 0.0f;
	rx.m[2][0] = 0.0f; rx.m[2][1] = sinf(x); rx.m[2][2] = cosf(x) ; rx.m[2][3] = 0.0f;
	rx.m[3][0] = 0.0f; rx.m[3][1] = 0.0f   ; rx.m[3][2] = 0.0f    ; rx.m[3][3] = 1.0f;

	ry.m[0][0] = cosf(y); ry.m[0][1] = 0.0f; ry.m[0][2] = -sinf(y); ry.m[0][3] = 0.0f;
	ry.m[1][0] = 0.0f   ; ry.m[1][1] = 1.0f; ry.m[1][2] = 0.0f    ; ry.m[1][3] = 0.0f;
	ry.m[2][0] = sinf(y); ry.m[2][1] = 0.0f; ry.m[2][2] = cosf(y) ; ry.m[2][3] = 0.0f;
	ry.m[3][0] = 0.0f   ; ry.m[3][1] = 0.0f; ry.m[3][2] = 0.0f    ; ry.m[3][3] = 1.0f;

	rz.m[0][0] = cosf(z); rz.m[0][1] = -sinf(z); rz.m[0][2] = 0.0f; rz.m[0][3] = 0.0f;
	rz.m[1][0] = sinf(z); rz.m[1][1] = cosf(z) ; rz.m[1][2] = 0.0f; rz.m[1][3] = 0.0f;
	rz.m[2][0] = 0.0f   ; rz.m[2][1] = 0.0f    ; rz.m[2][2] = 1.0f; rz.m[2][3] = 0.0f;
	rz.m[3][0] = 0.0f   ; rz.m[3][1] = 0.0f    ; rz.m[3][2] = 0.0f; rz.m[3][3] = 1.0f;

	*this = rz * ry * rx;
}

template <class T>
void Matrix4<T>::InitTranslationTransform(float x, float y, float z)
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

template <class T>
void Matrix4<T>::InitCameraTransform(const Vector3f& Target, const Vector3f& Up)
{
	Vector3f N = Target;
	N.Normalize();
	Vector3f U = Up;
	U.Normalize();
	U = U.Cross(N);
	Vector3f V = N.Cross(U);

	m[0][0] = U.x;   m[0][1] = U.y;   m[0][2] = U.z;   m[0][3] = 0.0f;
	m[1][0] = V.x;   m[1][1] = V.y;   m[1][2] = V.z;   m[1][3] = 0.0f;
	m[2][0] = N.x;   m[2][1] = N.y;   m[2][2] = N.z;   m[2][3] = 0.0f;
	m[3][0] = 0.0f;  m[3][1] = 0.0f;  m[3][2] = 0.0f;  m[3][3] = 1.0f;
}

template <class T>
void Matrix4<T>::InitPersProjTransform(const PersProjInfo& p)
{
	const float ar         = p.Width / p.Height;
	const float zRange     = p.zNear - p.zFar;
	const float tanHalfFOV = tanf(ToRadian(p.FOV / 2.0f));

	m[0][0] = 1.0f/(tanHalfFOV * ar); m[0][1] = 0.0f;            m[0][2] = 0.0f;            m[0][3] = 0.0;
	m[1][0] = 0.0f;                   m[1][1] = 1.0f/tanHalfFOV; m[1][2] = 0.0f;            m[1][3] = 0.0;
	m[2][0] = 0.0f;                   m[2][1] = 0.0f;            m[2][2] = (-p.zNear - p.zFar)/zRange ; m[2][3] = 2.0f*p.zFar*p.zNear/zRange;
	m[3][0] = 0.0f;                   m[3][1] = 0.0f;            m[3][2] = 1.0f;            m[3][3] = 0.0;    
}

template <class T>
T* Matrix4<T>::GetInternalValues()
{
	return &m[0][0];
}

template <class T>
const T* Matrix4<T>::GetInternalValues() const
{
	return &m[0][0];
}



/*
template <class T>
class Matrix4
{
public:
	typedef T Type;

public:
	static const Matrix4<T> ZERO;
	static const Matrix4<T> IDENTITY;

public:
	Matrix4();
	Matrix4(const T& v);
	Matrix4(const Matrix4<T>& m);
	Matrix4(const T& m_11, const T& m_12, const T& m_13, const T& m_14, 
		const T& m_21, const T& m_22, const T& m_23, const T& m_24, 
		const T& m_31, const T& m_32, const T& m_33, const T& m_34, 
		const T& m_41, const T& m_42, const T& m_43, const T& m_44);

	Matrix4(const aiMatrix4x4& AssimpMatrix);
	Matrix4(const aiMatrix3x3& AssimpMatrix);

	const T& Get11() const;
	const T& Get12() const;
	const T& Get13() const;
	const T& Get14() const;
	const T& Get21() const;
	const T& Get22() const;
	const T& Get23() const;
	const T& Get24() const;
	const T& Get31() const;
	const T& Get32() const;
	const T& Get33() const;
	const T& Get34() const;
	const T& Get41() const;
	const T& Get42() const;
	const T& Get43() const;
	const T& Get44() const;

	Matrix4<T>& operator=(const Matrix4<T>& m);

	Matrix4<T> operator+(const Matrix4<T>& m) const;
	const Matrix4<T>& operator+=(const Matrix4<T>& m);

	Matrix4<T> operator-(const Matrix4<T>& m) const;
	Matrix4<T> operator-() const;
	const Matrix4<T>& operator-=(const Matrix4<T>& m);

	Matrix4<T> operator*(const Matrix4<T>& m) const;
	Matrix4<T> operator*(const T& v) const;
	const Matrix4<T>& operator*=(const Matrix4<T>& m);
	const Matrix4<T>& operator*=(const T& v);

	Matrix4<T> operator/(const T& v) const;
	const Matrix4<T>& operator/=(const T& v);


	bool operator==(const Matrix4<T>& m) const;
	bool operator!=(const Matrix4<T>& m) const;

	void SetZero();
	void SetIdentity();
	void SetPerspectiveProjection(const T& fov, const T& aspect, const T& nearPlane, const T& farPlane);
	void SetOrthographicProjection(const T& left, const T& right, const T& bottom, const T& top, const T& nearPlane, const T& farPlane);

	void InitPersProjTransform(const PersProjInfo& p)
	{
		const float ar         = p.Width / p.Height;
		const float zRange     = p.zNear - p.zFar;
		const float tanHalfFOV = tanf(ToRadian(p.FOV / 2.0f));

		SetZero();

		m_11 = 1.0f/(tanHalfFOV * ar);	                        
        m_22 = 1.0f/tanHalfFOV;           
        m_33 = (-p.zNear - p.zFar)/zRange ; 
		m_34 = 2.0f*p.zFar*p.zNear/zRange;
        m_43 = 1.0f;              
	}

	void SetLookAt(const Vector3<T>& eyePosition, const Vector3<T>& lookAtPosition, Vector3<T> upVector = Vector3<T>(T(0), T(1), T(0)));

	bool IsZero() const;
	bool IsIdentity() const;

	void InitTranslationTransform(const T& x, const T& y, const T&z);
	void InitRotateTransform(const T& angle, const T& x, const T& y, const T&z);
	void InitRotateTransform(const T& _x, const T& _y, const T& _z);
	void InitScaleTransform(const T& x, const T& y, const T&z);

	Vector3<T> GetTranslation() const;

	const T* GetInternalValues() const;
	T* GetInternalValues();
	std::string ToString(const std::string& lineBegin = "|", const std::string& lineEnd = "|\n") const;
	void Matrix4<T>::InitCameraTransform(const Vector3f& Target, const Vector3f& Up);



private:
	union
	{
		// column-major matrix
		struct { T m_11, m_21, m_31, m_41, m_12, m_22, m_32, m_42, m_13, m_23, m_33, m_43, m_14, m_24, m_34, m_44; };
		T m_values[16];
	};
};

template <class T>
void Matrix4<T>::InitRotateTransform( const T& _x, const T& _y, const T& _z )
{
	Matrix4f rx, ry, rz;

	const float x = ToRadian(_x);
	const float y = ToRadian(_y);
	const float z = ToRadian(_z);

	rx.m_11 = 1.0f; rx.m_12 = 0.0f   ; rx.m_13 = 0.0f    ; rx.m_14 = 0.0f;
	rx.m_21 = 0.0f; rx.m_22 = cosf(x); rx.m_23 = -sinf(x); rx.m_24 = 0.0f;
	rx.m_31 = 0.0f; rx.m_32 = sinf(x); rx.m_33 = cosf(x) ; rx.m_34 = 0.0f;
	rx.m_41 = 0.0f; rx.m_42 = 0.0f   ; rx.m_43 = 0.0f    ; rx.m_44 = 1.0f;

	ry.m_11 = cosf(y); ry.m_12 = 0.0f; ry.m_13 = -sinf(y); ry.m_14 = 0.0f;
	ry.m_21 = 0.0f   ; ry.m_22 = 1.0f; ry.m_23 = 0.0f    ; ry.m_24 = 0.0f;
	ry.m_31 = sinf(y); ry.m_32 = 0.0f; ry.m_33 = cosf(y) ; ry.m_34 = 0.0f;
	ry.m_41 = 0.0f   ; ry.m_42 = 0.0f; ry.m_43 = 0.0f    ; ry.m_44 = 1.0f;

	rz.m_11 = cosf(z); rz.m_12 = -sinf(z); rz.m_13 = 0.0f; rz.m_14 = 0.0f;
	rz.m_21 = sinf(z); rz.m_22 = cosf(z) ; rz.m_23 = 0.0f; rz.m_24 = 0.0f;
	rz.m_31 = 0.0f   ; rz.m_32 = 0.0f    ; rz.m_33 = 1.0f; rz.m_34 = 0.0f;
	rz.m_41 = 0.0f   ; rz.m_42 = 0.0f    ; rz.m_43 = 0.0f; rz.m_44 = 1.0f;

	*this = rz * ry * rx;
}


typedef Matrix4<int> Matrix4i;
typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;

template <class T>
const Matrix4<T> Matrix4<T>::ZERO = Matrix4<T>(0);

template <class T>
const Matrix4<T> Matrix4<T>::IDENTITY = Matrix4<T>(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1);

template <class T>
std::ostream& operator<<(std::ostream& out, const Matrix4<T>& m)
{
	out << m.ToString();
	return out;
}

template <class T>
Matrix4<T>::Matrix4()
{
	SetIdentity();
}

template <class T>
Matrix4<T>::Matrix4(const T& v)
{
	for(int i = 0; i < 16; ++i)
		m_values[i] = v;
}

template <class T>
Matrix4<T>::Matrix4(const Matrix4<T>& m)
{
	for(int i = 0; i < 16; ++i)
		m_values[i] = m.m_values[i];
}

template <class T>
Matrix4<T>::Matrix4(const T& m_11, const T& m_12, const T& m_13, const T& m_14, 
					const T& m_21, const T& m_22, const T& m_23, const T& m_24, 
					const T& m_31, const T& m_32, const T& m_33, const T& m_34, 
					const T& m_41, const T& m_42, const T& m_43, const T& m_44)
{
	this->m_11 = m_11;
	this->m_12 = m_12;
	this->m_13 = m_13;
	this->m_14 = m_14;
	this->m_21 = m_21;
	this->m_22 = m_22;
	this->m_23 = m_23;
	this->m_24 = m_24;
	this->m_31 = m_31;
	this->m_32 = m_32;
	this->m_33 = m_33;
	this->m_34 = m_34;
	this->m_41 = m_41;
	this->m_42 = m_42;
	this->m_43 = m_43;
	this->m_44 = m_44;
}

template <class T>
Matrix4<T>::Matrix4(const aiMatrix4x4& AssimpMatrix)
{
	m_11 = AssimpMatrix.a1; m_12 = AssimpMatrix.a2; m_13 = AssimpMatrix.a3; m_14 = AssimpMatrix.a4;
	m_21 = AssimpMatrix.b1; m_22 = AssimpMatrix.b2; m_23 = AssimpMatrix.b3; m_24 = AssimpMatrix.b4;
	m_31 = AssimpMatrix.c1; m_32 = AssimpMatrix.c2; m_33 = AssimpMatrix.c3; m_34 = AssimpMatrix.c4;
	m_41 = AssimpMatrix.d1; m_42 = AssimpMatrix.d2; m_43 = AssimpMatrix.d3; m_44 = AssimpMatrix.d4;
}

template <class T>
Matrix4<T>::Matrix4(const aiMatrix3x3& AssimpMatrix)
{
	m_11 = AssimpMatrix.a1; m_12 = AssimpMatrix.a2; m_13 = AssimpMatrix.a3; m_14 = 0.f;
	m_21 = AssimpMatrix.b1; m_22 = AssimpMatrix.b2; m_23 = AssimpMatrix.b3; m_24 = 0.f;
	m_31 = AssimpMatrix.c1; m_32 = AssimpMatrix.c2; m_33 = AssimpMatrix.c3; m_34 = 0.f;
	m_41 = 0.f;				m_42 = 0.f;				m_43 = 0.f;				m_44 = 1.f;
}

template <class T>
const T& Matrix4<T>::Get11() const
{
	return m_11;
}

template <class T>
const T& Matrix4<T>::Get12() const
{
	return m_12;
}

template <class T>
const T& Matrix4<T>::Get13() const
{
	return m_13;
}

template <class T>
const T& Matrix4<T>::Get14() const
{
	return m_14;
}

template <class T>
const T& Matrix4<T>::Get21() const
{
	return m_21;
}

template <class T>
const T& Matrix4<T>::Get22() const
{
	return m_22;
}

template <class T>
const T& Matrix4<T>::Get23() const
{
	return m_23;
}

template <class T>
const T& Matrix4<T>::Get24() const
{
	return m_24;
}

template <class T>
const T& Matrix4<T>::Get31() const
{
	return m_31;
}

template <class T>
const T& Matrix4<T>::Get32() const
{
	return m_32;
}

template <class T>
const T& Matrix4<T>::Get33() const
{
	return m_33;
}

template <class T>
const T& Matrix4<T>::Get34() const
{
	return m_34;
}

template <class T>
const T& Matrix4<T>::Get41() const
{
	return m_41;
}

template <class T>
const T& Matrix4<T>::Get42() const
{
	return m_42;
}

template <class T>
const T& Matrix4<T>::Get43() const
{
	return m_43;
}

template <class T>
const T& Matrix4<T>::Get44() const
{
	return m_44;
}


template <class T>
Matrix4<T>& Matrix4<T>::operator=(const Matrix4<T>& m)
{
	if(this != &m)
	{
		for(int i = 0; i < 16; ++i)
			m_values[i] = m.m_values[i];
	}

	return *this;
}

template <class T>
Matrix4<T> Matrix4<T>::operator+(const Matrix4<T>& m) const
{
	return Matrix4<T>(
		m_11 + m.m_11, m_12 + m.m_12, m_13 + m.m_13, m_14 + m.m_14,
		m_21 + m.m_21, m_22 + m.m_22, m_23 + m.m_23, m_24 + m.m_24,
		m_31 + m.m_31, m_32 + m.m_32, m_33 + m.m_33, m_34 + m.m_34,
		m_41 + m.m_41, m_42 + m.m_42, m_43 + m.m_43, m_44 + m.m_44);
}

template <class T>
const Matrix4<T>& Matrix4<T>::operator+=(const Matrix4<T>& m)
{
	*this = *this + m;
	return *this;
}

template <class T>
Matrix4<T> Matrix4<T>::operator-(const Matrix4<T>& m) const
{
	return Matrix4<T>(
		m_11 - m.m_11, m_12 - m.m_12, m_13 - m.m_13, m_14 - m.m_14,
		m_21 - m.m_21, m_22 - m.m_22, m_23 - m.m_23, m_24 - m.m_24,
		m_31 - m.m_31, m_32 - m.m_32, m_33 - m.m_33, m_34 - m.m_34,
		m_41 - m.m_41, m_42 - m.m_42, m_43 - m.m_43, m_44 - m.m_44);
}

template <class T>
Matrix4<T> Matrix4<T>::operator-() const
{
	return Matrix4<T>(
		-m_11, -m_12, -m_13, -m_14,
		-m_21, -m_22, -m_23, -m_24,
		-m_31, -m_32, -m_33, -m_34,
		-m_41, -m_42, -m_43, -m_44);
}

template <class T>
const Matrix4<T>& Matrix4<T>::operator-=(const Matrix4<T>& m)
{
	*this = *this - m;
	return *this;
}

template <class T>
Matrix4<T> Matrix4<T>::operator*(const Matrix4<T>& m) const
{
	return Matrix4<T>(
		m_11 * m.m_11 + m_12 * m.m_21 + m_13 * m.m_31 + m_14 * m.m_41,
		m_11 * m.m_12 + m_12 * m.m_22 + m_13 * m.m_32 + m_14 * m.m_42,
		m_11 * m.m_13 + m_12 * m.m_23 + m_13 * m.m_33 + m_14 * m.m_43,
		m_11 * m.m_14 + m_12 * m.m_24 + m_13 * m.m_34 + m_14 * m.m_44,

		m_21 * m.m_11 + m_22 * m.m_21 + m_23 * m.m_31 + m_24 * m.m_41,
		m_21 * m.m_12 + m_22 * m.m_22 + m_23 * m.m_32 + m_24 * m.m_42,
		m_21 * m.m_13 + m_22 * m.m_23 + m_23 * m.m_33 + m_24 * m.m_43,
		m_21 * m.m_14 + m_22 * m.m_24 + m_23 * m.m_34 + m_24 * m.m_44,

		m_31 * m.m_11 + m_32 * m.m_21 + m_33 * m.m_31 + m_34 * m.m_41,
		m_31 * m.m_12 + m_32 * m.m_22 + m_33 * m.m_32 + m_34 * m.m_42,
		m_31 * m.m_13 + m_32 * m.m_23 + m_33 * m.m_33 + m_34 * m.m_43,
		m_31 * m.m_14 + m_32 * m.m_24 + m_33 * m.m_34 + m_34 * m.m_44,

		m_41 * m.m_11 + m_42 * m.m_21 + m_43 * m.m_31 + m_44 * m.m_41,
		m_41 * m.m_12 + m_42 * m.m_22 + m_43 * m.m_32 + m_44 * m.m_42,
		m_41 * m.m_13 + m_42 * m.m_23 + m_43 * m.m_33 + m_44 * m.m_43,
		m_41 * m.m_14 + m_42 * m.m_24 + m_43 * m.m_34 + m_44 * m.m_44);
}

template <class T>
Matrix4<T> Matrix4<T>::operator*(const T& v) const
{
	return Matrix4<T>(
		m_11 * v, m_12 * v, m_13 * v, m_14 * v,
		m_21 * v, m_22 * v, m_23 * v, m_24 * v,
		m_31 * v, m_32 * v, m_33 * v, m_34 * v,
		m_41 * v, m_42 * v, m_43 * v, m_44 * v);
}

template <class T>
const Matrix4<T>& Matrix4<T>::operator*=(const Matrix4<T>& m)
{
	*this = *this * m;
	return *this;
}

template <class T>
const Matrix4<T>& Matrix4<T>::operator*=(const T& v)
{
	*this = *this * v;
	return *this;
}

template <class T>
Matrix4<T> Matrix4<T>::operator/(const T& v) const
{
	return Matrix4<T>(
		m_11 / v, m_12 / v, m_13 / v, m_14 / v,
		m_21 / v, m_22 / v, m_23 / v, m_24 / v,
		m_31 / v, m_32 / v, m_33 / v, m_34 / v,
		m_41 / v, m_42 / v, m_43 / v, m_44 / v);
}

template <class T>
const Matrix4<T>& Matrix4<T>::operator/=(const T& v)
{
	*this = *this / v;
	return *this;
}

template <class T>
bool Matrix4<T>::operator==(const Matrix4<T>& m) const
{
	for(int i = 0; i < 16; ++i)
		if(m_values[i] != m.m_values[i])
			return false;

	return true;
}

template <class T>
bool Matrix4<T>::operator!=(const Matrix4<T>& m) const
{
	return !(*this == m);
}

template <class T>
void Matrix4<T>::SetZero()
{
	*this = ZERO;
}

template <class T>
void Matrix4<T>::SetIdentity()
{
	*this = IDENTITY;
}

template <class T>
void Matrix4<T>::InitCameraTransform(const Vector3f& Target, const Vector3f& Up)
{
	Vector3f N = Target;
	N.Normalize();
	Vector3f U = Up;
	U.Normalize();
	U = U.Cross(N);
	Vector3f V = N.Cross(U);

	m_11 = U.x;   m_12 = U.y;   m_13 = U.z;   m_14 = 0.0f;
	m_21 = V.x;   m_22 = V.y;   m_23 = V.z;   m_24 = 0.0f;
	m_31 = N.x;   m_32 = N.y;   m_33 = N.z;   m_34 = 0.0f;
	m_41 = 0.0f;  m_42 = 0.0f;  m_43 = 0.0f;  m_44 = 1.0f;
}


template <class T>
void Matrix4<T>::SetPerspectiveProjection(const T& fov, const T& aspect, const T& nearPlane, const T& farPlane)
{
	const float h = T(1) / tan(ToRadian(fov / 2.f));
	T negDepth = nearPlane - farPlane;

	SetZero();

	m_11 = h / aspect;
	m_22 = h;
	m_33 = (farPlane + nearPlane) / negDepth;
	m_34 = T(2) * (nearPlane * farPlane) / negDepth;
	m_43 = -T(1);
}

template <class T>
void Matrix4<T>::SetOrthographicProjection(const T& left, const T& right, const T& bottom, const T& top, const T& nearPlane, const T& farPlane)
{
	m_11 = T(2) / (right - left);
	m_12 = T(0);
	m_13 = T(0);
	m_14 = -(right + left) / (right - left);

	m_21 = T(0);
	m_22 = T(2) / (top - bottom);
	m_23 = T(0);
	m_24 = -(top + bottom) / (top - bottom);

	m_31 = T(0);
	m_32 = T(0);
	m_33 = -T(2) / (farPlane - nearPlane);
	m_34 = -(farPlane + nearPlane) / (farPlane - nearPlane);

	m_41 = T(0);
	m_42 = T(0);
	m_43 = T(0);
	m_44 = T(1);
}

template <class T>
void Matrix4<T>::SetLookAt(const Vector3<T>& eyePosition, const Vector3<T>& lookAtPosition, Vector3<T> upVector)
{
	Vector3f L = lookAtPosition - eyePosition;
	L.Normalize();

	upVector.Normalize();
	Vector3f S = L.Cross(upVector);
	S.Normalize();

	Vector3f U = S.Cross(L);

	Matrix4<T> M;
	M.m_11 = S.x;
	M.m_12 = S.y;
	M.m_13 = S.z;
	M.m_14 = 0;

	M.m_21 = U.x;
	M.m_22 = U.y;
	M.m_23 = U.z;
	M.m_24 = 0;

	M.m_31 = -L.x;
	M.m_32 = -L.y;
	M.m_33 = -L.z;
	M.m_34 = 0;

	M.m_41 = 0;
	M.m_42 = 0;
	M.m_43 = 0;
	M.m_44 = 1.f;


	SetIdentity();
	*this *= M;
	InitTranslationTransform(-eyePosition.x, -eyePosition.y, -eyePosition.z);
}

template <class T>
void Matrix4<T>::InitTranslationTransform(const T& x, const T& y, const T&z)
{
	Matrix4<T> tmp(
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1);

	*this *= tmp;
}

template <class T>
void Matrix4<T>::InitRotateTransform(const T& angle, const T& x, const T& y, const T&z)
{
	// TODO axis (x, y, z) must be normalized...

	T s = sin(DEGTORAD(angle));
	T c = cos(DEGTORAD(angle));
	T ic = T(1) - c;

	Matrix4<T> tmp(
		x * x * ic + c,     y * x * ic + (z*s),  z * x * ic - (y*s),   0,
		x * y * ic - (z*s), y * y * ic + c,      z * y * ic + (x*s),   0,
		x * z * ic + (y*s), y * z * ic - (x*s),  z * z * ic + c,       0,
		0,                  0,                   0,                    1);

	*this *= tmp;
}

template <class T>
void Matrix4<T>::InitScaleTransform(const T& x, const T& y, const T&z)
{
	Matrix4<T> tmp(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1);

	*this *= tmp;
}

template <class T>
Vector3<T> Matrix4<T>::GetTranslation() const
{
	// NOTE: Works only if the matrix doesn't contains scale information (only rotation and translation)
	// Reference: http://www.gamedev.net/topic/397751-how-to-get-camera-position/
	T x = -(m_11 * m_14 + m_21 * m_24 + m_31 * m_34);
	T y = -(m_12 * m_14 + m_22 * m_24 + m_32 * m_34);
	T z = -(m_13 * m_14 + m_23 * m_24 + m_33 * m_34);

	return Vector3<T>(x, y, z);
}

template <class T>
T* Matrix4<T>::GetInternalValues()
{
	return m_values;
}

template <class T>
const T* Matrix4<T>::GetInternalValues() const
{
	return m_values;
}

template <class T>
std::string Matrix4<T>::ToString(const std::string& lineBegin, const std::string& lineEnd) const
{
	std::ostringstream ss;
	ss << lineBegin << m_11 << " " << m_12 << " " << m_13 << " " << m_14 << lineEnd;
	ss << lineBegin << m_21 << " " << m_22 << " " << m_23 << " " << m_24 << lineEnd;
	ss << lineBegin << m_31 << " " << m_32 << " " << m_33 << " " << m_34 << lineEnd;
	ss << lineBegin << m_41 << " " << m_42 << " " << m_43 << " " << m_44 << lineEnd;

	return ss.str();
}

*/

#endif // MATRIX4_H__
