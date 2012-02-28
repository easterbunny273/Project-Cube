/*
 * fragment shader for parallax lighting
 *
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 11/2010
 */

#version 400 core

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
in vec3 my_Normal_EM;
in vec3 my_Tangent;
in vec4 my_ScreenPosition;
in vec3 my_Color;
in vec4 my_Position;
in vec3 my_ViewDir_EM;

uniform mat3 NormalMatrix;

uniform sampler2D color_texture;
uniform sampler2D normal_texture;
uniform sampler2D specular_texture;

uniform samplerCube cubemap_texture;

uniform int nUseParallax;
uniform bool bIsSphere;

vec2 GetParallaxOcclusionOffset(vec2 original_Texcoords, bool bDoBinarySearch)
{
    vec3 vVec = (my_EyeDir);
    vec3 vVec2 = vVec;
    vVec2.y = -vVec2.y;

    float dHeightScale = 0.02;

    vec2 vCurrentTexCoords = original_Texcoords;

    float fCurrentTexHeight = (1.0 - 1.0 * dHeightScale) + texture2D(normal_texture, vCurrentTexCoords).a * dHeightScale;
    float fCurrentHeight = 1.0;
    float nNumSamples = mix(8, 1, my_EyeDir.z);

    float fCurrentPreviousHeight = fCurrentHeight;
    vec2 vCurrentPreviousTexCoords = vCurrentTexCoords;

    while (fCurrentHeight > fCurrentTexHeight)
    {
        vCurrentPreviousTexCoords = vCurrentTexCoords;
        vCurrentTexCoords = vCurrentTexCoords + vVec2.xy / nNumSamples;

        fCurrentTexHeight = (1.0 - 1.0 * dHeightScale) + texture2D(normal_texture, vCurrentTexCoords).a * dHeightScale;

        fCurrentPreviousHeight = fCurrentHeight;
        fCurrentHeight = fCurrentHeight + vVec2.z / nNumSamples;

        if (fCurrentHeight > fCurrentPreviousHeight)
            break;

        if (fCurrentHeight < 0.0)
            break;
     }

    vec2 dBegin = vCurrentTexCoords;
    vec2 dEnd = vCurrentPreviousTexCoords;
    vec2 dMiddle = 0.5 * dBegin + 0.5 * dEnd;
    float dTexHeightAtMiddle = (1.0 - 1.0 * dHeightScale) + texture2D(normal_texture, dMiddle).a * dHeightScale;

    float dEyeHeightAtBegin = fCurrentHeight;
    float dEyeHeightAtEnd = fCurrentPreviousHeight;
    float dEyeHeightAtMiddle = dEyeHeightAtBegin * 0.5 + dEyeHeightAtEnd * 0.5;

    int iAbort = 0;

    while (bDoBinarySearch && abs(dTexHeightAtMiddle - dEyeHeightAtMiddle) > 0.0001 && length(dEnd - dBegin) > 0.0001)
    {
        iAbort += 1;

        if (iAbort > 20)
            break;

        if (dTexHeightAtMiddle > dEyeHeightAtMiddle)
        {
            dBegin = dMiddle;
            dMiddle = 0.5 * dBegin + 0.5 * dEnd;
            dTexHeightAtMiddle = (1.0 - 1.0 * dHeightScale) + texture2D(normal_texture, dMiddle).a * dHeightScale;

            dEyeHeightAtBegin = dEyeHeightAtMiddle;
            dEyeHeightAtMiddle = dEyeHeightAtBegin * 0.5 + dEyeHeightAtEnd * 0.5;
        }
        else
        {
            dEnd = dMiddle;
            dMiddle = 0.5 * dBegin + 0.5 * dEnd;
            dTexHeightAtMiddle = (1.0 - 1.0 * dHeightScale) +  texture2D(normal_texture, dMiddle).a * dHeightScale;

            dEyeHeightAtEnd = dEyeHeightAtMiddle;
            dEyeHeightAtMiddle = dEyeHeightAtBegin * 0.5 + dEyeHeightAtEnd * 0.5;
        }
    }

    return (dMiddle - original_Texcoords);

}

vec2 GetParallaxOffset(vec2 original_Texcoords)
{
    vec3 vVec = normalize(my_EyeDir);
    vec3 vVec2 = vVec;
    vVec2.x = -vVec2.x;	//dont know why this is needed - maybe a bug in viewvector calculation?

    float scale = 0.01f;
    float height = texture2D(normal_texture, my_Texcoord.xy).a;
    float offset = scale * (2.0 * height - 1.0);

    return  (offset * vVec2.xy);// / vVec2.z;
}

void main(void)
{

   /*vec2 vTexOffset;

   float fDepth = my_ScreenPosition.z / my_ScreenPosition.w;

   float fNear = 0.8;
   float fFar = 0.9;

   if (fDepth < fNear)
    vTexOffset = GetParallaxOcclusionOffset(my_Texcoord.xy, true);
   else if (fDepth >= fNear && fDepth <= fFar)
   {
       float f2 = (fDepth - fNear) / (fFar - fNear);
       float f1 = 1.0 - f2;

    vTexOffset = GetParallaxOcclusionOffset(my_Texcoord.xy, true) * f1 + GetParallaxOffset(my_Texcoord.xy) * f2;
   }
   else*/

    vec2 vTexOffset;

    /*if (nUseParallax == 3)
        vTexOffset = GetParallaxOcclusionOffset2(my_Texcoord.xy, true);*/
    if (nUseParallax == 1)
        vTexOffset = GetParallaxOcclusionOffset(my_Texcoord.xy, true);
   /* else if (nUseParallax == 1)
        vTexOffset = GetParallaxOffset(my_Texcoord.xy);*/
   else if (nUseParallax == 0)
       vTexOffset = vec2(0.0);


    if (bIsSphere)
    {
      vec3 modified_Normal = my_Normal_EM;
      //modified_Normal.x = - modified_Normal.x;

      vec3 reflectVector = normalize(reflect(normalize(my_ViewDir_EM), normalize(modified_Normal)));

      out_Albedo = vec4(texture(cubemap_texture, reflectVector).rgb, 1.0);
      //out_Albedo += vec4(reflect(my_EyeDir, normalize(my_Normal)).rgb, 1.0) * 0.01;
      //out_Albedo += vec4(normalize(my_Normal), 1.0);
    }
      //out_Albedo = vec4(texture(cubemap_texture, vec3(0.0, 1.0, 0.0)).rgb, 1.0);
    else
      out_Albedo = vec4(texture2D(color_texture, my_Texcoord.xy + vTexOffset).rgb, 1.0) * 0.1;
   //out_Albedo = vec4(vec3(normalize(my_EyeDir).z , 0.0, 0.0), 1.0);
   // out_Albedo = vec4(texcoord_offset, 0.0, 1.0);

   // write depth into alpha
   out_Albedo.a = my_ScreenPosition.z / my_ScreenPosition.w;

   out_Normal = vec4(my_Normal, 1.0);
   out_NormalMap = texture2D(normal_texture, my_Texcoord.xy + vTexOffset);
   out_Tangent = vec4(my_Tangent, 1.0);
   out_Specular = vec4(texture2D(color_texture, my_Texcoord.xy  + vTexOffset).a);
   out_Position = (my_Position / my_Position.w);
}
