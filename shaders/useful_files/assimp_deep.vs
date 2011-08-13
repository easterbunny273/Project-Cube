/*
 * vertex shader for phong shading
 *
 * does simple phong lighting
 * TODO: set lighting parameters with uniforms or uniform buffer objects
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 11/2010
 */

#version 330 core

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix[128];
uniform mat3 NormalMatrix;

in vec3 in_Position;
in vec3 in_Normal;
in vec3 in_Texcoord;

out vec4 finalPosition;

/*
layout(shared) uniform lightingParam
{
    vec4 LightPosition;
    vec3 LightColorDiffuse;
    vec3 LightColorSpecular;
    vec3 MaterialDiffuse;
    vec3 MaterialSpecular;
    float MaterialShininess;
};*/

void main(void)
{
  mat4 ProjectionMatrix = ProjectionMatrix;
  mat4 ModelViewMatrix = ViewMatrix * ModelMatrix[gl_InstanceID];

  finalPosition = ProjectionMatrix * ModelViewMatrix * vec4(in_Position, 1.0);
  gl_Position = finalPosition;
}
