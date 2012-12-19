uniform sampler2D tex;
varying vec4 light;
varying float fogFactor;
varying vec3 texpos;

void main()
{
	vec4 texel;
    //texel = texture2D(tex,gl_TexCoord[0].st);
	vec4 texel = texture2DArray(tex,texpos);

    texel *= light;

    gl_FragColor = mix(gl_Fog.color, texel, fogFactor);
}
