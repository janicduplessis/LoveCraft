#ifndef THIRDPERSONCAMERA_H__
#define THIRDPERSONCAMERA_H__

#include "camera.h"
#include "player.h"

class ThirdPersonCamera : public Camera
{
    public:
        ThirdPersonCamera(Player* player, float distance);
        virtual ~ThirdPersonCamera();

        virtual void Update(float elapsedTime);
		void SetCamRadius(float m_camRadius);

		virtual void MouseMoveEvent(const MouseEventArgs& e);

    private:
        Player* m_player;
        float m_distance;
};

#endif // TAK_THIRDPERSONCAMERA_H__
