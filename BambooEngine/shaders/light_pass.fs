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
uniform sampler2D specular_texture;

uniform sampler2D shadowmap;
uniform sampler2D spotmask;

uniform mat4 Light_ViewProjectionMatrix;
uniform mat4 ModelViewProjectionMatrix;
uniform mat4 ViewMatrix;

uniform vec3 vLightColor;
uniform vec3 LightPosition;

in vec4 finalPosition;

const float epsilon = 0.00002;

float GetLitFactor(float fDepth, vec2 vTexCoords)
{
  float fTexelSize = 1 / 512.0f;


  float fDepthInShadowMap1 = texture(shadowmap, vTexCoords - 0.5 * vec2(fTexelSize, fTexelSize)).a;
  float fDepthInShadowMap2 = texture(shadowmap, vTexCoords + 0.5 * vec2(fTexelSize, fTexelSize)).a;
  float fDepthInShadowMap3 = texture(shadowmap, vTexCoords + 0.5 * vec2(-fTexelSize, fTexelSize)).a;
  float fDepthInShadowMap4 = texture(shadowmap, vTexCoords + 0.5 * vec2(fTexelSize, -fTexelSize)).a;

  float fDepthInShadowMap5 = texture(shadowmap, vTexCoords - 1.5 * vec2(fTexelSize, fTexelSize)).a;
  float fDepthInShadowMap6 = texture(shadowmap, vTexCoords + 1.5 * vec2(fTexelSize, fTexelSize)).a;
  float fDepthInShadowMap7 = texture(shadowmap, vTexCoords + 1.5 * vec2(-fTexelSize, fTexelSize)).a;
  float fDepthInShadowMap8 = texture(shadowmap, vTexCoords + 1.5 * vec2(fTexelSize, -fTexelSize)).a;

  float fDepthInShadowMap9 = texture(shadowmap, vTexCoords - vec2(1.5 * fTexelSize, 0.5 * fTexelSize)).a;
  float fDepthInShadowMap10 = texture(shadowmap, vTexCoords - vec2(1.5 * fTexelSize, -0.5 * fTexelSize)).a;
  float fDepthInShadowMap11 = texture(shadowmap, vTexCoords - vec2(1.5 * -fTexelSize, 0.5 * fTexelSize)).a;
  float fDepthInShadowMap12 = texture(shadowmap, vTexCoords - vec2(1.5 * -fTexelSize, 0.5 * fTexelSize)).a;

  float fDepthInShadowMap13 = texture(shadowmap, vTexCoords - vec2(0.5 * fTexelSize, 1.5 * fTexelSize)).a;
  float fDepthInShadowMap14 = texture(shadowmap, vTexCoords - vec2(-0.5 * fTexelSize, 1.5 * fTexelSize)).a;
  float fDepthInShadowMap15 = texture(shadowmap, vTexCoords - vec2(0.5 * fTexelSize, -1.5 * fTexelSize)).a;
  float fDepthInShadowMap16 = texture(shadowmap, vTexCoords - vec2(-0.5 * fTexelSize, -1.5 * fTexelSize)).a;

  float fLitFactor = 0.0;

  if (fDepth < fDepthInShadowMap1 + epsilon)
    fLitFactor += 1 / 16.0;

  if (fDepth < fDepthInShadowMap2 + epsilon)
    fLitFactor += 1 / 16.0;

  if (fDepth < fDepthInShadowMap3 + epsilon)
    fLitFactor += 1 / 16.0;

  if (fDepth < fDepthInShadowMap4 + epsilon)
    fLitFactor += 1 / 16.0;

  if (fDepth < fDepthInShadowMap5 + epsilon)
  fLitFactor += 1 / 16.0;

  if (fDepth < fDepthInShadowMap6 + epsilon)
  fLitFactor += 1 / 16.0;

  if (fDepth < fDepthInShadowMap7 + epsilon)
  fLitFactor += 1 / 16.0;

  if (fDepth < fDepthInShadowMap8 + epsilon)
  fLitFactor += 1 / 16.0;

  if (fDepth < fDepthInShadowMap9 + epsilon)
  fLitFactor += 1 / 16.0;

  if (fDepth < fDepthInShadowMap10 + epsilon)
  fLitFactor += 1 / 16.0;

  if (fDepth < fDepthInShadowMap11 + epsilon)
  fLitFactor += 1 / 16.0;

  if (fDepth < fDepthInShadowMap12 + epsilon)
  fLitFactor += 1 / 16.0;

  if (fDepth < fDepthInShadowMap13 + epsilon)
  fLitFactor += 1 / 16.0;

  if (fDepth < fDepthInShadowMap14 + epsilon)
  fLitFactor += 1 / 16.0;

  if (fDepth < fDepthInShadowMap15 + epsilon)
  fLitFactor += 1 / 16.0;

  if (fDepth < fDepthInShadowMap16 + epsilon)
  fLitFactor += 1 / 16.0;

  return fLitFactor;
}

void main()
{
    //FragColor = vec4(vLightColor, 1.0);
    //return;
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

    vec3 vMaskValue = texture(spotmask, vTexCoordsShadowMap).rgb;


    if (vMaskValue.r < 0.001)
        discard;

    //FragColor = texture(shadowmap, vTexCoordsShadowMap);



    float fLitFactor = GetLitFactor(fDepth, vTexCoordsShadowMap);

    if (fLitFactor > 0.0)
    {
        vec3 vAlbedo = texture(color_texture, vTexCoords).rgb * 9;

        vec3 vNormal = texture(normal_texture, vTexCoords).rgb;
        vec3 vTangent = texture(tangent_texture, vTexCoords).rgb;
        vec3 vBitangent = cross(vNormal, vTangent);

        vec4 vVertexVS = (ViewMatrix * v4Position);
        vec3 vVertex = vVertexVS.xyz / vVertexVS.w;
        vec4 lightVecVS = ViewMatrix * vec4(LightPosition, 1.0);
        vec3 tmpVec = lightVecVS.xyz / lightVecVS.w - vVertex;

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
        float distSqr = 1.0 * dot(lightVec, lightVec);
        float invRadius = 0.5;
        float att = clamp(1.0 - invRadius * sqrt(distSqr), 0.0, 1.0);
        vec3 lVec = lightVec * inversesqrt(distSqr);

        vec3 vVec = normalize(eyeVec);

        float fMinColorReflection = 0.0;

        vec4 base = vec4(max(vAlbedo.r, fMinColorReflection), max(vAlbedo.g, fMinColorReflection), max(vAlbedo.b, fMinColorReflection), 1.0);
        //vec4 base = vec4(vAlbedo, 1.0);

        vec3 bump = normalize( texture(normalmap_texture, vTexCoords).xyz * 2.0 - 1.0);

        vec4 vAmbient = vec4(0.0);

        float diffuse = max( dot(lVec, bump), 0.0 );

        vec4 vDiffuse = vec4(vLightColor * diffuse, 1.0);

        float specular_factor_texture = texture(specular_texture, vTexCoords).r;

        float specular = pow(clamp(dot(reflect(-lVec, bump), vVec), 0.0, 1.0),
                         100.0 ) * specular_factor_texture;


        vec4 vSpecular = vec4(vLightColor * specular, 1.0);

        FragColor = ( vAmbient*base + vDiffuse*base + vSpecular) * att * vMaskValue.r * fLitFactor;
        //FragColor = vec4(vAlbedo, 1.0);
        //FragColor = vec4(texture(normalmap_texture, vTexCoords).aaa, 1.0);
        //FragColor = vec4(diffuse, 0.0, specular, 1.0);

        //FragColor = vec4(vTangent, 1.0);
// -----------------------------
        //FragColor = vec4(fRed, fGreen, fBlue, 1.0);
    }
    else
        discard;

    //FragColor = vec4(vec3(fDepth), 1.0);
    //FragColor = vec4(vec3(fDepthInShadowMap), 1.0);
    //FragColor = vec4(fDepthInShadowMap, fDepth, 1.0, 1.0);



    //FragColor = vec4(texture(shadowmap, vTexCoordsShadowMap).rgb, 1.0);

   // FragColor = vec4(vScreenSpaceTest.rgb / vScreenSpaceTest.w, 1.0);


   // if (texture(depth_texture, vTexCoords).r > (finalPosition.z / finalPosition.w))

   // else
    //    discard;
}
