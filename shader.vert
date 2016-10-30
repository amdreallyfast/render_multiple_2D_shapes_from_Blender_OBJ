#version 440

layout (location = 0) in vec4 pos;

uniform mat4 translateMatrixWindowSpace;

// must have the same name as its corresponding "in" item in the frag shader
smooth out vec3 vertOutColor;

void main()
{
    // no colors from the Blender OBJ files; just hard-code white
    vertOutColor = vec3(1.0f, 1.0f, 1.0f);

	gl_Position = translateMatrixWindowSpace * pos;
}

