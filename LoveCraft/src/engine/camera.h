#ifndef CAMERA_H__
#define CAMERA_H__

#include "util/vector2.h"
#include "util/vector3.h"
#include "define.h"

class Camera
{
public:
	enum Mode {CAM_FIRST_PERSON, CAM_THIRD_PERSON};

	/**
	 * Constructeur de la classe.  Valeurs par defaut a 0
	 * 
	 * @param posisiton Position initiale de la camera
	 * @param rotation Angle de rotation initial de la camera
	 */
	Camera(Vector3f position = Vector3f(), Vector2f rotation = Vector2f());

	Vector2f GetRotation () const;

	void SetRotation (Vector2f rot);

	/**
	 * Position de la camera
	 * @return Vector3f
	 */
	Vector3f GetPosition() const;

	void SetPosition (Vector3f pos);

	/**
	 * Applique la rotation de la caméra en fonction des données de
	 * positionnement et d'angle du joueur. À appeler après la déclaration
	 * de la matrice identitée principale
	 */
	void ApplyRotation () const ;

	/**
	 * Applique la translation du joueur en fonction des données de
	 * positionnement et d'angle du joueur. À appeler après la déclaration
	 * de la matrice identitée principale
	 */
	void ApplyTranslation () const ;

	Mode GetMode() const;
	void SetMode(Mode mode);

	void TurnTopBottom ( float value );
	void TurnLeftRight ( float value );

	~Camera();
	void SetCamRadius(float radius);
	Vector3f GetRealPosition() const;
private:
	Vector3f m_pos;
	Vector2f m_rot;

	float m_camRadius;

	Mode m_mode;
};

#endif // !CAMERA_H__
