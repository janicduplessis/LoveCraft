#ifndef MOUSEEVENTARGS_H__
#define MOUSEEVENTARGS_H__

#include "define.h"
#include "util/vector2.h"

typedef uint32 MouseButton;
enum MOUSE_BUTTON {
	MOUSE_BUTTON_NONE       = 0x00,
	MOUSE_BUTTON_LEFT       = 0x01,
	MOUSE_BUTTON_MIDDLE     = 0x02,
	MOUSE_BUTTON_RIGHT      = 0x04,
	MOUSE_BUTTON_WHEEL_UP   = 0x08,
	MOUSE_BUTTON_WHEEL_DOWN = 0x10
};

class MouseEventArgs
{
public:
	MouseEventArgs();

	Vector2i GetPosition() const;
	void SetPosition(const Vector2i& val);

	Vector2i GetLastPosition() const;
	Vector2i GetDeltaPosition() const;

	MouseButton GetMouseButtons() const;
	void SetMouseButtons(const MouseButton& val);

private:
	Vector2i m_position;
	Vector2i m_lastPosition;
	MouseButton m_mouseButtons;
};

#endif