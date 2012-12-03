uniform sampler2D tex;
varying vec4 diffuse;
varying vec4 color;

void main()
{
	//gl_FragColor = (gl_LightSource[0].ambient + diffuse) * vec4(1,0,0,1);
	gl_FragColor = vec4(1,0,0,1);
}
