varying vec3 vertex_light_position;
varying vec3 vertex_normal;

varying float fogFactor;

void main() {            
    // Calculate the normal value for this vertex, in world coordinates (multiply by gl_NormalMatrix)
    vertex_normal = normalize(gl_Normal);
    // Calculate the light position for this vertex
    vertex_light_position = normalize(gl_LightSource[0].position.xyz);

    // Set the position of the current vertex 
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

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