#ifndef _BAMBOO_CAMERA_HEADER
#define _BAMBOO_CAMERA_HEADER

#include "Graphic.h"

class Bamboo::ICamera
{
public:
    /*! \name Public attributes */
    //@{
        /// returns the ViewMatrix
        glm::mat4 GetViewMatrix() const;

        /// returns the translation matrix
        glm::mat4 GetTranslationMatrix() const;

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

        /// sets the position
        void SetPosition(glm::vec3 vPosition);
    //@}

    /*! \name Methods to set up the projection */
    //@{
        /// sets up an perspective projection matrix
        void SetPerspectiveProjection (float fFieldOfView, float fAspectRatio, float fNearPlane, float fFarPlane);

        /// sets up an orthographic projection matrix
        void SetOrthoProjection (float fLeft, float fRight, float fTop, float fBottom, float fNear, float fFar);
    //@}

protected:
    /*! \name Construction / Destruction */
    //@{
        /// constructor
        ICamera();
    //@}

    /*! \name Protected members */
    //@{
        bool        m_bInitialized;         ///< whether the projection matrix was set up
        glm::mat4   m_m4ProjectionMatrix;
        glm::mat4   m_m4ViewMatrix;
        glm::mat4   m_m4TranslateMatrix;
        glm::vec3   m_v3MoveVector;
        glm::vec3   m_v3CameraPosition;

        float       m_fRotationHorizontal;
        float       m_fRotationVertical;
    //@}
};

class Bamboo::PerspectiveCamera : public Bamboo::ICamera
{
public:
    static std::shared_ptr<Bamboo::ICamera> Create(float fFieldOfView,
                                                  float fAspectRatio,
                                                  float fNearPlane,
                                                  float fFarPlane,
                                                  glm::vec3 vInitialPosition,
                                                  float fInitialRotationX,
                                                  float fInitialRotationY);
};

class Bamboo::OrthogonalCamera : public Bamboo::ICamera
{
public:
    static std::shared_ptr<Bamboo::ICamera> Create(float fLeft,
                                                  float fRight,
                                                  float fTop,
                                                  float fBottom,
                                                  float fNear,
                                                  float fFar,
                                                  glm::vec3 vInitialPosition,
                                                  glm::vec3 vLookDirection);
};

#endif
