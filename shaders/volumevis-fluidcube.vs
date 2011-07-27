/*
 * simple shader for color cubes
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 05/2011
 */

#version 410 core

uniform mat4 ModelViewProjectionMatrix;

in vec3 in_Position;
in vec2 in_Texcoord;
out vec2 my_Texcoords;

void main(void)
{
  gl_Position = ModelViewProjectionMatrix * vec4(in_Position, 1.0);

  my_Texcoords = in_Texcoord;
}
