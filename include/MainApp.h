/*
 * header file for MainApp class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * written by: florian spechtenhauser <florian.spechtenhauser@gmail.com>
 * 07/2011, Project Cube
 */

#pragma once
#ifndef __MAINAPP_PROJECT_CUBE_HEADER
#define __MAINAPP_PROJECT_CUBE_HEADER

#include "Settings.h"
#include "Logger.h"
#include "Graphics/Graphic.h"

class MainApp
{
public:
    /*! \name Access to the subsystems */
    //@{
        // returns ptr to the graphic subsystem
        Graphic * GetGraphic();

        // returns ptr to the core settings
        Settings * GetCoreSettings();

        // dummy
        //GameLogic GetGameLogic();
    //@}

    /*! \name Run methods */
    //@{
        void Run();
    //@}

    /*! \name Get singelton instance */
    //@{
        static MainApp *GetInstance();
    //@}

private:
    /*! \name Construction / Destruction */
    //@{
        /// constructor
        MainApp();

        /// destructor
        ~MainApp();
    //@}

    /*! \name Private members */
    //@{
        Graphic m_Graphic;

        Settings m_CoreSettings;
    //@}
};

#endif
