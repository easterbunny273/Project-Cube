#include "DummyGame/SampleObject.h"
#include "SemanticSceneNodes/Light_SemSceneNode.h"

glm::mat4 SampleObject::GetTransformation()
{
    return glm::mat4();
}


std::string SampleObject::GetObjectType()
{
    return std::string("object1");
}

std::shared_ptr<ISemanticSceneNode> SampleObject::GetSceneNode()
{
	return Light_SemSceneNode::Create(glm::vec3(-0.2f, 0.10f, 0.14f), glm::vec3(1.0f, -0.4f, -1.0f), 50.0f, glm::vec3(1.0, 1.0, 1.0), 0.1, 50.0f);
}

std::string SampleObject::GetName()
{
	return "SampleObject";
}

