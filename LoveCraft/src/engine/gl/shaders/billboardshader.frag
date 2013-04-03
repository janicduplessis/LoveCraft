#version 330

uniform sampler2D gColorMap;
uniform vec3 gColor;

in vec2 TexCoord;
in float Alpha1;
out vec4 FragColor;

void main()
{
    FragColor = vec4(gColor, (texture2D(gColorMap, TexCoord).w * Alpha1));
}