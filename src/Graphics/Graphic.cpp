/*
* source file for Graphic class
* written by: christian moellinger <ch.moellinger@gmail.com>
* 10/2010
*/

#include <string>
#include <sstream>
#include <memory>

//#include <glm/gtc/matrix_projection.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Graphics/common_gl.h"
#include "Graphics/ShaderManager.h"
#include "Graphics/TextureManager.h"
#include "Logger.h"
#include "Graphics/SceneObject.h"
#include "Graphics/SceneObjects/SceneObject_Sphere.h"
#include "Graphics/SceneObjects/SceneObject_PostEffect.h"
#include "Graphics/SceneObjects/SceneObject_LightSphere.h"
#include "Graphics/SceneObjects/SceneObject_RenderPass.h"
#include "Graphics/SceneObjects/SceneObject_EmptyNode.h"
#include "Graphics/SceneObjects/SceneObject_Camera.h"
#include "Graphics/SceneObjects/SceneObject_VolumeVisRaycaster.h"
#include "Graphics/SceneObjects/SceneObject_FluidDynamics.h"
#include "Graphics/SceneObjects/SceneObject_FluidDynamicsStep.h"
#include "Graphics/SceneObjects/SceneObject_FBO.h"

#include "Graphics/SceneObjects/SceneObject_AssimpImport.h"

#include "Camera.h"
#include "Graphics/Graphic.h"

using namespace std;

Graphic::Graphic(int width, int height, bool fullscreen) : m_iWidth(width), m_iHeight(height)
{
	int flags = (fullscreen) ? GLFW_FULLSCREEN : GLFW_WINDOW;	//if fullscreen is true, flags is set to GLFW_FULLSCREEN, else to GLFW_WINDOW

	// Set flags so GLFW creates the desired OpenGL context
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Activate 4x antialiasing
	//glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);

	if (glfwOpenWindow(m_iWidth, m_iHeight, 0,0,0,0, 24, 8, flags) != GL_TRUE)
		Logger::fatal() << "failed to initialize OpenGL window" << Logger::endl;
	else
		Logger::debug() << "OpenGL window initialized" << Logger::endl;

	glfwSetWindowTitle("Free Pool (NG) written by Christian Moellinger");

	// Disable the mouse cursor
	//glfwDisable(GLFW_MOUSE_CURSOR);
	
	// Disable VSync
	//glfwSwapInterval(0);

	// start up GLEW
	glewExperimental = GL_TRUE; //set experimental flag to true, needed for correct importing of function pointers when using core-profile
	if (glewInit() != GLEW_OK)
		Logger::fatal() << "glew initialization failed" << Logger::endl;
	else
		Logger::debug() << "glew initialized" << Logger::endl;

	Logger::debug() << "Opened context with OpenGL Version " << (char *) glGetString(GL_VERSION) << Logger::endl;

	if (GLEW_VERSION_3_2)
	{
		GLint profile;

		// check if we have a core-profile
		glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
                /*if (profile == GL_CONTEXT_CORE_PROFILE_BIT)
			Logger::debug() << "got rendering context with core profile" << Logger::endl;
		else
			Logger::fatal() << "got rendering context with compatibility profile instead of core profile" << Logger::endl;
                        */
	}
	else
		Logger::fatal() << "OpenGL version 3.2 is needed but not supported" << Logger::endl;

	// Enable sRGB gamma correction for framebuffer output.
	glEnable(GL_FRAMEBUFFER_SRGB);

	//Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//Enable face culling (default: backface culling)
	glEnable(GL_CULL_FACE);

	glPolygonOffset(1.1f, 4.0f);

	//set viewport
	glViewport(0,0, m_iWidth, m_iHeight);

	//clear error status
	glGetError();

	m_spMainCamera = shared_ptr<Camera> (new Camera(glm::vec3(4,3,6), 210, -15, 45.0, 1.33, 0.1, 100.0));
	m_spDebugCamera = shared_ptr<Camera> (new Camera(glm::vec3(0,5,10), 180, 0, 45.0, 1.33, 0.1, 100.0));
	m_spLightCamera = shared_ptr<Camera> (new Camera(glm::vec3(0,6,0), 180, -90, 50.0, 1.0, 2.5, 10.0));

	Logger::debug() << "graphic initialized" << Logger::endl;

}


Graphic::~Graphic()
{
	//nothing to do so far
}

void Graphic::DrawRenderPath(std::string sRenderPath)
{
    static int fps=0;
    static double last_time;

    static int count=0;

    if (count==0)
	this->initSceneObjects();

    count++;
    fps++;

    m_spMainCamera->move();
    m_spDebugCamera->move();
    m_spLightCamera->move();

    //clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw the scene
    if (m_vRenderPaths.find(sRenderPath) != m_vRenderPaths.end())
	m_vRenderPaths[sRenderPath]->Render();
    else
	Logger::error() << "RenderPath " << sRenderPath << " does not exist!" << Logger::endl;


    // swap back and front buffers
    glfwSwapBuffers();

    if (glfwGetTime() - last_time >= 1.0)
    {
	last_time = glfwGetTime();
	stringstream puffer;
	string puffer2;
	puffer << fps;
	puffer >> puffer2;

	string puffer3 = string("Free Pool (NG) written by Christian Moellinger, FPS:") + puffer2;
	glfwSetWindowTitle(puffer3.data());
	fps=0;
    }


}

void Graphic::initSceneObjects()
{
	float fbo_bloom_factor = 0.5f;
	float fbo_antialias_factor = 1.0f;


	/************************************************************************/
	/* Test                                                                 */
	/************************************************************************/

	//SceneObject_AssimpImport *test_import = new SceneObject_AssimpImport("models/test.dae");
	shared_ptr<SceneObject_Camera> spCamera (new SceneObject_Camera(GetMainCamera()));
	shared_ptr<SceneObject_Camera> spCamera2 (new SceneObject_Camera(GetDebugCamera(), false));
	shared_ptr<SceneObject_AssimpImport> spTableObject (new SceneObject_AssimpImport("models/freepool-ng-table.3ds"));
	shared_ptr<SceneObject_VolumeVisRaycaster> spVolumeVisRaycaster (new SceneObject_VolumeVisRaycaster());	
	shared_ptr<SceneObject_FluidDynamics> spFluidDynamics (new SceneObject_FluidDynamics());
	shared_ptr<SceneObject_Sphere> spShadowSphere (new SceneObject_Sphere(1.0f, "sphere0"));

	spShadowSphere->SetTransformMatrix(glm::scale(0.1f, 0.1f, 0.1f) * glm::translate(0.0f, 20.0f, 0.0f));

	spVolumeVisRaycaster->SetTransformMatrix(glm::scale(0.5f, 0.5f, 0.5f) * glm::translate(0.0f, 2.0f, 0.0f));
	spFluidDynamics->SetTransformMatrix(glm::translate(4.0f, 1.3f, 0.0f));

	spTableObject->SetTransformMatrix(glm::translate(0.0f, -1.0f, 0.0f));

	/************************************************************************/
	/* FBOs                                                                 */
	/************************************************************************/
	//SceneObject_FBO *fbo_bloom_s1 = new SceneObject_FBO(m_iWidth * fbo_bloom_factor, m_iHeight * fbo_bloom_factor, "bloom-only");
	shared_ptr<SceneObject_FBO> fbo_bloom_s2 (new SceneObject_FBO(m_iWidth * fbo_bloom_factor, m_iHeight * fbo_bloom_factor, "bloom-blurred-vertical"));
	shared_ptr<SceneObject_FBO> fbo_bloom_s3 (new SceneObject_FBO(m_iWidth * fbo_bloom_factor, m_iHeight * fbo_bloom_factor, "bloom-blurred-horizontal"));
	shared_ptr<SceneObject_FBO> fbo_bloom_s4 (new SceneObject_FBO(m_iWidth * fbo_bloom_factor, m_iHeight * fbo_bloom_factor, "fbo_bloom"));
	shared_ptr<SceneObject_FBO> fbo_full_scene (new SceneObject_FBO(m_iWidth * fbo_antialias_factor, m_iHeight * fbo_antialias_factor, "fbo_scene", "fbo_scene_Z"));
	shared_ptr<SceneObject_FBO> fbo_bloom_done (new SceneObject_FBO(m_iWidth * fbo_antialias_factor, m_iHeight * fbo_antialias_factor, "fbo_bloom_done"));

	shared_ptr<SceneObject_FBO> fbo_deep_opycity_map_depth (new SceneObject_FBO(m_iWidth, m_iHeight, "fbo_deep_opacity_map_depth", true, false));
	shared_ptr<SceneObject_FBO> fbo_deep_opycity_map_opacities (new SceneObject_FBO(m_iWidth, m_iHeight, "fbo_deep_opacity_map_opacities", true, false));

	/************************************************************************/
	/* Test Objects                                                   */
	/************************************************************************/

	shared_ptr<SceneObject_Camera> test (new SceneObject_Camera(GetLightCamera()));

	/************************************************************************/
	/* Renderstate Objects                                                   */
	/************************************************************************/
	shared_ptr<SceneObject_RenderPass> lightascam (new SceneObject_RenderPass(SceneObject_RenderPass::RENDERPASS_SHADOWMAP));

	shared_ptr<SceneObject_RenderPass> deep_opacity_pass1 (new SceneObject_RenderPass(SceneObject_RenderPass::RENDERPASS_DEEP_OPACITY_MAP1));
	shared_ptr<SceneObject_RenderPass> deep_opacity_pass2 (new SceneObject_RenderPass(SceneObject_RenderPass::RENDERPASS_DEEP_OPACITY_MAP2));

	/************************************************************************/
	/* PostEffect Objects                                                   */
	/************************************************************************/
	shared_ptr<SceneObject> posteffect_bloom_s1 (new SceneObject_PostEffect("bloom-step1", "fbo_scene"));
	shared_ptr<SceneObject> posteffect_bloom_s2 (new SceneObject_PostEffect("bloom-step2", "bloom-blurred-vertical"));
	shared_ptr<SceneObject> posteffect_bloom_s3  (new SceneObject_PostEffect("bloom-step3", "bloom-blurred-horizontal"));
	shared_ptr<SceneObject> posteffect_bloom_final (new SceneObject_PostEffect("bloom-combine", "fbo_scene", "fbo_bloom", "fbo_scene_Z"));

	/************************************************************************/
	/* SceneObjects                                                         */
	/************************************************************************/
	shared_ptr<SceneObject> realRoot (new SceneObject_EmptyNode());
	shared_ptr<SceneObject> root (new SceneObject_EmptyNode());
	shared_ptr<SceneObject> scene_root (new SceneObject_EmptyNode());
	shared_ptr<SceneObject> lightSphere (new SceneObject_LightSphere());


	/************************************************************************/
	/* New SceneObjects                                                         */
	/************************************************************************/
	shared_ptr<SceneObject_Sphere> sphere0 (new SceneObject_Sphere(1.0f, "sphere0"));
	shared_ptr<SceneObject_Sphere> sphere1 (new SceneObject_Sphere(1.0f, "sphere1"));
	shared_ptr<SceneObject_Sphere> sphere2 (new SceneObject_Sphere(1.0f, "sphere2"));
	shared_ptr<SceneObject_Sphere> sphere3 (new SceneObject_Sphere(1.0f, "sphere3"));
	shared_ptr<SceneObject_Sphere> sphere4 (new SceneObject_Sphere(1.0f, "sphere4"));
	shared_ptr<SceneObject_Sphere> sphere5 (new SceneObject_Sphere(1.0f, "sphere5"));
	shared_ptr<SceneObject_Sphere> sphere6 (new SceneObject_Sphere(1.0f, "sphere6"));
	shared_ptr<SceneObject_Sphere> sphere7 (new SceneObject_Sphere(1.0f, "sphere7"));
	shared_ptr<SceneObject_Sphere> sphere8 (new SceneObject_Sphere(1.0f, "sphere8"));
	shared_ptr<SceneObject_Sphere> sphere9 (new SceneObject_Sphere(1.0f, "sphere9"));
	shared_ptr<SceneObject_Sphere> sphere10 (new SceneObject_Sphere(1.0f, "sphere10"));
	shared_ptr<SceneObject_Sphere> sphere11 (new SceneObject_Sphere(1.0f, "sphere11"));
	shared_ptr<SceneObject_Sphere> sphere12 (new SceneObject_Sphere(1.0f, "sphere12"));
	shared_ptr<SceneObject_Sphere> sphere13 (new SceneObject_Sphere(1.0f, "sphere13"));
	shared_ptr<SceneObject_Sphere> sphere14 (new SceneObject_Sphere(1.0f, "sphere14"));
	shared_ptr<SceneObject_Sphere> sphere15 (new SceneObject_Sphere(1.0f, "sphere15"));



	shared_ptr<SceneObject> scaledown (new SceneObject_EmptyNode());
	scaledown->SetTransformMatrix(glm::scale(0.2f, 0.2f, 0.2f));

	/************************************************************************/
	/* Fill Scenegraph                                                      */
	/************************************************************************/
	//realRoot->addChild(posteffect_bloom_final);
	//realRoot->addChild(fbo_light_z);

	//std::shared_ptr<SceneObject_FBO> copy_scene_z_FBO (new SceneObject_FBO(m_iWidth, m_iHeight, "dev/null", "fbo_scene_Z_copied"));
	//copy_scene_z_FBO->AddChild(scaledown);

	realRoot->AddChild(posteffect_bloom_final);
	//spCamera->AddChild(copy_scene_z_FBO);
	//realRoot->AddChild(posteffect_bloom_final);

		    fbo_bloom_done->AddChild(posteffect_bloom_final);

			posteffect_bloom_final->AddChild(fbo_full_scene); // before last step blooming is applied

				/*fbo_full_scene->AddChild(fbo_light_z); // render the whole scene into this FBO
				    fbo_light_z->AddChild(lightascam);
					lightascam->AddChild(test);
					    test->AddChild(scene_root);*/

				fbo_full_scene->AddChild(fbo_deep_opycity_map_depth);
				    fbo_deep_opycity_map_depth->AddChild(deep_opacity_pass1);
					deep_opacity_pass1->AddChild(test);
					//test->AddChild(scene_root);
                                        //test->AddChild(spVolumeVisRaycaster);
					test->AddChild(spShadowSphere);

				fbo_full_scene->AddChild(fbo_deep_opycity_map_opacities);
				    fbo_deep_opycity_map_opacities->AddChild(deep_opacity_pass2);
					deep_opacity_pass2->AddChild(test);


				fbo_full_scene->AddChild(root); // render the whole scene into this FBO

			posteffect_bloom_final->AddChild(fbo_bloom_s4);

				fbo_bloom_s4->AddChild(posteffect_bloom_s3);
				posteffect_bloom_s3->AddChild(fbo_bloom_s3);
				fbo_bloom_s3->AddChild(posteffect_bloom_s2);
				posteffect_bloom_s2->AddChild(fbo_bloom_s2);
				fbo_bloom_s2->AddChild(posteffect_bloom_s1);

			//root->AddChild(scene_root);
			    scene_root->AddChild(lightSphere);
			    scene_root->AddChild(scaledown);
                            //scene_root->AddChild(spFluidDynamics);
			    scene_root->AddChild(spShadowSphere);
			    scene_root->AddChild(scaledown);
                            //scene_root->AddChild(spVolumeVisRaycaster);


			    scaledown->AddChild(spTableObject);
			    scaledown->AddChild(sphere0);
			    scaledown->AddChild(sphere1);
			    scaledown->AddChild(sphere2);
			    scaledown->AddChild(sphere3);
			    scaledown->AddChild(sphere4);
			    scaledown->AddChild(sphere5);
			    scaledown->AddChild(sphere6);
			    scaledown->AddChild(sphere7);
			    scaledown->AddChild(sphere8);
			    scaledown->AddChild(sphere9);
			    scaledown->AddChild(sphere10);
			    scaledown->AddChild(sphere11);
			    scaledown->AddChild(sphere12);
			    scaledown->AddChild(sphere13);
			    scaledown->AddChild(sphere14);
			    scaledown->AddChild(sphere15);


    //spCamera->AddChild(realRoot);

    root->AddChild(spCamera);
    spCamera->AddChild(spCamera2);
    spCamera2->AddChild(scene_root);
    scene_root->SetLightSourceForShadowMapping(GetLightCamera());

    this->AddRenderPath(realRoot, "default");
    this->AddRenderPath(test, "no-effects");
    //this->AddRenderPath(spCamera, "no-effects");
}

std::shared_ptr<Camera> Graphic::GetMainCamera()
{
    return m_spMainCamera;
}

std::shared_ptr<Camera> Graphic::GetDebugCamera()
{
    return m_spDebugCamera;
}

void Graphic::AddRenderPath(std::shared_ptr<SceneObject> spRoot, std::string sRenderPath)
{
    this->m_vRenderPaths[sRenderPath] = spRoot;
}

std::shared_ptr<Camera> Graphic::GetLightCamera()
{
    return m_spLightCamera;
}

