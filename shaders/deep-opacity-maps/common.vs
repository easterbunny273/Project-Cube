/*
 * vertex shader for deep opacity maps
 *
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 06/2011
 */

#version 410 core

uniform mat4 ModelViewProjectionMatrix;

in vec3 in_Position;

out vec4 FinalPosition;

void main(void)
{
  FinalPosition = ModelViewProjectionMatrix * vec4(in_Position, 1.0);
  gl_Position = FinalPosition;
}
