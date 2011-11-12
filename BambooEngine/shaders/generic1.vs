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
in vec3 in_Color;
in vec3 color_texture_coords;

//in vec3 in_Bitangent;

out vec3 my_Texcoord;
out vec3 my_LightDir;
out vec3 my_EyeDir;
out vec3 my_ObjPosition;
out vec4 my_ScreenPosition;
out vec3 my_Normal;
out vec3 my_Color;

void main(void)
{
  mat4 ModelViewMatrix = ViewMatrix * ModelMatrix;

  gl_Position = ModelViewProjectionMatrix * vec4(in_Position, 1.0);

  my_ObjPosition = in_Position;
  my_ScreenPosition = gl_Position;

  vec3 vVertex = (ModelViewMatrix * vec4(in_Position, 1.0)).xyz;
  vec3 my_Lightpos = (ViewMatrix * Light0Position).xyz;

  vec3 tempVec = my_Lightpos - vVertex;

  my_Texcoord = color_texture_coords;
  my_EyeDir = vVertex;

  vec3 n = normalize(NormalMatrix * in_Normal);
  vec3 t = normalize(NormalMatrix * in_Tangent);
  vec3 b = cross(n, t);

  vec3 v;
  v.x = dot(tempVec, t);
  v.y = dot(tempVec, b);
  v.z = dot(tempVec, n);
  my_LightDir = normalize(v);

  tempVec = -vVertex;
  v.x = dot(tempVec, t);
  v.y = dot(tempVec, b);
  v.z = dot(tempVec, n);

  my_EyeDir = normalize(v);

  my_Normal = in_Normal;

    my_Color = in_Color;
}
