#include "DummyGame/SampleObject.h"

glm::mat4 SampleObject::GetTransformation()
{
    return glm::mat4();
}


std::string SampleObject::GetObjectType()
{
    return std::string("object1");
}
