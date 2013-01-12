#ifndef TEST_BILL_H__
#define TEST_BILL_H__

#include "util/matrix4.h"
#include "billboardshader.h"
#include "texture.h"
#include "define.h"

class TestBillboard
{
public:
	TestBillboard() {}
	~TestBillboard() {}

	void Init()
	{
		m_technique.Init();
		m_technique.Use();
		m_technique.SetBillboardSize(1);

		m_numPoints = 1;
		Vector3f* points = new Vector3f[m_numPoints];
		points[0] = Vector3f(5,0,5);
		m_texture.Load(TEXTURE_PATH "b_checker.bmp");

		glGenBuffers(1, &m_VB);
		glBindBuffer(GL_ARRAY_BUFFER, m_VB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * m_numPoints, points, GL_STATIC_DRAW);

		delete [] points;

		Shader::Disable();
	}

	void Render(const Matrix4f& VP, const Vector3f& CameraPos)
	{
		m_technique.Use();
		m_technique.SetVP(VP);
		m_technique.SetCameraPosition(CameraPos);

		m_texture.Bind();

		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, m_VB);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0); // position 

		glDrawArrays(GL_POINTS, 0, m_numPoints);

		glDisableVertexAttribArray(0);

		Shader::Disable();
	}

private:
	BillboardShader m_technique;
	Texture m_texture;
	GLuint m_VB;
	uint32 m_numPoints;
};

#endif