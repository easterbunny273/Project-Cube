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

float final_depth = 1.0;

bool bDepthNotSet = true;

vec4 vStepSize = (p2 - p1) / iSteps;
vec4 color;

float depth_front = LinearizeDepth(texture(cube_front, my_Texcoord.xy).a);
float depth_back = LinearizeDepth(texture(cube_back, my_Texcoord.xy).a);

for (int i=0; i < iSteps; i++)
{
  vec4 smokevalue = texture(smoke, (p1 + i*vStepSize).xy * 256);

  //float depth = depth_back - depth_front;
  float depth = (depth_front + i * ((depth_back - depth_front) / iSteps));

  if (((smokevalue.r > 0.001)||(smokevalue.g > 0.001)||(smokevalue.b > 0.001)) && bDepthNotSet)
  {
    bDepthNotSet = false;
    final_depth = UnLinearizeDepth(depth);
    break;
  }
}

FragColor = vec4(final_depth, 0.0, 0.0, 1.0);
}

