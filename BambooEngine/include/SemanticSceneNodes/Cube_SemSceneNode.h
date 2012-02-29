#ifndef __CUBE_SEMSCENENODE_BAMBOOENGINE
#define __CUBE_SEMSCENENODE_BAMBOOENGINE

#include "ISemanticSceneNode.h"     // is a ISemanticSceneNode

#include <memory>                   // uses shared_ptr

#include "Gamelogic/Cube.h"         // holds a Cube

class Cube_SemSceneNode : public ISemanticSceneNode
{
public:
  /*! \name Creation */
  //@{
    /// static constructor
    static std::shared_ptr<Cube_SemSceneNode> Create(Cube *pCube);

    /// destructor
    ~Cube_SemSceneNode();
  //@}

  /*! \name Class identification */
  //@{
    static t_classID ClassID();
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
    //@}
};

#endif
