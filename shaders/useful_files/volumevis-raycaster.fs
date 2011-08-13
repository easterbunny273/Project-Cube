/*
 * fragment shader volmevis raycasting
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 06/2011
 */

#version 330 core

uniform sampler2D cube_front;
uniform sampler2D cube_back;
uniform sampler2DRect smoke;
//uniform sampler2D scene_depth_tex;

uniform sampler2D opacity_depth;
uniform sampler2D opacity_opacity;

uniform mat4 Camera_TempTransformMatrix;

uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

uniform int bInterpolate;
uniform int bUseShadow;
uniform int bShowLevels;

//in vec4 my_Texcoord;
out vec4 FragColor;
in vec4 FinalPosition;
in vec4 ObjectPosition;

const int iSteps = 100;

const float n = 0.1; // camera z near
const float f = 100.0; // camera z far

const float n2 = 2.5; // camera z near
const float f2 = 10.0; // camera z far


float LinearizeDepth(float z)
{
  return (2.0 * n) / (f + n - z * (f - n));
}

float UnLinearizeDepth(float w)
{
  return -(((2*n)/w) - f - n) / (f - n);
}

float LinearizeDepth2(float z)
{
  return (2.0 * n2) / (f2 + n2 - z * (f2 - n2));
}

float UnLinearizeDepth2(float w)
{
  return -(((2*n2)/w) - f2 - n2) / (f2 - n2);
}

vec3 GetPositionFromColor(vec3 color)
{
    //return color * 2.0 - vec3(1.0);
    return (color - vec3(0.5)) * 2.0;
}

vec4 calcLight_DeepOpacity(vec3 vCurrentPosition)
{
    const float dStep = 0.025;

    vec4 shadowcoords_f = Camera_TempTransformMatrix * ModelMatrix * vec4(vCurrentPosition, 1.0);
    vec2 shadowcoords = (shadowcoords_f / shadowcoords_f.w).xy;
    shadowcoords += 1.0;
    shadowcoords *= 0.5;

    vec4 OpacityValues = texture(opacity_opacity, shadowcoords);
    vec4 OpacityStartingDepth = texture(opacity_depth, shadowcoords.xy);

    float LinearizedStartingDepth = LinearizeDepth2(OpacityStartingDepth.r);
    float LinearizedSceneZ = LinearizeDepth2(shadowcoords_f.z / shadowcoords_f.w);
    float opacity = 0.0;

    if (LinearizedSceneZ > LinearizedStartingDepth + 3 * dStep)
    {
	float faktor = (LinearizedSceneZ - (LinearizedStartingDepth + 3*dStep)) / dStep;

	faktor = min(1.0, faktor);

	opacity = OpacityValues.r;
	opacity += OpacityValues.g;
	opacity += OpacityValues.b;

	if (bInterpolate == 1)
	    opacity += OpacityValues.a * faktor;
    }
    else
    if (LinearizedSceneZ > LinearizedStartingDepth + 2 * dStep)
    {
	float faktor = (LinearizedSceneZ - (LinearizedStartingDepth + 2*dStep)) / dStep;

	opacity = OpacityValues.r;
	opacity += OpacityValues.g;

	if (bInterpolate == 1)
	    opacity += OpacityValues.b * faktor;
	//opacity += OpacityValues.a * faktor;
    }
    else
    if (LinearizedSceneZ > LinearizedStartingDepth + dStep)
    {
	float faktor = (LinearizedSceneZ - (LinearizedStartingDepth + dStep)) / dStep;

	opacity = OpacityValues.r;

	if (bInterpolate == 1)
	    opacity += OpacityValues.g * faktor;
	//opacity += OpacityValues.b * faktor;
    }
    else
    if (LinearizedSceneZ > LinearizedStartingDepth)
    {
	float faktor = (LinearizedSceneZ - LinearizedStartingDepth) / dStep;

	if (bInterpolate == 1)
	    opacity = OpacityValues.r * faktor;
	//opacity += OpacityValues.g * faktor;
    }

    if (bShowLevels == 1)
    {
    if (LinearizedSceneZ > LinearizedStartingDepth + 3*dStep)
	return vec4(0.0, 0.0, 1.0, 1.0);
    else
    if (LinearizedSceneZ > LinearizedStartingDepth + 2*dStep)
	return vec4(0.0, 1.0, 0.0, 1.0);
    else if (LinearizedSceneZ > LinearizedStartingDepth + dStep)
	return vec4(1.0, 0.0, 0.0, 1.0);
    else if (LinearizedSceneZ > LinearizedStartingDepth)
	return vec4(1.0, 1.0, 0.0, 1.0);
    else
	return vec4(0.0, 0.0, 0.0, 0.0);
    }

    if (opacity > 1.0)
	opacity = 1.0;

    return vec4(opacity, opacity, opacity, opacity);
}

float phong_shading(vec3 normal)
{
    vec3 Light0Diffuse = vec3(0.6);
    vec3 MaterialDiffuse = vec3(1.0);

    vec3 light_value = vec3(0.1);
    vec3 my_LightDir = vec3(-1.0, 0.0, 0.0);

    vec3 N = normalize(normal);
    vec3 L = normalize(my_LightDir);

    float lambertTerm = dot(N, L);

    if(lambertTerm > 0.0)
    {
	    light_value += Light0Diffuse.xyz * MaterialDiffuse.xyz * lambertTerm;

	  /*  vec3 E = normalize(my_EyeDir);
	    vec3 R = reflect(-L, N);

	    float specular = pow(max(dot(R, E), 0.0), 50.0);

	    light_value += Light0Specular.xyz * MaterialSpecular.xyz * specular;*/
    }

    return 1.0 - abs(normal.x);
}

void main(void)
{
vec4 my_Texcoord = FinalPosition / FinalPosition.w;
my_Texcoord = my_Texcoord + 1.0;
my_Texcoord /= 2.0;

vec4 p1 = texture(cube_front, my_Texcoord.xy);
vec4 p2 = texture(cube_back, my_Texcoord.xy);

vec3 p1_Pos = GetPositionFromColor(p1.xyz);
vec3 p2_Pos = GetPositionFromColor(p2.xyz);

float depth = 0.5;
float final_depth = depth;

bool bDepthNotSet = true;

vec4 vStepSize = (p2 - p1) / iSteps;
vec4 vSummedSmoke;

vec4 p2Top1 = p2 - p1;

float length = sqrt(p2Top1.x * p2Top1.x + p2Top1.y * p2Top1.y + p2Top1.z * p2Top1.z);

for (int i=0; i < iSteps; i++)
{
 vec4 CurrentPosition = p1 + (p2 - p1) * (i / float(iSteps));
 vec3 CurrentObjPosition = p1_Pos + (p2_Pos - p1_Pos) * (i / float(iSteps));

  vec4 smokevalue = (texture(smoke, (p1 + i*vStepSize).xy * 256.0)) * length * (2.0 / iSteps);

  smokevalue.a = max(smokevalue.r, max(smokevalue.g, smokevalue.b));

    vec4 SmokeColor;

    if (bUseShadow != 0)
	SmokeColor = vec4(smokevalue.rgb * (1.0 - calcLight_DeepOpacity(CurrentObjPosition).r), smokevalue.a * 8);
    else
	SmokeColor = vec4(smokevalue.rgb, smokevalue.a * 8);

  vec4 smokevalue_left = (texture(smoke, (p1 + i*vStepSize).xy * 256.0 - vec2(1.0, 0.0))) * length * (2.0 / iSteps);
  vec4 smokevalue_right = (texture(smoke, (p1 + i*vStepSize).xy * 256.0 + vec2(1.0, 0.0))) * length * (2.0 / iSteps);

  float DifLeft = smokevalue.r - smokevalue_left.r;
  float DifRight = smokevalue_right.r - smokevalue.r;

  vec3 Normal = vec3(500 * (DifRight + DifLeft), 1.0, 0.0);

  Normal = normalize(Normal);

  if(bShowLevels == 0)
    {
	SmokeColor.xyz *= phong_shading(Normal);
	vSummedSmoke += SmokeColor * (1.0 - vSummedSmoke.a);
    }
  else
    vSummedSmoke += vec4(calcLight_DeepOpacity(CurrentObjPosition));

  if (vSummedSmoke.a >= 1.0)
  {
    vSummedSmoke.a = 1.0;
    break;
  }

}

FragColor = vSummedSmoke;

}

