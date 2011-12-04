/*
 * fragment shader for parallax lighting
 *
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 11/2010
 */

#version 330 core

layout (location = 0) out vec4 out_Albedo;
layout (location = 1) out vec4 out_Normal;
layout (location = 6) out vec4 out_NormalMap;
layout (location = 2) out vec4 out_Tangent;
layout (location = 3) out vec4 out_Specular;
layout (location = 5) out vec4 out_Position;

in vec3 my_Texcoord;
in vec3 my_LightDir;
in vec3 my_ObjPosition;
in vec3 my_EyeDir;
in vec3 my_Normal;
in vec3 my_Tangent;
in vec4 my_ScreenPosition;
in vec3 my_Color;
in vec4 my_Position;

uniform mat3 NormalMatrix;

uniform sampler2D color_texture;
uniform sampler2D normal_texture;
uniform sampler2D specular_texture;

void main(void)
{

    vec3 vVec = normalize(my_EyeDir);
    vec3 vVec2 = vVec;
    vVec2.x = -vVec2.x;	//dont know why this is needed - maybe a bug in viewvector calculation?

    float scale = 0.0125f;
    float height = texture(normal_texture, my_Texcoord.xy).a;
    float offset = scale * (2.0 * height - 1.0);

    vec2 texcoord_offset = (offset * vVec2.xy);// / vVec2.z;


   out_Albedo = vec4(texture(color_texture, my_Texcoord.xy + texcoord_offset).rgb, 1.0) * 0.1;
   // out_Albedo = vec4(texcoord_offset, 0.0, 1.0);

   // write depth into alpha
   out_Albedo.a = my_ScreenPosition.z / my_ScreenPosition.w;

   out_Normal = vec4(my_Normal, 1.0);
   out_NormalMap = texture(normal_texture, my_Texcoord.xy + texcoord_offset);
   out_Tangent = vec4(my_Tangent, 1.0);


   out_Specular = vec4(texture(color_texture, my_Texcoord.xy  + texcoord_offset).a);

   out_Position = (my_Position / my_Position.w);
}
