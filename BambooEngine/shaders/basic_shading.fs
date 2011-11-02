/*
 * fragment shader for basic shading
 * Project CUBE, 2011
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 06/2011
 */

#version 330 core

out vec4 out_Color;

in vec3 position_as_color;
in vec2 my_Texcoord;

uniform sampler2D texture1;

void main(void)
{
  //out_Color = vec4(vec3(my_Texcoord, 0.0) , 1.0);
  out_Color = texture(texture1, my_Texcoord);
}
