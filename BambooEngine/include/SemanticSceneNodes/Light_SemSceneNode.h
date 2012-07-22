#ifndef __LIGHT_SEMSCENENODE_BAMBOOENGINE
#define __LIGHT_SEMSCENENODE_BAMBOOENGINE

#include "ISemanticSceneNode.h"     // is a ISemanticSceneNode

#include <memory>                   // uses shared_ptr

#include "BambooLib/include/IIdentifyable.h"

class Light_SemSceneNode : public ISemanticSceneNode, virtual public BambooLib::IIdentifyable
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


    /*! \name IIdentifyable interface */
    //@{
      static BambooLib::t_classID ClassID() { return s_ClassID; }
      static Light_SemSceneNode * Cast(BambooLib::IIdentifyable *pObject) { return dynamic_cast<Light_SemSceneNode *>(pObject); }
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

	/// Sets the light's position
	void SetPosition(glm::vec3 vPosition);

	/// Sets the light's look direction
	void SetLookDirection(glm::vec3 vLookDirection);

	/// Sets the light's FOV
	void SetFOV(float fFOV);

	/// Sets the light's color
	void SetColor(glm::vec3 vColor);

	/// Sets the light's nearplane
	void SetNearplane(float fNearplane);

	/// Sets the light's farplane
	void SetFarplane(float fFarplane);

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

        static BambooLib::t_classID s_ClassID;

    //@}
};

#endif
