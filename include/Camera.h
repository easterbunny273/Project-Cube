/*
 * header file for Camera class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 10/2010 Project "BambooIsland"
 * 03/2011 Project "Free Pool", Project "Bamboo Engine"
 */

#pragma once

#ifndef __realtime_lu_camera
#define __realtime_lu_camera

#include <glm/glm.hpp>
#include <fstream>

class Camera
{
    public:
    /*! \name Public Types */
    //@{
	/// The camera supports either recording a new path or replaying the recorder path. This enum represents the states
	enum CameraMode
	{
	    Record, Replay
	};
    //@}

    /*! \name Construcors / Destructors */
    //@{
	/// Constructor
	Camera(glm::vec3 position, float fRotationX, float fRotationY, float fFieldOfView, float fAspectRatio, float fNearPlane, float fFarPlane);

	Camera(glm::vec3 position, float fRotationX, float fRotationY, float fLeft, float fRight, float fBottom, float fTop, float fNear, float fFar);

	/// Destructor
	~Camera();
    //@}

    /*! \name Access */
    //@{
	/// adds a vector to the move vector. Note that if you add a vector, you must subtract it later if you want the camera to stop this movement
	void addToMoveVector(glm::vec3 v3Vector);

	/// increases the x rotation (negative value decreases it)
	void incRotationX(float value);

	/// increases the y rotation (negative value decreases it)
	void incRotationY(float value);

	/// sets the camera mode (PLAY or RECORD; \sa CameraMode )
	void setMode(CameraMode mode);

	/// returns the ModelviewMatrix
	glm::mat4 getViewMatrix() const;

	/// returns the projection matrix
	glm::mat4 getProjectionMatrix() const;

	/// returns the used temporary matrix
	glm::mat4 getUsedTemporaryMatrix() const;

	/// moves the camera along the current move vector,
	/// note that the move-vector is always relative to the view-direction
	void move();
    //@}

private:
    /*! \name Private members */
    //@{
	/// Which camera mode is currently active?
	CameraMode m_mode;

	/// The time offset for recorder/player. When a new mode is set, this value is set to current time because recording/replaying needs the time since the last mode change
	double m_dRecordReplayTimer;

	/// The timestep which was last replayed
	double m_dLastReplayedTime;

	/// The projection matrix of the camera
	glm::mat4 m_m4ProjectionMatrix;

	/// The modelview matrix of the camera
	glm::mat4 m_m4ViewMatrix;

	/// The position vector of the camera
	glm::vec3 m_v3Position;

	/// The moving vector of the camera
	glm::vec3 m_v3MovingVector;

	/// The rotation value of the camera. 0 means that the camera looks along the z-direction at the near plane, while 180 means that the camera looks towards the far plane.
	double m_dRotationX;

	/// The rotation value of the camera
	double m_dRotationY;

	/// The output stream for camera records
	std::ofstream m_camera_recorder;

	/// The input stream for camera records
	std::ifstream m_camera_path;
    //@}
};

#endif // QFP_CAMERA_H
