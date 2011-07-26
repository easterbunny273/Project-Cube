/*
 * header file for Cube class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * written by: florian spechtenhauser <florian.spechtenhauser@gmail.com>
 * 07/2011, Project Cube
 */

#pragma once
#ifndef __GAMELOGIC_PROJECT_CUBE_HEADER
#define __GAMELOGIC_PROJECT_CUBE_HEADER

#include "ICube.h"
#include "Grid.h"

class Cube : ICube
{
public:
	/*! \name Construction / Destruction */
    //@{
        /// constructor
        Cube();

        /// destructor
        ~Cube();
    //@}

	/*! \name Public methods */
	//@{
		///Rotates the cube around its x-axis for the given factor
		void RotateX(const int iFactor);

		///Rotates the cube around its y-axis for the given factor
		void RotateY(const int iFactor);

		///Rotates the cube around its z-axis for the given factor
		void RotateZ(const int iFactor);
	//@}

private:
	/*! \name Private members */
	//@{
		Grid m_Xplus;
		Grid m_Xminus;
		Grid m_Yplus;
		Grid m_Yminus;
		Grid m_Zplus;
		Grid m_Zminus;
	//@}
}

#endif //__GAMELOGIC_PROJECT_CUBE_HEADER