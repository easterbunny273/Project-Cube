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

uniform int ColorTextureAvailable;
uniform int NormalTextureAvailable;

uniform sampler2D color_texture;
uniform sampler2D normal_texture;
uniform sampler2D height_texture;

uniform mat3 NormalMatrix;

uniform sampler2D shadowmap;

uniform sampler2D opacity_depth;
uniform sampler2D opacity_opacity;

uniform mat4 ModelMatrix;
uniform mat4 Camera_TempTransformMatrix;

const float invRadius = 0.1;

const vec4 Light0Ambient = vec4(0.0);
const vec4 Light0Diffuse = vec4(1.0);
const vec4 Light0Specular = vec4(1.0);

uniform vec4 MaterialAmbient;
uniform vec4 MaterialDiffuse;
uniform vec4 MaterialSpecular;

uniform float MaterialShininess;
uniform float MaterialShininessStrength;

uniform int bUseShadow;

const float scale = 0.014;

const float n = 2.5; // camera z near
const float f = 7.0; // camera z far

float LinearizeDepth(float z)
{
  return (2.0 * n) / (f + n - z * (f - n));
}

float UnLinearizeDepth(float w)
{
  return -(((2*n)/w) - f - n) / (f - n);
}

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



float calcLight_DeepOpacity()
{
    const float dStep = 0.025;

    vec4 shadowcoords_f = Camera_TempTransformMatrix * ModelMatrix * vec4(my_ObjPosition, 1.0);
    vec2 shadowcoords = (shadowcoords_f / shadowcoords_f.w).xy;
    shadowcoords += 1.0;
    shadowcoords *= 0.5;

    vec4 OpacityValues = texture(opacity_opacity, shadowcoords);
    float OpacityStartingDepth = texture(opacity_depth, shadowcoords).r;

    float LinearizedSceneZ = LinearizeDepth(shadowcoords_f.z / shadowcoords_f.w);
    float LinearizedStartingDepth = LinearizeDepth(OpacityStartingDepth);

    float opacity = 0.0;

    if (LinearizedSceneZ > LinearizedStartingDepth + 3 * dStep)
    {
	opacity = OpacityValues.r;
	opacity += OpacityValues.g;
	opacity += OpacityValues.b;
	opacity += OpacityValues.a;
    }
    else
    if (LinearizedSceneZ > LinearizedStartingDepth + 2 * dStep)
    {
	float faktor = (LinearizedSceneZ - (LinearizedStartingDepth + 2*dStep)) / dStep;

	opacity = OpacityValues.r;
	opacity += OpacityValues.g;
	opacity += OpacityValues.b;
	opacity += OpacityValues.a * faktor;
    }
    else
    if (LinearizedSceneZ > LinearizedStartingDepth + dStep)
    {
	float faktor = (LinearizedSceneZ - (LinearizedStartingDepth + dStep)) / dStep;

	opacity = OpacityValues.r;
	opacity += OpacityValues.g;
	opacity += OpacityValues.b * faktor;
    }
    else
    if (LinearizedSceneZ > LinearizedStartingDepth)
    {
	float faktor = (LinearizedSceneZ - LinearizedStartingDepth) / dStep;

	opacity = OpacityValues.r;
	opacity += OpacityValues.g * faktor;
    }

    if ((shadowcoords.x <= 0.01 || shadowcoords.x >= 0.99)||
	(shadowcoords.y <= 0.01 || shadowcoords.y >= 0.99) || bUseShadow==0)
	return 1.0;
    else
	return 1.0 - opacity;
}

void main(void)
{
    float distSqr = dot(my_LightDir, my_LightDir);

    vec3 lVec = my_LightDir * inversesqrt(distSqr);

    vec3 vVec = normalize(my_EyeDir);

    vec4 texture_value = texture(color_texture, my_Texcoord.st);

    if (NormalTextureAvailable == 1)
    {
	vec4 bump_value = texture(normal_texture, my_Texcoord.st);

	vec4 base = texture_value;

	vec3 bump = normalize(bump_value.rgb * 2.0 - 1.0);

	vec4 vAmbient = Light0Ambient * MaterialAmbient;

	float diffuse = max( dot(lVec, bump), 0.0 );

	vec4 vDiffuse = Light0Diffuse * MaterialDiffuse * diffuse;

	float specular = pow(clamp(dot(reflect(-lVec, bump), vVec), 0.0, 1.0), 10);

	vec4 vSpecular = Light0Specular * MaterialSpecular * specular;

	vec3 light_value = (vAmbient).rgb + (vDiffuse).rgb + vSpecular.rgb;

	//vec4 light_value = (vAmbient*base + vDiffuse*base + vSpecular) * att;

	out_Color = vec4(light_value * texture_value.rgb * calcLight_DeepOpacity(), 1.0);
    }
    else
    {
	vec4 vAmbient = Light0Ambient * MaterialAmbient;

	vec3 bump = normalize(NormalMatrix * vec3(-1.0, 1.0, -1.0));

	float diffuse = max( dot(lVec, bump), 0.0 );

	vec4 vDiffuse = Light0Diffuse * MaterialDiffuse * diffuse;

	float specular = pow(clamp(dot(reflect(-lVec, bump), vVec), 0.0, 1.0), 10);

	vec4 vSpecular = Light0Specular * MaterialSpecular * specular;

	vec3 light_value = (vAmbient).rgb + (vDiffuse).rgb;// + vSpecular.rgb;

	out_Color =  texture_value * calcLight_DeepOpacity();
    }
}
