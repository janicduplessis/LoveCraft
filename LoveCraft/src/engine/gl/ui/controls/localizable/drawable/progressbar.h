#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

#include "../../../control.h"

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
	* Destructeur par défaut de la classe
	*/
	~ProgressBar();
	void Init(BarMode mode, Texture* bartext, bool showback, unsigned short bordersize, float max = 100);
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

	void SP(PropBool boolprop, bool value);
	void SP(PropFloat floatprop, float value);
	void SP(PropUShort ushortprop, unsigned short value);
	void SP(PropTexture textureprop, Texture* value);

	bool GP(PropBool boolprop) const;
	float GP(PropFloat floatprop) const;
	unsigned short GP(PropUShort ushortprop) const;
	Texture* GP(PropTexture textureprop) const;

private:
	void Render(Texture* back, Texture* front);
	/**
	* Obtient la proportion de la barre qui est remplie
	* 
	* @return float
	*/
	float ValuePerc() const;
	void RenderSquare(const Vector2i& position, const Vector2i& size, Texture* texture, bool repeat = true);
	void RenderCircle(const Vector2i& origin, float rayon, Texture* texture, bool repeat = true);
	void ApplyRotationTransformation(BarMode type, Vector2i size) const;
	bool m_background;
	unsigned short m_borderSize;
	BarMode m_mode;
	Texture* m_textureBar;
	float m_minimum;
	float m_maximum;
	float m_value;
};

#endif