/*
 * fragment shader for postprocess bloom effect
 * final step, combines "normal" rendering result with bloom-texture
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 11/2010
 */

#version 330 core

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

uniform vec4 Light0Position;

in vec3 my_Texcoord;
out vec4 FragColor;

vec4 PoissonFilter (sampler2D tSource, vec2 vTexCoord, float fRadius)
{
    // Hard coded texel size: Assumes 1024x768 source texture
    vec2 vTexelSize = vec2( 1.0/1024.0, 1.0/768.0 );

    // Tap locations for unit disc generated with coderhaus tool
    vec2 poissonDisk[14];

    poissonDisk[0]= vec2(-0.2374331 , 0.8638624);
    poissonDisk[1]= vec2(0.2641038 , 0.8474243);
    poissonDisk[2]= vec2(0.1076127 , 0.273064);
    poissonDisk[3]= vec2(-0.3770348 , 0.329707);
    poissonDisk[4]= vec2(-0.7949351 , 0.504249);
    poissonDisk[5]= vec2(-0.5139115 , -0.1127825);
    poissonDisk[6]= vec2(-0.9599211 , -0.2194951);
    poissonDisk[7]= vec2(-0.04343576 , -0.4397294);
    poissonDisk[8]= vec2(-0.5187711 , -0.674867);
    poissonDisk[9]= vec2(0.07915238 , -0.8755394);
    poissonDisk[10]= vec2(0.3603834 , -0.1431846);
    poissonDisk[11]= vec2(0.7132849 , 0.6049219);
    poissonDisk[12]= vec2(0.455669 , -0.6152478);
    poissonDisk[13]= vec2(0.8725635 , -0.01751214);

    // Take a sample at the center of the disc
    vec4 cSampleAccum = texture2D( tSource, vTexCoord.xy );

    // Take 14 samples in disc
    for ( int nTapIndex = 0; nTapIndex < 14; nTapIndex++ )
    {
	// Compute new texture coord inside disc
	vec2 vTapCoord = vTexCoord + vTexelSize * poissonDisk[nTapIndex] * fRadius;

	// Accumulate samples
	cSampleAccum += texture2D( tSource, vTapCoord.xy);
    }
    return cSampleAccum / 14.0; // Return average
}

void main()
{
    float blurlevel = 0.0;

    float Z = texture2D(texture3, my_Texcoord.st).r;    // depth found in Z-Buffer

    // compute a Circle of Confusion size based on the Depth
    blurlevel = clamp(abs(Z*1.015*100-98), 0.0, 3.0) * 1.0;

    vec4 sceneColor = PoissonFilter(texture1, my_Texcoord.st, blurlevel);
    //vec4 sceneColor = texture2D(texture1, my_Texcoord.st);

    //vec4 bloomColor = texture2D(texture2, my_Texcoord.st);

    //vec4 BaseColor = (sceneColor + bloomColor);

    FragColor = sceneColor;// + BaseColor * 0.001;
}

