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
#include <glm/core/type_mat4x4.hpp>

// forward declarations
class SceneObject;
class SceneObject_RenderTarget;
class Camera;

class IInputEventListener;

class Graphic
{
public:
    /*! \name Nested classes */
    //@{
        class Camera
        {
            friend class Graphic;
        public:
            /*! \name Public attributes */
            //@{
                /// returns the ModelviewMatrix
                glm::mat4 GetViewMatrix() const;

                /// returns the projection matrix
                glm::mat4 GetProjectionMatrix() const;
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

        private:
            /*! \name Construction / Destruction */
            //@{
                /// constructor
                Camera();
            //@}

            /*! \name Construction / Destruction */
            //@{
                bool        m_bInitialized;         ///< wheter the projection matrix was set up
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
        Graphic();

        /// destructor
        ~Graphic();
    //@}

    /*! \name Initialization / Cleanup */
    //@{
        /// initializes the open gl stuff and creates the window + context.
        bool InitializeOpenGL();

        /// shuts down the context + window
        bool ShutDown();
    //@}

    /*! \name Methods for input event handling */
    //@{
        /// registers an input handler to the graphic engine, which
        /// will redirect all input events of the opengl window to this handler
        /// (observer-pattern, methods of IInputEventListener will be called)
        void RegisterInputHandler(IInputEventListener *pListener);

        /// returns wheter an input event handler is registered
        bool IsInputHandlerRegistered();

	void HideAndLockMouseToWindowCenter();

	void UnHideAndUnLockMouse();
    //@}

    /*! \name Methods for render paths */
    //@{
        /// registers a new render path with the given root node and given name
        void AddRenderPath(std::shared_ptr<SceneObject> spRoot, std::string sRenderPath);

        /// draws the given render path
        void SetActiveRenderPath(std::string sRenderPath);
    //@}

    /*! \name Public Attributes */
    //@{
	Camera * GetCamera() { return &m_Camera; }
        Camera * GetDebugCamera() { return &m_DebugCamera; }
    //@}

    /*! \name Methods for render */
    //@{
        void Render();
    //@}

private:
    /*! \name Static helper methods */
    //@{
        /// handles keyboard events and sends signals to listener
        static void ItlStaticHandleKeyboardEvent(int iKeyIdentifier, int iNewKeyState);

        static void ItlStaticHandleMousePos(int iX, int iY);

        static void ItlStaticHandleMouseWheel(int iPosition);

        static void ItlStaticHandleMouseButton(int iButton, int iAction);
    //@}

    /*! \name Internal helper methods */
    //@{
        /// handles keyboard events and sends signals to listener
        void ItlHandleKeyboardEvent(int iKeyIdentifier, int iNewKeyState);

        void ItlHandleMousePos(int iX, int iY);

        void ItlHandleMouseWheel(int iPosition);

        void ItlHandleMouseButton(int iButton, int iAction);

        void ItlCreateOpenGLWindow();

        void ItlInitializeOpenGLStates();

	void ItlLoadShaderPrograms();

	void ItlCreateBaseRenderPath();
    //@}

    /*! \name Private members */
    //@{
	std::map<std::string, std::shared_ptr<SceneObject> >    m_vRenderPaths;

	std::list<std::shared_ptr<SceneObject> >                m_vSceneGraphs;


	int		    m_iWidth;			///< width of the created opengl window
	int		    m_iHeight;			///< height of the created opengl window
	bool		    m_bWindowOpenened;		///< whether an output window is currently open

	int		    m_iFramesInThisSecondYet;
	int		    m_iFramesPerSecond;
	float		    m_fTimeOfLastRenderCall;

	bool		    m_bIsMouseLocked;		///< whether the mouse is currently locked to the center

	std::string	    m_sActiveRenderPath;

	Camera		    m_Camera;			///< the main camera, also used for culling
	Camera		    m_DebugCamera;		///< the debug camera, used to debug things like culling process

	static int	    s_iInstances;		///< how many instances are created ?
	static Graphic *    s_pInstance;

	IInputEventListener *m_pInputEventListener;
    //@}
};

#endif
