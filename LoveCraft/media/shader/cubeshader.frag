uniform sampler2D tex;
varying vec4 light;
varying float fogFactor;

void main()
{
	vec4 texel;
    texel = texture2D(tex,gl_TexCoord[0].st);

    texel *= light;

    gl_FragColor = mix(gl_Fog.color, texel, fogFactor);
}
