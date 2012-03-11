#ifndef __FRACTALTERRAIN_SEMSCENENODE_BAMBOOENGINE
#define __FRACTALTERRAIN_SEMSCENENODE_BAMBOOENGINE

#include "ISemanticSceneNode.h"     // is a ISemanticSceneNode

#include <memory>                   // uses shared_ptr
#include <string>                   // uses string

class FractalTerrain_SemSceneNode : public ISemanticSceneNode
{
public:
  /*! \name Creation */
  //@{
    /// static constructor
    static std::shared_ptr<FractalTerrain_SemSceneNode> Create(std::string sFilename);

    /// destructor
    ~FractalTerrain_SemSceneNode();
  //@}

  /*! \name Class identification */
  //@{
    static t_classID ClassID();
  //@}

  /*! \name Public methods */
  //@{
    /// returns the filename of the loaded fractal data
    std::string GetFilename();

    std::string SetFilename(std::string sFilename);

    /// todo: should be able to use the generated data directly, without writing in a file
    std::string SetFromCudaMemory();
  //@}

private:
    /*! \name Constructor / Destructor */
    //@{
        /// constructor
        FractalTerrain_SemSceneNode();
    //@}

    /*! \name Private members */
    //@{
        std::string   m_sFilename;
    //@}
};

#endif
