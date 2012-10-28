/*
 * source file for SceneObject_FluidDynamics class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 06/2011
 */
#include <string>
#include <sstream>
#include <vector>

//#include <glm/gtc/matrix_projection.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include <memory>

#include "Graphics/TextureManager.h"
#include "Logger.h"
#include "Settings.h"
#include "Graphics/ShaderManager.h"
#include "Graphics/SceneObjects/SceneObject_FluidDynamics.h"
#include "Graphics/SceneObjects/SceneObject_FBO.h"
#include "Graphics/SceneObjects/SceneObject_RenderPass.h"
#include "Graphics/SceneObjects/SceneObject_PostEffect.h"
#include "Graphics/SceneObjects/SceneObject_FluidDynamicsStep.h"

using namespace std;

const int iGridSize = 256;
const float fSize = (float) iGridSize;
const float fTimestep = 0.3;
//const float fViscosity = 0.00000000000000000000000000000000000000001f;
//const float fViscosity = 0.000000000000000000000000001f;
//const float fViscosity = 0.00000001f;
//const float fViscosity = 0.01f;
const float fViscosity = 0.00000000001f;

SceneObject_FluidDynamics::SceneObject_FluidDynamics()
    : SceneObject()
{
    GLdouble *vertexArray;
    GLuint *indexArray;

    vertexArray = new GLdouble[8 * 3 + 8 * 2];

    const float fMinX = -1.0f;
    const float fMinY = -1.0f;
    const float fMinZ = -1.0f;
    const float fMaxX = 1.0f;
    const float fMaxY = 1.0f;
    const float fMaxZ = 1.0f;

    //0 links unten vorne
    vertexArray[0] = fMinX; vertexArray[1] = fMinY; vertexArray[2] = fMinZ;

    //1 links oben vorne
    vertexArray[3] = fMinX; vertexArray[4] = fMaxY; vertexArray[5] = fMinZ;

    //2 rechts oben vorne
    vertexArray[6] = fMaxX; vertexArray[7] = fMaxY; vertexArray[8] = fMinZ;

    //3 rechts unten vorne
    vertexArray[9] = fMaxX; vertexArray[10] = fMinY; vertexArray[11] = fMinZ;

    //4 links unten hinten
    vertexArray[12] = fMinX; vertexArray[13] = fMinY; vertexArray[14] = fMaxZ;

    //5 links oben hinten
    vertexArray[15] = fMinX; vertexArray[16] = fMaxY; vertexArray[17] = fMaxZ;

    //6 rechts oben hinten
    vertexArray[18] = fMaxX; vertexArray[19] = fMaxY; vertexArray[20] = fMaxZ;

    //7 rechts unten hinten
    vertexArray[21] = fMaxX; vertexArray[22] = fMinY; vertexArray[23] = fMaxZ;

    // links unten vorne texcoords
    vertexArray[24] = 0.0f;
    vertexArray[25] = 0.0f;

    // links oben vorne texcoords
    vertexArray[26] = 0.0f;
    vertexArray[27] = fSize;

    // rechts oben vorne texcoords
    vertexArray[28] = fSize;
    vertexArray[29] = fSize;

    // rechts unten vorne texcoords
    vertexArray[30] = fSize;
    vertexArray[31] = 0.0f;

    // links unten hinten texcoords
    vertexArray[32] = 0.0f;
    vertexArray[33] = 0.0f;

    // links oben hinten texcoords
    vertexArray[34] = 0.0f;
    vertexArray[35] = fSize;

    // rechts oben hinten texcoords
    vertexArray[36] = fSize;
    vertexArray[37] = fSize;

    // rechts unten hinten texcoords
    vertexArray[38] = fSize;
    vertexArray[39] = 0.0f;


    indexArray = new GLuint[36];

    //2 triangles vorne
    indexArray[0] = 0;
    indexArray[1] = 1;
    indexArray[2] = 2;

    indexArray[3] = 2;
    indexArray[4] = 3;
    indexArray[5] = 0;

    //2 triangles hinten
    indexArray[6] = 7;
    indexArray[7] = 6;
    indexArray[8] = 5;

    indexArray[9] = 5;
    indexArray[10] = 4;
    indexArray[11] = 7;

    //2 triangles links
    indexArray[12] = 4;
    indexArray[13] = 5;
    indexArray[14] = 1;

    indexArray[15] = 1;
    indexArray[16] = 0;
    indexArray[17] = 4;

    //2 triangles rechts
    indexArray[18] = 3;
    indexArray[19] = 2;
    indexArray[20] = 6;

    indexArray[21] = 6;
    indexArray[22] = 7;
    indexArray[23] = 3;

    //2 triangles oben
    indexArray[24] = 1;
    indexArray[25] = 5;
    indexArray[26] = 6;

    indexArray[27] = 6;
    indexArray[28] = 2;
    indexArray[29] = 1;

    //2 triangles unten
    indexArray[30] = 0;
    indexArray[31] = 3;
    indexArray[32] = 7;

    indexArray[33] = 7;
    indexArray[34] = 4;
    indexArray[35] = 0;


    glGenVertexArrays(1, &m_nVertexArrayObject);
    glBindVertexArray(m_nVertexArrayObject);

    /* Vertexbuffer generieren */
    glGenBuffers(1,&m_nVertexBufferObject);
    glGenBuffers(1,&m_nIndexBufferObject);

    /* zum befüllen mal aktivieren ...*/
    glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObject);
    /* befüllen ... */
    glBufferData(GL_ARRAY_BUFFER, 40 * sizeof(vertexArray[0]), vertexArray, GL_STATIC_DRAW);

    /* nun nur noch den Index-Array in den Bufferbereich schreiben */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(indexArray[0]), indexArray, GL_STATIC_DRAW);

    m_iIndexArraySize = 36;

    delete[] vertexArray;
    delete[] indexArray;

    //check for error
    GLenum error = glGetError();

    if (error != GL_NO_ERROR)
        Logger::error() << "glGetError: " << TranslateGLerror(error) << Logger::endl;

    // create pipeline
    CreateFluidPipeline();

    Logger::debug() << "SceneObject_FluidDynamics created" << Logger::endl;

}

SceneObject_FluidDynamics::~SceneObject_FluidDynamics()
{

}
bool SceneObject_FluidDynamics::ItlTestSkipRendering()
{
    if (m_pCurrentRenderInfo->tCurrentRenderPass == SceneObject_RenderPass::RENDERPASS_SHADOWMAP)
	return true;
    else
	return false;
}

void SceneObject_FluidDynamics::ItlPreRenderChildren()
{
    bool bAddForces;

    Settings::instance()->GetGroup("shadow")->GetValueOrDefault("bAddForces", false, bAddForces);

    m_spAddForces->SetUniform("bAddForces", (int)bAddForces);

    bool bReInit;

    Settings::instance()->GetGroup("shadow")->GetValueOrDefault("bReInit", false, bReInit);

    if (bReInit)
    {
	m_vChildren.insert(m_vChildren.begin(), m_spInitVelocityFBO);
	m_vChildren.insert(m_vChildren.begin(), m_spInitTintFBO);

	m_bInitStepsAttached = true;

	Settings::instance()->GetGroup("shadow")->SetValue("bReInit", false);
    }
}

void SceneObject_FluidDynamics::ItlPreRender()
{
    //TextureManager::instance()->swapTextureAndDeleteOld("fluid-dynamics-tex-pressure", "fluid-dynamics-tex-jacobi-pressure59");
    glBindVertexArray(m_nVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObject);

    ShaderManager::instance()->pushActiveShader();
    /*if (m_pCurrentRenderInfo->tCurrentRenderPass == SceneObject_RenderPass::RENDERPASS_SHADOWMAP)
	ShaderManager::instance()->activateShader("fluidcube");
    else
	*/
    ShaderManager::instance()->activateShader("fluidcube");
}

void SceneObject_FluidDynamics::ItlPostRender()
{
    ShaderManager::instance()->popActiveShader();
}

void SceneObject_FluidDynamics::ItlRender()
{
    if (m_bInitStepsAttached)
    {
	this->m_vChildren.erase(this->m_vChildren.begin());
	this->m_vChildren.erase(this->m_vChildren.begin());

	m_bInitStepsAttached = false;
	//this->m_vChildren.erase(this->m_vChildren.begin());
    }

//    const GLint l_in_Position(ShaderManager::instance()->getAttribute("in_Position"));

//    const GLint l_in_Texcoords(ShaderManager::instance()->getAttribute("in_Texcoord"));

//    assert (l_in_Position != -1);

//    if (l_in_Position != -1)
//    {
//	glVertexAttribPointer(l_in_Position, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(GLdouble), NULL);
//	glEnableVertexAttribArray(l_in_Position);
//    }

//    assert (l_in_Texcoords != -1);

//    if (l_in_Texcoords != -1)
//    {
//	glVertexAttribPointer(l_in_Texcoords, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(GLdouble), (GLvoid *) (24 * sizeof(GLdouble)));
//	glEnableVertexAttribArray(l_in_Texcoords);
//    }

//    static bool bSwitch = false;

//    bSwitch = !bSwitch;

//    std::string sTextureName("fluid-dynamics-tex-tint");


//    GLuint nUsedTextureUnit = TextureManager::instance()->useTexture(sTextureName);

//    GLint iTexture1Location = ShaderManager::instance()->getUniform("texture1");

//    assert (iTexture1Location != -1);

//    glUniform1i(iTexture1Location, nUsedTextureUnit);

//    float fPreviousLineWidth;

//    glGetFloatv(GL_LINE_WIDTH, &fPreviousLineWidth);

//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferObject);
//    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, NULL);

//    TextureManager::instance()->unuseTexture(sTextureName);


}

void SceneObject_FluidDynamics::CreateFluidPipeline()
{
    GLint iTarget = GL_TEXTURE_RECTANGLE;
    GLint iFormat = GL_RGBA32F;
    GLint iClampMode = GL_CLAMP_TO_EDGE;
    GLint iFiltering = GL_LINEAR;

    TextureManager::instance()->CreateSampler("fluid-dynamics-tex-velocity", iTarget, iFormat, iGridSize, iGridSize, iClampMode, iFiltering);
    TextureManager::instance()->CreateSampler("fluid-dynamics-tex-velocity2", iTarget, iFormat, iGridSize, iGridSize, iClampMode, iFiltering);
    TextureManager::instance()->CreateSampler("fluid-dynamics-tex-tint", iTarget, iFormat, iGridSize, iGridSize, iClampMode, iFiltering);
    TextureManager::instance()->CreateSampler("fluid-dynamics-tex-pressure", iTarget, iFormat, iGridSize, iGridSize, iClampMode, iFiltering);
    TextureManager::instance()->CreateSampler("fluid-dynamics-divergence-temp", iTarget, iFormat, iGridSize, iGridSize, iClampMode, iFiltering);
    TextureManager::instance()->CreateSampler("fluid-dynamics-computed-pressure", iTarget, iFormat, iGridSize, iGridSize, iClampMode, iFiltering);
    TextureManager::instance()->CreateSampler("fluid-dynamics-tex-velocity-temp2", iTarget, iFormat, iGridSize, iGridSize, iClampMode, iFiltering);
    TextureManager::instance()->CreateSampler("fluid-dynamics-tint-temp", iTarget, iFormat, iGridSize, iGridSize, iClampMode, iFiltering);
    TextureManager::instance()->CreateSampler("fluid-dynamics-tex-velocity-with-forces", iTarget, iFormat, iGridSize, iGridSize, iClampMode, iFiltering);



    shared_ptr<SceneObject_FluidDynamicsStep> spInitVelocity(new SceneObject_FluidDynamicsStep("fluid-dynamics-texturewrite", -1.0f));
    spInitVelocity->SetUniformSampler("texture1",  "fluid-dynamics-velocity-start");

    m_spInitVelocityFBO = std::shared_ptr<SceneObject_FBO>(new SceneObject_FBO(true, fSize, fSize, "fluid-dynamics-tex-velocity"));

    m_spInitVelocityFBO->AddChild(spInitVelocity);
    AddChild(m_spInitVelocityFBO);

    shared_ptr<SceneObject_FluidDynamicsStep> spInitTint (new SceneObject_FluidDynamicsStep("fluid-dynamics-texturewrite"));
    spInitTint->SetUniformSampler("texture1", "fluid-dynamics-tint-start");
    m_spInitTintFBO = std::shared_ptr<SceneObject_FBO> (new SceneObject_FBO(true, fSize, fSize, "fluid-dynamics-tex-tint"));

    m_spInitTintFBO->AddChild(spInitTint);
    AddChild(m_spInitTintFBO);

    m_bInitStepsAttached = true;

    ////////////

    std::shared_ptr<SceneObject_FluidDynamicsStep> spInitPressure (new SceneObject_FluidDynamicsStep("fluid-dynamics-texturewrite", 1.0f));
    spInitPressure->SetUniformSampler("texture1", "fluid-dynamics-pressure-start");
    std::shared_ptr<SceneObject_FBO> spInitPressureFBO (new SceneObject_FBO(true, fSize, fSize, "fluid-dynamics-tex-pressure"));

    spInitPressureFBO->AddChild(spInitPressure);
    AddChild(spInitPressureFBO);

    /* TODO: remove this; as long as there is the "return" statement, the simulation is never started */
    //return;

    std::shared_ptr<SceneObject_FluidDynamicsStep> spAdvect (new SceneObject_FluidDynamicsStep("fluid-dynamics-advection"));
    std::shared_ptr<SceneObject_FBO> spFBOAdvect (new SceneObject_FBO(true, fSize, fSize, "fluid-dynamics-tex-velocity2"));

    spAdvect->SetUniformSampler("u", "fluid-dynamics-tex-velocity");
    spAdvect->SetUniformSampler("x", "fluid-dynamics-tex-velocity");
    spAdvect->SetUniform("timestep", fTimestep);
    spAdvect->SetUniform("rdx", fSize);


    spFBOAdvect->AddChild(spAdvect);
    AddChild(spFBOAdvect);


    std::shared_ptr<SceneObject_FluidDynamicsStep> spBoundary1 (new SceneObject_FluidDynamicsStep("fluid-dynamics-texturewrite", -1.0f));
    spBoundary1->SetUniformSampler("texture1", "fluid-dynamics-tex-velocity2");
    std::shared_ptr<SceneObject_FBO> spBoundary1FBO (new SceneObject_FBO(true, fSize, fSize, "fluid-dynamics-tex-velocity"));
    spBoundary1FBO->AddChild(spBoundary1);
    AddChild(spBoundary1FBO);

    int iLoops = 15;

    std::string sTextureName_LastJacobiStep;

    for (int i=0; i < iLoops; i++)
    {
	std::stringstream buffer;
	std::string sTextureName_PreviousStep;
	std::string sTextureName_CurrentStep;
	std::string sTextureName_CurrentStep_Temp;

	float dX_squared = (1.0f / fSize) * (1.0f / fSize);
	float fAlpha = dX_squared / (fViscosity * fTimestep);

	if (i == 0)
	    buffer << "fluid-dynamics-tex-velocity";
	else
	    buffer << "fluid-dynamics-tex-jacobi" << (i-1);
	buffer >> sTextureName_PreviousStep;

	buffer.clear();
	buffer << "fluid-dynamics-tex-jacobi" << i;
	buffer >> sTextureName_CurrentStep;

	sTextureName_CurrentStep_Temp = sTextureName_CurrentStep;
	sTextureName_CurrentStep_Temp.append("temp");

	sTextureName_LastJacobiStep = sTextureName_CurrentStep;

	TextureManager::instance()->CreateSampler(sTextureName_CurrentStep_Temp, iTarget, iFormat, iGridSize, iGridSize, iClampMode, iFiltering);
	TextureManager::instance()->CreateSampler(sTextureName_CurrentStep, iTarget, iFormat, iGridSize, iGridSize, iClampMode, iFiltering);

	std::shared_ptr<SceneObject_FluidDynamicsStep> spJacobiPE (new SceneObject_FluidDynamicsStep("fluid-dynamics-jacobi"));
	std::shared_ptr<SceneObject_FBO> spJacobiFBO (new SceneObject_FBO(true, fSize, fSize, sTextureName_CurrentStep_Temp.c_str()));

	spJacobiPE->SetUniformSampler("x", sTextureName_PreviousStep);
	spJacobiPE->SetUniformSampler("b", sTextureName_PreviousStep);
	spJacobiPE->SetUniform("alpha", fAlpha);
	spJacobiPE->SetUniform("rBeta", (1.0f / (4.0f + fAlpha)));

	spJacobiFBO->AddChild(spJacobiPE);
	AddChild(spJacobiFBO);

	std::shared_ptr<SceneObject_FluidDynamicsStep> spBoundary (new SceneObject_FluidDynamicsStep("fluid-dynamics-texturewrite", -1.0f));
	std::shared_ptr<SceneObject_FBO> spBoundaryFBO (new SceneObject_FBO(true, fSize, fSize, sTextureName_CurrentStep.c_str()));

	spBoundary->SetUniformSampler("texture1", sTextureName_CurrentStep_Temp);

	spBoundaryFBO->AddChild(spBoundary);
	AddChild(spBoundaryFBO);
    }




    m_spAddForces = shared_ptr<SceneObject_FluidDynamicsStep> (new SceneObject_FluidDynamicsStep("fluid-dynamics-addforces"));
    m_spAddForces->SetUniformSampler("texture1", sTextureName_LastJacobiStep);
    m_spAddForces->SetUniformSampler("texture2", "fluid-dynamics-velocity-start");
    shared_ptr<SceneObject_FBO> spAddForcesFBO (new SceneObject_FBO(true, fSize, fSize, "fluid-dynamics-tex-velocity-with-forces"));
    spAddForcesFBO->AddChild(m_spAddForces);
    AddChild(spAddForcesFBO);

    std::shared_ptr<SceneObject_FluidDynamicsStep> spVelocitySwap (new SceneObject_FluidDynamicsStep("fluid-dynamics-texturewrite", -1.0f));
    spVelocitySwap->SetUniformSampler("texture1",  "fluid-dynamics-tex-velocity-with-forces");
    std::shared_ptr<SceneObject_FBO> spVelocitySwapFBO (new SceneObject_FBO(true, fSize, fSize, "fluid-dynamics-tex-velocity"));

    spVelocitySwapFBO->AddChild(spVelocitySwap);
    AddChild(spVelocitySwapFBO);


    shared_ptr<SceneObject_FluidDynamicsStep> spComputePressure (new SceneObject_FluidDynamicsStep("fluid-dynamics-divergence"));
    shared_ptr<SceneObject_FBO> spComputePressureFBO (new SceneObject_FBO(true, fSize, fSize, "fluid-dynamics-divergence-temp"));

    spComputePressure->SetUniformSampler("w", "fluid-dynamics-tex-velocity");
    spComputePressure->SetUniform("halfrdx", 0.5f * fSize);

    spComputePressureFBO->AddChild(spComputePressure);
    AddChild(spComputePressureFBO);

    std::shared_ptr<SceneObject_FluidDynamicsStep> spPressureBoundary (new SceneObject_FluidDynamicsStep("fluid-dynamics-texturewrite", 1.0f));
    std::shared_ptr<SceneObject_FBO> spPressureBoundaryFBO (new SceneObject_FBO(true, fSize, fSize, "fluid-dynamics-computed-pressure"));

    spPressureBoundary->SetUniformSampler("texture1", "fluid-dynamics-divergence-temp");

    spPressureBoundaryFBO->AddChild(spPressureBoundary);
    AddChild(spPressureBoundaryFBO);

    int iLoops2 = 50;

    for (int i=0; i < iLoops2; i++)
    {
	std::stringstream buffer;
	std::string sTextureName_PreviousStep;
	std::string sTextureName_CurrentStep;
	std::string sTextureName_CurrentStep_Temp;

	if (i == 0)
	    buffer << "fluid-dynamics-tex-pressure";
	else
	    buffer << "fluid-dynamics-tex-jacobi-pressure" << (i-1);
	buffer >> sTextureName_PreviousStep;

	buffer.clear();
	buffer << "fluid-dynamics-tex-jacobi-pressure" << i;
	buffer >> sTextureName_CurrentStep;

	sTextureName_CurrentStep_Temp = sTextureName_CurrentStep;
	sTextureName_CurrentStep_Temp.append("temp");


	sTextureName_LastJacobiStep = sTextureName_CurrentStep;

	TextureManager::instance()->CreateSampler(sTextureName_CurrentStep_Temp, iTarget, iFormat, iGridSize, iGridSize, iClampMode, iFiltering);
	TextureManager::instance()->CreateSampler(sTextureName_CurrentStep, iTarget, iFormat, iGridSize, iGridSize, iClampMode, iFiltering);

	std::shared_ptr<SceneObject_FluidDynamicsStep> spJacobiPE (new SceneObject_FluidDynamicsStep("fluid-dynamics-jacobi"));
	std::shared_ptr<SceneObject_FBO> spJacobiFBO (new SceneObject_FBO(true, fSize, fSize, sTextureName_CurrentStep_Temp.c_str()));

	spJacobiPE->SetUniformSampler("x", sTextureName_PreviousStep);
	spJacobiPE->SetUniformSampler("b", "fluid-dynamics-computed-pressure");

	float dX_squared = (1.0f / fSize) * (1.0f / fSize);

	spJacobiPE->SetUniform("alpha", -dX_squared);
	spJacobiPE->SetUniform("rBeta", 0.25f);

	spJacobiFBO->AddChild(spJacobiPE);
	AddChild(spJacobiFBO);

	std::shared_ptr<SceneObject_FluidDynamicsStep> spBoundary (new SceneObject_FluidDynamicsStep("fluid-dynamics-texturewrite", 1.0f));
	std::shared_ptr<SceneObject_FBO> spBoundaryFBO (new SceneObject_FBO(true, fSize, fSize, sTextureName_CurrentStep.c_str()));

	spBoundary->SetUniformSampler("texture1", sTextureName_CurrentStep_Temp);

	spBoundaryFBO->AddChild(spBoundary);
	AddChild(spBoundaryFBO);
    }

    std::shared_ptr<SceneObject_FluidDynamicsStep> spPressureSwap (new SceneObject_FluidDynamicsStep("fluid-dynamics-texturewrite", 1.0f));
    spPressureSwap->SetUniformSampler("texture1", sTextureName_LastJacobiStep);
    std::shared_ptr<SceneObject_FBO> spPressureSwapFBO (new SceneObject_FBO(true, fSize, fSize, "fluid-dynamics-tex-pressure"));
    spPressureSwapFBO->AddChild(spPressureSwap);
    AddChild(spPressureSwapFBO);

    shared_ptr<SceneObject_FluidDynamicsStep> spSubtractPressureGradient (new SceneObject_FluidDynamicsStep("fluid-dynamics-gradient"));
    shared_ptr<SceneObject_FBO> spSubtractPressureGradientFBO (new SceneObject_FBO(true, fSize, fSize, "fluid-dynamics-tex-velocity-temp2"));

    spSubtractPressureGradient->SetUniformSampler("p", "fluid-dynamics-tex-pressure");
    spSubtractPressureGradient->SetUniformSampler("w", "fluid-dynamics-tex-velocity");
    spSubtractPressureGradient->SetUniform("halfrdx", 0.5f * fSize);

    spSubtractPressureGradientFBO->AddChild(spSubtractPressureGradient);
    AddChild(spSubtractPressureGradientFBO);

    std::shared_ptr<SceneObject_FluidDynamicsStep> spBoundary3 (new SceneObject_FluidDynamicsStep("fluid-dynamics-texturewrite", -1.0f));
    std::shared_ptr<SceneObject_FBO> spBoundary3FBO (new SceneObject_FBO(true, fSize, fSize, "fluid-dynamics-tex-velocity"));

    spBoundary3->SetUniformSampler("texture1", "fluid-dynamics-tex-velocity-temp2");

    spBoundary3FBO->AddChild(spBoundary3);
    AddChild(spBoundary3FBO);

    /* advect tint */

    std::shared_ptr<SceneObject_FluidDynamicsStep> spAdvectTint (new SceneObject_FluidDynamicsStep("fluid-dynamics-advection"));
    std::shared_ptr<SceneObject_FBO> spFBOAdvectTint (new SceneObject_FBO(true, fSize, fSize, "fluid-dynamics-tint-temp"));

    spAdvectTint->SetUniformSampler("u", "fluid-dynamics-tex-velocity");
    spAdvectTint->SetUniformSampler("x", "fluid-dynamics-tex-tint");
    spAdvectTint->SetUniform("timestep", fTimestep);
    spAdvectTint->SetUniform("rdx", fSize);

    spFBOAdvectTint->AddChild(spAdvectTint);
    AddChild(spFBOAdvectTint);

    /* write tint-temp texture to tint texture */

    std::shared_ptr<SceneObject_FluidDynamicsStep> spSwapTintTexture (new SceneObject_FluidDynamicsStep("fluid-dynamics-texturewrite"));
    std::shared_ptr<SceneObject_FBO> spSwapTintTextureFBO (new SceneObject_FBO(true, fSize, fSize, "fluid-dynamics-tex-tint"));

    spSwapTintTexture->SetUniformSampler("texture1", "fluid-dynamics-tint-temp");

    spSwapTintTextureFBO->AddChild(spSwapTintTexture);
    AddChild(spSwapTintTextureFBO);
}

