#version 330 core

in vec3 coords;
out vec4 uNew;		    // output color, new velocity

uniform float halfrdx;	    // 0.5 / gridscale

uniform sampler2DRect p;	    // pressure

uniform sampler2DRect w;	    // velocity

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
    float pL = Oto1ToMinus1To1(texture(p, coords.xy - vec2(1, 0))).r;
    float pR = Oto1ToMinus1To1(texture(p, coords.xy + vec2(1, 0))).r;
    float pB = Oto1ToMinus1To1(texture(p, coords.xy - vec2(0, 1))).r;
    float pT = Oto1ToMinus1To1(texture(p, coords.xy + vec2(0, 1))).r;

    vec4 temp;
    temp = Oto1ToMinus1To1(texture(w, coords.xy));
    temp.rg -= halfrdx * vec2((pR - pL), (pT - pB));

    uNew = Minus1To1To0To1(temp);
}
