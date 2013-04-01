#include "camera.h"

Camera::Camera() : m_pos(0, 0, 0), m_target(0, 0, 1), m_up(0, 1, 0)
{
}

Camera::~Camera()
{
}

void Camera::KeyPressEvent(unsigned char key)
{
    // Default camera behavior, do nothing
}

void Camera::KeyReleaseEvent(unsigned char key)
{
    // Default camera behavior, do nothing
}

void Camera::MouseMoveEvent(const MouseEventArgs& e)
{
    // Default camera behavior, do nothing
}

void Camera::MousePressEvent(const MouseEventArgs& e)
{
    // Default camera behavior, do nothing
}

void Camera::MouseReleaseEvent(const MouseEventArgs& e)
{
    // Default camera behavior, do nothing
}

void Camera::WindowFocusEvent(bool hasFocus)
{
    // Default camera behavior, do nothing
}

void Camera::WindowResizeEvent(int width, int height)
{
    // Default camera behavior, do nothing
}

Vector3f Camera::GetPosition() const
{
	return m_pos;
}

Vector3f Camera::GetTarget() const
{
	return m_target;
}

Vector3f Camera::GetUp() const
{
	return m_up;
}



