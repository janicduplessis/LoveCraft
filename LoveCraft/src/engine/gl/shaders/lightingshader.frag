#version 330
#extension GL_EXT_gpu_shader4 : enable

in vec3 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;

out vec4 FragColor;

struct DirectionalLight
{
   	 vec3 Color;
   	 float AmbientIntensity;
	 vec3 Direction;
	 float DiffuseIntensity;
};

uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gMatSpecularPower;
uniform DirectionalLight gDirectionalLight;
uniform sampler2DArray gSampler;


void main()
{
	vec4 texel = texture2DArray(gSampler, TexCoord0);
	if (texel.x == 0.f && texel.y == 0.f && texel.z == 0.f) {
        discard;
    }

	// Variables pour calculer le lighting
	vec4 AmbientColor = vec4(gDirectionalLight.Color, 1.0f) * gDirectionalLight.AmbientIntensity;
	vec3 LightDirection = -gDirectionalLight.Direction;
	vec3 Normal = normalize(Normal0);

	// Diffuse Lighting
	float DiffuseFactor = dot(Normal, LightDirection);

	vec4 DiffuseColor = vec4(0,0,0,0);
	vec4 SpecularColor = vec4(0,0,0,0);

	if (DiffuseFactor > 0) {
		DiffuseColor = vec4(gDirectionalLight.Color, 1.0f) * gDirectionalLight.DiffuseIntensity *
					   DiffuseFactor;
		
		// Specular Lighting
		vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);
		vec3 LightReflect = normalize(reflect(gDirectionalLight.Direction, Normal));
		float SpecularFactor = dot(VertexToEye, LightReflect);
		SpecularFactor = pow(SpecularFactor, gMatSpecularPower);
		if (SpecularFactor > 0) {
			SpecularColor = vec4(gDirectionalLight.Color, 1.0f) * gMatSpecularIntensity * SpecularFactor;
		}
	}

	// Final Color
   	FragColor = texel * (AmbientColor + DiffuseColor + SpecularColor);
}