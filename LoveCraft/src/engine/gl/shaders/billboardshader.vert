#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in float Alpha;

out float Alpha0;

uniform float LifeTime;

void main()
{
	Alpha0 = Alpha;
    gl_Position = vec4(Position, 1);
}