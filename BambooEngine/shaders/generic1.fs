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

void main(void)
{
   //mat3 mTransformObjectToTangentSpace = mat3(my_Tangent, my_Bitangent, my_Normal);
   //mat3 mTransformTangentToObjectSpace = transpose(mTransformObjectToTangentSpace);

   //vec3 vNormalFromMapInTS = (texture(normal_texture, my_Texcoord.xy).rgb - vec3(0.5)) * 2.0;
   //vec3 vNormalFromMapInTS2 = vec3(vNormalFromMapInTS.x, vNormalFromMapInTS.z, vNormalFromMapInTS.y);

   //vec3 vNormalFromMapInOS = normalize(mTransformObjectToTangentSpace * vNormalFromMapInTS2);


   out_Albedo = vec4(texture(color_texture, my_Texcoord.xy).rgb, 1.0);

   // write depth into alpha
   out_Albedo.a = my_ScreenPosition.z / my_ScreenPosition.w;

   out_Normal = vec4(my_Normal, 1.0);
   out_NormalMap = texture(normal_texture, my_Texcoord.xy);
   out_Tangent = vec4(my_Tangent, 1.0);
   out_Specular = vec4(1.0);
   out_Position = (my_Position / my_Position.w);
}
