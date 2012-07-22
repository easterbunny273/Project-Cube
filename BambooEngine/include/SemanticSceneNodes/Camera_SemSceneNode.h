#ifndef __CAMERA_SEMSCENENODE_BAMBOOENGINE
#define __CAMERA_SEMSCENENODE_BAMBOOENGINE

#include "ISemanticSceneNode.h"     // is a ISemanticSceneNode
#include "Graphic.h"

#include "BambooLib/include/IIdentifyable.h"

#include <memory>                   // uses shared_ptr


class Camera_SemSceneNode : public ISemanticSceneNode, virtual public BambooLib::IIdentifyable
{
public:
  /*! \name Creation */
  //@{
    /// static constructor
    static std::shared_ptr<Camera_SemSceneNode> Create(std::shared_ptr<Bamboo::ICamera> spCamera);

    /// destructor
    ~Camera_SemSceneNode();
  //@}

  /*! \name IIdentifyable interface */
  //@{
    static BambooLib::t_classID ClassID() { return s_ClassID; }
    static Camera_SemSceneNode * Cast(BambooLib::IIdentifyable *pObject) { return dynamic_cast<Camera_SemSceneNode *>(pObject); }
  //@}

  /*! \name Public methods */
  //@{
    /*
    /// sets the perspective projection parameters
    void  SetPerspectiveProjection(float fFOV, float fRatio, float fNearplane, float fFarplane);

    /// returns the perspective projection parameters
    void  GetPerspectiveProjection(float &rfFOV, float &rfRatio, float &rfNearplane, float &rfFarplane);
    */
    std::shared_ptr<Bamboo::ICamera> GetCamera() { return m_spCamera; }
  //@}

private:
    /*! \name Constructor / Destructor */
    //@{
        /// constructor
        Camera_SemSceneNode();
    //@}

    /*! \name Private members */
    //@{
       /* float         m_fFOV,
                      m_fRatio,
                      m_fNearplane,
                      m_fFarplane;*/
        std::shared_ptr<Bamboo::ICamera> m_spCamera;

        static BambooLib::t_classID s_ClassID;
    //@}
};

#endif
