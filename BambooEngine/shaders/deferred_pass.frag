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
in vec3 my_Tangent;
in vec4 my_ScreenPosition;
in vec3 my_Color;
in vec4 my_Position;

uniform mat3 NormalMatrix;

uniform sampler2D color_texture;
uniform sampler2D normal_texture;
uniform sampler2D specular_texture;

uniform int nUseParallax;

float lerp(float a, float b, float s)

{

    return float(a + (b - a) * s);

}


vec2 GetParallaxOcclusionOffset(vec2 original_Texcoords)
{
    highp dvec3 vVec = (my_EyeDir);
    highp dvec3 vVec2 = vVec;
    vVec2.y = -vVec2.y;

    highp double dHeightScale = 0.02;

    highp dvec2 vCurrentTexCoords = original_Texcoords;

    highp double fCurrentTexHeight = texture2D(normal_texture, vCurrentTexCoords).a * dHeightScale;
    highp double fCurrentHeight = dHeightScale;
    highp double nNumSamples = mix(60,10, my_EyeDir.z);

    highp double fCurrentPreviousHeight = fCurrentHeight;
    highp dvec2 vCurrentPreviousTexCoords = vCurrentTexCoords;

    while (fCurrentHeight > fCurrentTexHeight)
    {
        vCurrentPreviousTexCoords = vCurrentTexCoords;
        vCurrentTexCoords = vCurrentTexCoords + vVec2.xy / nNumSamples;

        fCurrentTexHeight = texture2D(normal_texture, vCurrentTexCoords).a * dHeightScale;

        fCurrentPreviousHeight = fCurrentHeight;
        fCurrentHeight = fCurrentHeight + vVec2.z / nNumSamples;

        if (fCurrentHeight > fCurrentPreviousHeight)
            break;

        if (fCurrentHeight < 0.0)
            break;
     }

    highp dvec2 dBegin = vCurrentTexCoords;
    highp dvec2 dEnd = vCurrentPreviousTexCoords;
    highp dvec2 dMiddle = 0.5 * dBegin + 0.5 * dEnd;
    highp double dTexHeightAtMiddle = texture2D(normal_texture, dMiddle).a * dHeightScale;

    highp double dEyeHeightAtBegin = fCurrentHeight;
    highp double dEyeHeightAtEnd = fCurrentPreviousHeight;
    highp double dEyeHeightAtMiddle = dEyeHeightAtBegin * 0.5 + dEyeHeightAtEnd * 0.5;

    int iAbort = 0;

    while (abs(dTexHeightAtMiddle - dEyeHeightAtMiddle) > 0.0001 && length(dEnd - dBegin) > 0.00001)
    {
        iAbort += 1;

        if (iAbort > 20)
            break;

        if (dTexHeightAtMiddle > dEyeHeightAtMiddle)
        {
            dBegin = dMiddle;
            dMiddle = 0.5 * dBegin + 0.5 * dEnd;
            dTexHeightAtMiddle = texture2D(normal_texture, dMiddle).a * dHeightScale;

            dEyeHeightAtBegin = dEyeHeightAtMiddle;
            dEyeHeightAtMiddle = dEyeHeightAtBegin * 0.5 + dEyeHeightAtEnd * 0.5;
        }
        else
        {
            dEnd = dMiddle;
            dMiddle = 0.5 * dBegin + 0.5 * dEnd;
            dTexHeightAtMiddle = texture2D(normal_texture, dMiddle).a * dHeightScale;

            dEyeHeightAtEnd = dEyeHeightAtMiddle;
            dEyeHeightAtMiddle = dEyeHeightAtBegin * 0.5 + dEyeHeightAtEnd * 0.5;
        }
    }

  //  highp double fFactorBack = (fCurrentTexHeight - fCurrentHeight) / (fCurrentPreviousHeight - fCurrentHeight);

//    vCurrentTexCoords = vCurrentTexCoords * fFactorBack + vCurrentPreviousTexCoords * (1.0 - fFactorBack);
    //vCurrentTexCoords = vCurrentTexCoords * (1.0 - fFactorBack) + vCurrentPreviousTexCoords * (fFactorBack);
    //vCurrentTexCoords = dMiddle;

    return (dMiddle - original_Texcoords);
    //return (vCurrentTexCoords - original_Texcoords);
}

vec2 GetParallaxOffset(vec2 original_Texcoords)
{
    vec3 vVec = normalize(my_EyeDir);
    vec3 vVec2 = vVec;
    vVec2.x = -vVec2.x;	//dont know why this is needed - maybe a bug in viewvector calculation?

    float scale = 0.015f;
    float height = texture2D(normal_texture, my_Texcoord.xy).a;
    float offset = scale * (2.0 * height - 1.0);

    return  (offset * vVec2.xy);// / vVec2.z;
}

void main(void)
{

   vec2 vTexOffset;

   if (nUseParallax == 2)
    vTexOffset = GetParallaxOcclusionOffset(my_Texcoord.xy);
   else if (nUseParallax == 1)
    vTexOffset = GetParallaxOffset(my_Texcoord.xy);
   else if (nUseParallax == 0)
       vTexOffset = vec2(0.0);



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
