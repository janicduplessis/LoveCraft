#include "mouseeventargs.h"


MouseEventArgs::MouseEventArgs() : m_mouseButtons(MOUSE_BUTTON_NONE), m_position(0, 0), m_lastPosition(0, 0)
{

}

void MouseEventArgs::SetMouseButtons( const MouseButton& val )
{
	m_mouseButtons = val;
}

MouseButton MouseEventArgs::GetMouseButtons() const
{
	return m_mouseButtons;
}

Vector2i MouseEventArgs::GetLastPosition() const
{
	return m_lastPosition;
}

void MouseEventArgs::SetPosition( const Vector2i& val )
{
	m_lastPosition = m_position;
	m_position = val;
}

Vector2i MouseEventArgs::GetPosition() const
{
	return m_position;
}

Vector2i MouseEventArgs::GetDeltaPosition() const
{
	return m_position - m_lastPosition;
}
