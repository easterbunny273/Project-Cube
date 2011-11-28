/*
 * header file for Bamboo::LightManager class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 11/2011, Project "BambooEngine"
 */

#pragma once

#ifndef __BAMBOOENGINE_LIGHTMANAGER_HEADER
#define __BAMBOOENGINE_LIGHTMANAGER_HEADER

#include "common_gl.h"
#include "Graphic.h"

/*
class Bamboo::LightManager
{
public:
    class PointLight : public ILight
    {

    };

    class SpotLight : public ILight, public IIdentifiable
    {
    private:
        SpotLight() : IIdentifiable(s_nClassID) {}

        static IIdentifiable::t_classID     s_nClassID;
    public:
        static IIdentifiable::t_classID     ClassID() { return s_nClassID; }

        static std::shared_ptr<SpotLight> Create(glm::vec3 vPosition, glm::vec3 vDirection, float fRadius, glm::vec3 vColor)
        {
            return std::shared_ptr<SpotLight>(new SpotLight());
        }
    };

    class AmbientLight : public ILight
    {

    };

    class SunLight : public ILight
    {

    };


    void AddLight(std::weak_ptr<ILight> spLight);
    void RemoveLight(std::weak_ptr<ILight> spLight);

private:
    std::list<std::weak_ptr<ILight> >   m_lwpLights;

};*/

#endif
