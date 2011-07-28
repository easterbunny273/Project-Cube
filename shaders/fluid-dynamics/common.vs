#version 330 core

in vec3 in_Position;
in vec3 in_Texcoord;
out vec3 coords;

void main(void)
{
    coords = in_Texcoord;

    gl_Position = vec4(in_Position, 1.0);
}
