#include "DeferredNodeTranslator/FractalTerrain_RuleObject.h"
#include "SemanticSceneNodes/FractalTerrain_SemSceneNode.h"
#include "RenderNodes/RenderNode_Generic.h"
#include "RenderNodes/RenderNode_Camera.h"
#include "RenderNodes/RenderNode_Deferred.h"
#include "Camera.h"
#include <IL/il.h>
#include <assert.h>
#include "PC_Logger.h"

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
      long int iHeight, iWidth, iFormat, iBPP;
      unsigned char *szData=0;

      iWidth=ilGetInteger(IL_IMAGE_WIDTH);		    // Breite des Bildes holen
      iHeight=ilGetInteger(IL_IMAGE_HEIGHT);	    // Höhe des Bildes holen
      iBPP=ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL); // Farbtiefe des Bildes
      iFormat=ilGetInteger(IL_IMAGE_FORMAT);	    // Format des Bildes z.B. RGB RGBA BGR BGRA usw.

      szData = ilGetData();

      int * piHeight = new int[iWidth * iHeight];

      for (unsigned int iX = 0; iX < iWidth; iX++)
        for (unsigned int iY = 0; iY < iHeight; iY++)
          {
            piHeight[iX + iY*iWidth] = (int) *(szData + (iY*iWidth + iX) * iBPP);
          }

      std::vector<float> vfVertices;

      //get vertices
      for (unsigned int iX = 0; iX < iWidth; iX++)
        for (unsigned int iY = 0; iY < iHeight; iY++)
        {
            vfVertices.push_back((iX - iWidth*0.5) / iWidth);
            vfVertices.push_back(piHeight[iX + iY*iWidth]);
            vfVertices.push_back((iY - iHeight*0.5) / iHeight);
        }

      GeometryData::GenericObject *pNewObject = new GeometryData::GenericObject(1);

      std::shared_ptr<GeometryData::GenericMesh> spNewMesh(pNewObject->GetMesh(0).lock());
      //GeometryData::GenericMesh *pNewMesh;

      spNewMesh->AddAttributeValues(GeometryData::GenericData::DATA_VERTICES,
                                   vfVertices.size(),
                                   &vfVertices[0]);

      return std::shared_ptr<GeometryData::GenericObject>(pNewObject);
    }

}

void DeferredNodeTranslator::FractalTerrain_RuleObject::Action()
{
  assert (m_pTranslator->m_spRootNode);
  assert (m_pTranslator->m_spDeferredNode);

  if (m_spCorrespondingRenderingNode)
    {
     // only update values

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

