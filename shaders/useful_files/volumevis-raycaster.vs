/*
 * vertex shader for postprocess
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 11/2010
 */

#version 330 core

uniform mat4 ModelViewProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

in vec3 in_Position;
in vec3 in_Texcoord;
//out vec3 my_Texcoord;

out vec4 FinalPosition;
out vec4 ObjectPosition;

void main(void)
{
    ObjectPosition = vec4(in_Position, 1.0);

    gl_Position = ModelViewProjectionMatrix * vec4(in_Position, 1.0);

    FinalPosition = ModelViewProjectionMatrix * vec4(in_Position, 1.0);
}
