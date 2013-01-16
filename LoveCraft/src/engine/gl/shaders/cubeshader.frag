#version 110
#extension GL_EXT_gpu_shader4 : enable
uniform sampler2DArray tex;

varying vec4 light;
varying float fogFactor;
varying vec3 texpos;

void main()
{
	vec4 texel = texture2DArray(tex,texpos);

    texel *= light;

    gl_FragColor = mix(gl_Fog.color, texel, fogFactor);
}
