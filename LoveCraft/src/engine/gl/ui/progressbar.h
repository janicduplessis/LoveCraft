#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

#include "control.h"

class ProgressBar : public Control
{
public:
	/**
	* Les différents modes disponibles pour la progressbar
	* BARMODE_HORIZONTAL_RTL et BARMODE_VERTICAL_UTD ne fonctionnent pas correctement
	* BARMODE_CIRCLE_CLKW et BARMODE_CIRCLE_ACLKW non implémentés
	*/
	enum BarMode
	{
		BARMODE_HORIZONTAL_LTR,
		BARMODE_VERTICAL_DTU,
		BARMODE_HORIZONTAL_RTL,
		BARMODE_VERTICAL_UTD,
		BARMODE_CIRCLE_CLKW,
		BARMODE_CIRCLE_ACLKW,
	};
	/**
	* Constructeur par défaut de la classe
	*/
	ProgressBar();
	/**
	* Constructeur de la classe
	* 
	* @param size		La taille de la barre Largeur x Hauteur
	* @param position	La position du coin inférieur gauche du contrôle
	* @param mode		Le mode d'affichage de la barre
	* @param parent		Le panel dans lequel est emmagasiné le controle
	*/
	ProgressBar(Control* parent, Vector2i &position, Vector2i &size, Texture* textureFront, Texture* textureBack, BarMode mode, bool back, unsigned short border, const std::string& name);
	/**
	* Destructeur par défaut de la classe
	*/
	~ProgressBar();
	/**
	* À appeler pour dessiner le contrôle
	* 
	* @param back	Texture qui sera affichée en arrière plan de la barre
	* @param front	Texture qui sera affichée au premier plan de la barre
	*/
	void Render(Texture* back, Texture* front);
	void Render();
	/**
	* Obtient le mode d'affichage de la barre
	*/
	BarMode Mode() const;
	/**
	* Définit la valeur de la barre entre son minimum et son maximum
	* 
	* @param value	La valeur à attribuer
	* 
	* @return BarMode
	*/
	void SetValue(const float value);
	/**
	* Obtient la valeur courante de la barre
	*/
	float Value() const;
	/**
	* Obtient le maximum que la barre pourra atteindre
	* 
	* @return float
	*/
	float Maximum() const;
	/**
	* Obtient le minimum que la barre pourra atteindre
	* 
	* @return float
	*/
	float Minimum() const;
	/**
	* Obtient la proportion de la barre qui est remplie
	* 
	* @return float
	*/
	float ValuePerc() const;
	/**
	* Obtient la valeur indiquant si le fond des bars est affiché
	* 
	* @return bool
	*/
	bool GetBeckground() const;
	/**
	* Définit la valeur indiquant si le fond des bars est affiché
	*/
	void SetBackgroundTo(const bool value);
	/**
	* Obtient la valeur indiquant la taille de la bordure
	* 
	* @return unsigned short
	*/
	unsigned short GetBordersize() const;
	/**
	* Définit la valeur indiquant la taille de la bordure
	*/
	void SetBorderSize(unsigned short size);

	void SetProperty(PropBool boolprop, bool value);
	void SetProperty(PropVector2 vector2prop, Vector2i value);
	void SetProperty(PropString stringprop, std::string value);
	void SetProperty(PropTexture textureprop, Texture* value);

	bool GetProperty(PropBool boolprop) const;
	Vector2i GetProperty(PropVector2 vector2prop) const;
	std::string GetProperty(PropString stringprop) const;
	Texture* GetProperty(PropTexture textureprop) const;

private:
	void RenderSquare(const Vector2i& position, const Vector2i& size, Texture* texture, bool repeat = true);
	void RenderCircle(const Vector2i& origin, float rayon, Texture* texture, bool repeat = true);
	void ApplyRotationTransformation(BarMode type, Vector2i size) const;
	bool m_background;
	unsigned short m_borderSize;
	BarMode m_mode;
	Texture* m_textureBack;
	float m_minimum;
	float m_maximum;
	float m_value;
};

#endif