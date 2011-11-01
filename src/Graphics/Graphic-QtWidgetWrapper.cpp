#ifdef QT_OPENGL_LIB

#include "GL/glew.h"

#include "Graphics/Graphic-QtWidgetWrapper.h"


Bamboo::QtWidgetWrapper::QtWidgetWrapper()
{
    m_pGLWidget = NULL;
    m_iWidth = 0;
    m_iHeight = 0;
}

Bamboo::QtWidgetWrapper::~QtWidgetWrapper()
{
    // nothing to do so far
    delete m_pGLWidget;
}

std::shared_ptr<Bamboo::QtWidgetWrapper> Bamboo::QtWidgetWrapper::Create(QWidget *pWidget)
{
    std::shared_ptr<QtWidgetWrapper> spWrapper(new QtWidgetWrapper());
    QGLFormat fmt;
    fmt.setVersion(3,3);
    fmt.setProfile(QGLFormat::CoreProfile);

    QGLWidget *pGLWidget = new QGLWidget(fmt, pWidget);
    pGLWidget->makeCurrent();
    pGLWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    pGLWidget->setFixedWidth(512);
    pGLWidget->setFixedHeight(384);
    spWrapper->m_pGLWidget = pGLWidget;
    spWrapper->m_iWidth = pGLWidget->width();
    spWrapper->m_iHeight = pGLWidget->height();

    // start up GLEW

    static bool sbGlewInitialized = false;

    if (sbGlewInitialized == false)
    {
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
            assert(!"glew initialization failed");
        sbGlewInitialized = true;
    }

    return spWrapper;
}

void Bamboo::QtWidgetWrapper::ClearBuffers()
{
    m_pGLWidget->makeCurrent();

    // Enable sRGB gamma correction for framebuffer output.
    glEnable(GL_FRAMEBUFFER_SRGB);

    //Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //Enable face culling (default: backface culling)
    //glDisable(GL_CULL_FACE);
    glEnable(GL_CULL_FACE);

    glPolygonOffset(1.1f, 4.0f);

    //set viewport
    glViewport(0,0, m_iWidth, m_iHeight);

    //clear error status
    glGetError();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Bamboo::QtWidgetWrapper::SwapBuffers()
{
    assert (m_pGLWidget != NULL);

    m_pGLWidget->adjustSize();
    m_iWidth = m_pGLWidget->width();
    m_iHeight = m_pGLWidget->height();
    m_pGLWidget->swapBuffers();
    m_pGLWidget->makeCurrent();
}

#endif
