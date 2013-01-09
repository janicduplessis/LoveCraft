#ifndef LABEL_H_
#define LABEL_H_

#include "interface.h"
#include "control.h"

/**
* Obient D�finit la valeur de
*/

class Label : public Control
{
public:
	/**
	* Affichera le texte � l'endroit sp�cifi� par rapport au conteneur
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
	* Constructeur par d�faut
	*/
	Label();
	Label(Control* parent, const Vector2i& position, Texture* color, const std::string& message, Docking dock, 
		bool italic, float charHeight, float charWidth, float charinterval, const Vector2f& offset, const std::string& name);
	/**
	* Destructeur par d�faut
	*/
	~Label();
	/**
	* D�finit le message qui sera affich� dans le label
	*
	* @param message	Le message � afficher
	*/
	void SetMessage(const std::string& message);
	std::string GetMsg() const;
	/**
	* D�finit la texture de la couleur du texte
	*
	* @param texture	La r�f�rence de la texture repr�sentant la couleur
	*/
	void SetColor(Texture* texture);
	/**
	* Obient la texture de la couleur du texte
	*
	* @return Texture*
	*/
	Texture* GetColor() const;
	/**
	* D�finit la position de l'ancrage lorsque la position du controle est (0, 0)
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
	* D�finit si le texte est �crit en italique
	*
	* @param value		Texte en italic
	*/
	void SetItalic(bool value);
	/**
	* Obient si le texte est �crit en italique
	*
	* @return bool
	*/
	bool GetItalic() const;
	/**
	* D�finit la taille en largeur que chaque lettre aura
	*
	* @param value		Largeur de chaque lettre - D�faut 12.0f
	*/
	void SetFontWidth(float value);
	/**
	* Obient la taille en largeur que chaque lettre a
	*
	* @return float
	*/
	float GetFontWidth() const;
	/**
	* D�finit la taille en hauteur que chaque lettre aura
	*
	* @param value		Hauteur de chaque lettre - D�faut 12.0f
	*/
	void SetFontHeight(float value);
	/**
	* Obient la taille en hauteur que chaque lettre a
	*
	* @return float
	*/
	float GetFontHeight() const;
	/**
	* Obient le Vector correspondant au d�calage
	*
	* @return Vector2f
	*/
	Vector2f GetOffset() const;
	/**
	* D�finit la valeur du d�calage
	*
	* @param offset		Le Vector repr�sentant le d�calage
	*/
	void SetOffset(Vector2f offset);
	/**
	* D�finit la valeur de l'espacement entre les lettres - d�faut 8
	*
	* @param interval	La valeur enti�re repr�sentant le d�calage
	*/
	void SetInterval(unsigned short interval);
	/**
	* D�finit le pourcentage d'espacement entre les lettres - d�faut 66%
	*
	* @param interval	La valeur float repr�sentant le d�calage
	*/
	void SetInterval(float interval);
	/**
	* Dessine le contr�le
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