#ifndef	PROJECTILE_H__
#define PROJECTILE_H__

#include "util/vector3.h"
#include "util/quaternion.h"

/**
 * @brief Classe de projectile aka exam de physique
 */
class Projectile
{
public:
	Projectile();
	virtual ~Projectile();

	/**
	 * Deplace le projectile en fonction de ses parametres
	 * et du temps depuis la derniere execution de move
	 */
	virtual void Move(float elapsedTime);

	/**
	 * Lance le projectile
	 */
	virtual void Shoot();
	
	/**
	 * Le projectile atteint sa destination
	 */
	virtual void Hit();

	/**
	 * Vitesse et rotation initiale
	 */
	void Init(float speed, const Quaternion& rot);

	/**
	 * Acceleration
	 */
	void SetAcceleration(const Vector3f& acc);
	Vector3f GetAcceleration() const;

	/**
	 * Vitesse
	 */
	Vector3f GetSpeed() const;

	/**
	 * Position
	 */
	void SetPosition(const Vector3f& pos);
	Vector3f GetPosition() const;

	/**
	 * Destination (0 pour infini)
	 */
	void SetDestination(const Vector3f& dest);
	Vector3f GetDestination() const;

	/**
	 * Temps de vie du projectile
	 */
	void SetTimeToLive(float time);
	float GetTimeToLive() const;

	/**
	 * Si affecté par la gravité
	 */
	void SetHasMass(bool hasMass);
	bool HasMass() const;

	void SetMaxRot( float maxRot );
	void SetCollisionRadius(const Vector3f& rad);
protected:
	Vector3f m_speed;
	float m_speedIni;
	Vector3f m_pos;
	Vector3f m_posIni;
	Vector3f m_acceleration;

	Quaternion m_rot;
	Quaternion m_rotIni;
	
	Vector3f m_destination;
	float m_timeToLive;
	float m_maxRot;
	Vector3f m_collisionRadius;

	bool m_hasMass;
	bool m_shot;
};

#endif