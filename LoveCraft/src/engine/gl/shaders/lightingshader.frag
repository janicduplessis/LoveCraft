#version 330
#extension GL_EXT_gpu_shader4 : enable

struct BaseLight
{
   	vec3 Color;
   	float AmbientIntensity;
	float DiffuseIntensity;
};

struct DirectionalLight
{
	struct BaseLight Base;
	vec3 Direction;
};

struct Attenuation
{
	float Constant;
	float Linear;
	float Exp;
};

struct PointLight
{
	struct BaseLight Base;
	vec3 Position;
	Attenuation Atten;
};

const int MAX_POINT_LIGHTS = 2;

in vec3 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;

out vec4 FragColor;

uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gMatSpecularPower;
uniform DirectionalLight gDirectionalLight;
uniform int gNumPointLights;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];

uniform sampler2DArray gArraySampler;
uniform sampler2D gSampler;
uniform int gSamplerType;


vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal)
{
    vec4 AmbientColor = vec4(Light.Color, 1.0f) * Light.AmbientIntensity;
    float DiffuseFactor = dot(Normal, -LightDirection);

    vec4 DiffuseColor = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);

    if (DiffuseFactor > 0) {
        DiffuseColor = vec4(Light.Color, 1.0f) * Light.DiffuseIntensity * DiffuseFactor;
        vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);
        SpecularFactor = pow(SpecularFactor, gMatSpecularPower);
        if (SpecularFactor > 0) {
            SpecularColor = vec4(Light.Color, 1.0f) *
            gMatSpecularIntensity * SpecularFactor;
        }
    }

    return (AmbientColor + DiffuseColor + SpecularColor);
}

vec4 CalcDirectionalLight(vec3 Normal)
{
	return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, Normal);
}

vec4 CalcPointLight(int Index, vec3 Normal)
{
	vec3 LightDirection = WorldPos0 - gPointLights[Index].Position;
	float Distance = length(LightDirection);
	LightDirection = normalize(LightDirection);

	vec4 Color = CalcLightInternal(gPointLights[Index].Base, LightDirection, Normal);
	float Attenuation = gPointLights[Index].Atten.Constant + 
						gPointLights[Index].Atten.Linear * Distance +
						gPointLights[Index].Atten.Exp * Distance * Distance;

	return Color / Attenuation;
}

void main()
{
	vec4 texel = vec4(0,0,0,0);
	if (gSamplerType == 0) {
		texel = texture2D(gSampler, TexCoord0.xy);
	} else {
		texel = texture2DArray(gArraySampler, TexCoord0);
	}


	//vec4 texel = vec4(0.4,0.4,0.4, 1);
	//if (texel.x == 0.f && texel.y == 0.f && texel.z == 0.f) {
    //    discard;
    //}

	vec3 Normal = normalize(Normal0);
	vec4 TotalLight = CalcDirectionalLight(Normal);

	for (int i = 0; i < gNumPointLights; i++) {
		TotalLight += CalcPointLight(i, Normal);
	}

	// Final Color
   	FragColor = texel * TotalLight;
}