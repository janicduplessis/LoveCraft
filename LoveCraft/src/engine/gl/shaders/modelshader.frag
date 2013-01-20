varying vec3 vertex_light_position;
varying vec3 vertex_normal;

varying float fogFactor;

void main() {
    // Set the diffuse value (darkness). This is done with a dot product between the normal and the light
    // and the maths behind it is explained in the maths section of the site.
    float diffuse_value = max(dot(vertex_normal, vertex_light_position), 0.0);
	vec4 ambiant = vec4(0, 0.15, 0, 0);

	vec4 texel;
    texel = diffuse_value * vec4(0, 0.4, 0, 1) + ambiant;

    // Set the output color of our current pixel
    gl_FragColor = mix(gl_Fog.color, texel, fogFactor);
}