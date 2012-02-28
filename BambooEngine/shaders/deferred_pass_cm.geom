#version 400 core

layout(triangles) in;
layout(triangle_strip, max_vertices=18) out;

uniform mat4 ModelViewProjectionMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 TranslationMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

in vec3 attr_Normal[];
in vec3 attr_Tangent[];
in vec3 attr_Color[];
in vec4 attr_Position[];

out vec3 my_Texcoord;
out vec3 my_LightDir;
out vec3 my_EyeDir;
out vec3 my_ObjPosition;
out vec4 my_ScreenPosition;
out vec3 my_Normal;
out vec3 my_Tangent;
out vec3 my_Color;
out vec4 my_Position;
out vec3 my_Layer;
in vec3 texcoords[];

void main(void)
{
  mat4 mRotations[6];
  mRotations[0] = mat4(0.0, 0.0, -1.0, 0.0,
                      0.0, -1.0, 0.0, 0.0,
                      -1.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 1.0);

  mRotations[1] = mat4(0.0, 0.0, 1.0, 0.0,
                      0.0, -1.0, 0.0, 0.0,
                      1.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 1.0);

  mRotations[3] = mat4(1.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0,
                      0.0, -1.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 1.0);

  mRotations[2] = mat4(1.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, -1.0, 0.0,
                      0.0, 1.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 1.0);

  mRotations[4] = mat4(1.0, 0.0, 0.0, 0.0,
                      0.0, -1.0, 0.0, 0.0,
                      0.0, 0.0, -1.0, 0.0,
                      0.0, 0.0, 0.0, 1.0);

  mRotations[5] = mat4(-1.0, 0.0, 0.0, 0.0,
                      0.0, -1.0, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0,
                      0.0, 0.0, 0.0, 1.0);

  for (int iLayer = 0; iLayer < 6; iLayer++)
  {

    for (int tri_vert=0; tri_vert < 3; tri_vert++)
    {
      vec3 in_Position = attr_Position[tri_vert].xyz;

      gl_Layer = iLayer;
      my_Layer.x = iLayer;
      gl_Position = ProjectionMatrix * mRotations[iLayer] * TranslationMatrix * ModelMatrix * vec4(in_Position, 1.0);

      my_ObjPosition = in_Position;
      my_ScreenPosition = gl_Position;

      my_Texcoord = texcoords[tri_vert];

      my_Normal = normalize(NormalMatrix * attr_Normal[tri_vert]);
      my_Tangent = normalize(NormalMatrix * attr_Tangent[tri_vert]);

      my_Position = ModelMatrix * vec4(in_Position, 1.0);
      my_Color = attr_Color[tri_vert];

      vec4 v4Vertex = ViewMatrix * my_Position;
      vec3 vVertex = v4Vertex.xyz / v4Vertex.w;

      vec3 eyeVec;
      eyeVec.x = dot(vVertex, my_Tangent);
      eyeVec.y = dot(vVertex, cross(my_Normal, my_Tangent));
      eyeVec.z = dot(vVertex, my_Normal);

      my_EyeDir = eyeVec;

      EmitVertex();
    }
    EndPrimitive();
  }
}
