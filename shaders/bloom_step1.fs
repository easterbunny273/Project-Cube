/*
 * fragment shader for postprocess bloom effect
 * first step, writes glow-information in alpha channel (and
 * puts color in color-channel, to allow reuse of created fbo-color-texture)
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 11/2010
 */

#version 330 core

uniform sampler2D texture1;

in vec3 my_Texcoord;
out vec4 FragColor;

void main()
{
    vec4 texColor = texture2D(texture1, my_Texcoord.st);

    vec4 color = texColor * pow((texColor.r * texColor.g * texColor.b), 2) * 8;
    float max_color = max(color.r, max(color.g, color.b));

    FragColor = vec4(color.rgb, max_color);

}

