varying vec4 light;
varying float fogFactor;
varying vec3 texpos;

uniform vec3 Texture;

void main()
{
	gl_Position = ftransform();
    light = gl_Color;
    //gl_TexCoord[0] = gl_MultiTexCoord0;
	texpos = Texture;

	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	const float LOG2 = 1.442695;
	gl_FogFragCoord = length(vVertex);
	fogFactor = exp2( -gl_Fog.density * 
					   gl_Fog.density * 
					   gl_FogFragCoord * 
					   gl_FogFragCoord * 
					   LOG2 );
	fogFactor = clamp(fogFactor, 0.0, 1.0);
}
