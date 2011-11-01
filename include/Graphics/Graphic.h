/*
 * header file for Graphic class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 10/2010 Projekt "BambooIsland"
 * 03/2011 Projekt "Free Pool"
 */

#ifndef __realtime_lu_graphic
#define __realtime_lu_graphic

// stl includes
#include <memory>
#include <list>
#include <string>
#include <map>

// glm includes
//#include <glm/core/type_mat4x4.hpp>
#include <glm/glm.hpp>

// settings class
//#include "Settings.h"

// eventmanager
//#include "EventManager.h"

// forward declarations
class SceneObject_RenderTarget;
class Camera;

class Bamboo
{
private:
    /*! \name Internal classes */
    //@{
        class Shader;
        class ShaderManager;
        class TextureManager;

        class IRenderNode;
        class IRenderNode_Cullable;

        class RN_PostEffect;
        class RN_Camera;
        class RN_Cube;
        class RN_FBO;
        class RN_RenderPass;
        class RN_BoundingBox;
        class RN_AssimpImport;
    //@}

public:
    /*! \name Nested classes */
    //@{
        /// forward declarations
#ifdef GLFW
        class GlfwWindow;
#endif
#ifdef QT_OPENGL_LIB
        class QtWidgetWrapper;
#endif
        class Camera;

        class ISceneObject;
        class SO_Cube;
        class SO_LoadedModel;

        class IRenderTarget
        {
        public:
            virtual void ClearBuffers() = 0;
            virtual void SwapBuffers() = 0;

        protected:
            IRenderTarget() {}
            virtual ~IRenderTarget() {}
        };

        class IInputEventListener
        {
        public:
            /// handles keyboard events and sends signals to listener
            virtual void ItlHandleKeyboardEvent(int iKeyIdentifier, int iNewKeyState) = 0;

            /// handles mouse movements and sends signals to the listener
            virtual void ItlHandleMousePos(int iX, int iY) = 0;

            /// handles mouse wheel input events and sends signals to the listener
            virtual void ItlHandleMouseWheel(int iPosition) = 0;

            /// handles mouse button events and sends signals to the listener
            virtual void ItlHandleMouseButton(int iButton, int iAction) = 0;
        };

        class Scene
        {
        public:
            static std::shared_ptr<Scene> Create(std::shared_ptr<Bamboo::Camera> spCamera);

            void CallRenderPath(Bamboo *pGraphicCore);

            void AttachObject(std::shared_ptr<ISceneObject> spSceneObject);

            void InvalidateObjectData(std::shared_ptr<ISceneObject> spSceneObject) {}

            void CreateRenderGraph(Bamboo *pGraphicCore);
        private:
            std::list<std::shared_ptr<ISceneObject> >     m_lSceneObjects;
            std::shared_ptr<Bamboo::Camera>        m_spCamera;
            std::shared_ptr<IRenderNode>            m_spRenderGraphRoot;
        };
    //@}

    /*! \name Nested classes */
    //@{
        class Camera //: public EventManager::IEventListener
        {
        public:
            /*! \name Construction / Destruction */
            //@{
                /// constructor
                Camera();
            //@}

            /*! \name Public attributes */
            //@{
                /// returns the ModelviewMatrix
                glm::mat4 GetViewMatrix() const;

                /// returns the projection matrix
                glm::mat4 GetProjectionMatrix() const;
            //@}

	    /*! \name EventManager::IEventListener interface */
	    //@{
                //virtual bool OnEvent(std::shared_ptr<EventManager::IEvent> spEvent);
	    //@}


            /*! \name Methods to manipulate the camera view direction or position */
            //@{
                /// rotates the camera in vertical direction
                void RotateVertical(float fValue);

                /// rotates the camera in horizontal direction
                void RotateHorizontal(float fValue);

                /// adds the given vector to the move vector.
                /// the move vector is always relative to the
                /// current camera viewing direction
                void AddToMoveVector(glm::vec3 vVector);

                /// moves the camera along the move vector.
                /// must be called! otherwise, the view matrices are not up to date
                void Move(float fFactor);
            //@}

            /*! \name Methods to set up the projection */
            //@{
                /// sets up an perspective projection matrix
                void SetPerspectiveProjection (float fFieldOfView, float fAspectRatio, float fNearPlane, float fFarPlane);

                /// sets up an orthographic projection matrix
                void SetOrthoProjection (float fLeft, float fRight, float fTop, float fBottom, float fNear, float fFar);
            //@}

	    /*! \name Other Methods */
	    //@{
                void SetActive(bool bActive) { m_bActive = bActive; }
	    //@}

        private:


	    /*! \name Private members */
            //@{
		bool        m_bActive;		    ///< whether this camera is the currently active one
		bool        m_bInitialized;         ///< whether the projection matrix was set up
                glm::mat4   m_m4ProjectionMatrix;
                glm::mat4   m_m4ViewMatrix;
                glm::vec3   m_v3MoveVector;
                glm::vec3   m_v3CameraPosition;

                float       m_fRotationHorizontal;
                float       m_fRotationVertical;
            //@}
        };

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
        void Render();
    //@}

    /*! \name new methods - refactoring - comments tbd */
    //@{
        int AddRenderLoop(std::shared_ptr<IRenderTarget> spRenderTarget,
                          std::shared_ptr<Camera> spCamera,
                          std::shared_ptr<Scene> spScene);

        void RemoveRenderLoop(int iLoopID);
    //@}

private:
    /*! \name new methods - refactoring - comments tbd */
    //@{
        struct TItlRenderLoop
        {
            std::shared_ptr<IRenderTarget>      spRenderTarget;
            std::shared_ptr<Camera>             spCamera;
            std::shared_ptr<Scene>              spScene;
        };

        std::map<int, TItlRenderLoop>   m_mRenderLoops;
    //@}

    /*! \name Private members */
    //@{
        std::map<std::string, std::shared_ptr<Bamboo::IRenderNode> >    m_vRenderPaths;

        ShaderManager       * m_pShaderManager;
        TextureManager      * m_pTextureManager;
    //@}

    /*! \name Static members */
    //@{
        static Bamboo * s_pInstance;
    //@}
};

#endif
