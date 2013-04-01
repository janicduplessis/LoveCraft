#ifndef CAMERA_H__
#define CAMERA_H__

#include "define.h"

#include "util/vector3.h"
#include "util/vector2.h"

#include "mouseeventargs.h"

class Camera
{
    public:
        Camera();
        virtual ~Camera();

        virtual void KeyPressEvent(unsigned char key);
        virtual void KeyReleaseEvent(unsigned char key);
        virtual void MouseMoveEvent(const MouseEventArgs& e);
        virtual void MousePressEvent(const MouseEventArgs& e);
        virtual void MouseReleaseEvent(const MouseEventArgs& e);
        virtual void WindowFocusEvent(bool hasFocus);
        virtual void WindowResizeEvent(int width, int height);

        virtual void Update(float elapsedTime) = 0;
		Vector3f GetPosition() const;
		Vector3f GetTarget() const;
		Vector3f GetUp() const;

    protected:
        Vector3f m_pos;
		Vector3f m_target;
		Vector3f m_up;
};

#endif // CAMERA_H__
