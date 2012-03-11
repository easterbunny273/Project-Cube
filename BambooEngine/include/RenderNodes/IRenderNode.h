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
#include <stack>
#include <memory>
#include <iostream>

#include "Graphic.h"
//#include "common_gl.h"


/**
  * A RenderNode is an object to be placed in a tree based render graph
  *
*/

class TRenderPass;

class Bamboo::IRenderNode
{
public:
    /*! \name Public types */
    //@{

    //@}

    /*! \name Constructor / Destructor */
    //@{
	/// Constructor
        IRenderNode();

	/// Destructor
        virtual ~IRenderNode();
    //@}

    /*! \name Manipulate SceneGraph (Tree structure of SceneObjects) */
    //@{
	/*! adds a child to the node */
        void AddChild(std::shared_ptr<IRenderNode> spNode);

	/*! removes the given child if found and returns true, else it returns false */
        bool RemoveChild(std::shared_ptr<IRenderNode> spNode);

	void ClearChilds();
    //@}


    /*! \name Manipulating the transform matrices */
    //@{
	void SetTransformMatrix(glm::mat4 rToWorld, glm::mat4 rFromWorld);

	void SetTransformMatrix(glm::mat4 rToWorld);

	void GetTransformMatrix(glm::mat4 &rToWorld, glm::mat4 &rFromWorld) const;
    //@}

    /*! \name Setting the graphic core (for using shader + texture manager */
    //@{
        void SetGraphicCore(Bamboo *pGraphicCore) { m_pGraphicCore = pGraphicCore; }
    //@}

    /*! \name Operations */
    //@{
	//! renders the sceneobject and its children (first, it renders the children).
	/*! First, the itlTestSkipRendering() method is called, if false, itlBeforeRender() gets called.
	 *  Then, the render()-method of the children is called and finally the own itlRender()-method is called. */
        void Render();

        void Render(IRenderNode *pStart);
    //@}


    void SetLightSourceForShadowMapping(std::shared_ptr<Bamboo::ICamera> spShadowCaster, bool bSetForChildren = true);

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

        void SetInitialViewportInformation(int iWidth, int iHeight) { assert(s_ViewportInformation.size() == 0); s_ViewportInformation.push(std::pair<int, int>(iWidth, iHeight));}
protected:
    /*! \name Internal types */
    //@{
        struct TItlRenderInfo
        {
            glm::mat4 ProjectionMatrix;
            glm::mat4 ViewMatrix;
            glm::mat4 TranslationMatrix;
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
        /// this method should load all necessary ressources
        //virtual void ItlLoadRessources() = 0;

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

        inline void ItlPushFBO(GLuint nFBO) { s_snBoundFBOs.push(nFBO); }
        inline void ItlPopFBO() { s_snBoundFBOs.pop(); }
        inline GLuint ItlGetTopFBO() { return s_snBoundFBOs.top(); }
        inline bool ItlIsNestedFBO () { return !s_snBoundFBOs.empty(); }

        inline void ItlPushViewportInformation(int iWidth, int iHeight) { s_ViewportInformation.push(std::pair<int, int>(iWidth, iHeight)); }
        inline void ItlPopViewportInformation() { s_ViewportInformation.pop(); }
        inline void ItlGetTopViewportInformation(int &riWidth, int &riHeight) { std::pair<int, int> info = s_ViewportInformation.top(); riWidth = info.first; riHeight = info.second; }



        Bamboo * ItlGetGraphicCore();
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

        std::vector<std::shared_ptr<Bamboo::IRenderNode> > m_vChildren;

        static std::stack<GLuint>                         s_snBoundFBOs;
        static std::stack<std::pair<int, int> >           s_ViewportInformation;

	bool m_bHasChildren;

        Bamboo         * m_pGraphicCore;
    //@}

};

class Bamboo::IRenderNode_Cullable : public Bamboo::IRenderNode, public Bamboo::IRenderNode::IHasVertices
{
    friend class Bamboo::IRenderNode;

protected:
    float m_fMinX, m_fMaxX;
    float m_fMinY, m_fMaxY;
    float m_fMinZ, m_fMaxZ;
    bool m_bInitialized;

    //std::shared_ptr<SceneObject::IHasVertices> m_pSceneObject;

    /*! \name Constructor */
    //@{
	//BoundingBox(SceneObject::IHasVertices *pSceneObject) : m_pSceneObject(pSceneObject), m_bInitialized(false) {};
        IRenderNode_Cullable() : m_bInitialized(false) {};
    //@}

    /*! \name Methods */
    //@{
	virtual bool ItlInitializeBoundingBox();

	virtual bool ItlTestIfVisible();

        //@}

};


#endif
