#ifdef QT_OPENGL_LIB

#include "GL/glew.h"
#include "Graphic-QtWidgetWrapper.h"
#include "KeyIdentifiers.h"

#include <QMouseEvent>
#include <QCursor>

#include <iostream>

bool bMouseLocked = false;

namespace BambooGraphics
{
GraphicsCore::QtWidgetWrapper::QtWidgetWrapper()
{
    m_pGLWidget = NULL;
    m_iWidth = 0;
    m_iHeight = 0;
}

GraphicsCore::QtWidgetWrapper::~QtWidgetWrapper()
{
    // nothing to do so far
    delete m_pGLWidget;
}

std::shared_ptr<GraphicsCore::QtWidgetWrapper> GraphicsCore::QtWidgetWrapper::Create(QWidget *pWidget)
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

void GraphicsCore::QtWidgetWrapper::ClearBuffers()
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

void GraphicsCore::QtWidgetWrapper::SwapBuffers()
{
    assert (m_pGLWidget != NULL);

    m_pGLWidget->adjustSize();
    m_iWidth = m_pGLWidget->width();
    m_iHeight = m_pGLWidget->height();
    m_pGLWidget->swapBuffers();
    m_pGLWidget->makeCurrent();
}
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
    BambooGraphics::TMouseButton eButton = ItlTranslateQtButtonToBambooGraphicsButton(event->button());

    if (m_spInputEventListener)
    {
        if (eButton == BambooGraphics::BUTTON_LEFT)
            bMouseLocked = true;

        m_spInputEventListener->ItlHandleMouseButton(eButton, BambooGraphics::EVENT_PRESSED);
    }
}

void TGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    BambooGraphics::TMouseButton eButton = ItlTranslateQtButtonToBambooGraphicsButton(event->button());

    if (m_spInputEventListener)
    {
        if (eButton == BambooGraphics::BUTTON_LEFT)
            bMouseLocked = false;

        m_spInputEventListener->ItlHandleMouseButton(eButton, BambooGraphics::EVENT_RELEASED);
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
    BambooGraphics::TKey eKey = ItlTranslateQtKeyToBambooGraphicsKey(event->key());

    m_spInputEventListener->ItlHandleKeyboardEvent(eKey, BambooGraphics::EVENT_PRESSED);
}

void TGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    BambooGraphics::TKey eKey = ItlTranslateQtKeyToBambooGraphicsKey(event->key());

    m_spInputEventListener->ItlHandleKeyboardEvent(eKey, BambooGraphics::EVENT_RELEASED);
}

BambooGraphics::TKey TGLWidget::ItlTranslateQtKeyToBambooGraphicsKey(int iKey)
{
    BambooGraphics::TKey eKey = BambooGraphics::KEY_UNKNOWN;

    switch (iKey)
    {
    case Qt::Key_1: eKey = BambooGraphics::KEY_1; break;
    case Qt::Key_2: eKey = BambooGraphics::KEY_2; break;
    case Qt::Key_3: eKey = BambooGraphics::KEY_3; break;
    case Qt::Key_4: eKey = BambooGraphics::KEY_4; break;
    case Qt::Key_5: eKey = BambooGraphics::KEY_5; break;
    case Qt::Key_6: eKey = BambooGraphics::KEY_6; break;
    case Qt::Key_7: eKey = BambooGraphics::KEY_7; break;
    case Qt::Key_8: eKey = BambooGraphics::KEY_8; break;
    case Qt::Key_9: eKey = BambooGraphics::KEY_9; break;
    case Qt::Key_0: eKey = BambooGraphics::KEY_0; break;
    case Qt::Key_A: eKey = BambooGraphics::KEY_A; break;
    case Qt::Key_B: eKey = BambooGraphics::KEY_B; break;
    case Qt::Key_C: eKey = BambooGraphics::KEY_C; break;
    case Qt::Key_D: eKey = BambooGraphics::KEY_D; break;
    case Qt::Key_E: eKey = BambooGraphics::KEY_E; break;
    case Qt::Key_F: eKey = BambooGraphics::KEY_F; break;
    case Qt::Key_G: eKey = BambooGraphics::KEY_G; break;
    case Qt::Key_H: eKey = BambooGraphics::KEY_H; break;
    case Qt::Key_I: eKey = BambooGraphics::KEY_I; break;
    case Qt::Key_J: eKey = BambooGraphics::KEY_J; break;
    case Qt::Key_K: eKey = BambooGraphics::KEY_K; break;
    case Qt::Key_L: eKey = BambooGraphics::KEY_L; break;
    case Qt::Key_M: eKey = BambooGraphics::KEY_M; break;
    case Qt::Key_N: eKey = BambooGraphics::KEY_N; break;
    case Qt::Key_O: eKey = BambooGraphics::KEY_O; break;
    case Qt::Key_P: eKey = BambooGraphics::KEY_P; break;
    case Qt::Key_Q: eKey = BambooGraphics::KEY_Q; break;
    case Qt::Key_R: eKey = BambooGraphics::KEY_R; break;
    case Qt::Key_S: eKey = BambooGraphics::KEY_S; break;
    case Qt::Key_T: eKey = BambooGraphics::KEY_T; break;
    case Qt::Key_U: eKey = BambooGraphics::KEY_U; break;
    case Qt::Key_V: eKey = BambooGraphics::KEY_V; break;
    case Qt::Key_W: eKey = BambooGraphics::KEY_W; break;
    case Qt::Key_X: eKey = BambooGraphics::KEY_X; break;
    case Qt::Key_Y: eKey = BambooGraphics::KEY_Y; break;
    case Qt::Key_Z: eKey = BambooGraphics::KEY_Z; break;
    case Qt::Key_Space:     eKey = BambooGraphics::KEY_SPACE; break;
    case Qt::Key_Control:   eKey = BambooGraphics::KEY_CONTROL; break;
    case Qt::Key_Shift:     eKey = BambooGraphics::KEY_SHIFT; break;
    case Qt::Key_Tab:       eKey = BambooGraphics::KEY_TAB; break;
    case Qt::Key_Escape:    eKey = BambooGraphics::KEY_ESCAPE; break;
    }


    return eKey;
}

BambooGraphics::TMouseButton TGLWidget::ItlTranslateQtButtonToBambooGraphicsButton(int iButton)
{
    BambooGraphics::TMouseButton eButton = BambooGraphics::BUTTON_UNKNOWN;

    switch (iButton)
    {
    case Qt::LeftButton: eButton = BambooGraphics::BUTTON_LEFT; break;
    case Qt::MiddleButton: eButton = BambooGraphics::BUTTON_MIDDLE; break;
    case Qt::RightButton: eButton = BambooGraphics::BUTTON_RIGHT; break;
    }

   return eButton;
}

#endif






