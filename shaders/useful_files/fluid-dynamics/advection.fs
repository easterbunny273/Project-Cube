#version 330 core

uniform sampler2DRect u;		// input velocity
uniform sampler2DRect x;		// qty to advect

uniform float timestep;		// timestamp
uniform float rdx;		// 1 / grid scale

in vec3 coords;
out vec4 xNew;		    // output color

vec2 normalize(vec2 old)
{
    vec2 new = old * 2.0 - vec2(1.0);

    return new;
}

void main(void)
{
    // follow the velocity field "back in time"
    vec2 pos = coords.xy - timestep * normalize(texture(u, coords.xy).rg) * rdx;

    xNew = texture(x, pos);
}
