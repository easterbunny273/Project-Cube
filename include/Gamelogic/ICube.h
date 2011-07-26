/*
 * header file for ICube interface
 * written by: christian moellinger <ch.moellinger@gmail.com>
 *             florian spechtenhauser <florian.spechtenhauser@gmail.com>
 *
 * 07/2011, Project "Project Cube"
 */

#pragma once
#ifndef __ICUBE_PROJECTCUBE_HEADER
#define __ICUBE_PROJECTCUBE_HEADER

class ICube
{
public:
    /*! \name Public methods */
    //@{
    /// returns the id of the cube
    virtual unsigned int GetCubeID() = 0;

    /// returns the position of the cube in the big cube matrix
    virtual glm::vec3 GetCubePosition() = 0;

    /// returns the current transformation matrix of the object
    virtual glm::mat4 GetTransformation() = 0;
    //@}

};

#endif // __ICUBE_PROJECTCUBE_HEADER
