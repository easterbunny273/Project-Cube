#version 330 core

in vec3 coords;
out vec4 div;		    // output color

uniform float halfrdx;	    // 0.5 / gridscale

uniform sampler2DRect w;	    // vector field

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
    vec4 wL = Oto1ToMinus1To1(texture(w, coords.xy - vec2(1, 0)));
    vec4 wR = Oto1ToMinus1To1(texture(w, coords.xy + vec2(1, 0)));
    vec4 wB = Oto1ToMinus1To1(texture(w, coords.xy - vec2(0, 1)));
    vec4 wT = Oto1ToMinus1To1(texture(w, coords.xy + vec2(0, 1)));

    float fDivergence = halfrdx * ((wR.x - wL.x) + (wT.y - wB.y));

    div = Minus1To1To0To1(vec4(fDivergence, 0.0, 0.0, 1.0));
}
