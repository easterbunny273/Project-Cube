/*
 * header file for Object class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * written by: florian spechtenhauser <f.spechtenhauser@gmail.com>
 * 07/2012, Project Cube
 */

#ifndef __OBJECT_PROJECTCUBE_HEADER
#define __OBJECT_PROJECTCUBE_HEADER

#include "Gamelogic/IObject.h"
#include "SemanticSceneNodes/LoadedModel_SemSceneNode.h"
  
class Object: public IObject
{
public:
	/*! \name Construction / Destruction */

	/// Constructor
	Object(std::string sName, std::string sFilename);

	/// Destructor
	virtual ~Object();

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

	/// translates the object
	void Translate(float fX, float fY, float fZ);

	/// scales the object
	void Scale(float fX, float fY, float fZ);

	/// Activates Environment mapping
	void ActivateEnvironmentMapping();

	/// Deactivates Environment mapping
	void DeactivateEnvironmentMapping();


	//@}
private:
	std::shared_ptr<LoadedModel_SemSceneNode> m_spObjectSceneNode;

	std::string m_sName;
};


#endif //__OBJECT_PROJECTCUBE_HEADER
