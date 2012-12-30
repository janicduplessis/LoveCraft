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
		BARMODE_HORIZONTAL_LTR,
		BARMODE_HORIZONTAL_RTL,
		BARMODE_VERTICAL_UTD,
		BARMODE_VERTICAL_DTU,
		BARMODE_CIRCLE_CLKW,
		BARMODE_CIRCLE_ACLKW,
		BARMODE_LAST
	};
	ProgressBar();
	ProgressBar(Vector2i &size, Vector2i &position, BarMode mode = BARMODE_HORIZONTAL_LTR);
	~ProgressBar();
	void Render(Texture& back, Texture& front);
	BarMode Mode() const;
	void SetValue(const float value);
	float Value() const;
	float Maximum() const;
	float Minimum() const;
	float ValueWidth() const;
	Vector2i Size() const;
	Vector2i Position() const;
private:
	void RenderSquare(const Vector2i& position, const Vector2i& size, Texture& texture, bool repeat = true);
	void RenderCircle(const Vector2i& origin, float rayon, Texture& texture, bool repeat = true);
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