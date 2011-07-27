/*
 * simple shader for color cubes
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 05/2011
 */

#version 330 core

uniform mat4 ModelViewProjectionMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix[128];

in vec3 in_Position;

out vec4 finalPosition;
out vec3 my_Position;
out vec4 color;

void main(void)
{
  my_Position = in_Position;
  color = (vec4(in_Position, 1.0) + vec4(1.0)) * 0.5;
  finalPosition = ModelViewProjectionMatrix * vec4(in_Position, 1.0);
  gl_Position = finalPosition;
}
