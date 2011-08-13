#version 330 core

in vec3 coords;
out vec4 xNew;		    // output color

uniform float alpha;
uniform float rBeta;

uniform sampler2DRect x;
uniform sampler2DRect b;

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
    //left, right, bottom, and top x samples
    vec4 xL = Oto1ToMinus1To1(texture(x, coords.xy - vec2(1, 0)));
    vec4 xR = Oto1ToMinus1To1(texture(x, coords.xy + vec2(1, 0)));
    vec4 xB = Oto1ToMinus1To1(texture(x, coords.xy - vec2(0, 1)));
    vec4 xT = Oto1ToMinus1To1(texture(x, coords.xy + vec2(0, 1)));

    // b sample, from center

    vec4 bC = Oto1ToMinus1To1(texture(b, coords.xy));

    // evaluate Jacobi iteration

    xNew = Minus1To1To0To1((xL + xR + xB + xT + alpha * bC) * rBeta);
}
