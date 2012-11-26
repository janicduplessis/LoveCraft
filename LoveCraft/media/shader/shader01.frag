uniform sampler2D tex;
varying vec4 diffuse;
varying vec4 color;

void main()
{
	gl_FragColor = (gl_LightSource[0].ambient + diffuse) * color;
}
