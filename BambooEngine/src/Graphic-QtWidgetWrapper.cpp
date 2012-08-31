#ifdef QT_OPENGL_LIB

#include "GL/glew.h"
#include "Graphic-QtWidgetWrapper.h"

#include <QMouseEvent>
#include <QCursor>

#include <iostream>

bool bMouseLocked = false;

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

    TGLWidget *pGLWidget = new TGLWidget(fmt, pWidget);
    pGLWidget->makeCurrent();
    pGLWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    pGLWidget->setFixedWidth(1024);
    pGLWidget->setFixedHeight(768);
    pGLWidget->show();

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

TGLWidget::TGLWidget(const QGLFormat &fmt, QWidget *parent)
    : QGLWidget(fmt, parent)
{
    grabKeyboard();
}

TGLWidget::TGLWidget(QWidget *parent)
    : QGLWidget (parent)
{
    releaseKeyboard();
}


void TGLWidget::mousePressEvent(QMouseEvent *event)
{
    std::cout << "mouse press" << std::endl;

    if (m_spInputEventListener)
    {
        if (event->button() == Qt::LeftButton)
        {
            bMouseLocked = true;
            m_spInputEventListener->ItlHandleMouseButton(GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS);
        }
        else if (event->button() == Qt::MiddleButton)
            m_spInputEventListener->ItlHandleMouseButton(GLFW_MOUSE_BUTTON_MIDDLE, GLFW_PRESS);
        else if (event->button() == Qt::RightButton)
            m_spInputEventListener->ItlHandleMouseButton(GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS);
    }
}

void TGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    std::cout << "mouse release" << std::endl;

    if (m_spInputEventListener)
    {
        if (event->button() == Qt::LeftButton)
        {
            bMouseLocked = false;
            m_spInputEventListener->ItlHandleMouseButton(GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE);
        }
        else if (event->button() == Qt::MiddleButton)
            m_spInputEventListener->ItlHandleMouseButton(GLFW_MOUSE_BUTTON_MIDDLE, GLFW_RELEASE);
        else if (event->button() == Qt::RightButton)
            m_spInputEventListener->ItlHandleMouseButton(GLFW_MOUSE_BUTTON_RIGHT, GLFW_RELEASE);
    }
}

void TGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (bMouseLocked)
            QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));

    if (m_spInputEventListener)
        m_spInputEventListener->ItlHandleMousePos(event->x(), event->y());

}

void TGLWidget::keyPressEvent(QKeyEvent *event)
{
    std::cout << "key press" << std::endl;

    int iGLFWKey = GLFW_KEY_UNKNOWN;

    switch (event->key())
    {
    case Qt::Key_1: iGLFWKey = '1'; break;
    case Qt::Key_2: iGLFWKey = '2'; break;
    case Qt::Key_3: iGLFWKey = '3'; break;
    case Qt::Key_4: iGLFWKey = '4'; break;
    case Qt::Key_5: iGLFWKey = '5'; break;
    case Qt::Key_6: iGLFWKey = '6'; break;
    case Qt::Key_7: iGLFWKey = '7'; break;
    case Qt::Key_8: iGLFWKey = '8'; break;
    case Qt::Key_9: iGLFWKey = '9'; break;
    case Qt::Key_0: iGLFWKey = '0'; break;
    case Qt::Key_A: iGLFWKey = 'A'; break;
    case Qt::Key_B: iGLFWKey = 'B'; break;
    case Qt::Key_C: iGLFWKey = 'C'; break;
    case Qt::Key_D: iGLFWKey = 'D'; break;
    case Qt::Key_E: iGLFWKey = 'E'; break;
    case Qt::Key_F: iGLFWKey = 'F'; break;
    case Qt::Key_G: iGLFWKey = 'G'; break;
    case Qt::Key_H: iGLFWKey = 'H'; break;
    case Qt::Key_I: iGLFWKey = 'I'; break;
    case Qt::Key_J: iGLFWKey = 'J'; break;
    case Qt::Key_K: iGLFWKey = 'K'; break;
    case Qt::Key_L: iGLFWKey = 'L'; break;
    case Qt::Key_M: iGLFWKey = 'M'; break;
    case Qt::Key_N: iGLFWKey = 'N'; break;
    case Qt::Key_O: iGLFWKey = 'O'; break;
    case Qt::Key_P: iGLFWKey = 'P'; break;
    case Qt::Key_Q: iGLFWKey = 'Q'; break;
    case Qt::Key_R: iGLFWKey = 'R'; break;
    case Qt::Key_S: iGLFWKey = 'S'; break;
    case Qt::Key_T: iGLFWKey = 'T'; break;
    case Qt::Key_U: iGLFWKey = 'U'; break;
    case Qt::Key_V: iGLFWKey = 'V'; break;
    case Qt::Key_W: iGLFWKey = 'W'; break;
    case Qt::Key_X: iGLFWKey = 'X'; break;
    case Qt::Key_Y: iGLFWKey = 'Y'; break;
    case Qt::Key_Z: iGLFWKey = 'Z'; break;
    }

    m_spInputEventListener->ItlHandleKeyboardEvent(iGLFWKey, GLFW_PRESS);
}

void TGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    std::cout << "key release" << std::endl;

    int iGLFWKey = GLFW_KEY_UNKNOWN;

    switch (event->key())
    {
    case Qt::Key_1: iGLFWKey = '1'; break;
    case Qt::Key_2: iGLFWKey = '2'; break;
    case Qt::Key_3: iGLFWKey = '3'; break;
    case Qt::Key_4: iGLFWKey = '4'; break;
    case Qt::Key_5: iGLFWKey = '5'; break;
    case Qt::Key_6: iGLFWKey = '6'; break;
    case Qt::Key_7: iGLFWKey = '7'; break;
    case Qt::Key_8: iGLFWKey = '8'; break;
    case Qt::Key_9: iGLFWKey = '9'; break;
    case Qt::Key_0: iGLFWKey = '0'; break;
    case Qt::Key_A: iGLFWKey = 'A'; break;
    case Qt::Key_B: iGLFWKey = 'B'; break;
    case Qt::Key_C: iGLFWKey = 'C'; break;
    case Qt::Key_D: iGLFWKey = 'D'; break;
    case Qt::Key_E: iGLFWKey = 'E'; break;
    case Qt::Key_F: iGLFWKey = 'F'; break;
    case Qt::Key_G: iGLFWKey = 'G'; break;
    case Qt::Key_H: iGLFWKey = 'H'; break;
    case Qt::Key_I: iGLFWKey = 'I'; break;
    case Qt::Key_J: iGLFWKey = 'J'; break;
    case Qt::Key_K: iGLFWKey = 'K'; break;
    case Qt::Key_L: iGLFWKey = 'L'; break;
    case Qt::Key_M: iGLFWKey = 'M'; break;
    case Qt::Key_N: iGLFWKey = 'N'; break;
    case Qt::Key_O: iGLFWKey = 'O'; break;
    case Qt::Key_P: iGLFWKey = 'P'; break;
    case Qt::Key_Q: iGLFWKey = 'Q'; break;
    case Qt::Key_R: iGLFWKey = 'R'; break;
    case Qt::Key_S: iGLFWKey = 'S'; break;
    case Qt::Key_T: iGLFWKey = 'T'; break;
    case Qt::Key_U: iGLFWKey = 'U'; break;
    case Qt::Key_V: iGLFWKey = 'V'; break;
    case Qt::Key_W: iGLFWKey = 'W'; break;
    case Qt::Key_X: iGLFWKey = 'X'; break;
    case Qt::Key_Y: iGLFWKey = 'Y'; break;
    case Qt::Key_Z: iGLFWKey = 'Z'; break;
    }

    m_spInputEventListener->ItlHandleKeyboardEvent(iGLFWKey, GLFW_RELEASE);
}

#endif






