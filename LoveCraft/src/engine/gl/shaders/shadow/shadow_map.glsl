struct VSInput 
{
    vec3  Position;
    vec2  TexCoord;
    vec3  Normal;
};

struct VSInputBoneInfo
{
	ivec4 BoneIDs;
    vec4  Weights;
};

interface VSOutput
{         
    vec2 TexCoord;                                                                 
};

const int MAX_BONES = 100;

uniform mat4 gWVP;
uniform mat4 gBones[MAX_BONES];

shader VSStaticMain(in VSInput VSin:0, out VSOutput VSout)
{
    gl_Position = gWVP * vec4(VSin.Position, 1.0);
    VSout.TexCoord = VSin.TexCoord;
}

shader VSBoneMain(in VSInput VSin:0, in VSInputBoneInfo BoneInfo:4, out VSOutput VSout)
{
	mat4 BoneTransform = gBones[BoneInfo.BoneIDs[0]] * BoneInfo.Weights[0];
    BoneTransform     += gBones[BoneInfo.BoneIDs[1]] * BoneInfo.Weights[1];
    BoneTransform     += gBones[BoneInfo.BoneIDs[2]] * BoneInfo.Weights[2];
    BoneTransform     += gBones[BoneInfo.BoneIDs[3]] * BoneInfo.Weights[3];
	
    vec4 PosL      = BoneTransform * vec4(VSin.Position, 1.0);
    gl_Position = gWVP * PosL;
	VSout.TexCoord = VSin.TexCoord;
}

uniform sampler2D gShadowMap;

shader FSMain(in VSOutput FSin, out vec4 FragColor)
{
	//float Depth = texture(gShadowMap, FSin.TexCoord).x;
	//Depth = 1.0 - (1.0 - Depth) * 25.0;
	//FragColor = vec4(Depth);
	FragColor = vec4(texture(gShadowMap, FSin.TexCoord).xyz,1);
}

program BoneShadowMap
{
	vs(420)=VSBoneMain();
    fs(420)=FSMain();
};

program StaticShadowMap
{
	vs(420)=VSStaticMain();
    fs(420)=FSMain();
};