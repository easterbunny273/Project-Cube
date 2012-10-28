#ifndef __LOADEDMODEL_SEMSCENENODE_BAMBOOENGINE
#define __LOADEDMODEL_SEMSCENENODE_BAMBOOENGINE

#include "ISemanticSceneNode.h"     // is a ISemanticSceneNode

#include <memory>                   // uses shared_ptr
#include <string>                   // uses string

#include "BambooLib/include/IIdentifyable.h"

class LoadedModel_SemSceneNode : public ISemanticSceneNode, virtual public BambooLib::IIdentifyable
{
public:
  /*! \name Creation */
  //@{
    /// static constructor
    static LoadedModel_SemSceneNode *Create(std::string sFilename);

    /// destructor
    ~LoadedModel_SemSceneNode();
  //@}


    /*! \name IIdentifyable interface */
    //@{
      static BambooLib::t_classID ClassID() { return s_ClassID; }
      static LoadedModel_SemSceneNode * Cast(BambooLib::IIdentifyable *pObject) { return dynamic_cast<LoadedModel_SemSceneNode *>(pObject); }
    //@}

  /*! \name Public methods */
  //@{
    /// returns the filename of the loaded model
    std::string GetFilename();

    bool GetEnvironmentMapping() { return m_bEnvironmentMapping; }

    /// adds environment mapping effect
    void ActivateEnvironmentMapping();

    /// removes environment mapping effect
    void DeactivateEnvironmentMapping();
  //@}

private:
    /*! \name Constructor / Destructor */
    //@{
        /// constructor
        LoadedModel_SemSceneNode();
    //@}

    /*! \name Private members */
    //@{
        std::string   m_sFilename;
        bool          m_bEnvironmentMapping;

        static BambooLib::t_classID s_ClassID;
    //@}
};

#endif
