#ifndef LABEL_H_
#define LABEL_H_

#include "interfaceinfos.h"
#include "control.h"
#include "itext.h"

/**
* Obient Définit la valeur de
*/

class Label : public Control, public IText
{
public:
	/**
	* Affichera le texte à l'endroit spécifié par rapport au conteneur
	*/
	enum Docking
	{
		TEXTDOCK_NONE,
		TEXTDOCK_TOPLEFT,
		TEXTDOCK_TOPCENTER,
		TEXTDOCK_TOPRIGHT,
		TEXTDOCK_MIDDLELEFT,
		TEXTDOCK_MIDDLECENTER,
		TEXTDOCK_MIDDLERIGHT,
		TEXTDOCK_BOTTOMLEFT,
		TEXTDOCK_BOTTOMCENTER,
		TEXTDOCK_BOTTOMRIGHT,
		TEXTDOCK_LAST
	};
	enum PropDock
	{
		PROPDOCK_DOCKING
	};

	/**
	* Constructeur par défaut
	*/
	Label();
	/**
	* Destructeur par défaut
	*/
	~Label();
	void Init(Docking dock, const Vector2f& offset);
	void SetProperty(PropBool boolprop, bool value);
	void SetProperty(PropVector2 vector2prop, Vector2i value);
	void SetProperty(PropString stringprop, string value);
	void SetProperty(PropTexture textureprop, Texture* value);
	void SetProperty(PropFloat floatprop, float value);
	void SetProperty(PropDock dockprop, Docking value);

	bool GetProperty(PropBool boolprop) const;
	Vector2i GetProperty(PropVector2 vector2prop) const;
	string GetProperty(PropString stringprop) const;
	Texture* GetProperty(PropTexture textureprop) const;
	float GetProperty(PropFloat floatprop) const;
	Docking GetProperty(PropDock dockprop) const;

	/**
	* Dessine le contrôle
	*/
	void Render();
private:
	Vector2f GetNewPosition(unsigned short length);
	Docking m_docking;
	Vector2f m_offset;
	Vector2f m_newPosition;
};

#endif