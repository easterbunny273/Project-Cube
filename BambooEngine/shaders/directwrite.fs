/*
 * fragment shader for postprocess direct write
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 11/2011
 */

#version 330 core

uniform sampler2D texture1;

in vec3 my_Texcoord;
out vec4 FragColor;

void main()
{
    FragColor = texture(texture1, my_Texcoord.st);
}

