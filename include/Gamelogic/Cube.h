/*
 * header file for Cube class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * written by: florian spechtenhauser <florian.spechtenhauser@gmail.com>
 * 07/2011, Project Cube
 */

#pragma once
#ifndef __GAMELOGIC_PROJECT_CUBE_HEADER
#define __GAMELOGIC_PROJECT_CUBE_HEADER

#include "Gamelogic/ICube.h"
#include "GameLogic/Grid.h"

class Cube : public ICube
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
		/// returns the id of the cube
		virtual unsigned int GetCubeID();

	    /// returns the position of the cube in the big cube matrix
	    virtual glm::vec3 GetCubePosition();

	    /// returns the current transformation matrix of the object
	    virtual glm::mat4 GetTransformation();
		
		///Rotates the cube around its x-axis for the given factor
		bool RotateX(const int iFactor);

		///Rotates the cube around its y-axis for the given factor
		bool RotateY(const int iFactor);

		///Rotates the cube around its z-axis for the given factor
		bool RotateZ(const int iFactor);

		void SetGrids(Grid& xplus, Grid& xminus, Grid& yplus, Grid& yminus, Grid& zplus, Grid& zminus);
		
		Grid GetGrid(const unsigned int nID);
	//@}

private:
	/*! \name Private members */
	//@{
		unsigned int m_nCubeID;
		glm::vec3 m_CubePosition;
		glm::mat4 m_CubeTransformation;
		
		Grid m_Xplus;
		Grid m_Xminus;
		Grid m_Yplus;
		Grid m_Yminus;
		Grid m_Zplus;
		Grid m_Zminus;

		void itlSwitchGrids(Grid& grid1, Grid& grid2, Grid& grid3, Grid& grid4);
		void itlSwitchGrids(Grid& grid1, Grid& grid2);
	//@}
};

#endif //__GAMELOGIC_PROJECT_CUBE_HEADER