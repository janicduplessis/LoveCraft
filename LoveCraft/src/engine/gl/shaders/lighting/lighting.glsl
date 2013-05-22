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

struct FSOutput
{
	vec3 WorldPos;
	vec3 Diffuse;
	vec3 Normal;
	vec3 TexCoord;
};  

uniform sampler2D gColorTexture;
uniform sampler2D gNormalTexture;

uniform sampler2DArray gArraySampler;
uniform int gSamplerType;

shader FSmain(in VSOutput FSin, out FSOutput FSout)
{
	// Normal mapping
	vec3 Tangent = normalize(FSin.Tangent - dot(FSin.Tangent, FSin.Normal) * FSin.Normal);
    vec3 Bitangent = cross(Tangent, FSin.Normal);
    vec3 BumpMapNormal = texture(gNormalTexture, FSin.TexCoord.xy).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(Tangent, Bitangent, FSin.Normal);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
	
	// TODO 2 different programs
	vec4 texel = vec4(0,0,0,0);
	if (gSamplerType == 0) {
		texel = texture(gColorTexture, FSin.TexCoord.xy);
	} else {
		texel = texture2DArray(gArraySampler, FSin.TexCoord);
	}
	
	// TODO find better way for transparency
	if (texel.x == 1.f && texel.y == 1.f && texel.z == 1.f) {
        discard;
    }
	
	FSout.WorldPos = FSin.WorldPos;
	FSout.Diffuse = texel.xyz;
	FSout.Normal = NewNormal;
	FSout.TexCoord = vec3(FSin.TexCoord);
}

program Lighting
{
	vs(420)=VSmain();
	fs(420)=FSmain();
};