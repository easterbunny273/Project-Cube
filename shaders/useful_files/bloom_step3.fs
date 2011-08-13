/*
 * fragment shader for postprocess bloom effect
 * third step, vertical blur
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 11/2010
 */

#version 330 core

uniform sampler2D texture1;

in vec3 my_Texcoord;
out vec4 FragColor;

const float ddx = 0.000976 * 2;
const float ddy = 0.001302 * 2;

void main(void)
{
  //Initialisierungen
  vec4 outp = vec4(0.0, 0.0, 0.0, 0.0);

  // Texturen auslesen
  // und horizontal bluren (gauss)

  //eigentlich sind es ja 5 werte, aber die folgende zeile liest 2 texturwerte
  //auf einmal aus indem er zwischen 2 pixel ausliest und linear filtern laesst:
  /*outp += 1 * texture2D(texture1, my_Texcoord.xy + vec2(0.0, ddy*-3.0) );
  outp += 6 * texture2D(texture1, my_Texcoord.xy + vec2(0.0, ddy*-2.0) );
  outp += 15 * texture2D(texture1, my_Texcoord.xy + vec2(0.0, ddy*-1.0) );
  outp += 20 * texture2D(texture1, my_Texcoord.xy + vec2(0.0, ddy*0.0) );
  outp += 15 * texture2D(texture1, my_Texcoord.xy + vec2(0.0, ddy*1.0) );
  outp += 6 * texture2D(texture1, my_Texcoord.xy + vec2(0.0, ddy*2.0) );
  outp += 1 * texture2D(texture1, my_Texcoord.xy + vec2(0.0, ddy*3.0) );*/

  outp += 21 * texture2D(texture1, my_Texcoord.xy + vec2(0.0, ddy*-1.0 - 0.286 * ddy) );
  outp += 20 * texture2D(texture1, my_Texcoord.xy + vec2(0.0, 0.0) );
  outp += 21 * texture2D(texture1, my_Texcoord.xy + vec2(0.0, ddy*1.0 + 0.286 * ddy) );

  outp /= 62;

  FragColor =  outp;

 }
