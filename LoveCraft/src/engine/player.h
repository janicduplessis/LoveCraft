#ifndef PLAYER_H__
#define PLAYER_H__

#include "define.h"

#include "controls.h"
#include "gl/modelmesh.h"

#include <util/vector2.h>
#include <util/vector3.h>
#include <util/quaternion.h>

#include "game/character.h"

/**
 * @brief classe représentent le joueur dans l'environnement 3D
 */
class Player
{
public:
	/**
	 * Constructeur de la classe.  Valeurs par defaut a 0
	 * 
	 * @param posX Position initiale du joueur
	 * @param rotY Angle de rotation initial du joueur
	 */
	Player(Vector3f position = Vector3f(), Vector2f rotation = Vector2f());

	/**
	 * Destructeur par défaut de la classe
	 */
	~Player();

	void Init();

	/**
	 * Position du joueur
	 * @return Vector3f
	 */
	Vector3f Position() const;
	void SetPosition(Vector3f pos);

	/**
	 * Rotation du joueur
	 * @return Vector2f
	 */
	Vector2f Rotation() const;
	Quaternion RotationQ() const;

	/**
	* Vitesse du joueur
	* @return Vector3f
	*/
	Vector3f Speed() const;

	/**
	* Accélération du joueur
	* @return Vector3f
	*/
	Vector3f Acceleration() const;

	/**
	* Applique les valeurs de l'angle de rotation sur l'axe des Y
	* 
	* @param value Valeur de l'angle à modifier
	*/
	void TurnLeftRight ( float value );

	/**
	 * Applique les valeurs de l'angle de rotation sur l'axe des X
	 * 
	 * @param value Valeur de l'angle à modifier
	 */
	void TurnTopBottom ( float value );

	/**
	 * Fait bouger le personnage en fonction des touches enfoncées
	 * 
	 * @param controls		Le Array de touches
	 * @param elapsedTime	Temps écoulé depuis le dernier loop de jeu
	 */
	void Move(bool ghost, Character* cter, float elapsedTime);
	void Teleport();
	void Render (bool wireFrame = false);
	void Update();
	void SetRotation( Vector2f rot );
	void ResetPosition();

	Matrix4f GetWorldMatrix();
private:
	bool CheckCollision(const Vector3f& pos) const;
	

private:
	float m_energy; // Energie pour pouvoir courir (0 à 100)

	Vector3f m_pos; // Position du joueur
	Vector3f m_lastPos; // Position au frame precedent
	Vector2f m_rot; // Rotation en x et y;

	ModelMesh m_model;

	Vector3f m_speed;
	Vector3f m_accel;
};

#endif // PLAYER_H__