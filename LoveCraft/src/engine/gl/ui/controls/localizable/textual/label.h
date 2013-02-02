#ifndef LABEL_H_
#define LABEL_H_

#include "interfaceinfos.h"
#include "../../../control.h"
#include "../itext.h"

/**
* Obient Définit la valeur de
*/

class Label : public Control, public IText
{
public:
	/**
	* Constructeur par défaut
	*/
	Label();
	/**
	* Destructeur par défaut
	*/
	~Label();
	void Init(Docking dock, const Vector2f& offset);

	Vector2i AbsolutePosition() const;

	void SP(PropBool boolprop, bool value);
	void SP(PropVector2 vector2prop, Vector2i value);
	void SP(PropString stringprop, string value);
	void SP(PropTexture textureprop, Texture* value);
	void SP(PropFloat floatprop, float value);
	void SP(PropDock dockprop, Docking value);

	bool GP(PropBool boolprop) const;
	Vector2i GP(PropVector2 vector2prop) const;
	string GP(PropString stringprop) const;
	Texture* GP(PropTexture textureprop) const;
	float GP(PropFloat floatprop) const;
	Docking GP(PropDock dockprop) const;

	/**
	* Dessine le contrôle
	*/
	void Render();
private:
	Docking m_docking;
	Vector2f m_offset;
};

#endif