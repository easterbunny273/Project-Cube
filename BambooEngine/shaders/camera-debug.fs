/*
 * simple shader for bounding boxes
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 05/2011
 */

#version 330 core

layout (location = 0) out vec4 FragColor;

uniform sampler2D color_texture;
uniform sampler2D normal_texture;
uniform sampler2D depth_texture;
uniform sampler2D position_texture;

uniform sampler2D shadowmap;

uniform mat4 Light_ViewProjectionMatrix;
uniform mat4 ModelViewProjectionMatrix;

uniform vec3 vLightColor;

in vec4 finalPosition;

void main()
{
    vec2 vTexCoords = finalPosition.xy / finalPosition.w;
    vTexCoords += vec2(1.0);
    vTexCoords /= 2.0;

    vec4 v4Position = texture(position_texture, vTexCoords);

    vec4 vSSPosLight = Light_ViewProjectionMatrix * (v4Position / v4Position.w);
    vSSPosLight = vSSPosLight / vSSPosLight.w;

    vec2 vTexCoordsShadowMap = (vSSPosLight.xy + vec2(1.0)) / 2.0;

    float fDepthInShadowMap = texture(shadowmap, vTexCoordsShadowMap).a;
    float fDepth = vSSPosLight.z;

    if (vTexCoordsShadowMap.x > 1.0 || vTexCoordsShadowMap.x < 0.0)
        discard;
    if (vTexCoordsShadowMap.y > 1.0 || vTexCoordsShadowMap.y < 0.0)
        discard;

    //FragColor = texture(shadowmap, vTexCoordsShadowMap);

    if (fDepth < fDepthInShadowMap + 0.00001)
    {
        vec3 vAlbedo = texture(color_texture, vTexCoords).rgb;

        float fRed = vAlbedo.r * vLightColor.r;
        float fGreen = vAlbedo.g * vLightColor.g;
        float fBlue = vAlbedo.b * vLightColor.b;

        FragColor = vec4(fRed, fGreen, fBlue, 1.0);
    }
    else
    {
        vec3 vAlbedo = texture(color_texture, vTexCoords).rgb;
        FragColor = vec4(vAlbedo*0.01, 0.0);
    }

    //FragColor = vec4(vec3(fDepth), 1.0);
    //FragColor = vec4(vec3(fDepthInShadowMap), 1.0);
    //FragColor = vec4(fDepthInShadowMap, fDepth, 1.0, 1.0);



    //FragColor = vec4(texture(shadowmap, vTexCoordsShadowMap).rgb, 1.0);

   // FragColor = vec4(vScreenSpaceTest.rgb / vScreenSpaceTest.w, 1.0);
    //FragColor = vec4(finalPosition.rgb / finalPosition.w, 1.0);

   // if (texture(depth_texture, vTexCoords).r > (finalPosition.z / finalPosition.w))

   // else
    //    discard;
}
