#ifdef QT_OPENGL_LIB

#ifndef __GRAPHIC_QTWIDGETWRAPPER_HEADER
#define __GRAPHIC_QTWIDGETWRAPPER_HEADER

#include "Graphic.h"

#include <QtOpenGL/QGLWidget>
//#include <QtGui/QWidget>

class TGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    TGLWidget(const QGLFormat &fmt, QWidget *parent = NULL);
    TGLWidget(QWidget *parent = NULL);
    ~TGLWidget() {}

    void SetInputEventListener(std::shared_ptr<BambooGraphics::GraphicsCore::IRenderTarget::IInputEventListener> spListener) { m_spInputEventListener = spListener; }
    void UnsetInputEventListener(std::shared_ptr<BambooGraphics::GraphicsCore::IRenderTarget::IInputEventListener> spListener) { assert(m_spInputEventListener == spListener); m_spInputEventListener = NULL; }

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    std::shared_ptr<BambooGraphics::GraphicsCore::IRenderTarget::IInputEventListener>            m_spInputEventListener;
};

namespace BambooGraphics
{
class GraphicsCore::QtWidgetWrapper : public IRenderTarget
{

public:
    /*! \name Own types */
    //@{


    //@}

    /*! \name Construction / Destruction */
    //@{
        static std::shared_ptr<QtWidgetWrapper> Create(QWidget *pWidget);
        virtual ~QtWidgetWrapper();
    //@}

    /*! \name Implementing the IRenderTarget interface */
    //@{
        virtual void ClearBuffers();
        virtual void SwapBuffers();
    //@}

    /*! \name Methods for input event handling */
    //@{
        void SetInputEventListener(std::shared_ptr<IInputEventListener> spListener) { m_pGLWidget->SetInputEventListener(spListener); }
        void UnsetInputEventListener(std::shared_ptr<IInputEventListener> spListener) { m_pGLWidget->UnsetInputEventListener(spListener); }
    //@}

private:
    /*! \name Construction / Destruction */
    //@{
        /// private constructor (only the static method Create() is allowed to create a new instance)
        QtWidgetWrapper();
    //@}

    /*! \name Static helper methods */
    //@{

    //@}

    /*! \name Private members */
    //@{
        int                                             m_iWidth;
        int                                             m_iHeight;
        std::string                                     m_sWindowTitle;
        TGLWidget                                       *m_pGLWidget;
    //@}

    /*! \name Static members */
    //@{

    //@}
};

}

#endif // __GRAPHIC_QTWIDGETWRAPPER_HEADER

#endif //#ifdef QT_OPENGL_LIB
