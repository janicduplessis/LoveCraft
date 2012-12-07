#ifndef	PROJECTILE_H__
#define PROJECTILE_H__

#include "vector3.h"
#include "quaternion.h"

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
	 * Vitesse Initiale
	 */
	void SetInitialSpeed(const Vector3f& iniSpeed);
	Vector3f GetInitialSpeed() const;
	
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

private:
	Vector3f m_speed;
	Vector3f m_speedIni;
	Vector3f m_pos;
	Vector3f m_posIni;
	Vector3f m_acceleration;

	Quaternion m_rotation;
	
	Vector3f m_destination;
	float m_timeToLive;

	bool m_hasMass;
	bool m_shot;
};

#endif