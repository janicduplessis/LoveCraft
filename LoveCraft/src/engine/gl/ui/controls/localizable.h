#ifndef LOCALIZABLE_H__
#define LOCALIZABLE_H__

#include "../control.h"
#include "../theme.h"
#include "../effects/effect.h"

class Container;

class Localizable : public Control
{
public:
	Localizable(CONTROLTYPE type, CONTROLGENERICTYPE gentype);
	~Localizable();
	/**
	* Dessine l'objet à l'écran
	*/
	virtual void Render() = 0;
	virtual void InitLocalizable(Point position, Size size, IMAGE background, Container* parent, ORIGIN origin = ORIGIN_BOTTOMLEFT);

	virtual void SetParent(Container* parent);
	virtual Container* GetParent() const;
	virtual bool IsParent(Container* parent);

	virtual CONTROLGENERICTYPE GetGenType() const;
	virtual bool IsGenType(CONTROLGENERICTYPE gentype) const;

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

	virtual void SetTheme(ThemeSet theme);
	virtual void ApplyTheme(ThemeSet theme);
	virtual void RemoveTheme();
	virtual Theme* GetTheme() const;
	virtual bool IsTheme(ThemeSet theme) const;

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

	virtual void SetTooltipText(const string& text);
	virtual string GetTooltipText() const;
	virtual bool IsTooltipText(const string& text) const;

	virtual void AddEffect(Effect* effect);

	virtual bool MouseMoveEvents(int x, int y);
	virtual bool MousePressEvents(int x, int y);
	virtual bool MouseReleaseEvents(int x, int y);
	virtual bool KeyPressEvents(int keycode);
	virtual bool KeyReleaseEvents(int keycode);

	virtual bool IsWithinRange(int x, int y) const;

protected:
	virtual void DrawSquare();
	virtual void DrawingActivateBlend() const;
	virtual void DrawingBindTexture();
	virtual void DrawingGetGraphicReady() const;
	virtual void DrawingDrawSquare() const;
	virtual void DrawingDesactivateBlend() const;

	CONTROLGENERICTYPE m_gentype;
	Container* m_parent;
	Point m_position;
	ORIGIN m_origin;
	Size m_size;
	Theme* m_theme;
	BLENDTYPE m_blend;
	bool m_visible;
	Texture* m_background;
	string m_tooltipText;

	uint8 m_effectNbr;
	Effect** m_effects;
};

#endif