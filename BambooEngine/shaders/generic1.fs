/*
 * fragment shader for parallax lighting
 *
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 11/2010
 */

#version 330 core

out vec4 out_Color;

in vec3 my_Texcoord;
in vec3 my_LightDir;
in vec3 my_ObjPosition;
in vec3 my_EyeDir;
in vec3 my_Normal;
in vec4 my_ScreenPosition;
in vec3 my_Color;

uniform sampler2D color_texture;

void main(void)
{
   out_Color = vec4(texture(color_texture, my_Texcoord.xy).rgb, 1.0);
}
