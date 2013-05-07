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
    vec3 WorldPos;                                                                 
};

struct sVSOutput
{         
    vec3 WorldPos;                                                                 
};

const int MAX_BONES = 100;

uniform mat4 gVP;
uniform mat4 gWorld;
uniform mat4 gBones[100];

shader VSStaticMain(in VSInput VSin:0, out VSOutput VSout)
{
    vec4 PosL      = vec4(VSin.Position, 1.0);
    VSout.WorldPos = (gWorld * PosL).xyz;
}

shader VSBoneMain(in VSInput VSin:0, in VSInputBoneInfo BoneInfo:4, out VSOutput VSout)
{
	mat4 BoneTransform = gBones[BoneInfo.BoneIDs[0]] * BoneInfo.Weights[0];
    BoneTransform     += gBones[BoneInfo.BoneIDs[1]] * BoneInfo.Weights[1];
    BoneTransform     += gBones[BoneInfo.BoneIDs[2]] * BoneInfo.Weights[2];
    BoneTransform     += gBones[BoneInfo.BoneIDs[3]] * BoneInfo.Weights[3];
	
    vec4 PosL      = BoneTransform * vec4(VSin.Position, 1.0);
    VSout.WorldPos = (gWorld * PosL).xyz;
}


uniform vec3 gLightPos;

float EPSILON = 0.01;

void EmitQuad(int StartIndex, sVSOutput StartVertex, int EndIndex, sVSOutput EndVertex)
{
    vec3 LightDir = normalize(StartVertex.WorldPos - gLightPos);
    vec3 l = LightDir * EPSILON;
    gl_Position = gVP * vec4((StartVertex.WorldPos + l), 1.0);
    EmitVertex();
    
    gl_Position = gVP * vec4(LightDir, 0.0);
    EmitVertex();

    LightDir = normalize(EndVertex.WorldPos - gLightPos);
    l = LightDir * EPSILON;
    gl_Position = gVP * vec4((EndVertex.WorldPos + l), 1.0);
    EmitVertex();
    
    gl_Position = gVP * vec4(LightDir, 0.0);
    EmitVertex();

    EndPrimitive();            
}

shader GSmain(in VSOutput GSin[])
{
    vec3 e1 = GSin[2].WorldPos - GSin[0].WorldPos;
    vec3 e2 = GSin[4].WorldPos - GSin[0].WorldPos;
    vec3 e3 = GSin[1].WorldPos - GSin[0].WorldPos;
    vec3 e4 = GSin[3].WorldPos - GSin[2].WorldPos;
    vec3 e5 = GSin[4].WorldPos - GSin[2].WorldPos;
    vec3 e6 = GSin[5].WorldPos - GSin[0].WorldPos;

    vec3 Normal = cross(e1,e2);
    vec3 LightDir = gLightPos - GSin[0].WorldPos;

    if (dot(Normal, LightDir) > 0.000001) {

        struct sVSOutput StartVertex, EndVertex;

        Normal = cross(e3,e1);

        if (dot(Normal, LightDir) <= 0) {
            StartVertex.WorldPos = GSin[0].WorldPos;
            EndVertex.WorldPos   = GSin[2].WorldPos;
            EmitQuad(0, StartVertex, 2, EndVertex);
        }

        Normal = cross(e4,e5);
        LightDir = gLightPos - GSin[2].WorldPos;

        if (dot(Normal, LightDir) <= 0) {
            StartVertex.WorldPos = GSin[2].WorldPos;
            EndVertex.WorldPos   = GSin[4].WorldPos;
            EmitQuad(2, StartVertex, 4, EndVertex);
        }

        Normal = cross(e2,e6);
        LightDir = gLightPos - GSin[4].WorldPos;

        if (dot(Normal, LightDir) <= 0) {
            StartVertex.WorldPos = GSin[4].WorldPos;
            EndVertex.WorldPos   = GSin[0].WorldPos;
            EmitQuad(4, StartVertex, 0, EndVertex);
        }

        vec3 LightDir = (normalize(GSin[0].WorldPos - gLightPos)) * EPSILON;
        gl_Position = gVP * vec4((GSin[0].WorldPos + LightDir), 1.0);
        EmitVertex();

        LightDir = (normalize(GSin[2].WorldPos - gLightPos)) * EPSILON;
        gl_Position = gVP * vec4((GSin[2].WorldPos + LightDir), 1.0);
        EmitVertex();

        LightDir = (normalize(GSin[4].WorldPos - gLightPos)) * EPSILON;
        gl_Position = gVP * vec4((GSin[4].WorldPos + LightDir), 1.0);
        EmitVertex();
        EndPrimitive();
		
		// back cap
        LightDir = GSin[2].WorldPos - gLightPos;
        gl_Position = gVP * vec4(LightDir, 0.0);
        EmitVertex();

        LightDir = GSin[0].WorldPos - gLightPos;
        gl_Position = gVP * vec4(LightDir, 0.0);
        EmitVertex();

        LightDir = GSin[4].WorldPos - gLightPos;
        gl_Position = gVP * vec4(LightDir, 0.0); 
        EmitVertex();

        EndPrimitive();
    }
}


shader FSmain()
{      
}

program StaticShadowVolume
{
    vs(420)=VSStaticMain();
    gs(420)=GSmain() : in(triangles_adjacency), out(triangle_strip, max_vertices = 18);
    fs(420)=FSmain();
};

program BoneShadowVolume
{
    vs(420)=VSBoneMain();
    gs(420)=GSmain() : in(triangles_adjacency), out(triangle_strip, max_vertices = 18);
    fs(420)=FSmain();
};