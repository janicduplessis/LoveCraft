varying vec4 light;
varying float fogFactor;

void main()
{
	gl_Position = ftransform();
    light = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;
	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
    gl_Position = ftransform();

	const float LOG2 = 1.442695;
	gl_FogFragCoord = length(vVertex);
	fogFactor = exp2( -gl_Fog.density * 
					   gl_Fog.density * 
					   gl_FogFragCoord * 
					   gl_FogFragCoord * 
					   LOG2 );
	fogFactor = clamp(fogFactor, 0.0, 1.0);
}
