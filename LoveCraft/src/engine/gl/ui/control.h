#ifndef CONTROL_H_
#define CONTROL_H_

#include "define.h"
#include "../texture.h"
#include "util/vector2.h"
#include <string>
#include <iostream>

enum Image
{
	IMAGE_BOO,
	IMAGE_RUN,
	IMAGE_CROSSHAIR,
	IMAGE_PORTRAIT,
	IMAGE_HEALTH,
	IMAGE_ENERGY,
	IMAGE_MANA,
	IMAGE_LAST
};

class Control
{
public:
	enum Type
	{
		CTRLTYPE_PROGRESSBAR,
		CTRLTYPE_LABEL,
		CTRLTYPE_BOUTON,
		CTRLTYPE_PICTUREBOX,
		CTRLTYPE_PANEL
	};
	/**
	* Constructeur par défaut
	*/
	Control();
	/**
	* Constructeur de la classe
	*
	* @param type	Le type du controle - Voir Enum Type
	*/
	Control(Type type);
	/**
	* Destructeur par défaut
	*/
	virtual ~Control();
	/**
	* Dessine l'objet à l'écran
	*/
	virtual void Render();
	/**
	* Dessine l'objet à l'écran en utilisant une texture
	*/
	virtual void Render(Texture& texture);
	/**
	* Dessine l'objet à l'écran en spécifiant une texture de fond et de devant
	*/
	virtual void Render(Texture& textureBack, Texture& textureFront);
	/**
	* Obtient la valeur indiquant si le controle est visible
	* 
	* @return bool
	*/
	virtual bool Visible() const;
	/**
	* Définit la valeur indiquant si le controle est visible
	*/
	virtual void SetVisible(const bool value);
	/**
	* Obtient la taille du controle
	* 
	* @return Vector2i
	*/
	virtual Vector2i Size() const;
	/**
	* Définit la taille du controle
	*/
	virtual void SetSize(Vector2i value);
	/**
	* Obtient la position relative au conteneur du controle
	* 
	* @return Vector2i
	*/
	virtual Vector2i Position() const;
	/**
	* Définit la position relative au conteneur du controle
	*/
	virtual void SetPosition(Vector2i value);
	/**
	* Obtient la position absolue du controle
	* 
	* @return Vector2i
	*/
	virtual Vector2i AbsolutePosition() const;

protected:
	virtual void RenderSquare(const Vector2i& position, const Vector2i& size);
	virtual void RenderSquare(const Vector2i& position, const Vector2i& size, Texture& texture);

	std::string m_name;
	Type m_type;
	bool m_visible;
	Vector2i m_parentPosition;
	Vector2i m_position;
	Vector2i m_size;
};

#endif