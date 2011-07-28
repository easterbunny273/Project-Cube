#version 330 core

uniform sampler2D texture1;		// input texture

in vec3 coords;
out vec4 FragColor;			// output texture

void main(void)
{
    FragColor = texture(texture1, coords.xy);
}
