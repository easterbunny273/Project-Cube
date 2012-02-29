#ifndef __LIGHT_SEMSCENENODE_BAMBOOENGINE
#define __LIGHT_SEMSCENENODE_BAMBOOENGINE

#include "ISemanticSceneNode.h"     // is a ISemanticSceneNode

#include <memory>                   // uses shared_ptr

class Light_SemSceneNode : public ISemanticSceneNode
{
public:
  /*! \name Creation */
  //@{
    /// static constructor
    static std::shared_ptr<Light_SemSceneNode> Create(glm::vec3 vPosition,
                                                      glm::vec3 vLookDirection,
                                                      float fFOV,
                                                      glm::vec3 vLightColor,
                                                      float fNearplane,
                                                      float fFarplane);

    /// destructor
    ~Light_SemSceneNode();
  //@}

  /*! \name Class identification */
  //@{
    static t_classID ClassID();
  //@}

  /*! \name Public methods */
  //@{
    /// sets the light parameters
    void  SetLightParameters(glm::vec3 vPosition,
                             glm::vec3 vLookDirection,
                             float fFOV,
                             glm::vec3 vLightColor,
                             float fNearplane,
                             float fFarplane);


    /// returns the perspective projection parameters
    void  GetLightParameters(glm::vec3 &rvPosition,
                             glm::vec3 &rvLookDirection,
                             float &rfFOV,
                             glm::vec3 &rvLightColor,
                             float &rfNearplane,
                             float &rfFarplane);
  //@}

private:
    /*! \name Constructor / Destructor */
    //@{
        /// constructor
        Light_SemSceneNode();
    //@}

    /*! \name Private members */
    //@{
        glm::vec3     m_vLightColor;
        glm::vec3     m_vLightPosition;
        glm::vec3     m_vLightLookDirection;

        float         m_fFOV,
                      m_fNearplane,
                      m_fFarplane;

    //@}
};

#endif
