#include "RenderEngine-UE/Generate.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/type_ptr.hpp>

#define POSITION_MIN_X -1.0f
#define POSITION_MAX_X 1.0f
#define POSITION_MIN_Y -1.0f
#define POSITION_MAX_Y 1.0f
#define POSITION_MIN_Z -1.0f
#define POSITION_MAX_Z 1.0f

#define RADIUS_MIN  0.0005f
#define RADIUS_MAX 0.005f

#define PI 3.14159265

inline float randf()
{
  return (rand() % 1000) / 1000.0f;
}

inline float randf(float fMin, float fMax)
{
  float fDiff = fMax - fMin;
  float fRand = randf() * fDiff + fMin;

  return fRand;
}

std::vector<GeometryData::GenericObject*> RenderEngineUE_Generation::GenerateSpheres(unsigned int nNum, unsigned int nLats, unsigned int nLongs)
{
  // prepare vector for generated spheres
  std::vector<GeometryData::GenericObject *> vpSpheres;
  vpSpheres.resize(nNum);


  for (unsigned int nSphere=0; nSphere < nNum; nSphere++)
    {
      // generate new object
      GeometryData::GenericObject *pNewObject = new GeometryData::GenericObject(1);
      GeometryData::GenericMesh *pNewMesh = pNewObject->GetMeshPtr(0);

      // calculate number of vertices and indices
      unsigned int nNumVertices = nLats * (nLongs+1) * 6;
      unsigned int nNumIndices = nLats * (nLongs+1) * 6;

      // allocate memory for vertices & indices
      float *pfVertices = pNewMesh->AllocForAttribute(GeometryData::TDATA_VERTICES, nNumVertices);
      unsigned int *pnIndices = pNewMesh->AllocForIndices(nNumIndices);

         int t=0;	//for index array

         for(int i = 1; i <= nLats; i++)
         {
                 double lat0 = PI * (-0.5 + (double) (i - 1) / nLats);
                 double z0  = sin(lat0);
                 double zr0 =  cos(lat0);

                 double lat1 = PI * (-0.5 + (double) i / nLats);
                 double z1 = sin(lat1);
                 double zr1 = cos(lat1);

                 for(int j = 0; j <= nLongs; j++)
                 {
                     double lng = 2 * PI * (double) (j - 1) / nLongs;
                     double x = cos(lng);
                     double y = sin(lng);

                     *pfVertices++ = x*zr0;
                     *pfVertices++ = y*zr0;
                     *pfVertices++ = -z0;

                     /* next longitude */
                     *pfVertices++ = x*zr1;
                     *pfVertices++ = y*zr1;
                     *pfVertices++ = -z1;

                     *pnIndices++ = t;
                     *pnIndices++ = t+1;
                     *pnIndices++ = t+2;


                     *pnIndices++ = t+2;
                     *pnIndices++ = t+1;
                     *pnIndices++ = t+3;

                     t+= 2;
                 }
         }

         // calculate a random position
         glm::vec3 fRandomPosition = glm::vec3(randf(POSITION_MIN_X, POSITION_MAX_X),
                                               randf(POSITION_MIN_Y, POSITION_MAX_Y),
                                               randf(POSITION_MIN_Z, POSITION_MAX_Z));

         // calculate a random radius
         float fRadius = randf(RADIUS_MIN, RADIUS_MAX);

         // calculate model matrix
         glm::mat4 mModelMatrix = glm::translate(glm::mat4(), fRandomPosition);
         mModelMatrix = glm::scale(mModelMatrix, glm::vec3(fRadius, fRadius, fRadius));

         // get memory for modelmatrix, and copy
         float *pfModelMatrix = pNewMesh->GetModelMatrix();
         memcpy(pfModelMatrix, &mModelMatrix[0][0], 16*sizeof(float));

         // insert generated sphere
         vpSpheres[nSphere] = pNewObject;
    }

  return vpSpheres;
}
