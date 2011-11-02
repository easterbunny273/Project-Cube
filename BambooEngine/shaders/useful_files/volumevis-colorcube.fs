/*
 * simple shader for color cubes
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 05/2011
 */

#version 330 core

out vec4 FragColor;
in vec4 color;
in vec4 finalPosition;
in vec3 my_Position;

void main()
{
    vec3 color3 = (my_Position + vec3(1.0)) * 0.5;

    FragColor = vec4(color3, finalPosition.z / finalPosition.w);
}
