#ifndef LOCALIZABLE_H__
#define LOCALIZABLE_H__

#include "../control.h"
#include "../theme.h"

class Localizable : public Control
{
public:
	Localizable(CONTROLTYPE type);
	~Localizable();
	/**
	* Dessine l'objet à l'écran
	*/
	virtual void Render() = 0;
	virtual void InitLocalizable(Point position, Size size, Texture* background, Localizable* parent, ORIGIN origin = ORIGIN_BOTTOMLEFT);

	virtual void SetPosition(Point position);
	virtual void AddPosition(Point value);
	virtual Point GetPosition() const;
	virtual bool IsPosition(Point position) const;
	virtual Point AbsolutePosition() const;

	virtual void SetOrigin(ORIGIN origin);
	virtual ORIGIN GetOrigin() const;
	virtual bool IsOrigin(ORIGIN origin) const;

	virtual void SetSize(Size size);
	virtual void AddSize(Size value);
	virtual Size GetSize() const;
	virtual bool IsSize(Size size) const;

	virtual void SetBlend(BLENDTYPE btype);
	virtual BLENDTYPE GetBlend() const;
	virtual bool IsBlend(BLENDTYPE btype) const;

	virtual void Hide();
	virtual void Show();
	virtual bool IsVisible() const;

	virtual void SetBackground(Texture* texture);
	virtual void RemoveBackground();
	virtual Texture* GetBackground() const;
	virtual bool IsBackground(Texture* texture);

	virtual Localizable* GetParent() const;

	virtual bool MousePressEvents( int x, int y );
	virtual bool MouseReleaseEvents(int x, int y);
	virtual bool KeyPressEvents(int keycode);
	virtual bool KeyReleaseEvents(int keycode);
protected:
	virtual void DrawSquare();
	virtual void DrawingActivateBlend() const;
	virtual void DrawingBindTexture();
	virtual void DrawingGetGraphicReady() const;
	virtual void DrawingDrawSquare() const;
	virtual void DrawingDesactivateBlend() const;

	Localizable* m_parent;
	Point m_position;
	ORIGIN m_origin;
	Size m_size;
	BLENDTYPE m_blend;
	bool m_visible;
	Texture* m_background;
};

#endif