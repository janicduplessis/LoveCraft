uniform mat4 gWVP;

shader VSmain(in vec3 Position)
{ 
   	gl_Position = gWVP * vec4(Position, 1.0);
}


struct BaseLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
};

struct DirectionalLight
{
    BaseLight Base;
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
    BaseLight Base;
    vec3 Position;
    Attenuation Atten;
};

struct SpotLight
{
    PointLight Base;
    vec3 Direction;
    float Cutoff;
};

uniform sampler2D gPositionMap;
uniform sampler2D gColorMap;
uniform sampler2D gNormalMap;
uniform sampler2D gShadowMap;
uniform DirectionalLight gDirectionalLight;
uniform PointLight gPointLight;
uniform SpotLight gSpotLight;
uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gSpecularPower;
uniform int gLightType;
uniform vec2 gScreenSize;
uniform mat4 gLightWVP;

float CalcShadowFactor(vec4 LightSpacePos)
{
    vec3 ProjCoords = LightSpacePos.xyz / LightSpacePos.w;
    vec2 UVCoords;
    UVCoords.x = 0.5 * ProjCoords.x + 0.5;
    UVCoords.y = 0.5 * ProjCoords.y + 0.5;
    float z = 0.5 * ProjCoords.z + 0.5;
    float Depth = texture(gShadowMap, UVCoords).x;
    if (Depth < (z + 0.00001))
        return 0.5;
    else
        return 1.0;
}

vec4 CalcLightInternal(BaseLight Light,
					   vec3 LightDirection,
					   vec3 WorldPos,
					   vec3 Normal,
					   float ShadowFactor)
{
    vec4 AmbientColor = vec4(Light.Color, 1.0f) * Light.AmbientIntensity;
    float DiffuseFactor = dot(Normal, -LightDirection);

    vec4 DiffuseColor  = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);

    if (DiffuseFactor > 0) {
        DiffuseColor = vec4(Light.Color, 1.0f) * Light.DiffuseIntensity * DiffuseFactor;

        vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos);
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);
        SpecularFactor = pow(SpecularFactor, gSpecularPower);
        if (SpecularFactor > 0) {
            SpecularColor = vec4(Light.Color, 1.0f) * gMatSpecularIntensity * SpecularFactor;
        }
    }

    return (AmbientColor + ShadowFactor * (DiffuseColor + SpecularColor) + ShadowFactor - ShadowFactor);
}

vec4 CalcDirectionalLight(vec3 WorldPos, vec3 Normal)
{
    return CalcLightInternal(gDirectionalLight.Base,
							 gDirectionalLight.Direction,
							 WorldPos,
							 Normal,
							 1.0);
}

vec4 CalcPointLight(vec3 WorldPos, vec3 Normal, vec4 LightSpacePos)
{
    vec3 LightDirection = WorldPos - gPointLight.Position;
    float Distance = length(LightDirection);
    LightDirection = normalize(LightDirection);
	float ShadowFactor = CalcShadowFactor(LightSpacePos);

    vec4 Color = CalcLightInternal(gPointLight.Base, LightDirection, WorldPos, Normal, ShadowFactor);

    float Attenuation =  gPointLight.Atten.Constant +
                         gPointLight.Atten.Linear * Distance +
                         gPointLight.Atten.Exp * Distance * Distance;

    //Attenuation = min(1.0, Attenuation);

    return Color / Attenuation;
}

vec4 CalcSpotLight(vec3 WorldPos, vec3 Normal, vec4 LightSpacePos)
{
    vec3 LightToPixel = normalize(WorldPos - gSpotLight.Base.Position);
    float SpotFactor = dot(LightToPixel, gSpotLight.Direction);

    if (SpotFactor > gSpotLight.Cutoff) {
        vec4 Color = CalcPointLight(WorldPos, Normal, LightSpacePos);
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - gSpotLight.Cutoff));
    }
    else {
        return vec4(0,0,0,0);
    }
}

vec2 CalcTexCoord()
{
   return gl_FragCoord.xy / gScreenSize;
}

shader FSmainDirLight(out vec4 FragColor)
{
   	 vec2 TexCoord = CalcTexCoord();
   	 vec3 WorldPos = texture(gPositionMap, TexCoord).xyz;
   	 vec3 Color = texture(gColorMap, TexCoord).xyz;
   	 vec3 Normal = texture(gNormalMap, TexCoord).xyz;
   	 Normal = normalize(Normal);

   	 FragColor = vec4(Color, 1.0) * CalcDirectionalLight(WorldPos, Normal);
} 


shader FSmainPointLight(out vec4 FragColor)
{
   	 vec2 TexCoord = CalcTexCoord();
   	 vec3 WorldPos = texture(gPositionMap, TexCoord).xyz;
   	 vec3 Color = texture(gColorMap, TexCoord).xyz;
   	 vec3 Normal = texture(gNormalMap, TexCoord).xyz;
	 vec4 LightSpacePos = gLightWVP * vec4(WorldPos, 1.0);
   	 Normal = normalize(Normal);

   	 FragColor = vec4(Color, 1.0) * CalcPointLight(WorldPos, Normal, LightSpacePos);
}


program DirLightPass
{
    vs(420)=VSmain();
    fs(420)=FSmainDirLight();
};

program PointLightPass
{
    vs(420)=VSmain();
    fs(420)=FSmainPointLight();
};