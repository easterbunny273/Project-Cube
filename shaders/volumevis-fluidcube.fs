/*
 * simple shader for color cubes
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 05/2011
 */

#version 410 core

uniform sampler2DRect texture1;

in vec2 my_Texcoords;
out vec4 FragColor;

void main()
{
    FragColor = texture(texture1, my_Texcoords.xy);
}
