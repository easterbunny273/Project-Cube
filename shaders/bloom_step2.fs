/*
 * fragment shader for postprocess bloom effect
 * second step, horizontal blur
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 11/2010
 */

#version 330 core

uniform sampler2D texture1;

in vec3 my_Texcoord;
out vec4 FragColor;

//resolution^-1
const float ddx = 0.000976 * 2;
const float ddy = 0.001302 * 2;


void main(void)
{
  //Initialisierungen
  vec4 outp = vec4(0.0, 0.0, 0.0, 0.0);

  // Texturen auslesen
  // und vertikal bluren (gauss)

  //eigentlich sind es ja 5 werte und kernel 1-4-6-4-1, aber die folgende zeile liest 2 texturwerte
  //auf einmal aus indem er zwischen 2 pixel ausliest und linear filtern laesst:
 /* outp += 3 * texture2D(texture1, my_Texcoord.xy + vec2(ddx*-3.0 - 0.2*ddx, 0.0) );
  outp += 5 * texture2D(texture1, my_Texcoord.xy + vec2(ddx*-1.0 - 0.2*ddx, 0.0) );

  outp += 6 * texture2D(texture1, my_Texcoord.xy + vec2(0.0, 0.0) );

  outp += 5 * texture2D(texture1, my_Texcoord.xy + vec2(ddx*1.0 + 0.2*ddx, 0.0) );
  outp += 3 * texture2D(texture1, my_Texcoord.xy + vec2(ddx*3.0 + 0.2*ddx, 0.0) );*/


  outp += 21 * texture2D(texture1, my_Texcoord.xy + vec2(ddx*-1.0 - 0.286 * ddx, 0.0) );
  outp += 20 * texture2D(texture1, my_Texcoord.xy + vec2(0.0, 0.0) );
  outp += 21 * texture2D(texture1, my_Texcoord.xy + vec2(ddx*1.0 + 0.286 * ddx, 0.0) );

  outp /= 62;

  FragColor = outp;
 }
