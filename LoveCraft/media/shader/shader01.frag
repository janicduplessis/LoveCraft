uniform sampler2D tex;
varying vec4 diffuse;

void main()
{
	gl_FragColor = (gl_LightSource[0].ambient + diffuse) * vec4(0.8, 0.0, 0.0, 1.0);
}
