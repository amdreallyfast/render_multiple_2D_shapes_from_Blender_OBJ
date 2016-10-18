#version 440

// must have the same name as its corresponding "out" item in the vert shader
smooth in vec3 vertOutColor;

void main()
{
    gl_FragColor = vec4(vertOutColor, 1.0f);
}
