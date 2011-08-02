/*
 * vertex shader for basic shading
 * Project CUBE, 2011
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 06/2011
 */

#version 330 core

uniform mat4 ModelViewProjectionMatrix;

in vec3 in_Position;
in vec3 in_Normal;
in vec3 in_Texcoord;

void main(void)
{
  gl_Position = ModelViewProjectionMatrix * vec4(in_Position, 1.0);
}