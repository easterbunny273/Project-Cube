/*
 * fragment shader for phong shading, freepool project
 *
 * does simple phong lighting
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 03/2011
 */

#version 330 core

out vec4 out_Color;
in vec3 my_Normal;
in vec3 my_Texcoord;
in vec3 my_LightDir;
in vec3 my_EyeDir;
in vec3 my_ObjPosition;
in mat3 NormalMatrix2;
in vec4 my_WorldPosition;
in mat4 my_ModelTransformMatrix;

uniform sampler2D color_texture;
uniform mat3 NormalMatrix;

const vec4 Light0Ambient = vec4(0.01);
const vec4 Light0Diffuse = vec4(0.5);
const vec4 Light0Specular = vec4(0.9);

const vec4 MaterialAmbient = vec4(1.0);
const vec4 MaterialDiffuse = vec4(1.0);
const vec4 MaterialSpecular = vec4(1.0);

uniform sampler2D shadowmap;
uniform mat4 ModelMatrix;
uniform mat4 Camera_TempTransformMatrix;

uniform float MaterialShininess;
uniform float MaterialShininessStrength;

float calcLight()
{
    float light = 1.0;
    vec4 shadowcoords_f = Camera_TempTransformMatrix * ModelMatrix * vec4(my_ObjPosition, 1.0);
    vec2 shadowcoords = (shadowcoords_f / shadowcoords_f.w).xy;
    shadowcoords += 1.0;
    shadowcoords *= 0.5;

    float ShadowMapValue = texture(shadowmap, shadowcoords).r;

    float ComparedZ = shadowcoords_f.z / shadowcoords_f.w;

    if (ComparedZ >= ShadowMapValue + 0.001)
	return 0.2;
    else
	return 1.0;
}

void main(void)
{
    vec4 texture_value = texture(color_texture, my_Texcoord.st);

    vec3 light_value = Light0Ambient.xyz;

    vec3 N = normalize(my_Normal);
    vec3 L = normalize(my_LightDir);

    float lambertTerm = dot(N, L);

    if(lambertTerm > 0.0)
    {
	    light_value += Light0Diffuse.xyz * MaterialDiffuse.xyz * lambertTerm;

	    vec3 E = normalize(my_EyeDir);
	    vec3 R = reflect(-L, N);

	    float specular = pow(max(dot(R, E), 0.0), 50.0);

	    light_value += Light0Specular.xyz * MaterialSpecular.xyz * specular;
    }

    out_Color = vec4(light_value * texture_value.xyz * calcLight(), 1.0);
}
