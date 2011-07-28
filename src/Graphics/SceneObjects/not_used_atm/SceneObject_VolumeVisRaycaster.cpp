#include "Graphics/SceneObjects/SceneObject_VolumeVisRaycaster.h"
#include "Graphics/SceneObjects/SceneObject_VolumeVisColorCube.h"
#include "Graphics/SceneObjects/SceneObject_FBO.h"
#include "Graphics/SceneObjects/SceneObject_RenderPass.h"
#include "Logger.h"
#include "Graphics/ShaderManager.h"
#include "Graphics/TextureManager.h"
#include "Settings.h"
#include <memory>

SceneObject_VolumeVisRaycaster::SceneObject_VolumeVisRaycaster() : SceneObject()
{
    GLdouble *vertexArray;
    GLuint *indexArray;

    vertexArray = new GLdouble[8 * 3];

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
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vertexArray[0]), vertexArray, GL_STATIC_DRAW);

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

    std::shared_ptr<SceneObject> spFBO_Front(new SceneObject_FBO(1024, 768, "VolumeVisCube_Front", true, false));
    std::shared_ptr<SceneObject> spFBO_Back(new SceneObject_FBO(1024, 768, "VolumeVisCube_Back", true, false));

    std::shared_ptr<SceneObject> spColorCube_Front(new SceneObject_VolumeVisColorCube(GL_BACK));
    std::shared_ptr<SceneObject> spColorCube_Back(new SceneObject_VolumeVisColorCube(GL_FRONT));

    spFBO_Back->AddChild(spColorCube_Back);
    spFBO_Front->AddChild(spColorCube_Front);

    AddChild(spFBO_Back);
    AddChild(spFBO_Front);


    //AddChild(spColorCube_Front);

    Logger::debug() << "SceneObject_VolumeVisRaycaster created" << Logger::endl;

}

bool SceneObject_VolumeVisRaycaster::ItlTestSkipRendering()
{
    /*if (m_pCurrentRenderInfo->tCurrentRenderPass == SceneObject_RenderPass::RENDERPASS_SHADOWMAP)
	return true;
    else
	return false;*/
}

void SceneObject_VolumeVisRaycaster::ItlPreRender()
{
    ShaderManager::instance()->pushActiveShader();

    if (m_pCurrentRenderInfo->tCurrentRenderPass == SceneObject_RenderPass::RENDERPASS_DEEP_OPACITY_MAP1)
	ShaderManager::instance()->activateShader("deep_step1_shader");
    else if (m_pCurrentRenderInfo->tCurrentRenderPass == SceneObject_RenderPass::RENDERPASS_DEEP_OPACITY_MAP2)
	ShaderManager::instance()->activateShader("deep_step2_shader");
    else
	ShaderManager::instance()->activateShader("volumevis-raycaster");
}

void SceneObject_VolumeVisRaycaster::ItlRender()
{
    glBindVertexArray(m_nVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObject);

    const GLint l_in_Position(ShaderManager::instance()->getAttribute("in_Position"));

    assert (l_in_Position != -1);

    glVertexAttribPointer(l_in_Position, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(GLdouble), NULL);

    glEnableVertexAttribArray(l_in_Position);

    //load first texture and tell the shader the units id
    GLuint used_texture_unit1 = TextureManager::instance()->useTexture("VolumeVisCube_Front");	    //load texture in texture unit
    GLuint used_texture_unit2 = TextureManager::instance()->useTexture("VolumeVisCube_Back");	    //load texture in texture unit
    GLuint used_texture_unit4 = TextureManager::instance()->useTexture("fluid-dynamics-tex-tint");	    //load texture in texture unit
    GLuint used_texture_unit7 = 0;

    GLint l_opacity_depth_texture = ShaderManager::instance()->getUniform("opacity_depth");
    if (l_opacity_depth_texture != -1)
    {
	    GLuint texture_unit_opacity_depth = TextureManager::instance()->useTexture("fbo_deep_opacity_map_depth");	    //load texture in texture unit
	    glUniform1i(l_opacity_depth_texture, texture_unit_opacity_depth);   //tell the shader which texture unit was used
    }

    GLint l_opacity_opacity_texture = ShaderManager::instance()->getUniform("opacity_opacity");
    if (l_opacity_opacity_texture != -1)
    {
	    GLuint texture_unit_opacity_opacity = TextureManager::instance()->useTexture("fbo_deep_opacity_map_opacities");	    //load texture in texture unit
	    glUniform1i(l_opacity_opacity_texture, texture_unit_opacity_opacity);   //tell the shader which texture unit was used
    }

    GLuint iStep1TextureID=0;
    bool bOk = TextureManager::instance()->isTextureRegistered("fbo_deep_opacity_map_depth", iStep1TextureID);
    assert (bOk);
    bool bTextureInUse = TextureManager::instance()->isTextureLocked(iStep1TextureID);

    if (!bTextureInUse)
	used_texture_unit7 = TextureManager::instance()->useTexture("fbo_deep_opacity_map_depth");	    //load texture in texture unit

    GLint l_texture_step1_location = ShaderManager::instance()->getUniform("step1");

    assert (l_texture_step1_location == -1 || !bTextureInUse);

    if (l_texture_step1_location != -1)
	glUniform1i(l_texture_step1_location, used_texture_unit7);   //tell the shader which texture unit was used

    GLint l_location_cube_front = ShaderManager::instance()->getUniform("cube_front");

    if (l_location_cube_front != -1)
	glUniform1i(l_location_cube_front,used_texture_unit1);   //tell the shader which texture unit was used

    GLint l_location_cube_back = ShaderManager::instance()->getUniform("cube_back");

    if (l_location_cube_back != -1)
	glUniform1i(l_location_cube_back,used_texture_unit2);   //tell the shader which texture unit was used

    GLint l_location_smoke = ShaderManager::instance()->getUniform("smoke");

    if (l_location_smoke != -1)
	glUniform1i(l_location_smoke, used_texture_unit4);   //tell the shader which texture unit was used


    GLint l_use_shadow = ShaderManager::instance()->getUniform("bUseShadow");

    if (l_use_shadow != -1)
    {
	bool bUseShadow;

	Settings::instance()->GetGroup("shadow")->GetValueOrDefault("bUseShadow", true, bUseShadow);
	glUniform1i(l_use_shadow, (int)bUseShadow);   //tell the shader which texture unit was used
    }


    GLint l_show_levels = ShaderManager::instance()->getUniform("bShowLevels");

    if (l_show_levels != -1)
    {
	bool bShowLevels;
	Settings::instance()->GetGroup("shadow")->GetValueOrDefault("bShowLevels", false, bShowLevels);
	glUniform1i(l_show_levels, (int)bShowLevels);   //tell the shader which texture unit was used
    }

    GLint l_interpolate = ShaderManager::instance()->getUniform("bInterpolate");

    if (l_interpolate != -1)
    {
	bool bInterpolate;
	Settings::instance()->GetGroup("shadow")->GetValueOrDefault("bInterpolate", true, bInterpolate);
	glUniform1i(l_interpolate, (int)bInterpolate);   //tell the shader which texture unit was used
    }

    if ((m_pCurrentRenderInfo->tCurrentRenderPass != SceneObject_RenderPass::RENDERPASS_DEEP_OPACITY_MAP1)
	&&(m_pCurrentRenderInfo->tCurrentRenderPass != SceneObject_RenderPass::RENDERPASS_DEEP_OPACITY_MAP2))
    {
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    //draw data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferObject);
    glDrawElements(GL_TRIANGLES, m_iIndexArraySize, GL_UNSIGNED_INT, NULL);

    glDisable(GL_BLEND);

    //release used texture units
    TextureManager::instance()->unuseTexture("VolumeVisCube_Front");			    //tell the texturemanager to release the used texture unit
    TextureManager::instance()->unuseTexture("VolumeVisCube_Back");			    //tell the texturemanager to release the used texture unit
    TextureManager::instance()->unuseTexture("fluid-dynamics-tex-tint");			    //tell the texturemanager to release the used texture unit

    if (l_opacity_depth_texture != -1)
	TextureManager::instance()->unuseTexture("fbo_deep_opacity_map_depth");	    //load texture in texture unit

    if (l_opacity_opacity_texture != -1)
	TextureManager::instance()->unuseTexture("fbo_deep_opacity_map_opacities");	    //load texture in texture unit


    if (!bTextureInUse)
	TextureManager::instance()->unuseTexture("fbo_deep_opacity_map_depth");	    //load texture in texture unit



    ShaderManager::instance()->popActiveShader();
}
