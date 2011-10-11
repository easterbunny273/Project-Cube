//#include <glm/gtc/matrix_projection.hpp>
//#include <glm/gtx/inverse_transpose.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Graphics/Graphic.h"
#include "Graphics/ShaderManager.h"
#include "Graphics/RenderNode.h"
#include "Graphics/RenderNodes/RenderNode_BoundingBox.h"

void RenderNode::ItlSendTransformMatrices()
{
    //first, get the positions
    const GLint l_projection_matrix = ShaderManager::instance()->GetUniform("ProjectionMatrix");
    const GLint l_view_matrix = ShaderManager::instance()->GetUniform("ViewMatrix");
    const GLint l_modelviewprojection_matrix = ShaderManager::instance()->GetUniform("ModelViewProjectionMatrix");
    const GLint l_normal_matrix = ShaderManager::instance()->GetUniform("NormalMatrix");
    const GLint l_model_matrix = ShaderManager::instance()->GetUniform("ModelMatrix");
    //const GLint l_lastFrameTransformMatrix = ShaderManager::instance()->getUniform("Camera_TransformMatrix_LastFrame");
    const GLint l_tempTransformMatrix = ShaderManager::instance()->GetUniform("Camera_TempTransformMatrix");

    //if a position is -1, it was not found, so we write only in uniforms where we found a valid position

    if (l_projection_matrix != -1)
	glUniformMatrix4fv(l_projection_matrix, 1, GL_FALSE, &m_pCurrentRenderInfo->ProjectionMatrix[0][0]);

    if (l_view_matrix != -1)
	glUniformMatrix4fv(l_view_matrix, 1, GL_FALSE, &m_pCurrentRenderInfo->ViewMatrix[0][0]);

    if (l_modelviewprojection_matrix != -1)
	glUniformMatrix4fv(l_modelviewprojection_matrix, 1, GL_FALSE, &m_pCurrentRenderInfo->ModelViewProjectionMatrix[0][0]);

    if (l_model_matrix != -1)
	glUniformMatrix4fv(l_model_matrix, 1, GL_FALSE, &m_pCurrentRenderInfo->ModelMatrix[0][0]);

/*    if (l_lastFrameTransformMatrix != -1)
	glUniformMatrix4fv(l_lastFrameTransformMatrix, 1, GL_FALSE, &Camera::instance()->getLastFrameModelViewMatrix()[0][0]);

    if (l_tempTransformMatrix != -1)
	glUniformMatrix4fv(l_tempTransformMatrix, 1, GL_FALSE, &Camera::instance()->getUsedLightProjModelViewMatrix()[0][0]);*/

    if (l_tempTransformMatrix != -1)
    {
	if (m_spShadowCaster.get() != 0)
	{
            glm::mat4 puffer = m_spShadowCaster->GetProjectionMatrix() * m_spShadowCaster->GetViewMatrix();
	    glUniformMatrix4fv(l_tempTransformMatrix, 1, GL_FALSE, &puffer[0][0]);
	}
    }

    if (l_normal_matrix != -1)
        glUniformMatrix3fv(l_normal_matrix, 1, GL_FALSE, &(glm::mat3(glm::transpose(glm::inverse(m_pCurrentRenderInfo->ViewMatrix * m_pCurrentRenderInfo->ModelMatrix))))[0][0]);


    ItlSendLightPosition();
}

void RenderNode::ItlSendLightPosition()
{
    GLint l_Light0Position = ShaderManager::instance()->GetUniform("Light0Position");

    if (l_Light0Position != -1)
    {
        float *light0Position = new float[3];
        light0Position[0] = light0Position[1] = light0Position[2] = 0.0f; //LightManager::instance()->getCurrentLightPosition();
	glUniform4f(l_Light0Position, light0Position[0], light0Position[1], light0Position[2], light0Position[3]);
    }

}


void RenderNode::SetLightSourceForShadowMapping(std::shared_ptr<Graphic::Camera> spShadowCaster, bool bRecursivelySetForChildren)
{
    m_spShadowCaster = spShadowCaster;

    if (bRecursivelySetForChildren)
        for (unsigned int a=0; a < m_vChildren.size(); a++)
	    m_vChildren[a]->SetLightSourceForShadowMapping(spShadowCaster, true);
}

void RenderNode::Render()
{
    std::shared_ptr<TItlRenderInfo> pRenderInfo (new TItlRenderInfo);

    this->Render(pRenderInfo);
}

void RenderNode::Render(std::shared_ptr<TItlRenderInfo> pCurrentRenderInfo)
{
    m_pCurrentRenderInfo = pCurrentRenderInfo;

    if (!ItlTestSkipRendering())
    {
	//store old matrices
	glm::mat4 SavedModelViewProjectionMatrix = pCurrentRenderInfo->ModelViewProjectionMatrix;
	glm::mat4 SavedInverseModelViewProjectionMatrix = pCurrentRenderInfo->InverseModelViewProjectionMatrix;
	glm::mat4 SavedModelMatrix = pCurrentRenderInfo->ModelMatrix;
	glm::mat4 SavedModelViewProjectionMatrix_ForFrustumCulling = pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling;

	//update matrices by multiplying with our matrices
	pCurrentRenderInfo->ModelViewProjectionMatrix = pCurrentRenderInfo->ModelViewProjectionMatrix * m_mTransformMatrixToWorld;
	pCurrentRenderInfo->InverseModelViewProjectionMatrix = pCurrentRenderInfo->InverseModelViewProjectionMatrix * m_mTransformMatrixFromWorld;
	pCurrentRenderInfo->ModelMatrix = pCurrentRenderInfo->ModelMatrix * m_mTransformMatrixToWorld;
	pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling = pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling * m_mTransformMatrixToWorld;

	if (ItlTestIfVisible())
	{
	    if (m_bHasChildren)
	    {
		//now recursively render children and scenenode
		ItlPreRenderChildren();

		//call render method in child nodes
                for (unsigned int a=0; a < m_vChildren.size(); a++)
		    m_vChildren[a]->Render(pCurrentRenderInfo);

		ItlPostRenderChildren();
	    }

	    ItlPreRender();
	    ItlSendTransformMatrices();
	    ItlRender();
	    ItlPostRender();
	}


	//write old matrices
	pCurrentRenderInfo->ModelViewProjectionMatrix = SavedModelViewProjectionMatrix;
	pCurrentRenderInfo->InverseModelViewProjectionMatrix = SavedInverseModelViewProjectionMatrix;
	pCurrentRenderInfo->ModelMatrix = SavedModelMatrix;
	pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling = SavedModelViewProjectionMatrix_ForFrustumCulling;
    }

    m_pCurrentRenderInfo = std::shared_ptr<TItlRenderInfo>();
}

RenderNode::RenderNode()
    : m_bHasChildren(false)
{

}

RenderNode::~RenderNode()
{

}


bool RenderNode::ItlTestIfVisible()
{
    return true;
}

bool RenderNode::ItlTestSkipRendering()
{
    return false;
}

void RenderNode::AddChild(std::shared_ptr<RenderNode> spNode)
{
    m_vChildren.push_back(spNode);

    m_bHasChildren = true;

}

bool RenderNode::RemoveChild(std::shared_ptr<RenderNode> spNode)
{
    bool bFound = false;
    auto iterator = m_vChildren.begin();

    while (iterator != m_vChildren.end())
    {
	if (*iterator == spNode)
	{
	    m_vChildren.erase(iterator);

	    bFound = true;

	    break;
	}
    }


    if (m_vChildren.empty())
	m_bHasChildren = false;

    return bFound;
}

void RenderNode::ClearChilds()
{
    m_vChildren.clear();
}

void RenderNode::GetTransformMatrix(glm::mat4 &rToWorld, glm::mat4 &rFromWorld) const
{
    rToWorld = m_mTransformMatrixToWorld;
    rFromWorld = m_mTransformMatrixFromWorld;
}

void RenderNode::SetTransformMatrix(glm::mat4 rToWorld, glm::mat4 rFromWorld)
{
    m_mTransformMatrixToWorld = rToWorld;
    m_mTransformMatrixFromWorld = rFromWorld;
}

void RenderNode::SetTransformMatrix(glm::mat4 rToWorld)
{
    glm::mat4 mFromWorld;

    mFromWorld = glm::core::function::matrix::inverse(rToWorld);

    SetTransformMatrix(rToWorld, mFromWorld);
}

bool SceneObject_BoundingBoxed::ItlInitializeBoundingBox()
{
    float *fVertices = GetVertices();
    int iNumVertices = NumVertices();

    m_fMinX = m_fMaxX = 0.0f;
    m_fMinY = m_fMaxY = 0.0f;
    m_fMinZ = m_fMaxZ = 0.0f;

    if (iNumVertices > 0)
    {
	m_fMinX = m_fMaxX = fVertices[0];
	m_fMinY = m_fMaxY = fVertices[1];
	m_fMinZ = m_fMaxZ = fVertices[2];

	for (int i=1; i < iNumVertices; i++)
	{
	    float fCurrentX = fVertices[3*i + 0];
	    float fCurrentY = fVertices[3*i + 1];
	    float fCurrentZ = fVertices[3*i + 2];

	    //max x
	    if (fCurrentX > m_fMaxX)
		m_fMaxX = fCurrentX;

	    //min x
	    if (fCurrentX < m_fMinX)
		m_fMinX = fCurrentX;

	    //max y
	    if (fCurrentY > m_fMaxY)
		m_fMaxY = fCurrentY;

	    //min y
	    if (fCurrentY < m_fMinY)
		m_fMinY = fCurrentY;

	    //max z
	    if (fCurrentZ > m_fMaxZ)
		m_fMaxZ = fCurrentZ;

	    //min z
	    if (fCurrentZ < m_fMinZ)
		m_fMinZ = fCurrentZ;
	}
    }
	return true;
}

bool SceneObject_BoundingBoxed::ItlTestIfVisible()
{
    if (m_bInitialized == false)
    {
	ItlInitializeBoundingBox();

	std::shared_ptr<RenderNode> pBoundingBox(new SceneObject_BoundingBox(m_fMinX, m_fMaxX, m_fMinY, m_fMaxY, m_fMinZ, m_fMaxZ));
	AddChild(pBoundingBox);

	m_bInitialized = true;
    }

    glm::vec4 vFrontLeftBottom	(m_fMinX, m_fMinY, m_fMinZ, 1.0);
    glm::vec4 vFrontLeftTop	(m_fMinX, m_fMaxY, m_fMinZ, 1.0);
    glm::vec4 vFrontRightBottom	(m_fMaxX, m_fMinY, m_fMinZ, 1.0);
    glm::vec4 vFrontRightTop	(m_fMaxX, m_fMaxY, m_fMinZ, 1.0);

    glm::vec4 vBackLeftBottom	(m_fMinX, m_fMinY, m_fMaxZ, 1.0);
    glm::vec4 vBackLeftTop	(m_fMinX, m_fMaxY, m_fMaxZ, 1.0);
    glm::vec4 vBackRightBottom	(m_fMaxX, m_fMinY, m_fMaxZ, 1.0);
    glm::vec4 vBackRightTop	(m_fMaxX, m_fMaxY, m_fMaxZ, 1.0);

    glm::vec4 vFrontLeftBottom_SS = m_pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling * vFrontLeftBottom;
    glm::vec4 vFrontLeftTop_SS = m_pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling * vFrontLeftTop;
    glm::vec4 vFrontRightBottom_SS = m_pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling * vFrontRightBottom;
    glm::vec4 vFrontRightTop_SS = m_pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling * vFrontRightTop;
    glm::vec4 vBackLeftBottom_SS = m_pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling * vBackLeftBottom;
    glm::vec4 vBackLeftTop_SS = m_pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling * vBackLeftTop;
    glm::vec4 vBackRightBottom_SS = m_pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling * vBackRightBottom;
    glm::vec4 vBackRightTop_SS = m_pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling * vBackRightTop;

    vFrontLeftBottom_SS /= vFrontLeftBottom_SS.w;
    vFrontLeftTop_SS /= vFrontLeftTop_SS.w;
    vFrontRightBottom_SS /= vFrontRightBottom_SS.w;
    vFrontRightTop_SS /= vFrontRightTop_SS.w;
    vBackLeftBottom_SS /= vBackLeftBottom_SS.w;
    vBackLeftTop_SS /= vBackLeftTop_SS.w;
    vBackRightBottom_SS /= vBackRightBottom_SS.w;
    vBackRightTop_SS /= vBackRightTop_SS.w;

    const float fRightClippingPlane_X = 1.0f;
    const float fLeftClippingPlane_X = -1.0f;
    const float fTopClippingPlane = 1.0f;
    const float fBottomClippingPlane = -1.0f;
    const float fNearClippingPlane = -1.0f;
    const float fFarClippingPlane = 1.0f;

    //right frustum plane
    if (vFrontLeftBottom_SS.x > fRightClippingPlane_X)
	if (vFrontLeftTop_SS.x > fRightClippingPlane_X)
	    if (vFrontRightBottom_SS.x > fRightClippingPlane_X)
		if (vFrontRightTop_SS.x > fRightClippingPlane_X)
		    if (vBackLeftBottom_SS.x > fRightClippingPlane_X)
			if (vBackLeftTop_SS.x > fRightClippingPlane_X)
			    if (vBackRightBottom_SS.x > fRightClippingPlane_X)
				if (vBackRightTop_SS.x > fRightClippingPlane_X)
				    return false;

    //left frustum plane
    if (vFrontLeftBottom_SS.x < fLeftClippingPlane_X)
	if (vFrontLeftTop_SS.x < fLeftClippingPlane_X)
	    if (vFrontRightBottom_SS.x < fLeftClippingPlane_X)
		if (vFrontRightTop_SS.x < fLeftClippingPlane_X)
		    if (vBackLeftBottom_SS.x < fLeftClippingPlane_X)
			if (vBackLeftTop_SS.x < fLeftClippingPlane_X)
			    if (vBackRightBottom_SS.x < fLeftClippingPlane_X)
				if (vBackRightTop_SS.x < fLeftClippingPlane_X)
				    return false;

    //top frustum plane
    if (vFrontLeftBottom_SS.y > fTopClippingPlane)
	if (vFrontLeftTop_SS.y > fTopClippingPlane)
	    if (vFrontRightBottom_SS.y > fTopClippingPlane)
		if (vFrontRightTop_SS.y > fTopClippingPlane)
		    if (vBackLeftBottom_SS.y > fTopClippingPlane)
			if (vBackLeftTop_SS.y > fTopClippingPlane)
			    if (vBackRightBottom_SS.y > fTopClippingPlane)
				if (vBackRightTop_SS.y > fTopClippingPlane)
				    return false;

    //bottom frustum plane
    if (vFrontLeftBottom_SS.y < fBottomClippingPlane)
	if (vFrontLeftTop_SS.y < fBottomClippingPlane)
	    if (vFrontRightBottom_SS.y < fBottomClippingPlane)
		if (vFrontRightTop_SS.y < fBottomClippingPlane)
		    if (vBackLeftBottom_SS.y < fBottomClippingPlane)
			if (vBackLeftTop_SS.y < fBottomClippingPlane)
			    if (vBackRightBottom_SS.y < fBottomClippingPlane)
				if (vBackRightTop_SS.y < fBottomClippingPlane)
				    return false;

    //near frustum plane
    if (vFrontLeftBottom_SS.z < fNearClippingPlane)
	if (vFrontLeftTop_SS.z < fNearClippingPlane)
	    if (vFrontRightBottom_SS.z < fNearClippingPlane)
		if (vFrontRightTop_SS.z < fNearClippingPlane)
		    if (vBackLeftBottom_SS.z < fNearClippingPlane)
			if (vBackLeftTop_SS.z < fNearClippingPlane)
			    if (vBackRightBottom_SS.z < fNearClippingPlane)
				if (vBackRightTop_SS.z < fNearClippingPlane)
				    return false;
    //far frustum plane
    if (vFrontLeftBottom_SS.z > fFarClippingPlane)
	if (vFrontLeftTop_SS.z > fFarClippingPlane)
	    if (vFrontRightBottom_SS.z > fFarClippingPlane)
		if (vFrontRightTop_SS.z > fFarClippingPlane)
		    if (vBackLeftBottom_SS.z > fFarClippingPlane)
			if (vBackLeftTop_SS.z > fFarClippingPlane)
			    if (vBackRightBottom_SS.z > fFarClippingPlane)
				if (vBackRightTop_SS.z > fFarClippingPlane)
				    return false;

    return true;
}
