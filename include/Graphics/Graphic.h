/*
 * header file for Graphic class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 10/2010 Projekt "BambooIsland"
 * 03/2011 Projekt "Free Pool"
 */

#ifndef __realtime_lu_graphic
#define __realtime_lu_graphic

#include <memory>
#include <list>
#include <string>
#include <map>

class SceneObject;
class SceneObject_RenderTarget;
class Camera;

class Graphic
{
public:
    /*! \name Nested classes */
    //@{

    //@}



    void AddRenderPath(std::shared_ptr<SceneObject> spRoot, std::string sRenderPath);

    void DrawRenderPath(std::string sRenderPath);

    std::shared_ptr<Camera> GetMainCamera();
    std::shared_ptr<Camera> GetDebugCamera();
    std::shared_ptr<Camera> GetLightCamera();

    Graphic(int iWidth, int iHeight, bool bFullscreen);
    ~Graphic();

private:
    std::map<std::string, std::shared_ptr<SceneObject> >    m_vRenderPaths;

    std::list<std::shared_ptr<SceneObject> >	m_vSceneGraphs;
    std::shared_ptr<Camera>			m_spMainCamera;
    std::shared_ptr<Camera>			m_spDebugCamera;
    std::shared_ptr<Camera>			m_spLightCamera;



    int m_iWidth, m_iHeight;			///width and height of the graphic window

    void initSceneObjects();			///init sceneobject-structure -> TODO: think about a SceneObjectManager or something else for better handling
};

#endif
