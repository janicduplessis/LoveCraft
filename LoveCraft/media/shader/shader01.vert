varying vec4 diffuse;
varying vec4 color;

void main()
{
    gl_Position = ftransform(); 
	gl_TexCoord[0] = gl_MultiTexCoord0;  
	color = gl_Color;
	 
	vec3 normal = gl_Normal.xyz;
	vec3 lightVector = normalize(gl_LightSource[0].position.xyz);
	float nxDir = max(0.0, dot(normal, lightVector));

	diffuse = gl_LightSource[0].diffuse * nxDir;
}
