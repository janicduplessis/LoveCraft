#ifndef	QUATERNION_H__
#define QUATERNION_H__

#include "matrix4.h"
#include "vector3.h"

/**
 * @brief Quaternion
 * 
 * Sphere en 4 dimentions qui représente un angle (w) de
 * rotation autour d'un axe quelconque (x, y, z)
 * 
 * q = w + xi + yj + zk
 * ou w,x,y,z sont des nombres reels et 
 * i,j,k des nombres imaginaires
 */
class Quaternion 
{
public:
	/**
	 * Quaternion initialisé
	 */
	Quaternion(float w = 1, float x = 0, float y = 0, float z = 0);

	/**
	 * Multiplication de quaternions
	 *
	 * (Q1 * Q2).w = (w1w2 - x1x2 - y1y2 - z1z2)
	 * (Q1 * Q2).x = (w1x2 + x1w2 + y1z2 - z1y2)
	 * (Q1 * Q2).y = (w1y2 - x1z2 + y1w2 + z1x2)
	 * (Q1 * Q2).z = (w1z2 + x1y2 - y1x2 + z1w2
	 */
	Quaternion operator*(const Quaternion& q) const;

	/**
	 * Normalise le quaternion
	 */
	void Normalize();

	/**
	 * Doit etre presque egal a 1 sinon
	 * le quaternion doit etre normalisé
	 */
	float Magnitude();

	/**
	 * Matrice qui represente la rotation du quaternion
	 *
	 * @return La matrice de rotation
	 */
	Matrix4f RotationMatrix();

	/**
	 * Donne une valeur d'angle de rotation sur un axe
	 */
	void SetRotation(float angle, Vector3f axis);

private:
	float m_w;
	float m_x;
	float m_y;
	float m_z;
};

#endif