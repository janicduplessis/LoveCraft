#ifndef CAMERA_H__
#define CAMERA_H__

#include "vector2.h"
#include "vector3.h"

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
	 * Applique la rotation de la cam�ra en fonction des donn�es de
	 * positionnement et d'angle du joueur. � appeler apr�s la d�claration
	 * de la matrice identit�e principale
	 */
	void ApplyRotation () const ;

	/**
	 * Applique la translation du joueur en fonction des donn�es de
	 * positionnement et d'angle du joueur. � appeler apr�s la d�claration
	 * de la matrice identit�e principale
	 */
	void ApplyTranslation () const ;

	Mode GetMode() const;
	void SetMode(Mode mode);

	~Camera();

private:
	Vector3f m_pos;
	Vector2f m_rot;

	Mode m_mode;
};

#endif // !CAMERA_H__
