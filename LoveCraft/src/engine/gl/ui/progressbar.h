#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

#include "define.h"

#include "../texture.h"

#include "util/vector2.h"

class ProgressBar
{
public:
	enum BarMode
	{
		BARMODE_HORIZONTAL_LTR = 0,
		BARMODE_VERTICAL_DTU = 90,
		BARMODE_HORIZONTAL_RTL = 180,
		BARMODE_VERTICAL_UTD = 270,
		BARMODE_CIRCLE_CLKW,
		BARMODE_CIRCLE_ACLKW,
	};
	ProgressBar();
	ProgressBar(Vector2i &size, Vector2i &position, BarMode mode);
	~ProgressBar();
	void Render(Texture& back, Texture& front);
	BarMode Mode() const;
	void SetValue(const float value);
	float Value() const;
	float Maximum() const;
	float Minimum() const;
	float ValuePerc() const;
	Vector2i Size() const;
	Vector2i Position() const;
private:
	void RenderSquare(const Vector2i& position, const Vector2i& size, Texture& texture, bool rotate = false);
	void RenderCircle(const Vector2i& origin, float rayon, Texture& texture, bool repeat = true);
	void ApplyRotationTransformation(BarMode type, Vector2i size) const;
	BarMode m_mode;
	float m_minimum;
	float m_maximum;
	float m_value;
	Texture m_textBack;
	Texture m_textFront;
	Vector2i m_size;
	Vector2i m_position;
};

#endif