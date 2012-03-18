#include "DeferredNodeTranslator/FractalTerrain_RuleObject.h"
#include "SemanticSceneNodes/FractalTerrain_SemSceneNode.h"
#include "RenderNodes/RenderNode_Generic.h"
#include "RenderNodes/RenderNode_Camera.h"
#include "RenderNodes/RenderNode_Deferred.h"
#include "Camera.h"
#include <IL/il.h>
#include <assert.h>
#include "PC_Logger.h"
#include <fstream>

std::vector<ISemanticSceneNode::t_classID> DeferredNodeTranslator::FractalTerrain_RuleObject::GetAcceptedNodeIDs() const
{
  std::vector<ISemanticSceneNode::t_classID> vAcceptedIDs;

  // this rule object only accepts LoadedModel_SemSceneNode
  vAcceptedIDs.push_back(FractalTerrain_SemSceneNode::ClassID());

  return vAcceptedIDs;
}

std::shared_ptr<GeometryData::GenericObject> DeferredNodeTranslator::FractalTerrain_RuleObject::ItlGenerateMeshFromFile(std::string sFilename)
{
  ILuint iImage = ilGenImage();

  ilBindImage(iImage);

  bool bOk = ilLoadImage(sFilename.c_str());

  if (!bOk)
  {

  ILenum Error;
  Error = ilGetError();
  ilDeleteImages(1,&iImage);		// bei Fehlschlag wieder Index freigeben

  Logger::error() << "Could not find texture file " << sFilename << ", DevIL error: " << Error << Logger::endl;
  return std::shared_ptr<GeometryData::GenericObject>(new GeometryData::GenericObject(0));
  }
  else
    {
      std::ifstream bin_input("/tmp/terrain.bin", std::ios::binary);

      unsigned int nSize;

      bin_input.read((char *) &nSize, sizeof(unsigned int));

      std::cout << nSize << std::endl;

      float *piHeight = new float[nSize * nSize];

      int iWidth = nSize;
      int iHeight = nSize;


      bin_input.read((char *) piHeight, sizeof(float) * nSize * nSize);

      bin_input.close();

      std::vector<float> vfVertices;
      std::vector<float> vfNormals;
      std::vector<float> vfTexcoords;

      //calculate vertices and normals, and texcoords
      for (unsigned int iX = 0; iX < iWidth; iX++)
        for (unsigned int iY = 0; iY < iHeight; iY++)
        {
            vfVertices.push_back((iX - iWidth*0.5) / (double)iWidth);
            //vfVertices.push_back(0.0f);
            vfVertices.push_back(0.1 * piHeight[iX + iY*iWidth] / 255.0);
            vfVertices.push_back((iY - iHeight*0.5) / (double)iHeight);

            if (iX < iWidth - 1)
              {
                if (iX > 0)
                  vfNormals.push_back(piHeight[(iX-1)+iY*iWidth] - piHeight[(iX+1)+iY*iWidth]);
                else
                  vfNormals.push_back(piHeight[iX+iY*iWidth]*0.5 - piHeight[(iX+1)+iY*iWidth]*0.5);
              }
            else
              {
                assert (iX > 0);
                vfNormals.push_back(piHeight[(iX-1)+iY*iWidth]*0.5 - piHeight[iX+iY*iWidth]*0.5);
              }


            vfNormals.push_back(1.0);

            if (iY < iHeight-1)
              {
                if (iY > 0)
                  vfNormals.push_back(piHeight[iX+(iY-1)*iWidth] - piHeight[iX+(iY+1)*iWidth]);
                else
                  vfNormals.push_back(piHeight[iX+iY*iWidth]*0.5 - piHeight[iX+(iY+1)*iWidth]*0.5);
              }
            else
              {
                assert(iY > 0);
                vfNormals.push_back(piHeight[iX+(iY-1)*iWidth] * 0.5 - piHeight[iX+(iY)*iWidth] * 0.5);
              }

            vfTexcoords.push_back(iX / (float)iWidth);
            vfTexcoords.push_back(iY / (float)iHeight);

        }

      std::vector<unsigned int> viIndices;

      for (int iX=0; iX < iWidth-1; iX++)
        for (int iY=0; iY < iHeight-1; iY++)
        {
          viIndices.push_back(iX + iY*iWidth);
          viIndices.push_back(iX+1 + iY*iWidth);
          viIndices.push_back(iX + (iY+1)*iWidth);

          viIndices.push_back(iX + (iY+1)*iWidth);
          viIndices.push_back(iX+1 + iY*iWidth);
          viIndices.push_back((iX+1) + (iY+1)*iWidth);
        }

      GeometryData::GenericObject *pNewObject = new GeometryData::GenericObject(1);

      std::shared_ptr<GeometryData::GenericMesh> spNewMesh(pNewObject->GetMesh(0).lock());

      spNewMesh->AddAttributeValues(GeometryData::GenericData::DATA_VERTICES,
                                   vfVertices.size(),
                                   &vfVertices[0]);

      spNewMesh->AddAttributeValues(GeometryData::GenericData::DATA_NORMALS,
                                   vfNormals.size(),
                                   &vfNormals[0]);
      spNewMesh->AddIndices(viIndices.size(), &viIndices[0]);

      spNewMesh->SetTexturePath(GeometryData::TextureNames::ALBEDO, sFilename.c_str());
      spNewMesh->SetTexturePath(GeometryData::TextureNames::NORMAL, sFilename.c_str());

      spNewMesh->SetTextureCoords(GeometryData::TextureNames::ALBEDO, vfTexcoords.size(), &vfTexcoords[0]);
      spNewMesh->SetTextureCoords(GeometryData::TextureNames::NORMAL, vfTexcoords.size(), &vfTexcoords[0]);


      return std::shared_ptr<GeometryData::GenericObject>(pNewObject);
    }

}

void DeferredNodeTranslator::FractalTerrain_RuleObject::Action()
{
  assert (m_pTranslator->m_spRootNode);
  assert (m_pTranslator->m_spDeferredNode);

  if (m_spCorrespondingRenderingNode)
    {
        static int iMagic = 0;
        int iNewMagic;

        std::ifstream bin_input("/tmp/terrain.update");
        bin_input >> iNewMagic;
        bin_input.close();

        if (iMagic != 0 && iMagic != iNewMagic)
          {
            m_spCorrespondingRenderingNode->UpdateGenericObject(ItlGenerateMeshFromFile(m_spSemNode->GetFilename()));
           /* m_pTranslator->m_vShadowCasterNodes.clear();
            m_pTranslator->m_spDeferredNode->RemoveChild(m_spCorrespondingRenderingNode);

            // create new node
            m_spCorrespondingRenderingNode = std::shared_ptr<Bamboo::RN_Generic>(new Bamboo::RN_Generic(ItlGenerateMeshFromFile(m_spSemNode->GetFilename())));
            assert (m_spCorrespondingRenderingNode);

            m_pTranslator->m_vShadowCasterNodes.push_back(m_spCorrespondingRenderingNode);

            m_spCorrespondingRenderingNode->SetGraphicCore(m_pTranslator->m_pCore);*/
          }

        iMagic = iNewMagic;

      //todo
    }
  else
    {
      // create new node
      m_spCorrespondingRenderingNode = std::shared_ptr<Bamboo::RN_Generic>(new Bamboo::RN_Generic(ItlGenerateMeshFromFile(m_spSemNode->GetFilename())));
      assert (m_spCorrespondingRenderingNode);

      m_pTranslator->m_vShadowCasterNodes.push_back(m_spCorrespondingRenderingNode);

      m_spCorrespondingRenderingNode->SetGraphicCore(m_pTranslator->m_pCore);

      // attach to deferred node
      m_pTranslator->m_spDeferredNode->AddChild(m_spCorrespondingRenderingNode);
      // create new node

      //todo
    }

}

DeferredNodeTranslator::IRuleObject *DeferredNodeTranslator::FractalTerrain_RuleObject::CloneFor(std::shared_ptr<ISemanticSceneNode> spSemNode,
                                                                                                 DeferredNodeTranslator *pTranslator)
{
  FractalTerrain_RuleObject *pNewObject = new FractalTerrain_RuleObject();

  pNewObject->m_pTranslator = pTranslator;

  pNewObject->m_spSemNode = std::dynamic_pointer_cast<FractalTerrain_SemSceneNode>(spSemNode);
  assert (pNewObject->m_spSemNode);


  return pNewObject;
}

