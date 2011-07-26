//#include <glm/gtc/matrix_projection.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>

#include "Graphics/common_gl.h"
#include "Camera.h"

using namespace std;

const float move_factor = 2.7f;
const double pi = 3.14159265;

const double DEFAULT_FOV = 45.0;
const double DEFAULT_RATIO = 1.33;
const double DEFAULT_NEARPLANE = 0.1f;
const double DEFAULT_FARPLANE = 100.0f;

const double x_border = 50.0;		//the border of the camera space in x direction - camera position is forced between [-x_border, x_border]
const double y_lower_border = -2.0;     //the border of the camera space in y direction - camera position is forced between [-y_border, y_border]
const double y_upper_border = 30.0;
const double z_border = 50.0;	    //the border of the camera space in z direction - camera position is forced between [-z_border, z_border]

Camera::Camera(glm::vec3 position, float fRotationX, float fRotationY, float fFieldOfView, float fAspectRatio, float fNearPlane, float fFarPlane)
{
    this->m_m4ProjectionMatrix = glm::perspective(fFieldOfView, fAspectRatio, fNearPlane, fFarPlane);
    this->m_m4ViewMatrix = glm::mat4();
    this->m_v3Position = position;
    this->m_dRotationX = static_cast<double>(fRotationX);
    this->m_dRotationY = static_cast<double>(fRotationY);

    this->move();

    setMode(Camera::Record);
}

Camera::Camera(glm::vec3 position, float fRotationX, float fRotationY, float fLeft, float fRight, float fBottom, float fTop, float fNear, float fFar)
{
    this->m_m4ProjectionMatrix = glm::gtc::matrix_transform::ortho(fLeft, fRight, fBottom, fTop, fNear,  fFar);
    this->m_m4ViewMatrix = glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    this->m_v3Position = position;
    this->m_dRotationX = static_cast<double>(fRotationX);
    this->m_dRotationY = static_cast<double>(fRotationY);

    this->move();

    setMode(Camera::Record);
}

Camera::~Camera()
{
    m_camera_recorder.close();
    m_camera_path.close();
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return this->m_m4ProjectionMatrix;
}

void Camera::setMode(CameraMode mode)
{
    if (m_camera_recorder.is_open())
	m_camera_recorder.close();

    if (m_camera_path.is_open())
	m_camera_path.close();

    if (mode == Camera::Record)
    {
	m_camera_recorder.open("camera.txt", ios::out);
    }
    else if (mode == Camera::Replay)
    {
	m_camera_path.open("camera.txt", ios::in);
    }

    m_mode = mode;
    m_dLastReplayedTime = 0;
    m_dRecordReplayTimer = glfwGetTime();
}

void Camera::move()
{
    if (m_mode == Camera::Record)
    {
	glm::vec3 move_x;
	glm::vec3 move_y;
	glm::vec3 move_z;

	if (this->m_v3MovingVector.x != 0)
	{
	    //if camera should move in x-direction, we must calculate the look-at point
	    //on the camera-sphere and use this vector as reference, and move normal to this vector

	    move_x.x = sin(((this->m_dRotationX+90) / 180.0) * pi);
	    move_x.y = 0.0;
	    move_x.z = cos(((this->m_dRotationX+90) / 180.0) * pi);

	    //normalize to project point on our sphere
	    move_x /= sqrt(move_x.x*move_x.x + move_x.y*move_x.y + move_x.z*move_x.z);

	    //and multiplicate with given x-factor to adjust movement length
	    move_x *= this->m_v3MovingVector.x;
	}

	if (this->m_v3MovingVector.y != 0)
	{
	    //if camera should move in y-direction, we can use the given value directly

	    move_x.y = this->m_v3MovingVector.y;
	}

	if (this->m_v3MovingVector.z != 0)
	{
	    //if camera should move in z-direction, we must calculate the look-at point
	    //on the camera-sphere and use this vector as reference, and move along this vector

	    move_z.x = sin((this->m_dRotationX / 180.0) * pi);
	    move_z.y = tan((this->m_dRotationY / 180.0) * pi);
	    move_z.z = cos((this->m_dRotationX / 180.0) * pi);

	    //normalize to project point on our sphere
	    move_z /= sqrt(move_z.x*move_z.x + move_z.y*move_z.y + move_z.z*move_z.z);

	    //and multiplicate with given z-factor to adjust movement length
	    move_z *= this->m_v3MovingVector.z;
	}

	this->m_v3Position += (move_x + move_y + move_z) * move_factor;

	/*if (this->m_v3Position.x > x_border)
	    this->m_v3Position.x = x_border;

	if (this->m_v3Position.x < -x_border)
	    this->m_v3Position.x = -x_border;

	if (this->m_v3Position.y > y_upper_border)
	    this->m_v3Position.y = y_upper_border;

	if (this->m_v3Position.y < y_lower_border)
	    this->m_v3Position.y = y_lower_border;

	if (this->m_v3Position.z > z_border)
	    this->m_v3Position.z = z_border;

	if (this->m_v3Position.z < -z_border)
	    this->m_v3Position.z = -z_border;*/

	// Output camera position and rotation



	m_camera_recorder << glfwGetTime() - m_dRecordReplayTimer << " " << m_v3Position.x << " " << m_v3Position.y << " " << m_v3Position.z << " " << m_dRotationX << " " << m_dRotationY << endl;
    }
    else if (m_mode == Camera::Replay)
    {

    static double pos_x, pos_y, pos_z, rot_x, rot_y;

    while (m_dLastReplayedTime < (glfwGetTime() - m_dRecordReplayTimer) && !m_camera_path.eof())
    {
	m_camera_path >> m_dLastReplayedTime;
	m_camera_path >> pos_x;
	m_camera_path >> pos_y;
	m_camera_path >> pos_z;
	m_camera_path >> rot_x;
	m_camera_path >> rot_y;
    }

    m_v3Position.x = pos_x;
    m_v3Position.y = pos_y;
    m_v3Position.z = pos_z;
    m_dRotationX = rot_x;
    m_dRotationY = rot_y;

    }

    //Calculate modelview matrix

    glm::vec3 lookAt;

    lookAt.x = sin((this->m_dRotationX / 180.0) * pi);
    lookAt.y = tan((this->m_dRotationY / 180.0) * pi);
    lookAt.z = cos((this->m_dRotationX / 180.0) * pi);

    //m_m4LastFrameModelViewMatrix = m_m4ViewMatrix;
    m_m4ViewMatrix = glm::lookAt(this->m_v3Position, this->m_v3Position + lookAt, glm::vec3(0,1,0));

   // m_camera_recorder << glfwGetTime() << " " << m_v3Position.x << " " << m_v3Position.y << " " << m_v3Position.z << " " << m_dRotationX << " " << m_dRotationY << endl;
}

glm::mat4 Camera::getViewMatrix() const
{
    return m_m4ViewMatrix;
}

void Camera::addToMoveVector(glm::vec3 vector)
{
    this->m_v3MovingVector += vector;
}

void Camera::incRotationX(float value)
{
    this->m_dRotationX += value;

    //map to [0.0, 360.0[
    if (this->m_dRotationX >= 360)
	this->m_dRotationX -= 360;

    if (this->m_dRotationX < 0)
	this->m_dRotationX += 360;
}

void Camera::incRotationY(float value)
{
    this->m_dRotationY += value;

    //restrict to [-89.5, 89.5]
    if (this->m_dRotationY > 89.5)
	this->m_dRotationY = 89.5;

    if (this->m_dRotationY < -89.5)
	this->m_dRotationY = -89.5;
}

/*
Camera *Camera::instance()
{
    static Camera camera_singelton_instance(glm::vec3(0,5,10), 180, 0);
    return &camera_singelton_instance;
}*/
