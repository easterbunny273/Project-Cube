#ifndef BAMBOO_RENDERENGINE_UE_GENERATION
#define BAMBOO_RENDERENGINE_UE_GENERATION

#include "GeometryData.h"

#include <vector>

class RenderEngineUE_Generation
{
public:
  std::vector<GeometryData::GenericObject *> GenerateSpheres(unsigned int nNum);
};

#endif
