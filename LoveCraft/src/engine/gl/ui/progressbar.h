#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

#include "control.h"

class ProgressBar : public Control
{
public:
	/**
	* Les diff�rents modes disponibles pour la progressbar
	* BARMODE_HORIZONTAL_RTL et BARMODE_VERTICAL_UTD ne fonctionnent pas correctement
	* BARMODE_CIRCLE_CLKW et BARMODE_CIRCLE_ACLKW non impl�ment�s
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
	* Constructeur par d�faut de la classe
	*/
	ProgressBar();
	/**
	* Constructeur de la classe
	* 
	* @param size		La taille de la barre Largeur x Hauteur
	* @param position	La position du coin inf�rieur gauche du contr�le
	* @param mode		Le mode d'affichage de la barre
	* @param parent		Le panel dans lequel est emmagasin� le controle
	*/
	ProgressBar(Vector2i parent, Vector2i &position, Vector2i &size, Texture* textureFront, Texture* textureBack, BarMode mode, const bool back, const std::string& name);
	/**
	* Destructeur par d�faut de la classe
	*/
	~ProgressBar();
	/**
	* � appeler pour dessiner le contr�le
	* 
	* @param back	Texture qui sera affich�e en arri�re plan de la barre
	* @param front	Texture qui sera affich�e au premier plan de la barre
	*/
	void Render(Texture* back, Texture* front);
	void Render();
	/**
	* Obtient le mode d'affichage de la barre
	*/
	BarMode Mode() const;
	/**
	* D�finit la valeur de la barre entre son minimum et son maximum
	* 
	* @param value	La valeur � attribuer
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
	* Obtient la valeur indiquant si le fond des bars est affich�
	* 
	* @return bool
	*/
	bool GetBeckground() const;
	/**
	* D�finit la valeur indiquant si le fond des bars est affich�
	*/
	void SetBackgroundTo(const bool value);
	ProgressBar& operator=(const ProgressBar& pgb);

private:
	void RenderSquare(const Vector2i& position, const Vector2i& size, Texture* texture);
	void RenderCircle(const Vector2i& origin, float rayon, Texture* texture, bool repeat = true);
	void ApplyRotationTransformation(BarMode type, Vector2i size) const;
	bool m_background;
	BarMode m_mode;
	Texture* m_textureBack;
	float m_minimum;
	float m_maximum;
	float m_value;
};

#endif