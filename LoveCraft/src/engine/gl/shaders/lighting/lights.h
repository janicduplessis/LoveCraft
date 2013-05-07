#ifndef LIGHTS_H__
#define LIGHTS_H__

#include "util/vector3.h"
#include "util/matrix4.h"

/**
 * Structure de base d'une light
 */
struct BaseLight
{
	Vector3f Color;
	float AmbientIntensity;
	float DiffuseIntensity;

	float TotalIntensity() {
		return AmbientIntensity + DiffuseIntensity;
	}

	BaseLight() : Color(0), AmbientIntensity(0), DiffuseIntensity(0) {}
};

/**
 * Directional Light
 * Light avec seulement une direction,
 * appliqué également partout
 */
struct DirectionalLight : public BaseLight
{
	Vector3f Direction;

	DirectionalLight() : BaseLight(), Direction(0) {}
};

/**
 * Point Light
 * Light avec une position et une atténuation
 * Son intensité diminue plus on est loin de sa position
 */
struct PointLight : public BaseLight
{
	Vector3f Position;

	struct {
		float Constant;
		float Linear;
		float Exp;
	} Attenuation;

	PointLight() : BaseLight(), Position(0) {
		Attenuation.Constant = 0;
		Attenuation.Linear = 0;
		Attenuation.Exp = 0;
	}
};

struct SpotLight : public PointLight
{
	Vector3f Direction;
	float Cutoff;

	SpotLight() : Direction(0), Cutoff(0) {}
};

#endif