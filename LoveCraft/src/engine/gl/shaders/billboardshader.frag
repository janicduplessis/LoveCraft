#version 330

uniform sampler2D gColorMap;

in vec2 TexCoord;
out vec4 FragColor;

void main()
{
    FragColor = texture2D(gColorMap, TexCoord);
}