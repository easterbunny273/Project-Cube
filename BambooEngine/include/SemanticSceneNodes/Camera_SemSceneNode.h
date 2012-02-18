#ifndef __CAMERA_SEMSCENENODE_BAMBOOENGINE
#define __CAMERA_SEMSCENENODE_BAMBOOENGINE

#include "ISemanticSceneNode.h"     // is a ISemanticSceneNode

#include <memory>                   // uses shared_ptr

class Camera_SemSceneNode : public ISemanticSceneNode
{
public:
  /*! \name Creation */
  //@{
    /// static constructor
    static std::shared_ptr<Camera_SemSceneNode> Create(float fFOV, float fRatio, float fNearplane, float fFarplane);

    /// destructor
    ~Camera_SemSceneNode();
  //@}

  /*! \name Class identification */
  //@{
    static t_classID ClassID();
  //@}

  /*! \name Public methods */
  //@{
    /// sets the perspective projection parameters
    void  SetPerspectiveProjection(float fFOV, float fRatio, float fNearplane, float fFarplane);

    /// returns the perspective projection parameters
    void  GetPerspectiveProjection(float &rfFOV, float &rfRatio, float &rfNearplane, float &rfFarplane);
  //@}

private:
    /*! \name Constructor / Destructor */
    //@{
        /// constructor
        Camera_SemSceneNode();
    //@}

    /*! \name Private members */
    //@{
        float         m_fFOV,
                      m_fRatio,
                      m_fNearplane,
                      m_fFarplane;
    //@}
};

#endif
