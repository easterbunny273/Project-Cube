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
    static std::shared_ptr<Light_SemSceneNode> Create(float fFOV,
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
    void  SetLightParameters(float fFOV,
                             glm::vec3 vLightColor,
                             float fNearplane,
                             float fFarplane);

    /// sets the transformation matrix (position + rotation) by "look-at" parameters,
    /// which define the position of the kamera and the look direction
    void SetTransformationMatrixByLookAtParameters(glm::vec3 vPosition,
                                                   glm::vec3 vLookDirection,
                                                   glm::vec3 vUp);

    /// returns the perspective projection parameters
    void  GetLightParameters(float &rfFOV,
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

        float         m_fFOV,
                      m_fNearplane,
                      m_fFarplane;
    //@}
};

#endif
