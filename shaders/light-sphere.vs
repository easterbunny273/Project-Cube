/*
 * vertex shader for the light sphere
 *
 * uses Light0Position as object position
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 01/2011
 */

#version 330 core

uniform mat4 Camera_ProjectionMatrix;
uniform mat4 Camera_TransformMatrix;
uniform mat4 Model_TransformMatrix;
uniform mat3 NormalMatrix;
uniform vec4 Light0Position;

in vec3 in_Position;
in vec3 in_Normal;
in vec3 in_Texcoord;

void main(void)
{
  gl_Position = Camera_ProjectionMatrix * Camera_TransformMatrix * vec4(Light0Position.xyz + in_Position, 1.0);
}
