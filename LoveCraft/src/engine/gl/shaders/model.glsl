struct VSInput 
{
    vec3  Position;                                             
    vec2  TexCoord;                                             
    vec3  Normal;    
	vec3  Tangent;
    ivec4 BoneIDs;
    vec4  Weights;
};

interface VSOutput
{                                                                                    
    vec2 TexCoord;                                                                 
    vec3 Normal;
	vec3 Tangent;
    vec3 WorldPos;                                                                 
};

const int MAX_BONES = 100;
const int MAX_POINT_LIGHTS = 3;

uniform mat4 gWVP;
uniform mat4 gWorld;
uniform mat4 gBones[MAX_BONES];

shader VSmain(in VSInput VSin:0, out VSOutput VSout)
{       
    mat4 BoneTransform = gBones[VSin.BoneIDs[0]] * VSin.Weights[0];
    BoneTransform     += gBones[VSin.BoneIDs[1]] * VSin.Weights[1];
    BoneTransform     += gBones[VSin.BoneIDs[2]] * VSin.Weights[2];
    BoneTransform     += gBones[VSin.BoneIDs[3]] * VSin.Weights[3];

    vec4 PosL      = BoneTransform * vec4(VSin.Position, 1.0);
    gl_Position    = gWVP * PosL;
    VSout.TexCoord = VSin.TexCoord;
    vec4 NormalL   = BoneTransform * vec4(VSin.Normal, 0.0);
    VSout.Normal   = (gWorld * NormalL).xyz;
	VSout.Tangent  = (gWorld * vec4(VSin.Tangent, 0.0)).xyz;
    VSout.WorldPos = (gWorld * PosL).xyz;                                
}

struct FSOutput
{
	vec3 WorldPos;
	vec3 Diffuse;
	vec3 Normal;
	vec3 TexCoord;
};                               

sampler Sampler {
	Dim = 2;
} ColorTexture:0, NormalTexture:2;


shader FSmain(in VSOutput FSin, out FSOutput FSout)
{   
	// Normal mapping
	vec3 Tangent = normalize(FSin.Tangent - dot(FSin.Tangent, FSin.Normal) * FSin.Normal);
    vec3 Bitangent = cross(Tangent, FSin.Normal);
    vec3 BumpMapNormal = texture(NormalTexture, FSin.TexCoord.xy).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(Tangent, Bitangent, FSin.Normal);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
	
	// TODO find better way for transparency
	vec4 texel = texture(ColorTexture, FSin.TexCoord.xy);
	if (texel.x == 1.f && texel.y == 1.f && texel.z == 1.f) {
        discard;
    }
	
	FSout.WorldPos = FSin.WorldPos;
	FSout.Diffuse = texel.xyz;
	FSout.Normal = NewNormal;
	FSout.TexCoord = vec3(FSin.TexCoord, 1);
}

program ModelGeometryPass
{
    vs(420)=VSmain();
    fs(420)=FSmain();
};