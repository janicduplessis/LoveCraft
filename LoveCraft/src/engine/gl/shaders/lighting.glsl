#extension GL_EXT_gpu_shader4 : enable

struct VSInput
{
	vec3 Position;
	vec3 TexCoord;
	vec3 Normal;
	vec3 Tangent;
};

interface VSOutput
{
	vec3 TexCoord;
	vec3 Normal;
	vec3 WorldPos;
	vec3 Tangent;
};

struct VSOutput1
{
	vec3 TexCoord;
	vec3 Normal;
	vec3 WorldPos;
	vec3 Tangent;
};


uniform mat4 gWVP;
uniform mat4 gWorld;

shader VSmain(in VSInput VSin:0, out VSOutput VSout)
{
    gl_Position		= gWVP * vec4(VSin.Position, 1.0f);
    VSout.TexCoord	= VSin.TexCoord;
	VSout.Normal	= (gWorld * vec4(VSin.Normal, 0.0f)).xyz;
	VSout.Tangent	= (gWorld * vec4(VSin.Tangent, 0.0)).xyz;
	VSout.WorldPos	= (gWorld * vec4(VSin.Position, 1.0f)).xyz;
}


const int MAX_POINT_LIGHTS = 3;

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
vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, VSOutput1 In)
{
    vec4 AmbientColor = vec4(Light.Color, 1.0f) * Light.AmbientIntensity;
    float DiffuseFactor = dot(In.Normal, -LightDirection);

    vec4 DiffuseColor = vec4(0, 0, 0.f, 0.f);
    vec4 SpecularColor = vec4(0, 0, 0, 0);

    if (DiffuseFactor > 0) {
        DiffuseColor = vec4(Light.Color, 1.0f) * Light.DiffuseIntensity * DiffuseFactor;
        vec3 VertexToEye = normalize(gEyeWorldPos - In.WorldPos);
        vec3 LightReflect = normalize(reflect(LightDirection, In.Normal));
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
vec4 CalcDirectionalLight(VSOutput1 In)
{
	return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, In);
}

// Calculate point light
vec4 CalcPointLight(PointLight l, VSOutput1 In)
{
	vec3 LightDirection = In.WorldPos - l.Position;
	float Distance = length(LightDirection);
	LightDirection = normalize(LightDirection);

	vec4 Color = CalcLightInternal(l.Base, LightDirection, In);
	float Attenuation = l.Atten.Constant + 
						l.Atten.Linear * Distance +
						l.Atten.Exp * Distance * Distance;

	return Color / Attenuation;
}

// Calculate bumb map
vec3 CalcBumpedNormal(VSOutput1 In)
{
    vec3 Tangent = normalize(In.Tangent - dot(In.Tangent, In.Normal) * In.Normal);
    vec3 Bitangent = cross(Tangent, In.Normal);
    vec3 BumpMapNormal = texture(gNormalSampler, In.TexCoord.xy).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(Tangent, Bitangent, In.Normal);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}

shader FSmain(in VSOutput FSin, out vec4 FragColor)
{
	VSOutput1 In;
	In.TexCoord = FSin.TexCoord;
	In.Normal = normalize(FSin.Normal);
	In.WorldPos = FSin.WorldPos;
	In.Tangent = normalize(FSin.Tangent);

	// TODO 2 different programs
	vec4 texel = vec4(0,0,0,0);
	if (gSamplerType == 0) {
		texel = texture2D(gColorSampler, In.TexCoord.xy);
	} else {
		texel = texture2DArray(gArraySampler, In.TexCoord);
	}

	// TODO find better way for transparency
	if (texel.x == 0.f && texel.y == 0.f && texel.z == 0.f) {
        discard;
    }

	vec3 Normal = CalcBumpedNormal(In);
	vec4 TotalLight = CalcDirectionalLight(In);

	for (int i = 0; i < gNumPointLights; i++) {
		TotalLight += CalcPointLight(gPointLights[i], In);
	}

	// Final Color
   	FragColor = texel * TotalLight;
}

program Lighting
{
	vs(330)=VSmain();
	fs(330)=FSmain();
};