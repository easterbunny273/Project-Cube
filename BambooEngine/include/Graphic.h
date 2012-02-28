/*
 * header file for Graphic class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 10/2010 Projekt "BambooIsland"
 * 03/2011 Projekt "Free Pool"
 */

#ifndef __realtime_lu_graphic
#define __realtime_lu_graphic

// opengl libs
#include "common_gl.h"

// stl includes
#include <memory>
#include <list>
#include <map>

//#ifdef GLFW_VERSION_MAJOR
#define GLFW
//#endif

// glm includes
#include <glm/glm.hpp>

// semantic scene node interface
#include "SemanticSceneNodes/ISemanticSceneNode.h"

// forward declarations
class SceneObject_RenderTarget;
class INodeTranslator;

class Bamboo
{
public:
    /*! \name Internal classes, forward declarations */
    //@{
        class Shader;
        class ShaderManager;
        class TextureManager;

        class IRenderNode;
        class IRenderNode_Cullable;

        class RN_PostEffect;
        class RN_Camera;
        class RN_FBO;
        class RN_RenderPass;
        class RN_BoundingBox;
        class RN_AssimpImport;
        class RN_Generic;
        class RN_Deferred;
        class RN_SpotLight;
        class RN_CubeMap;
        class RN_SpotLight_Model;
    //@}

public:
    /*! \name Nested classes, forward declarations */
    //@{
        class IRenderTarget
        {
        public:
            virtual void ClearBuffers() = 0;
            virtual void SwapBuffers() = 0;

        protected:
            IRenderTarget() {}
            virtual ~IRenderTarget() {}
        };
    //@}

    /*! \name Nested classes, forward declarations */
    //@{
#ifdef GLFW
        class GlfwWindow;
#endif
#ifdef QT_OPENGL_LIB
        class QtWidgetWrapper;
#endif
        class ICamera;
        class PerspectiveCamera;
        class OrthogonalCamera;

        class LightManager;
    //@}

    /*! \name Nested classes */
    //@{

    //@}

    /*! \name Construction / Destruction */
    //@{
        /// constructor
        Bamboo();

        /// destructor
        ~Bamboo();
    //@}

    /*! \name Public Attributes */
    //@{
	/// returns the responsible shader manager for this graphics instance
	ShaderManager * GetShaderManager();

	/// returns the responsible texture manager for this graphics instance
	TextureManager * GetTextureManager();

        /// workaround method, should be removed
        static Bamboo * GetSingleInstance() { return s_pInstance; }
    //@}

    /*! \name Methods for render */
    //@{
        /// renders all render loops to their render targets
        void Render();
    //@}

    /*! \name new methods - refactoring - comments tbd */
    //@{
        /// adds a render loop and returns its id
        int AddRenderLoop(std::shared_ptr<IRenderTarget> spRenderTarget,
                          std::shared_ptr<ISemanticSceneNode> spRootNode,
                          std::shared_ptr<INodeTranslator> spTranslator);

        /// removes a render loop
        void RemoveRenderLoop(int iLoopID);
    //@}

private:
    /*! \name Nested classes and structs */
    //@{
        struct TItlRenderLoop
        {
            std::shared_ptr<IRenderTarget>      spRenderTarget;
            std::shared_ptr<ISemanticSceneNode> spSceneRoot;
            std::shared_ptr<INodeTranslator>    spTranslator;
        };
    //@}

    /*! \name Internal helper methods */
    //@{

    //@}

    /*! \name Private members */
    //@{
        std::map<int, TItlRenderLoop>   m_mRenderLoops;

        ShaderManager       * m_pShaderManager;
        TextureManager      * m_pTextureManager;
    //@}

    /*! \name Static members */
    //@{
        static Bamboo * s_pInstance;
    //@}
};

#endif
