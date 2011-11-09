/*
 * header file for AssimpWrapper
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 11/2011 Project "BambooEngine"
  */

#pragma once

#ifndef __BAMBOO_ASSIMP_WRAPPER_HEADER

#include <string>
#include <memory>

#include "GeometryData.h"

/// this class provides methods to load a model file with assimp into a GeometryData structure
class AssimpWrapper
{
public:
    static std::shared_ptr<GeometryData::GenericObject> LoadModel(std::string sFilename);
};

#endif
