#ifndef PLAYER_H__
#define PLAYER_H__

#include "define.h"

#include "controls.h"
#include "gl/model.h"

#include "util/vector2.h"
#include "util/vector3.h"

/**
 * @brief classe repr�sentent le joueur dans l'environnement 3D
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
	 * Destructeur par d�faut de la classe
	 */
	~Player();

	void Init();

	/**
	 * Position du joueur
	 * @return Vector3f
	 */
	Vector3f Position() const;

	/**
	 * Rotation du joueur
	 * @return Vector2f
	 */
	Vector2f Rotation() const;

	/**
	* Vitesse du joueur
	* @return Vector3f
	*/
	Vector3f Speed() const;

	/**
	* Acc�l�ration du joueur
	* @return Vector3f
	*/
	Vector3f Acceleration() const;

	/**
	* Applique les valeurs de l'angle de rotation sur l'axe des Y
	* 
	* @param value Valeur de l'angle � modifier
	*/
	void TurnLeftRight ( float value );

	/**
	 * Applique les valeurs de l'angle de rotation sur l'axe des X
	 * 
	 * @param value Valeur de l'angle � modifier
	 */
	void TurnTopBottom ( float value );

	/**
	 * Fait bouger le personnage en fonction des touches enfonc�es
	 * 
	 * @param controls		Le Array de touches
	 * @param elapsedTime	Temps �coul� depuis le dernier loop de jeu
	 */
	void Move(bool ghost, float &health, float &energy, float elapsedTime);

	void Render (bool wireFrame = false);
	void SetRotation( Vector2f rot );

private:
	bool CheckCollision(const Vector3f& pos) const;

private:
	float m_energy; // Energie pour pouvoir courir (0 � 100)

	Vector3f m_pos; // Position du joueur
	Vector3f m_lastPos; // Position au frame precedent
	Vector2f m_rot; // Rotation en x et y;

	Model m_model;

	Vector3f m_speed;
	Vector3f m_accel;
};

#endif // PLAYER_H__