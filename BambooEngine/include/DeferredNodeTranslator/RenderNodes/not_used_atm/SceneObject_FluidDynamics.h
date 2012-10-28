#ifndef SCENEOBJECT_FLUIDDYNAMICS_H
#define SCENEOBJECT_FLUIDDYNAMICS_H

#include "Graphics/SceneObject.h"
#include <memory>

class SceneObject_FBO;
class SceneObject_FluidDynamicsStep;

class SceneObject_FluidDynamics : public SceneObject
{
public:
    /*! \name Constructor / Destructor */
    //@{
    SceneObject_FluidDynamics();
    virtual ~SceneObject_FluidDynamics();
    //@}

protected:
    /*! \name SceneObject Interface */
    //@{
	/*! this method is called before the sceneobject itself gets rendered.
	    shaders and things like that should be activated in this method */
	virtual void ItlPreRender();

	/*! this method is called to render the sceneobject.
	    Attention: If the correct shader program is not bound yet (should be done in itlBeforeRender()),
	    the transform matrices must be sent again */
	virtual void ItlRender();

	/*! this method is called after rendering the sceneobject itself. Cleaning up can be done here */
	virtual void ItlPostRender();

	virtual void ItlPreRenderChildren();

	virtual void ItlPostRenderChildren() {};

	virtual bool ItlTestSkipRendering();


    //@}

private:
    /*! \name Private helper methods */
    //@{
	void CreateFluidPipeline();

    //@}

    /*! \name Private members */
    //@{
    std::shared_ptr<SceneObject_FluidDynamicsStep> m_spAddForces;

    std::shared_ptr<SceneObject_FBO> m_spInitVelocityFBO;
    std::shared_ptr<SceneObject_FBO> m_spInitTintFBO;

    bool m_bInitStepsAttached;

    GLuint m_nVertexArrayObject;	///< The opengl name (=unsigned int) of the vertex array object
    GLuint m_nVertexBufferObject;	///< The opengl name (=unsigned int) of the vertex buffer object
    GLuint m_nIndexBufferObject;	///< The opengl name (=unsigned int) of the index buffer object
    int m_iIndexArraySize;		///< The size of the index array

    GLint m_iCullMode;
    //@}
};

#endif // SCENEOBJECT_FLUIDDYNAMICS_H
