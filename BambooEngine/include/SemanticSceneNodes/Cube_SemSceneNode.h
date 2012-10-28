#ifndef __CUBE_SEMSCENENODE_BAMBOOENGINE
#define __CUBE_SEMSCENENODE_BAMBOOENGINE

#include "ISemanticSceneNode.h"     // is a ISemanticSceneNode

#include <memory>                   // uses shared_ptr

#include "Gamelogic/Cube.h"         // holds a Cube

#include "BambooLib/include/IIdentifyable.h"

class Cube_SemSceneNode : public ISemanticSceneNode, virtual public BambooLib::IIdentifyable
{
public:
  /*! \name Creation */
  //@{
    /// static constructor
    static Cube_SemSceneNode *Create(Cube *pCube);

    /// destructor
    ~Cube_SemSceneNode();
  //@}

    /*! \name IIdentifyable interface */
    //@{
      static BambooLib::t_classID ClassID() { return s_ClassID; }
      static Cube_SemSceneNode * Cast(BambooLib::IIdentifyable *pObject) { return dynamic_cast<Cube_SemSceneNode *>(pObject); }
    //@}

  /*! \name Public methods */
  //@{
    Cube * GetCube() { return m_pCube; }
  //@}

private:
    /*! \name Constructor / Destructor */
    //@{
        /// constructor
        Cube_SemSceneNode();
    //@}

    /*! \name Private members */
    //@{
        Cube          * m_pCube;

        static BambooLib::t_classID s_ClassID;
    //@}
};

#endif
