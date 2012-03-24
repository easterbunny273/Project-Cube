#ifndef __GENERICOBJECT_SEMSCENENODE_BAMBOOENGINE
#define __GENERICOBJECT_SEMSCENENODE_BAMBOOENGINE

#include "ISemanticSceneNode.h"     // is a ISemanticSceneNode

#include <memory>                   // uses shared_ptr
#include <string>                   // uses string

#include "GeometryData.h"

class GenericObject_SemSceneNode : public ISemanticSceneNode
{
public:
  /*! \name Creation */
  //@{
    /// static constructor
    static std::shared_ptr<GenericObject_SemSceneNode> Create(GeometryData::GenericObject *pObject);

    /// destructor
    ~GenericObject_SemSceneNode();
  //@}

  /*! \name Class identification */
  //@{
    static t_classID ClassID();
  //@}

  /*! \name Public methods */
  //@{
    /// returns the filename of the loaded model
    GeometryData::GenericObject *GetObject();

    std::shared_ptr<GeometryData::GenericObject> GetObjectSP();
  //@}

private:
    /*! \name Constructor / Destructor */
    //@{
        /// constructor
        GenericObject_SemSceneNode();
    //@}

    /*! \name Private members */
    //@{
        std::shared_ptr<GeometryData::GenericObject> m_spObject;
    //@}
};

#endif
