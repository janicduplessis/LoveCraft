uniform mat4 gWVP;

shader VSStaticMain(in vec3 Position)
{
    gl_Position    = gWVP * vec4(Position, 1.0);
};

struct VSInputBoneInfo
{
	ivec4 BoneIDs;
    vec4  Weights;
};

const int MAX_BONES = 100;

uniform mat4 gBones[MAX_BONES];

shader VSBoneMain(in vec3 Position:0, in VSInputBoneInfo BoneInfo:4)
{
	mat4 BoneTransform = gBones[BoneInfo.BoneIDs[0]] * BoneInfo.Weights[0];
    BoneTransform     += gBones[BoneInfo.BoneIDs[1]] * BoneInfo.Weights[1];
    BoneTransform     += gBones[BoneInfo.BoneIDs[2]] * BoneInfo.Weights[2];
    BoneTransform     += gBones[BoneInfo.BoneIDs[3]] * BoneInfo.Weights[3];

    vec4 PosL      = BoneTransform * vec4(Position, 1.0);
    gl_Position    = gWVP * PosL;
};

shader FSmain()
{
};

program StaticNullShader
{
    vs(420) = VSStaticMain();
    fs(420) = FSmain();
};

program BoneNullShader
{
    vs(420) = VSBoneMain();
    fs(420) = FSmain();
};