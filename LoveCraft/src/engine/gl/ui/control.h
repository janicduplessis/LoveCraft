#ifndef CONTROL_H_
#define CONTROL_H_

#include "define.h"
#include "../texture.h"
#include "util/vector2.h"
#include <cassert>
#include <string>
#include <iostream>

/**
 * @Brief Controle de l'interface 2d
 */
class Control
{
public:
	enum Type
	{
		CTRLTYPE_NONE,
		CTRLTYPE_PROGRESSBAR,
		CTRLTYPE_LABEL,
		CTRLTYPE_BOUTON,
		CTRLTYPE_PICTUREBOX,
		CTRLTYPE_PANEL,
		CTRLTYPE_LISTBOX,
		CTRLTYPE_TEXTBOX
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
	* Constructeur de la classe
	*
	* @param type		Le type du controle - Voir Enum Type
	* @param parent		Le controle qui joue le rôle de parent
	* @param position	La poisition initiale du controle par rapport a son parent
	* @param size		La taille du controle
	* @param name		Le nom du controle
	*/
	Control(Type type, Control* parent, Vector2i position, Vector2i size, Texture* texture, const std::string& name);
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
	virtual void Render(Texture* texture);
	/**
	* Obtient le nom du cotnrole
	* 
	* @return std::string
	*/
	virtual std::string Name() const;
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
	/**
	* Définit la texture qui sera utilisée lors du Render
	*/
	virtual void SetTexture(Texture* text);
	/**
	* Obtient la texture qui sera utilisée lors du Render
	* 
	* @return Texture*
	*/
	virtual Texture* GetTexture() const;
	/**
	* Obtient la valeur indiquant si le blend doit être activé lors du Render
	* 
	* @return bool
	*/
	virtual bool GetPngBlend() const;
	/**
	* Définit la valeur indiquant si le blend doit être activé lors du Render
	*/
	virtual void SetPngBlend(const bool value);
	virtual bool GetRepeatTexture() const;
	virtual void SetRepeatTexture(const bool value);

	Control& operator=(const Control& c);

protected:
	virtual void RenderSquare(const Vector2i& position, const Vector2i& size);
	virtual void RenderSquare(const Vector2i& position, const Vector2i& size, Texture* texture);

	std::string m_name;
	Type m_type;
	bool m_pngBlend;
	bool m_repeatTexture;
	bool m_visible;
	Texture* m_texture;
	Control* m_parent;
	Vector2i m_position;
	Vector2i m_size;
};

#endif