#version 330 core

out vec4 FragColor;
in vec4 finalPosition;

void main()
{
    FragColor = vec4(finalPosition.z / finalPosition.w, 0.0, 0.0, 1.0);
}
