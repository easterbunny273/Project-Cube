/*
 * vertex shader for deferred shading, first pass
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 11/2010, 11/2011
 */

#version 400 core

uniform mat4 ModelViewProjectionMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

in vec3 in_Position;
in vec3 in_Normal;
in vec3 in_Texcoord;
in vec3 in_Tangent;
in vec3 in_Bitangent;
in vec3 in_Color;

in vec3 color_texture_coords;

out vec3 texcoords;

out vec3 attr_Normal;
out vec3 attr_Tangent;
out vec3 attr_Color;
out vec4 attr_Position;

void main(void)
{
  mat4 ModelViewMatrix = ViewMatrix * ModelMatrix;

  //gl_Position = vec4(in_Position, 1.0);


  texcoords = color_texture_coords;

  attr_Normal = in_Normal;
  attr_Tangent = in_Tangent;
  attr_Color = in_Color;
  attr_Position = vec4(in_Position, 1.0);
}
