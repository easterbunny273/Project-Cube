/*
 * header file for Graphic class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 10/2010 Projekt "BambooIsland"
 * 03/2011 Projekt "Free Pool"
 */

#ifndef __realtime_lu_graphic
#define __realtime_lu_graphic

// opengl libs
#include "GLUtils.h"

// stl includes
#include <memory>
#include <list>
#include <map>

//#ifdef GLFW_VERSION_MAJOR
//#define GLFW
//#endif

// glm includes
#include <glm/glm.hpp>

// semantic scene node interface
#include "SemanticSceneNodes/ISemanticSceneNode.h"
#include "KeyIdentifiers.h"

namespace BambooGraphics
{
    // forward declarations
    class SceneObject_RenderTarget;
    class INodeTranslator;

    class ShaderManager;
    class TextureManager;

    class GraphicsCore
    {
    public:
        /*! \name Internal classes, forward declarations */
        //@{
            class IRenderNode;
            class IRenderNode_Cullable;
        //@}

    public:
        /*! \name Nested classes, forward declarations */
        //@{
            class IRenderTarget
            {
            public:
                /*! \name Public interfaces */
                //@{
                    /// Interface for event listener, which receives the input events which happened in a glfw window
                    class IInputEventListener
                    {
                    public:
                        /// handles keyboard events and sends signals to listener
                        virtual void ItlHandleKeyboardEvent(TKey eKeyIdentifier, TInputEvent eEvent) = 0;

                        /// handles mouse movements and sends signals to the listener
                        virtual void ItlHandleMousePos(int iX, int iY) = 0;

                        /// handles mouse wheel input events and sends signals to the listener
                        virtual void ItlHandleMouseWheel(int iPosition) = 0;

                        /// handles mouse button events and sends signals to the listener
                        virtual void ItlHandleMouseButton(TMouseButton eButton, TInputEvent eEvent) = 0;
                    };
                //@}

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
            class GlfwWindowRenderTarget;
    #endif
    #ifdef QT_OPENGL_LIB
            class QtWidgetRenderTarget;
    #endif
            class ICamera;
            class PerspectiveCamera;
            class OrthogonalCamera;
        //@}

        /*! \name Nested classes */
        //@{

        //@}

        /*! \name Construction / Destruction */
        //@{
            /// constructor
            GraphicsCore();

            /// destructor
            ~GraphicsCore();
        //@}

        /*! \name Public Attributes */
        //@{
        /// returns the responsible shader manager for this graphics instance
        ShaderManager * GetShaderManager();

        /// returns the responsible texture manager for this graphics instance
        TextureManager * GetTextureManager();

            /// workaround method, should be removed
            static GraphicsCore * GetSingleInstance() { return s_pInstance; }
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
                              ISemanticSceneNode *pRootNode,
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
                ISemanticSceneNode * pSceneRoot;
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
            static GraphicsCore * s_pInstance;
        //@}
    };

}; // namespace

#endif
