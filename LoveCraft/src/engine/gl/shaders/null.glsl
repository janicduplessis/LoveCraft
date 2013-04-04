uniform mat4 gWVP;

shader VSmain(in vec3 Position)
{
    gl_Position = gWVP * vec4(Position, 1.0);
};

shader FSmain()
{
};

program NullShader
{
    vs(420) = VSmain();
    fs(420) = FSmain();
};