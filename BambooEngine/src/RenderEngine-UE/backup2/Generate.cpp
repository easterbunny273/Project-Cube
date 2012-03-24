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

#define RADIUS_MIN  0.01f
#define RADIUS_MAX 0.05f

#define PI 3.1415

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

std::vector<GeometryData::GenericObject*> RenderEngineUE_Generation::GenerateSpheres(unsigned int nNum)
{
  std::vector<GeometryData::GenericObject *> vpSpheres;
  vpSpheres.resize(nNum);

  for (unsigned int nSphere=0; nSphere < nNum; nSphere++)
    {
      GeometryData::GenericObject *pNewObject = new GeometryData::GenericObject(1);
      GeometryData::GenericMesh *pNewMesh = pNewObject->GetMeshPtr(0);

      std::vector<float> vfVertices;
      std::vector<unsigned int>vnIndices;

        int i, j;
         int t=0;	//for index array
         const int lats=24;
         const int longs=24;

         for(i = 1; i <= lats; i++)
         {
                 double lat_n1 = PI * (-0.5 + (double) (i - 2) / lats);
                 double z_n1  = sin(lat_n1);
                 double zr_n1 =  cos(lat_n1);

                 double lat0 = PI * (-0.5 + (double) (i - 1) / lats);
                 double z0  = sin(lat0);
                 double zr0 =  cos(lat0);

                 double lat1 = PI * (-0.5 + (double) i / lats);
                 double z1 = sin(lat1);
                 double zr1 = cos(lat1);

                 double lat2 = PI * (-0.5 + (double) (i+1) / lats);
                 double z2 = sin(lat2);
                 double zr2 = cos(lat2);

                 for(j = 0; j <= longs; j++)
                 {
                         double lng = 2 * PI * (double) (j - 1) / longs;
                         double x = cos(lng);
                         double y = sin(lng);

                         vfVertices.push_back(x * zr0);	    //x
                         vfVertices.push_back(y * zr0);	    //y
                         vfVertices.push_back(-z0);	    //z

                         //calculate tangent
                       /*  glm::vec3 tangent;
                         tangent = glm::normalize(glm::vec3((x*zr1 - x*zr_n1), y*zr1 - y*zr_n1, (-z1) - (-z_n1)));

                         puffer.push_back(tangent.x);
                         puffer.push_back(tangent.y);
                         puffer.push_back(tangent.z);

                         //now add texture coordinates

                         puffer.push_back((float)j / longs);	    //z
                         puffer.push_back((float)(i-1) / lats);	    //u*/
                         vnIndices.push_back(t++);	    //add index

                         /* next longitude */
                         vfVertices.push_back(x * zr1);
                         vfVertices.push_back(y * zr1);
                         vfVertices.push_back(-z1);

                         //calculate tangent
                       /*  tangent = glm::normalize(glm::vec3(x*zr2 - x*zr0, y*zr2 - y*zr0, (-z2) - (-z0)));

                         puffer.push_back(tangent.x);
                         puffer.push_back(tangent.y);
                         puffer.push_back(tangent.z);

                         puffer.push_back((float)(j) / longs);	    //u
                         puffer.push_back((float)(i) / lats);	    //z*/
                         vnIndices.push_back(t++);
                 }
         }

         pNewMesh->AddAttributeValues(GeometryData::GenericData::DATA_VERTICES, vfVertices.size(), &(vfVertices[0]));
         pNewMesh->AddIndices(vnIndices.size(), &(vnIndices[0]));

         glm::vec3 fRandomPosition = glm::vec3(randf(POSITION_MIN_X, POSITION_MAX_X),
                                               randf(POSITION_MIN_Y, POSITION_MAX_Y),
                                               randf(POSITION_MIN_Z, POSITION_MAX_Z));

         float fRadius = randf(RADIUS_MIN, RADIUS_MAX);

         glm::mat4 mModelMatrix = glm::translate(glm::mat4(), fRandomPosition);
         mModelMatrix = glm::scale(mModelMatrix, glm::vec3(fRadius, fRadius, fRadius));

         pNewMesh->SetModelMatrix(&mModelMatrix[0][0]);

         vpSpheres[nSphere] = pNewObject;
    }

}
