#ifdef QT_OPENGL_LIB

#ifndef __GRAPHIC_QTWIDGETWRAPPER_HEADER
#define __GRAPHIC_QTWIDGETWRAPPER_HEADER

#include "Graphic.h"
#include <QtGui/QWidget>
#include <QtOpenGL>

class Bamboo::QtWidgetWrapper : public IRenderTarget
{

public:
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
        //void SetInputEventListener(std::shared_ptr<IInputEventListener> spListener) { m_spInputEventListener = spListener; }
        //void UnsetInputEventListener(std::shared_ptr<IInputEventListener> spListener) { assert(m_spInputEventListener == spListener); m_spInputEventListener = NULL; }
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
        //std::shared_ptr<Bamboo::IInputEventListener>    m_spInputEventListener;
        int                                             m_iWidth;
        int                                             m_iHeight;
        std::string                                     m_sWindowTitle;
        QGLWidget                                       *m_pGLWidget;
    //@}

    /*! \name Static members */
    //@{

    //@}
};

#endif // __GRAPHIC_QTWIDGETWRAPPER_HEADER

#endif //#ifdef QT_OPENGL_LIB
