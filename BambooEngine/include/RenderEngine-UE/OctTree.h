#ifndef BAMBOO_RENDERENGINE_UE_OCTTREE
#define BAMBOO_RENDERENGINE_UE_OCTTREE

#include "GeometryData.h"

#include "Graphic.h"
#include "Camera.h"

class RenderEngineUE_OctTree
{
private:
  struct TBoundingBox
  {
    float fMinX, fMaxX;
    float fMinY, fMaxY;
    float fMinZ, fMaxZ;

    TBoundingBox(float _fMinX, float _fMaxX, float _fMinY, float _fMaxY, float _fMinZ, float _fMaxZ) : fMinX(_fMinX), fMaxX(_fMaxX), fMinY(_fMinY), fMaxY(_fMaxY), fMinZ(_fMinZ), fMaxZ(_fMaxZ) {}
    TBoundingBox() {}
  };

  class TNode
  {
  private:
    unsigned int m_nLevel;
    TNode * m_pChildNodes[8];
    std::vector<GeometryData::GenericObject *> m_vpSpheres;

   // void Split();
    //void Collapse();

    TBoundingBox m_tBoundingBox;

    bool ItlTestMatching(TBoundingBox tBoxNode, TBoundingBox tSphere, bool bMustBeFullIncluded);
    TBoundingBox ItlCalcBoundingBox(GeometryData::GenericObject *pObject);
  public:
    TNode(TBoundingBox tBoundingBox, std::vector<GeometryData::GenericObject*> vpInputSpheres, unsigned int nLevel);

    std::vector<GeometryData::GenericObject *> GetVisibleNodes(Bamboo::ICamera *pCamera);
    //void Update();
  };





  TNode *m_pRoot;

public:
  RenderEngineUE_OctTree();

  std::vector<GeometryData::GenericObject *> GetVisibleSpheres(std::vector<GeometryData::GenericObject *> vpInputSpheres,
                                                  Bamboo::ICamera *pCamera);
};

#endif
