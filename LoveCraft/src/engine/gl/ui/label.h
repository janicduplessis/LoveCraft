#ifndef LABEL_H_
#define LABEL_H_

#include "interfaceinfos.h"
#include "control.h"

/**
* Obient Définit la valeur de
*/

class Label : public Control
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
	Label(Control* parent, const Vector2i& position, Texture* color, const std::string& message, Docking dock, 
		bool italic, float charHeight, float charWidth, float charinterval, const Vector2f& offset, const std::string& name);
	/**
	* Destructeur par défaut
	*/
	~Label();

	void SetProperty(PropBool boolprop, bool value);
	void SetProperty(PropVector2 vector2prop, Vector2i value);
	void SetProperty(PropString stringprop, std::string value);
	void SetProperty(PropTexture textureprop, Texture* value);
	void SetProperty(PropFloat floatprop, float value);
	void SetProperty(PropDock dockprop, Docking value);

	bool GetProperty(PropBool boolprop) const;
	Vector2i GetProperty(PropVector2 vector2prop) const;
	std::string GetProperty(PropString stringprop) const;
	Texture* GetProperty(PropTexture textureprop) const;
	float GetProperty(PropFloat floatprop) const;
	Docking GetProperty(PropDock dockprop) const;

	/**
	* Dessine le contrôle
	*/
	void Render();
private:
	Vector2f GetNewPosition(unsigned short length);

	std::string m_message;
	Docking m_docking;
	bool m_italic;
	float m_fontWidth;
	float m_fontHeight;
	Vector2f m_offset;
	float m_charInterval;
	Vector2f m_newPosition;
};

#endif