#ifndef LABEL_H_
#define LABEL_H_

#include "interface.h"
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
	/**
	* Définit le message qui sera affiché dans le label
	*
	* @param message	Le message à afficher
	*/
	void SetMessage(const std::string& message);
	std::string GetMsg() const;
	/**
	* Définit la texture de la couleur du texte
	*
	* @param texture	La référence de la texture représentant la couleur
	*/
	void SetColor(Texture* texture);
	/**
	* Obient la texture de la couleur du texte
	*
	* @return Texture*
	*/
	Texture* GetColor() const;
	/**
	* Définit la position de l'ancrage lorsque la position du controle est (0, 0)
	*
	* @param dock		La position de l'ancrage
	*/
	void SetDock(Docking dock);
	/**
	* Obient la position de l'ancrage lorsque la position du controle est (0, 0)
	*
	* @return Docking (enum)
	*/
	Docking GetDock() const;
	/**
	* Définit si le texte est écrit en italique
	*
	* @param value		Texte en italic
	*/
	void SetItalic(bool value);
	/**
	* Obient si le texte est écrit en italique
	*
	* @return bool
	*/
	bool GetItalic() const;
	/**
	* Définit la taille en largeur que chaque lettre aura
	*
	* @param value		Largeur de chaque lettre - Défaut 12.0f
	*/
	void SetFontWidth(float value);
	/**
	* Obient la taille en largeur que chaque lettre a
	*
	* @return float
	*/
	float GetFontWidth() const;
	/**
	* Définit la taille en hauteur que chaque lettre aura
	*
	* @param value		Hauteur de chaque lettre - Défaut 12.0f
	*/
	void SetFontHeight(float value);
	/**
	* Obient la taille en hauteur que chaque lettre a
	*
	* @return float
	*/
	float GetFontHeight() const;
	/**
	* Obient le Vector correspondant au décalage
	*
	* @return Vector2f
	*/
	Vector2f GetOffset() const;
	/**
	* Définit la valeur du décalage
	*
	* @param offset		Le Vector représentant le décalage
	*/
	void SetOffset(Vector2f offset);
	/**
	* Définit la valeur de l'espacement entre les lettres - défaut 8
	*
	* @param interval	La valeur entière représentant le décalage
	*/
	void SetInterval(unsigned short interval);
	/**
	* Définit le pourcentage d'espacement entre les lettres - défaut 66%
	*
	* @param interval	La valeur float représentant le décalage
	*/
	void SetInterval(float interval);
	/**
	* Dessine le contrôle
	*/
	void Render();
	Label& operator=(const Label& l);
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