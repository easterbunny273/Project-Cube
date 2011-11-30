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
uniform sampler2D normalmap_texture;
uniform sampler2D normal_texture;
uniform sampler2D tangent_texture;
uniform sampler2D depth_texture;
uniform sampler2D position_texture;

uniform sampler2D shadowmap;

uniform mat4 Light_ViewProjectionMatrix;
uniform mat4 ModelViewProjectionMatrix;

uniform vec3 vLightColor;
uniform vec3 LightPosition;

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

        vec3 vNormal = texture(normal_texture, vTexCoords).rgb;
        vec3 vTangent = texture(tangent_texture, vTexCoords).rgb;
        vec3 vBitangent = cross(vNormal, vTangent);

        vec3 vVertex = (v4Position / v4Position.w).rgb;
        vec3 tmpVec = LightPosition.xyz - vVertex;

        vec3 lightVec;
        lightVec.x = dot(tmpVec, vTangent);
        lightVec.y = dot(tmpVec, vBitangent);
        lightVec.z = dot(tmpVec, vNormal);

        tmpVec = -vVertex;

        vec3 eyeVec;
        eyeVec.x = dot(tmpVec, vTangent);
        eyeVec.y = dot(tmpVec, vBitangent);
        eyeVec.z = dot(tmpVec, vNormal);

// -----------------------------
        float distSqr = dot(lightVec, lightVec);
        float invRadius = 1;
        float att = clamp(1.0 - invRadius * sqrt(distSqr), 0.0, 1.0);
        vec3 lVec = lightVec * inversesqrt(distSqr);

        vec3 vVec = normalize(eyeVec);

        vec4 base = vec4(vAlbedo, 1.0);

        vec3 bump = normalize( texture(normalmap_texture, vTexCoords).xyz * 2.0 - 1.0);

        vec4 vAmbient = vec4(0.0);

        float diffuse = max( dot(lVec, bump), 0.0 );

        vec4 vDiffuse = vec4(vLightColor * diffuse, 1.0);

        float specular = pow(clamp(dot(reflect(-lVec, bump), vVec), 0.0, 1.0),
                         500.0 );

        vec4 vSpecular = vec4(vLightColor * specular, 1.0);

        FragColor = ( vAmbient*base + vDiffuse*base + vSpecular);// * att;

        //FragColor = vec4(vTangent, 1.0);
// -----------------------------
        //FragColor = vec4(fRed, fGreen, fBlue, 1.0);
    }
    else
    {
        vec3 vAlbedo = texture(color_texture, vTexCoords).rgb;
        FragColor = vec4(vAlbedo*0.02, 0.0);
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
