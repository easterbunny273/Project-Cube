/*
 * vertex shader for bump lighting
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 11/2010
 */

#version 330 core

uniform mat4 ModelViewProjectionMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 Camera_TempTransformMatrix;
uniform mat3 NormalMatrix;
uniform vec4 Light0Position;

in vec3 in_Position;
in vec3 in_Normal;
in vec3 in_Texcoord;
in vec3 in_Tangent;
in vec3 in_Bitangent;
in vec3 in_Color;

in vec3 color_texture_coords;


out vec3 my_Texcoord;
out vec3 my_LightDir;
out vec3 my_EyeDir;
out vec3 my_ObjPosition;
out vec4 my_ScreenPosition;
out vec3 my_Normal;
out vec3 my_Tangent;
out vec3 my_Bitangent;
out vec3 my_Color;

void main(void)
{
  mat4 ModelViewMatrix = ViewMatrix * ModelMatrix;

  gl_Position = ModelViewProjectionMatrix * vec4(in_Position, 1.0);

  my_ObjPosition = in_Position;
  my_ScreenPosition = gl_Position;

  my_Texcoord = color_texture_coords;

  /*my_Normal = in_Normal;
  my_Tangent = in_Tangent;
  my_Bitangent = in_Bitangent;*/

  my_Normal = normalize(NormalMatrix * in_Normal);
  my_Tangent = normalize(NormalMatrix * in_Tangent);
  my_Bitangent = normalize(NormalMatrix * in_Bitangent);

  my_Color = in_Color;
}
