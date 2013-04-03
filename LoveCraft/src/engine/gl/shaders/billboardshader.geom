#version 330

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

uniform mat4 gVP;
uniform vec3 gCameraPos;
uniform float gBillboardSize;

in float[] Alpha0;

out vec2 TexCoord;
out float Alpha1;


void main()
{
    vec3 Pos = gl_in[0].gl_Position.xyz;
	float Alpha = Alpha0[0];
	vec3 toCamera = normalize(gCameraPos - Pos);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(toCamera, up)) * gBillboardSize;
	up = normalize(cross(toCamera, right)) * gBillboardSize;
	
	Pos -= right / 2;
	Pos -= up / 2;
    gl_Position = gVP * vec4(Pos, 1.0);
    TexCoord = vec2(0.0, 0.0);
	Alpha1 = Alpha;
    EmitVertex();

    Pos += up;
    gl_Position = gVP * vec4(Pos, 1.0);
    TexCoord = vec2(0.0, 1.0);
	Alpha1 = Alpha;
    EmitVertex();

    Pos -= up;
    Pos += right;
    gl_Position = gVP * vec4(Pos, 1.0);
    TexCoord = vec2(1.0, 0.0);
	Alpha1 = Alpha;
    EmitVertex();

    Pos += up;
    gl_Position = gVP * vec4(Pos, 1.0);
    TexCoord = vec2(1.0, 1.0);
	Alpha1 = Alpha;
    EmitVertex();

    EndPrimitive();
}