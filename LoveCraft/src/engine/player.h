#ifndef PLAYER_H__
#define PLAYER_H__

#include "define.h"

#include "controls.h"
#include "pipeline.h"
#include "gl/meshes/modelmesh.h"
#include "game/character.h"
#include "gl/shaders/null/bone_null_shader.h"
#include "gl/shaders/shadow/bone_shadow_map_shader.h"
#include "gl/shaders/model/bone_model_shader.h"

#include "util/vector2.h"
#include "util/vector3.h"
#include "util/quaternion.h"

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
	Player(Vector3f position = Vector3f(), Vector3f rotation = Vector3f());

	/**
	 * Destructeur par défaut de la classe
	 */
	~Player();

	void Init(BoneModelShader* modelShader, BoneNullShader* nullShader, BoneShadowMapShader* shadowShader);

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
	Vector3f Rotation() const;
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

	Vector3f Scale() const;
	void Scale(Vector3f val);

	/**
	 * Fait bouger le personnage en fonction des touches enfoncées
	 * 
	 * @param controls		Le Array de touches
	 * @param elapsedTime	Temps écoulé depuis le dernier loop de jeu
	 */
	void Move(bool ghost, Character* cter, float elapsedTime);
	void Teleport();
	void Render(Pipeline p);
	void RenderDepth(Pipeline p);
	void RenderShadowMap(Pipeline p);
	void Update(float gameTime);
	void SetRotation( Vector3f rot );
	void ResetPosition();
	void MouseMoveEvent( const MouseEventArgs& e );
	Matrix4f LanternBoneTrans() const { return m_lanternBoneTrans; }
	Matrix4f World() const { return m_world; }

private:
	bool CheckCollision(const Vector3f& pos) const;
	
private:
	float m_energy;		// Energie pour pouvoir courir (0 à 100)

	Vector3f m_pos;		// Position du joueur
	Vector3f m_lastPos; // Position au frame precedent
	Vector3f m_rot;		// Rotation en x,y,z;
	Vector3f m_scale;

	ModelMesh m_model;

	Vector3f m_speed;
	Vector3f m_accel;

	Matrix4f m_lanternBoneTrans;
	Matrix4f m_world;

	BoneModelShader* m_modelShader;
	BoneNullShader* m_boneNullShader;
	BoneShadowMapShader* m_shadowMapShader;
};

#endif // PLAYER_H__