#include "RenderEngine-UE/OctTree.h"

#define MAX_SPHERES_PER_NODE 10
#define MAX_OCTTREE_LEVELS 20

#include <iostream>

RenderEngineUE_OctTree::TBoundingBox RenderEngineUE_OctTree::TNode::ItlCalcBoundingBox(GeometryData::GenericObject *pObject)
{
  TBoundingBox tBoundingBox;

  for (unsigned int nMesh = 0; nMesh < pObject->NumMeshes(); nMesh++)
    {
      // get mesh
      GeometryData::GenericMesh *pMesh = pObject->GetMeshPtr(0);

      // get model matrix
      glm::mat4 *pmModelMatrix = (glm::mat4 *) pMesh->GetModelMatrix();

      // get vertices data
      unsigned int nNumEntries;
      float *pfVertices = pMesh->GetAttribute(GeometryData::GenericData::DATA_VERTICES, nNumEntries);

      // must be x*3
      assert (nNumEntries % 3 == 0);
      assert (nNumEntries >= 3);

      if (nMesh==0)
      {
        glm::vec4 v(pfVertices[0], pfVertices[1], pfVertices[2], 1.0);

        glm::vec4 vTransformed = (*pmModelMatrix) * v;

        tBoundingBox.fMinX = tBoundingBox.fMaxX = vTransformed.x;
        tBoundingBox.fMinY = tBoundingBox.fMaxY = vTransformed.y;
        tBoundingBox.fMinZ = tBoundingBox.fMaxZ = vTransformed.z;
      }

      for (unsigned int i=0; i < nNumEntries; i+=3)
      {
          glm::vec4 v(pfVertices[i], pfVertices[i+1], pfVertices[i+2], 1.0);

          glm::vec4 vTransformed = (*pmModelMatrix) * v;

          if (vTransformed.x < tBoundingBox.fMinX)
            tBoundingBox.fMinX = vTransformed.x;

          if (vTransformed.x > tBoundingBox.fMaxX)
            tBoundingBox.fMaxX = vTransformed.x;

          if (vTransformed.y < tBoundingBox.fMinY)
            tBoundingBox.fMinY = vTransformed.y;

          if (vTransformed.y > tBoundingBox.fMaxY)
            tBoundingBox.fMaxY = vTransformed.y;

          if (vTransformed.z < tBoundingBox.fMinZ)
            tBoundingBox.fMinZ = vTransformed.z;

          if (vTransformed.z > tBoundingBox.fMaxZ)
            tBoundingBox.fMaxZ = vTransformed.z;
      }
    }

  return tBoundingBox;
}

bool RenderEngineUE_OctTree::TNode::ItlTestMatching(RenderEngineUE_OctTree::TBoundingBox tBoxNode,
                                                   RenderEngineUE_OctTree::TBoundingBox tSphere,
                                                   bool bMustBeFullIncluded)
{
  bool bOutSide;

  if (bMustBeFullIncluded == false)
    {
      bool bXDiffers = (tSphere.fMinX > tBoxNode.fMaxX || tSphere.fMaxX < tBoxNode.fMinX);
      bool bYDiffers = (tSphere.fMinY > tBoxNode.fMaxY || tSphere.fMaxY < tBoxNode.fMinY);
      bool bZDiffers = (tSphere.fMinZ > tBoxNode.fMaxZ || tSphere.fMaxZ < tBoxNode.fMinZ);

      bOutSide = (bXDiffers || bYDiffers || bZDiffers);
    }
  else
    {
      assert(!"not implemented yet");
    }

  return !bOutSide;
}

RenderEngineUE_OctTree::RenderEngineUE_OctTree()
{
  m_pRoot = NULL;
}

std::vector<GeometryData::GenericObject*> RenderEngineUE_OctTree::GetVisibleSpheres(std::vector<GeometryData::GenericObject*> vpInputSpheres,
                                                                         Bamboo::ICamera *pCamera)
{
  if (m_pRoot == NULL)
  {
      std::cout << "create octtree" << std::endl;
      TBoundingBox tRoot(-1.1f, 1.1f, -1.1f, 1.1f, -1.1f, 1.1f);
      m_pRoot = new TNode(tRoot, vpInputSpheres, 1);

      std::cout << "octtree created" << std::endl;
      //m_pRoot->Update();
  }

  return m_pRoot->GetVisibleNodes(pCamera);
}

RenderEngineUE_OctTree::TNode::TNode(TBoundingBox tBoundingBox,
                                    std::vector<GeometryData::GenericObject*> vpInputSpheres,
                                     unsigned int nLevel)
{
  m_tBoundingBox = tBoundingBox;
  m_nLevel = nLevel;

  if (vpInputSpheres.size() > MAX_SPHERES_PER_NODE && nLevel < MAX_OCTTREE_LEVELS)
    {
      TBoundingBox tLeftUpperFront = TBoundingBox(tBoundingBox.fMinX,
                                               (tBoundingBox.fMaxX - tBoundingBox.fMinX) * 0.5f + tBoundingBox.fMinX,
                                               (tBoundingBox.fMaxY - tBoundingBox.fMinY) * 0.5f + tBoundingBox.fMinY,
                                               tBoundingBox.fMaxY,
                                               tBoundingBox.fMinZ,
                                               (tBoundingBox.fMaxZ - tBoundingBox.fMinZ) * 0.5f + tBoundingBox.fMinZ);

      TBoundingBox tRightUpperFront = TBoundingBox((tBoundingBox.fMaxX - tBoundingBox.fMinX) * 0.5f + tBoundingBox.fMinX,
                                                 tBoundingBox.fMaxX,
                                                 (tBoundingBox.fMaxY - tBoundingBox.fMinY) * 0.5f + tBoundingBox.fMinY,
                                                 tBoundingBox.fMaxY,
                                                 tBoundingBox.fMinZ,
                                                 (tBoundingBox.fMaxZ - tBoundingBox.fMinZ) * 0.5f + tBoundingBox.fMinZ);

      TBoundingBox tLeftUpperBack = TBoundingBox(tBoundingBox.fMinX,
                                               (tBoundingBox.fMaxX - tBoundingBox.fMinX) * 0.5f + tBoundingBox.fMinX,
                                               (tBoundingBox.fMaxY - tBoundingBox.fMinY) * 0.5f + tBoundingBox.fMinY,
                                               tBoundingBox.fMaxY,
                                               (tBoundingBox.fMaxZ - tBoundingBox.fMinZ) * 0.5f + tBoundingBox.fMinZ,
                                                 tBoundingBox.fMaxZ);

      TBoundingBox tRightUpperBack = TBoundingBox((tBoundingBox.fMaxX - tBoundingBox.fMinX) * 0.5f + tBoundingBox.fMinX,
                                                 tBoundingBox.fMaxX,
                                                 (tBoundingBox.fMaxY - tBoundingBox.fMinY) * 0.5f + tBoundingBox.fMinY,
                                                 tBoundingBox.fMaxY,
                                                  (tBoundingBox.fMaxZ - tBoundingBox.fMinZ) * 0.5f + tBoundingBox.fMinZ,
                                                  tBoundingBox.fMaxZ);

      TBoundingBox tLeftLowerFront = TBoundingBox(tBoundingBox.fMinX,
                                               (tBoundingBox.fMaxX - tBoundingBox.fMinX) * 0.5f + tBoundingBox.fMinX,
                                               tBoundingBox.fMinY,
                                               (tBoundingBox.fMaxY - tBoundingBox.fMinY) * 0.5f + tBoundingBox.fMinY,
                                               tBoundingBox.fMinZ,
                                               (tBoundingBox.fMaxZ - tBoundingBox.fMinZ) * 0.5f + tBoundingBox.fMinZ);

      TBoundingBox tRightLowerFront = TBoundingBox((tBoundingBox.fMaxX - tBoundingBox.fMinX) * 0.5f + tBoundingBox.fMinX,
                                                 tBoundingBox.fMaxX,
                                                   tBoundingBox.fMinY,
                                                   (tBoundingBox.fMaxY - tBoundingBox.fMinY) * 0.5f + tBoundingBox.fMinY,
                                                 tBoundingBox.fMinZ,
                                                 (tBoundingBox.fMaxZ - tBoundingBox.fMinZ) * 0.5f + tBoundingBox.fMinZ);

      TBoundingBox tLeftLowerBack = TBoundingBox(tBoundingBox.fMinX,
                                               (tBoundingBox.fMaxX - tBoundingBox.fMinX) * 0.5f + tBoundingBox.fMinX,
                                                 tBoundingBox.fMinY,
                                                 (tBoundingBox.fMaxY - tBoundingBox.fMinY) * 0.5f + tBoundingBox.fMinY,
                                               (tBoundingBox.fMaxZ - tBoundingBox.fMinZ) * 0.5f + tBoundingBox.fMinZ,
                                                 tBoundingBox.fMaxZ);

      TBoundingBox tRightLowerBack = TBoundingBox((tBoundingBox.fMaxX - tBoundingBox.fMinX) * 0.5f + tBoundingBox.fMinX,
                                                 tBoundingBox.fMaxX,
                                                  tBoundingBox.fMinY,
                                                  (tBoundingBox.fMaxY - tBoundingBox.fMinY) * 0.5f + tBoundingBox.fMinY,
                                                  (tBoundingBox.fMaxZ - tBoundingBox.fMinZ) * 0.5f + tBoundingBox.fMinZ,
                                                  tBoundingBox.fMaxZ);

      TBoundingBox **ppBoxes = new TBoundingBox * [8];

      ppBoxes[0] = &tLeftUpperFront;
      ppBoxes[1] = &tRightUpperFront;
      ppBoxes[2] = &tLeftUpperBack;
      ppBoxes[3] = &tRightUpperBack;
      ppBoxes[4] = &tLeftLowerFront;
      ppBoxes[5] = &tRightLowerFront;
      ppBoxes[6] = &tLeftLowerBack;
      ppBoxes[7] = &tRightLowerBack;

      std::vector<GeometryData::GenericObject *> vObjectsInChildNodes[8];

      for (unsigned int iChildNode = 0; iChildNode < 8; iChildNode++)
        {
          for (unsigned int iObject = 0; iObject < vpInputSpheres.size(); iObject++)
            {
              GeometryData::GenericObject *pSphere = vpInputSpheres[iObject];
              bool bInNode = ItlTestMatching(*(ppBoxes[iChildNode]), ItlCalcBoundingBox(pSphere), false);

              if (bInNode)
                vObjectsInChildNodes[iChildNode].push_back(vpInputSpheres[iObject]);
            }

          m_pChildNodes[iChildNode] = new TNode(*(ppBoxes[iChildNode]), vObjectsInChildNodes[iChildNode], nLevel+1);
        }

      m_vpSpheres = vpInputSpheres;
    }
  else
    {
      m_vpSpheres = vpInputSpheres;

      m_pChildNodes[0] = NULL;
      m_pChildNodes[1] = NULL;
      m_pChildNodes[2] = NULL;
      m_pChildNodes[3] = NULL;
      m_pChildNodes[4] = NULL;
      m_pChildNodes[5] = NULL;
      m_pChildNodes[6] = NULL;
      m_pChildNodes[7] = NULL;
  }
}

std::vector<GeometryData::GenericObject *> RenderEngineUE_OctTree::TNode::GetVisibleNodes(Bamboo::ICamera *pCamera)
{
  std::vector<GeometryData::GenericObject *> vVisibleObjects;

  glm::vec4 vPoints[8];

  // lower left front
  vPoints[0].x = m_tBoundingBox.fMinX;
  vPoints[0].y = m_tBoundingBox.fMinY;
  vPoints[0].z = m_tBoundingBox.fMinZ;
  vPoints[0].w = 1.0;

  // lower right front
  vPoints[1].x = m_tBoundingBox.fMaxX;
  vPoints[1].y = m_tBoundingBox.fMinY;
  vPoints[1].z = m_tBoundingBox.fMinZ;
  vPoints[1].w = 1.0;

  // upper left front
  vPoints[2].x = m_tBoundingBox.fMinX;
  vPoints[2].y = m_tBoundingBox.fMaxY;
  vPoints[2].z = m_tBoundingBox.fMinZ;
  vPoints[2].w = 1.0;

  // upper right front
  vPoints[3].x = m_tBoundingBox.fMaxX;
  vPoints[3].y = m_tBoundingBox.fMaxY;
  vPoints[3].z = m_tBoundingBox.fMinZ;
  vPoints[3].w = 1.0;

  // upper left back
  vPoints[4].x = m_tBoundingBox.fMinX;
  vPoints[4].y = m_tBoundingBox.fMaxY;
  vPoints[4].z = m_tBoundingBox.fMaxZ;
  vPoints[4].w = 1.0;

  // upper right back
  vPoints[5].x = m_tBoundingBox.fMaxX;
  vPoints[5].y = m_tBoundingBox.fMaxY;
  vPoints[5].z = m_tBoundingBox.fMaxZ;
  vPoints[5].w = 1.0;

  // lower left back
  vPoints[6].x = m_tBoundingBox.fMinX;
  vPoints[6].y = m_tBoundingBox.fMinY;
  vPoints[6].z = m_tBoundingBox.fMaxZ;
  vPoints[6].w = 1.0;

  // lower right back
  vPoints[7].x = m_tBoundingBox.fMaxX;
  vPoints[7].y = m_tBoundingBox.fMinY;
  vPoints[7].z = m_tBoundingBox.fMaxZ;
  vPoints[7].w = 1.0;


  glm::vec4 vTransformedPoints[8];
  glm::mat4 mViewProjectionMatrix = pCamera->GetProjectionMatrix() * pCamera->GetViewMatrix();

  for (unsigned int i=0; i < 8; i++)
    {
      vTransformedPoints[i] = mViewProjectionMatrix * vPoints[i];
      vTransformedPoints[i] /= vTransformedPoints[i].w;
    }

  // test if all points are outside the view frame

  bool bOutsideXLeft = true;
  bool bOutsideXRight = true;
  bool bOutsideYUp = true;
  bool bOutsideYDown = true;
  bool bOutsideZBack = true;
  bool bOutsideZFront = true;

  for (unsigned int i=0; i < 8; i++)
    {
      bOutsideXLeft   &= vTransformedPoints[i].x < -1.0;
      bOutsideXRight  &= vTransformedPoints[i].x > 1.0;

      bOutsideYUp   &= vTransformedPoints[i].y < -1.0;
      bOutsideYDown  &= vTransformedPoints[i].y > 1.0;

      bOutsideZFront   &= vTransformedPoints[i].z < -1.0;
      bOutsideZBack  &= vTransformedPoints[i].z > 1.0;
    }

  bool bNotVisible = (bOutsideXLeft || bOutsideXRight || bOutsideYUp || bOutsideYDown || bOutsideZBack || bOutsideZFront);

  if (bNotVisible == false)
    {
      // test if all points are visible

      bool bInsideX = true;
      bool bInsideY = true;
      bool bInsideZ = true;

      for (unsigned int i=0; i < 8; i++)
        {
          bInsideX &= (vTransformedPoints[i].x >= -1.0 && vTransformedPoints[i].x <= 1.0);
          bInsideY &= (vTransformedPoints[i].y >= -1.0 && vTransformedPoints[i].y <= 1.0);
          bInsideZ &= (vTransformedPoints[i].z >= -1.0 && vTransformedPoints[i].z <= 1.0);
        }

      bool bFullVisible = (bInsideX && bInsideY && bInsideZ);

      if (bFullVisible == false && m_pChildNodes[0] != NULL)
      {
          for (unsigned int iNode=0; iNode < 8; iNode++)
            {
              std::vector<GeometryData::GenericObject *> vpVisibleObjectsInSubNode;

              vpVisibleObjectsInSubNode = m_pChildNodes[iNode]->GetVisibleNodes(pCamera);

              for (unsigned int nVisibleObjects=0; nVisibleObjects < vpVisibleObjectsInSubNode.size(); nVisibleObjects++)
                vVisibleObjects.push_back(vpVisibleObjectsInSubNode[nVisibleObjects]);
            }
      }
      else
        vVisibleObjects = m_vpSpheres;
    }

  return vVisibleObjects;
}
