uniform sampler2D tex;

varying vec4 color;

void main() {
	vec4 texel;
    texel = texture2D(tex,gl_TexCoord[0].st);
	texel *= color;

    // Set the output color of our current pixel
    gl_FragColor = texel;
}