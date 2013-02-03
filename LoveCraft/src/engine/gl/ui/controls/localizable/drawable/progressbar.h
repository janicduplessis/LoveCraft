#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

#include "../drawable.h"
#include "picturebox.h"

class ProgressBar : public Drawable
{
public:
	enum BarMode
	{
		BARMODE_HORIZONTAL_LTR,
		BARMODE_VERTICAL_DTU,
		BARMODE_HORIZONTAL_RTL,
		BARMODE_VERTICAL_UTD,
		BARMODE_CIRCLE_CLKW,
		BARMODE_CIRCLE_ACLKW,
	};
	ProgressBar();
	virtual ~ProgressBar();
	virtual void Init(BarMode mode, Texture* foreground, uint8 bordersize = 0);
	virtual void Render();

	virtual BarMode GetMode() const;
	virtual bool IsMode(BarMode mode) const;

	virtual void SetValue(float value);
	virtual float GetValue() const;
	virtual bool IsValue(float value) const;
	virtual bool IsValue(float first, float second) const;

	virtual void SetForeground(Texture* foreground);
	virtual Texture* GetForeground() const;
	virtual bool IsForeground(Texture* foreground);

private:
	virtual void DrawSquare();
	virtual void DrawingDrawSquareLTR();
	virtual void DrawingDrawSquareDTU();
	virtual float FilledWidth() const;
	virtual float FilledHeight() const;
	virtual float FilledRadian() const;
	virtual void RenderCircle(const Vector2i& origin, float rayon, Texture* texture, bool repeat = true);
	PictureBox* m_backImage;
	uint8 m_borderSize;
	BarMode m_mode;
	Texture* m_foreground;
	float m_minimum;
	float m_maximum;
	float m_value;
};

#endif