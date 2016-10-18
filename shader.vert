#version 440

layout (location = 0) in vec2 pos;
layout (location = 1) in vec3 color;    

uniform mat4 translateMatrixWindowSpace;

// must have the same name as its corresponding "in" item in the frag shader
smooth out vec3 vertOutColor;

void main()
{
    vertOutColor = color;
    //gl_Position = vec4(pos, -1.0f, 1.0f);
	gl_Position = translateMatrixWindowSpace * vec4(pos, -1.0f, 1.0f);
}

