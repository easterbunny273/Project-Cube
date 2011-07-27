/*
 * vertex shader for phong shading, freepool project
 *
 * does simple phong lighting
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 03/2011
 */

#version 330 core

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ModelViewProjectionMatrix;

uniform mat4 Camera_TempTransformMatrix;
uniform mat3 NormalMatrix;
uniform vec4 Light0Position;

in vec3 in_Position;
in vec3 in_Normal;
in vec3 in_Texcoord;

out vec3 my_Normal;
out vec3 my_Texcoord;
out vec3 my_LightDir;
out vec3 my_EyeDir;
out vec3 my_ObjPosition;

void main(void)
{
  mat4 ModelViewMatrix = ViewMatrix * ModelMatrix;

  my_ObjPosition = in_Position;

  gl_Position = ModelViewProjectionMatrix * vec4(in_Position, 1.0);

  vec4 vVertex4 = (ModelViewMatrix * vec4(in_Position, 1.0));
  vec3 vVertex = vVertex4.xyz / vVertex4.w;
  vec3 my_Lightpos = (ViewMatrix * Light0Position).xyz;

  my_Normal = NormalMatrix * in_Normal;
  my_Texcoord = in_Texcoord;
  my_LightDir = my_Lightpos - vVertex;
  my_EyeDir = -vVertex;
}
