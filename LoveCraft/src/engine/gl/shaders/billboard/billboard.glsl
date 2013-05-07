struct VSInput
{
	vec3 Position;
	float Alpha;
};

interface VSOutput
{
	vec3 Position;
	float Alpha;
};

shader VSmain(in VSInput VSin:0, out VSOutput VSout)
{
	VSout.Alpha = VSin.Alpha;
    VSout.Position = VSin.Position;
}


struct GSOutput
{
	vec2 TexCoord;
	float Alpha;
};

uniform mat4 gVP;
uniform vec3 gCameraPos;
uniform float gBillboardSize;

shader GSmain(in VSOutput GSin[], out GSOutput GSout)
{
    vec3 Pos = GSin[0].Position.xyz;
	float Alpha = GSin[0].Alpha;
	vec3 toCamera = normalize(gCameraPos - Pos);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(toCamera, up)) * gBillboardSize;
	up = normalize(cross(toCamera, right)) * gBillboardSize;
	
	Pos -= right / 2;
	Pos -= up / 2;
    gl_Position = gVP * vec4(Pos, 1.0);
    GSout.TexCoord = vec2(0.0, 0.0);
	GSout.Alpha = Alpha;
    EmitVertex();

    Pos += up;
    gl_Position = gVP * vec4(Pos, 1.0);
    GSout.TexCoord = vec2(0.0, 1.0);
	GSout.Alpha = Alpha;
    EmitVertex();

    Pos -= up;
    Pos += right;
    gl_Position = gVP * vec4(Pos, 1.0);
    GSout.TexCoord = vec2(1.0, 0.0);
	GSout.Alpha = Alpha;
    EmitVertex();

    Pos += up;
    gl_Position = gVP * vec4(Pos, 1.0);
    GSout.TexCoord = vec2(1.0, 1.0);
	GSout.Alpha = Alpha;
    EmitVertex();

    EndPrimitive();
}


uniform sampler2D gColorMap;
uniform vec3 gColor;

shader FSmain(in GSOutput GSout, out vec4 FragColor)
{
    FragColor = vec4(gColor, (texture2D(gColorMap, GSout.TexCoord).w * GSout.Alpha));
}

program Billboard
{
	vs(420)=VSmain();
    gs(420)=GSmain() : in(points), out(triangle_strip, max_vertices = 4);
    fs(420)=FSmain();
};