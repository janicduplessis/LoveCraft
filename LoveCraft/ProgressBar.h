#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

#include <cmath>
#include <string>
#include "vector2.h"
#include "define.h"
#include <IL/ilu.h>
#include "openglcontext.h"
#include "texture.h"

class ProgressBar
{
public:
	ProgressBar();
	ProgressBar(Vector2i &size, Vector2i &position);
	~ProgressBar();
	void Render(const Vector2i &position);	//Ne fonctionne pas pour l'instant
	void SetValue(const float value);
	float Value() const;
	float Maximum() const;
	float Minimum() const;
	float ValueWidth() const;
	Vector2i Size() const;
	Vector2i Position() const;
private:
	float m_minimum;
	float m_maximum;
	float m_value;
	Vector2i m_size;
	Vector2i m_position;
};

#endif