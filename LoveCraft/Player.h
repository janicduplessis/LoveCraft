#ifndef PLAYER_H__
#define PLAYER_H__

#include "vector3.h"
#include "vector2.h"
#include "model.h"
#include "define.h"
#include "ArrayBool.h"

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

	/**
	* Rotation du joueur
	* @return Vector2f
	*/
	Vector2f Rotation() const;

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
	void Move(Array<bool>& controls, bool ghost, float elapsedTime );

	void Render (bool wireFrame = false);
	void SetRotation( Vector2f rot );
private:
	Vector3f m_pos; // Position du joueur
	Vector2f m_rot; // Rotation en x et y;

	Model m_model;
};

#endif // PLAYER_H__