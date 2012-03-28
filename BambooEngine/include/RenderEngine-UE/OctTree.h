#ifndef BAMBOO_RENDERENGINE_UE_OCTTREE
#define BAMBOO_RENDERENGINE_UE_OCTTREE

#include "GeometryData.h"

#include "Graphic.h"
#include "Camera.h"

class RenderEngineUE_OctTree
{
private:
  /*! \name Types */
  //@{
  // struct for bounding box
  struct TBoundingBox
  {
    float fMinX, fMaxX;
    float fMinY, fMaxY;
    float fMinZ, fMaxZ;

    TBoundingBox(float _fMinX, float _fMaxX, float _fMinY, float _fMaxY, float _fMinZ, float _fMaxZ) :
      fMinX(_fMinX), fMaxX(_fMaxX), fMinY(_fMinY), fMaxY(_fMaxY), fMinZ(_fMinZ), fMaxZ(_fMaxZ) {}

    TBoundingBox() {}
  };

  // a node of the octtree
  class TNode
  {
  private:
    // private members
    unsigned int m_nLevel;
    TBoundingBox m_tBoundingBox;
    TNode * m_pChildNodes[8];
    std::vector<GeometryData::GenericObject *> m_vpSpheres;

    // private helper methods
    bool ItlTestMatching(TBoundingBox tBoxNode, TBoundingBox tSphere, bool bMustBeFullIncluded);
    TBoundingBox ItlCalcBoundingBox(GeometryData::GenericObject *pObject);
  public:
    TNode(TBoundingBox tBoundingBox, std::vector<GeometryData::GenericObject*> vpInputSpheres, unsigned int nLevel);

    std::vector<GeometryData::GenericObject *> GetVisibleNodes(Bamboo::ICamera *pCamera);
  };
  //@}

  /*! \name Private members */
  //@{
  TNode *m_pRoot; ///< root node
  //@}

public:
  /*! \name Construction / Destruction */
  //@{
    RenderEngineUE_OctTree();
  //@}

  /*! \name Public methods */
  //@{
    /// returns a vector of visible spheres
    std::vector<GeometryData::GenericObject *> GetVisibleSpheres(std::vector<GeometryData::GenericObject *> vpInputSpheres,
                                                                Bamboo::ICamera *pCamera);
  //@}
};

#endif
