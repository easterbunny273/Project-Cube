/*
 * header file for SceneObject_AssimpImport class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 03 / 2011
 */

#ifndef __SceneObject_AssimpImport
#define __SceneObject_AssimpImport

#include <string>
#include <vector>

#include "Graphics/RenderNode.h"

//!  A SceneObject which renders a model which is loaded from file with Assimp.
/*!
    This SceneObject renders a model and uses the library Assimp to import it from a file.
    Theoretically, it should work with any model format which is supported by Assimp -
    but in practice, a 3ds-file exported from blender works best at the moment.
*/

class SceneObject_AssimpImport : public SceneObject_BoundingBoxed
{
public:
    /*! \name Constructor / Destructor */
    //@{
	/// constructor, creates scene object and imports model from given file
	SceneObject_AssimpImport(std::string sFilename);
    //@}

    /*! \name SceneObject::IHasVertices interface */
    //@{
	float *GetVertices();
	int *GetIndices();
	int NumIndices();
	int NumVertices();
    //@}

    /*! \name Some kind of an interface for bounding boxes - TODO: generalize and comment */
    //@{
	float CalculateBoundingRadius();
	void CalculateBoundingBox(float &fMinX, float &fMaxX, float &fMinY, float &fMaxY, float &fMinZ, float &fMaxZ);

	float m_fMinX, m_fMaxX;
	float m_fMinY, m_fMaxY;
	float m_fMinZ, m_fMaxZ;

    //@}

protected:


    /*! \name SceneObject Interface */
    //@{
	/*! this method is called before the render() method calls the render() methods of the children,
	    and can be used to bind a fbo (to render the children in this fbo) or something like that */
	virtual void ItlPreRenderChildren();

	/*! this method is called after all children were rendered,
	    and can be used to unbind a fbo or something like that */
	virtual void ItlPostRenderChildren();

	/*! this method is called before the sceneobject itself gets rendered.
	    shaders and things like that should be activated in this method */
	virtual void ItlPreRender();

	/*! this method is called to render the sceneobject.
	    Attention: If the correct shader program is not bound yet (should be done in itlBeforeRender()),
	    the transform matrices must be sent again */
	virtual void ItlRender();

	/*! this method is called after rendering the sceneobject itself. Cleaning up can be done here */
	virtual void ItlPostRender();

	virtual bool ItlTestSkipRendering();
	//virtual bool ItlTestIfVisible();
    //@}

private:
    /*! \name Internal types */
    //@{
	//only forward declarations
	struct TItlMeshData;
	struct TItlMaterialData;
    //@}

    /*! \name Internal helper methods */
    //@{
	static void ItlLoadGeneralRessources();
    //@}

    /*! \name Internal helper methods */
    //@{
	void ItlCreateVertexBufferObject(std::vector<GLfloat> &data);
	void ItlCreateIndexBufferObject(std::vector<GLuint> &data);

	void ItlCreateVerticesArray(std::vector<GLfloat> &data);
	void ItlCreateIndicesArray(std::vector<GLuint> &data);
    //@}

    /*! \name Private members */
    //@{
	GLuint m_nVAO;
	GLuint m_nVBO;
	GLuint m_nIBO;

	unsigned int m_nNumMeshes;

	std::vector<TItlMeshData *> m_vMeshData;
	std::vector<TItlMaterialData *> m_vMaterialData;

	float *m_pfVertices;
	int *m_piIndices;

	int m_iNumVertices;
	int m_iNumIndices;
    //@}

    /*! \name Static members */
    //@{
	static bool s_bGeneralRessourcesInitialized;	    ///< whether the general ressources like shaders are already loaded
    //@}


};

#endif

