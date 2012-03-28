/*
 * simple shader for bounding boxes
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 05/2011
 */

#version 330 core

layout (location = 0) out vec4 FragColor;

uniform sampler2D color_texture;
uniform sampler2D normal_texture;
uniform sampler2D depth_texture;
uniform sampler2D position_texture;

uniform sampler2D shadowmap;

uniform mat4 Light_ViewProjectionMatrix;
uniform mat4 ModelViewProjectionMatrix;

uniform vec3 vLightColor;

in vec4 finalPosition;

void main()
{
	FragColor = vec4(1.0, 1.0, 1.0, 1.0f);
}
