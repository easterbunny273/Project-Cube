/*
 * header file for LightObject class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * written by: florian spechtenhauser <f.spechtenhauser@gmail.com>
 * 07/2012, Project Cube
 */

#ifndef __LIGHTOBJECT_PROJECTCUBE_HEADER
#define __LIGHTOBJECT_PROJECTCUBE_HEADER

#include "Gamelogic/IObject.h"
#include "SemanticSceneNodes/Light_SemSceneNode.h"

class LightObject: public IObject
{
public:
	/*! \name Construction / Destruction */

	/// Constructor
	LightObject(std::string sName);

	/// Destructor
	virtual ~LightObject();

	/*! \name Derived methods from IObject */
    //@{
	
	/// returns the object type as a string. used to define the class of
    /// objects and to determinate which model should be rendered
	virtual std::string GetObjectType();

	/// returns the current transformation matrix of the object
    virtual glm::mat4 GetTransformation();

	/// returns the semantic scene node of this object
	virtual std::shared_ptr<ISemanticSceneNode> GetSceneNode();

	/// returns the user defined name of the object
	virtual std::string GetName();

	//@}

	/*! \name Public methods */
    //@{
	
	/// Sets the light's position
	void SetPosition(const float fX, const float fY, const float fZ);

	/// Sets the light's look direction
	void SetLookDirection(const float fX, const float fY, const float fZ);

	/// Sets the light's FOV
	void SetFOV(const float fFOV);

	/// Sets the light's color
	void SetColor(const float fR, const float fG, const float fB);

	/// Sets the light's nearplane
	void SetNearplane(const float fNearplane);

	/// Sets the light's farplane
	void SetFarplane(const float fFarplane);
	//@}
private:
	std::string m_sName;

	std::shared_ptr<Light_SemSceneNode> m_spLightSceneNode;
};


#endif //__LIGHTOBJECT_PROJECTCUBE_HEADER
