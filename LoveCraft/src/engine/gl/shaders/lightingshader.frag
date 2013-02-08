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
in vec3 Tangent0;

out vec4 FragColor;

uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gMatSpecularPower;
uniform DirectionalLight gDirectionalLight;
uniform int gNumPointLights;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];

uniform sampler2DArray gArraySampler;
uniform sampler2D gColorSampler;
uniform sampler2D gNormalSampler;
uniform int gSamplerType;


// Common light calculations
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

// Calculate directional light
vec4 CalcDirectionalLight(vec3 Normal)
{
	return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, Normal);
}

// Calculate point light
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

// Calculate bumb map
vec3 CalcBumpedNormal()
{
    vec3 Normal = normalize(Normal0);
    vec3 Tangent = normalize(Tangent0);
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
    vec3 Bitangent = cross(Tangent, Normal);
    vec3 BumpMapNormal = texture(gNormalSampler, TexCoord0.xy).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(Tangent, Bitangent, Normal);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}

void main()
{
	// TODO 2 different programs
	vec4 texel = vec4(0,0,0,0);
	if (gSamplerType == 0) {
		texel = texture2D(gColorSampler, TexCoord0.xy);
	} else {
		texel = texture2DArray(gArraySampler, TexCoord0);
	}

	// TODO find better way for transparency
	if (texel.x == 0.f && texel.y == 0.f && texel.z == 0.f) {
        discard;
    }

	vec3 Normal = CalcBumpedNormal();
	vec4 TotalLight = CalcDirectionalLight(Normal);

	for (int i = 0; i < gNumPointLights; i++) {
		TotalLight += CalcPointLight(i, Normal);
	}

	// Final Color
   	FragColor = texel * TotalLight;
}