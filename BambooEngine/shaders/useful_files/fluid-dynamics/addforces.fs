#version 330 core

in vec3 coords;
out vec4 xNew;		    // output color

uniform sampler2DRect texture1;
uniform sampler2DRect texture2;

uniform int bAddForces;

vec4 Oto1ToMinus1To1(vec4 old)
{
    vec4 new = old * 2 - vec4(1.0);

    return new;
}

vec4 Minus1To1To0To1(vec4 old)
{
    vec4 new = (old + vec4(1.0)) / 2.0;

    return new;
}

void main(void)
{
    vec4 temp = Oto1ToMinus1To1(texture(texture1, coords.xy)) * 0.99 + Oto1ToMinus1To1(texture(texture2, coords.xy)) * 0.01;

    if (bAddForces == 1)
	xNew = Minus1To1To0To1(temp);
    else
    {
	xNew = Minus1To1To0To1(Oto1ToMinus1To1(texture(texture1, coords.xy)));// * 0.999 + Oto1ToMinus1To1(vec4(0.5)) * 0.001);
    }
}
