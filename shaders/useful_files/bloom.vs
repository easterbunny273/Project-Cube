/*
 * vertex shader for postprocess bloom effect
 * used for all steps
 * the shader simply uses the given coordinates
 * as screen-coordinates, without transformation
 *
 * written by:
 * christian moellinger <ch.moellinger@gmail.com>
 * 11/2010
 */

#version 330 core

uniform mat4 Camera_ProjectionMatrix;
uniform mat4 Camera_TransformMatrix;
uniform mat4 Model_TransformMatrix;
uniform mat3 NormalMatrix;

in vec3 in_Position;
in vec3 in_Texcoord;
out vec3 my_Texcoord;

void main(void)
{
    mat4 ProjectionMatrix = Camera_ProjectionMatrix;
    mat4 ModelViewMatrix = Camera_TransformMatrix * Model_TransformMatrix;

    vec4 Position_with_transformation = ProjectionMatrix * ModelViewMatrix * vec4(in_Position, 1.0);

    gl_Position = vec4(in_Position, 1.0);

    my_Texcoord = in_Texcoord;
}
