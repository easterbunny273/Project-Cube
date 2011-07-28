/*
 * simple shader for bounding boxes
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

void main(void)
{
  gl_Position = ModelViewProjectionMatrix * vec4(in_Position, 1.0);
}
