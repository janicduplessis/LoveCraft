#ifndef PLAYER_H__
#define PLAYER_H__

#include "vector3.h"
#include "vector2.h"
#include "model.h"
#include "define.h"

#define MOUSE_SENSIBILITY 0.1f
#define MOUVEMENT_SPEED 0.2f
#define MOUVEMENT_SPEED_RUN 0.35f
#define MOUVEMENT_SPEED_BACKWARD 0.1f
#define PII 3.141592654f

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
	Vector3f GetPosition() const;

	/**
	 * Rotation du joueur
	 * @return Vector2f
	 */
	Vector2f GetRotation() const;

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
	 * @param front			Si la touche pour avancer est enfoncée
	 * @param back			Si la touche pour reculer est enfoncée
	 * @param left			Si la touche pour strafer à gauche est enfoncée
	 * @param right			Si la touche pour strafer à droite est enfoncée
	 * @param run			Si la touche pour courir est enfoncée
	 * @param elapsedTime	Temps écoulé depuis le dernier loop de jeu
	 */
	void Move ( bool front , bool back , bool left , bool right , bool run, bool ghost, float elapsedTime );

	void Render (bool wireFrame = false, bool rotateModel = false);

private:
	Vector3f m_pos; // Position du joueur
	Vector2f m_rot; // Rotation en x et y;

	Model m_model;
};

#endif // PLAYER_H__