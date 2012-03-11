/*
 * header file for SceneObject_AssimpImport class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 03 / 2011
 */

#ifndef __BAMBOO_RENDERNODE_GENERIC_HEADER
#define __BAMBOO_RENDERNODE_GENERIC_HEADER

#include <string>
#include <vector>

#include "RenderNodes/IRenderNode.h"
#include "GeometryData.h"

//!  A Rendernode which renders a given object
/*!
    tbd
*/

class Bamboo::RN_Generic : public Bamboo::IRenderNode
{
public:
    /*! \name Constructor / Destructor */
    //@{
    /// constructor, creates scene object and imports model from given file
    RN_Generic(std::shared_ptr<GeometryData::GenericObject> spObject);
    virtual ~RN_Generic();
    //@}

    void SetEnvironmentMapping(bool bEnabled, GLuint nTextureID = 0);

protected:


    /*! \name RenderNode Interface */
    //@{
    /*! this method is called before the render() method calls the render() methods of the children,
            and can be used to bind a fbo (to render the children in this fbo) or something like that */
    virtual void ItlPreRenderChildren();

    /*! this method is called after all children were rendered,
            and can be used to unbind a fbo or something like that */
    virtual void ItlPostRenderChildren();

    /*! this method is called before the sceneobject itself gets rendered.
            shaders and things like that should be activated in this method */
    virtual void ItlPreRender();

    /*! this method is called to render the sceneobject.
            Attention: If the correct shader program is not bound yet (should be done in itlBeforeRender()),
            the transform matrices must be sent again */
    virtual void ItlRender();

    /*! this method is called after rendering the sceneobject itself. Cleaning up can be done here */
    virtual void ItlPostRender();

    virtual bool ItlTestSkipRendering();
    //virtual bool ItlTestIfVisible();
    //@}

private:


    /*! \name Internal types */
    //@{
//    struct TItlTextureInformation
//    {
//        GeometryData::TextureType   tTextureType;
//        std::string                 sTextureName;
//        unsigned int                nVBOOffset;
//    };
    //@}

    /*! \name Internal helper methods */
    //@{
        void ItlLoadShader();
        void ItlDeleteBuffers();
        void ItlPrepareGLBuffers();
        void ItlPrepareTextures();
        void ItlDeleteTextures();
        void ItlPrepareVAO();
    //@}

    /*! \name Private members */
    //@{
    GLuint *m_pnVertexArrayObjects;

    GLuint *m_pnVertexBufferObjects;
    GLuint *m_pnIndexBufferObjects;

    std::vector<std::string> * m_pvsAttributeNames;
    std::vector<unsigned int> * m_pvnAttributeOffsets;

    //std::vector<std::vector<TItlTextureInformation> > m_vvTexureInformationPerMesh;
    std::vector<std::map<std::string, unsigned int> > m_vmTextureInformations;



    unsigned int *m_pnNumIndices;
    unsigned int m_nNumMeshes;

    GLuint *m_pnColorTexture;
    GLuint *m_pnNormalTexture;
    GLuint *m_pnSpecularTexture;
    GLuint *m_pnDisplaceTexture;

    GLuint  m_nEnvironmentMap;

    std::shared_ptr<GeometryData::GenericObject> m_spObject;

    bool m_bUseEnvironmentMapping;
    //@}

    /*! \name Static members */
    //@{

    //@}


};

#endif

