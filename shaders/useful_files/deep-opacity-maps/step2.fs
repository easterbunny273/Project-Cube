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
uniform sampler2D cube_depth_front;
uniform sampler2D cube_depth_back;
uniform sampler2DRect smoke;
uniform sampler2D step1;

uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

out vec4 FragColor;
in vec4 FinalPosition;

const int iSteps = 256;

const float n = 2.5; // camera z near
const float f = 10.0; // camera z far



float LinearizeDepth(float z)
{
  return (2.0 * n) / (f + n - z * (f - n));
}

float UnLinearizeDepth(float w)
{
  return -(((2*n)/w) - f - n) / (f - n);
}

void main(void)
{
vec4 my_Texcoord = FinalPosition / FinalPosition.w;
my_Texcoord = my_Texcoord + 1.0;
my_Texcoord /= 2.0;

vec4 p1 = texture(cube_front, my_Texcoord.xy);
vec4 p2 = texture(cube_back, my_Texcoord.xy);


bool bDepthNotSet = true;

vec4 vStepSize = (p2 - p1) / iSteps;

float depth_back = LinearizeDepth(p2.a);
float depth_front = LinearizeDepth(p1.a);

float step1_depth = LinearizeDepth(texture(step1, my_Texcoord.xy).r);

vec4 vSummedSmoke = vec4(0.0);


vec4 p2Top1 = p2 - p1;

float length = sqrt(p2Top1.x * p2Top1.x + p2Top1.y * p2Top1.y + p2Top1.z * p2Top1.z);

for (int i=0; i < iSteps; i++)
{
  vec4 smokevalue = (texture(smoke, (p1 + i*vStepSize).xy * 256)) * length;

  float depth = (depth_front + i * ((depth_back - depth_front) / iSteps));

  smokevalue.a = max(smokevalue.r, max(smokevalue.g, smokevalue.b)) / 80.0;
  //smokevalue.a *= smokevalue.a;

  const float dStep = 0.025;
  const float dEpsilon = 0.00;

  if (depth > step1_depth + dEpsilon)
  {
    if (depth <= step1_depth + dStep)
    {
	if (vSummedSmoke.r < 1.0)
	    vSummedSmoke.r += smokevalue.a;
    }
    else
    if (depth <= step1_depth + 2 * dStep)
    {
    	if (vSummedSmoke.g < 1.0)
	    vSummedSmoke.g += smokevalue.a;
    }
    else
    if (depth <= step1_depth + 3 * dStep)
    {
    	if (vSummedSmoke.b < 1.0)
	    vSummedSmoke.b += smokevalue.a;
    }
    else
    {
    	if (vSummedSmoke.a < 1.0)
	    vSummedSmoke.a += smokevalue.a;
	else
	    break;
    }
  }
}

FragColor = vSummedSmoke;
//FragColor = vec4(1.0, 0.75, 0.5, 0.25);//vSummedSmoke;//vec4(vec3(vSummedSmoke.rgb), 1.0);
}

