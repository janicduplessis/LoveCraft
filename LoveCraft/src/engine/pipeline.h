#ifndef PIPELINE_H
#define	PIPELINE_H

#include "util/matrix4.h"
#include "util/vector3.h"

class Pipeline
{
public:
    Pipeline()
    {
        m_scale      = Vector3f(1.0f, 1.0f, 1.0f);
        m_worldPos   = Vector3f(0.0f, 0.0f, 0.0f);
        m_rotateInfo = Vector3f(0.0f, 0.0f, 0.0f);
    }

    void Scale(float ScaleX, float ScaleY, float ScaleZ)
    {
        Scale(Vector3f(ScaleX, ScaleY, ScaleZ));
    }

	void Scale(const Vector3f& scale)
	{
		m_scale = scale;
	}

    void WorldPos(float x, float y, float z)
    {
        WorldPos(Vector3f(x, y, z));
    }

	void WorldPos(const Vector3f& pos)
	{
		m_worldPos = pos;
	}

    void Rotate(float RotateX, float RotateY, float RotateZ)
    {
        Rotate(Vector3f(RotateX, RotateY, RotateZ));
    }

	void Rotate(const Vector3f& rot)
	{
		m_rotateInfo = rot;
	}

    void SetPerspectiveProj(const PersProjInfo& p)
    {
        m_persProjInfo = p;
    }

    void SetCamera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
    {
        m_camera.Pos = Pos;
        m_camera.Target = Target;
        m_camera.Up = Up;
    }

    const Matrix4f& GetVPTrans();
    const Matrix4f& GetWVPTrans();
    const Matrix4f& GetWorldTrans();

private:
    Vector3f m_scale;
    Vector3f m_worldPos;
    Vector3f m_rotateInfo;

    PersProjInfo m_persProjInfo;

    struct {
        Vector3f Pos;
        Vector3f Target;
        Vector3f Up;
    } m_camera;

	Matrix4f m_WVPtransformation;
	Matrix4f m_VPTtransformation;
	Matrix4f m_WorldTransformation;
};


#endif	/* PIPELINE_H */

