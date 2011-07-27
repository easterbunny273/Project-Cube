#version 330 core

in vec3 coords;
out vec4 bv;		    // output balue

uniform sampler2DRect texture1;	    // state field
uniform float scale;
uniform float offset_x;
uniform float offset_y;

vec2 Oto1ToMinus1To1(vec2 old)
{
    vec2 new = old * 2 - vec2(1.0);

    return new;
}

vec2 Minus1To1To0To1(vec2 old)
{
    vec2 new = (old + vec2(1.0)) / 2.0;

    return new;
}


void main(void)
{
    vec2 Coords = vec2(coords.x, coords.y);
    vec2 Offset = vec2(offset_x, offset_y);

    if (scale == -1.0)
    {
    vec2 newValue = Minus1To1To0To1(-1.0 * Oto1ToMinus1To1(texture(texture1, Coords + Offset).rg));

    bv = vec4(newValue, 0.0, 1.0);
    }
    else
    {
    bv = texture(texture1, Coords + Offset);
    }

   // bv = vec4(1.0, 0.0, 0.0, 0.0) + bv * 0.00001;
}
