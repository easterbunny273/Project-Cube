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

out vec3 position_as_color;
out vec2 my_Texcoord;

void main(void)
{
  gl_Position = ModelViewProjectionMatrix * vec4(in_Position, 1.0);

  position_as_color = 0.5 * (in_Position + 1.0);

    my_Texcoord = in_Texcoord.st;
}
