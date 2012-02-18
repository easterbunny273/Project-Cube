#ifndef __LOADEDMODEL_SEMSCENENODE_BAMBOOENGINE
#define __LOADEDMODEL_SEMSCENENODE_BAMBOOENGINE

#include "ISemanticSceneNode.h"     // is a ISemanticSceneNode

#include <memory>                   // uses shared_ptr
#include <string>                   // uses string

class LoadedModel_SemSceneNode : public ISemanticSceneNode
{
public:
  /*! \name Creation */
  //@{
    /// static constructor
    static std::shared_ptr<LoadedModel_SemSceneNode> Create(std::string sFilename);

    /// destructor
    ~LoadedModel_SemSceneNode();
  //@}

  /*! \name Class identification */
  //@{
    static t_classID ClassID();
  //@}

  /*! \name Public methods */
  //@{
    /// returns the filename of the loaded model
    std::string GetFilename();
  //@}

private:
    /*! \name Constructor / Destructor */
    //@{
        /// constructor
        LoadedModel_SemSceneNode();
    //@}

    /*! \name Private members */
    //@{
        std::string m_sFilename;
    //@}
};

#endif
