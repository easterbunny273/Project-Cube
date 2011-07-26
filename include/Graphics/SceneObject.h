/*
 * header file for SceneObject class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 10/2010, Project "BambooIsland"
 * 03/2011, Project "Free Pool"
 */

#ifndef __realtime_sceneobject_header
#define __realtime_sceneobject_header

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "common_gl.h"


/**
  * A SceneObject is a object to be placed in a tree-based Scene-Structure
  *
*/

class TRenderPass;
class Camera;

class SceneObject
{
public:
    /*! \name Public types */
    //@{

    //@}

    /*! \name Constructor / Destructor */
    //@{
	/// Constructor
	SceneObject();

	/// Destructor
	virtual ~SceneObject();
    //@}

    /*! \name  Physics::TransformationChangeListener Interface */
    //@{
	/// sets the transformation matrix of the sphere
	virtual void setTransformMatrix(float *pfMatrix);
    //@}

    /*! \name Manipulate SceneGraph (Tree structure of SceneObjects) */
    //@{
	/*! adds a child to the node */
	void AddChild(std::shared_ptr<SceneObject> spNode);

	/*! removes the given child if found and returns true, else it returns false */
	bool RemoveChild(std::shared_ptr<SceneObject> spNode);

	void ClearChilds();
    //@}


    /*! \name Manipulating the transform matrices */
    //@{
	void SetTransformMatrix(glm::mat4 rToWorld, glm::mat4 rFromWorld);

	void SetTransformMatrix(glm::mat4 rToWorld);

	void GetTransformMatrix(glm::mat4 &rToWorld, glm::mat4 &rFromWorld) const;
    //@}

    /*! \name Operations */
    //@{
	//! renders the sceneobject and its children (first, it renders the children).
	/*! First, the itlTestSkipRendering() method is called, if false, itlBeforeRender() gets called.
	 *  Then, the render()-method of the children is called and finally the own itlRender()-method is called. */
	void Render();
    //@}


    void SetLightSourceForShadowMapping(std::shared_ptr<Camera> spShadowCaster, bool bSetForChildren = true);

    /*! \name Interfaces */
    //@{

	/*! \name Access to Vertices / Indices */
	class IHasVertices
	{
	public:
	    virtual float *GetVertices() = 0;
	    virtual int *GetIndices() = 0;
	    virtual int NumIndices() = 0;
	    virtual int NumVertices() = 0;
	};


    //@}

protected:
    /*! \name Internal types */
    //@{
	struct TItlRenderInfo
	{
	    glm::mat4 ProjectionMatrix;
	    glm::mat4 ViewMatrix;
	    glm::mat4 ModelMatrix;

	    glm::mat4 ModelViewProjectionMatrix;

	    glm::mat4 ModelViewProjectionMatrix_ForFrustumCulling;

	    glm::mat4 InverseModelViewProjectionMatrix;

	    int	tCurrentRenderPass;
	};
    //@}

    /*! \name Operations */
    //@{
	virtual void Render(std::shared_ptr<TItlRenderInfo> pRenderInfo);
    //@}

    /*! \name Methods which must be implemented by inherit classes */
    //@{
	/*! this method is called before the render() method calls the render() methods of the children,
	    and can be used to bind a fbo (to render the children in this fbo) or something like that */
	virtual void ItlPreRenderChildren() = 0;

	/*! this method is called after all children were rendered,
	    and can be used to unbind a fbo or something like that */
	virtual void ItlPostRenderChildren() = 0;

	/*! this method is called before the sceneobject itself gets rendered.
	    shaders and things like that should be activated in this method */
	virtual void ItlPreRender() = 0;

	/*! this method is called to render the sceneobject.
	    Attention: If the correct shader program is not bound yet (should be done in itlBeforeRender()),
	    the transform matrices must be sent again */
	virtual void ItlRender() = 0;

	/*! this method is called after rendering the sceneobject itself. Cleaning up can be done here */
	virtual void ItlPostRender() = 0;

	/*! this method should test if the object is visible (if not, it won't be drawn) */
	virtual bool ItlTestIfVisible();

	/*! this method should test if the rendering should be skipped because of the renderstate */
	virtual bool ItlTestSkipRendering();
    //@}



    /*! \name Internal helper methods */
    //@{
	void ItlSendTransformMatrices();		///<internal helper function which sends the current transormation matrices to the shader
	void ItlSendLightPosition();
    //@}

    /*! \name Internal members */
    //@{
	/// the transform matrix of this scene object for worldspace
	glm::mat4   m_mTransformMatrixToWorld;

	/// the transform matrix of this scene object for inverse worldspace
	glm::mat4   m_mTransformMatrixFromWorld;

	/// the complete transform matrix which was used at the last render call (from root to this)
	glm::mat4   m_mCompleteTransformMatrixToWorld;

	/// the complete transform matrix which was used at the last render call (from root to this)
	glm::mat4   m_mCompleteTransformMatrixFromWorld;

	std::shared_ptr<TItlRenderInfo> m_pCurrentRenderInfo;

	std::vector<std::shared_ptr<SceneObject> > m_vChildren;

	std::shared_ptr<Camera> m_spShadowCaster;

	bool m_bHasChildren;
    //@}

};

class SceneObject_BoundingBoxed : public SceneObject, public SceneObject::IHasVertices
{
    friend class SceneObject;

protected:
    float m_fMinX, m_fMaxX;
    float m_fMinY, m_fMaxY;
    float m_fMinZ, m_fMaxZ;
    bool m_bInitialized;

    //std::shared_ptr<SceneObject::IHasVertices> m_pSceneObject;

    /*! \name Constructor */
    //@{
	//BoundingBox(SceneObject::IHasVertices *pSceneObject) : m_pSceneObject(pSceneObject), m_bInitialized(false) {};
	SceneObject_BoundingBoxed() : m_bInitialized(false) {};
    //@}

    /*! \name Methods */
    //@{
	virtual bool ItlInitializeBoundingBox();

	virtual bool ItlTestIfVisible();

    //@}

};


#endif
