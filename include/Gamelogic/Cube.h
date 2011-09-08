/*
 * header file for Cube class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * written by: florian spechtenhauser <florian.spechtenhauser@gmail.com>
 * 07/2011, Project Cube
 */

#pragma once
#ifndef __CUBE_PROJECT_CUBE_HEADER
#define __CUBE_PROJECT_CUBE_HEADER

#include "Gamelogic/ICube.h"
#include "Gamelogic/Grid.h"

class Cube : public ICube
{
public:
	/*! \name Construction / Destruction */
    //@{
		// default constructor
		Cube();

        /// constructor
        Cube(unsigned int iCubeID, int iX, int iY, int iZ);

        /// destructor
        ~Cube();
    //@}

	/*! \name Public methods */
	//@{
		/// returns the id of the cube
		virtual unsigned int GetCubeID();

	    /// returns the position of the cube in the big cube matrix
	    virtual glm::ivec3 GetCubePosition();

	    /// returns the current transformation matrix of the object
	    virtual glm::mat4 GetTransformation();
		
		///Rotates the cube around its x-axis for the given factor
		bool RotateX(const int iFactor);

		///Rotates the cube around its y-axis for the given factor
		bool RotateY(const int iFactor);

		///Rotates the cube around its z-axis for the given factor
		bool RotateZ(const int iFactor);

		///Sets all 6 grids
		void SetGrids(Grid& xplus, Grid& xminus, Grid& yplus, Grid& yminus, Grid& zplus, Grid& zminus);
		
		void SetXplus(Grid& xplus);
		void SetXminus(Grid& xminus);
		void SetYplus(Grid& yplus);
		void SetYminus(Grid& yminus);
		void SetZplus(Grid& zplus);
		void SetZminus(Grid& zminus);
		
		///Returns the grid with the given id
		///1 = Xplus, 2 = Xminus, 3 = Yplus, 4 = Yminus, 5 = Zplus, 6 = Zminus
		///TODO: find a better solution
		Grid GetGrid(const unsigned int nID);

                int GetX();
                int GetY();
                int GetZ();

                void SetX(int iX);
                void SetY(int iY);
                void SetZ(int iZ);
	//@}

private:
	/*! \name Private members */
	//@{
		///Cube-ID
		unsigned int m_nCubeID;
		
		///Returns the relative position of the cube inside a level
		glm::ivec3 m_CubePosition;

		///Returns the transformation of a cube; not used yet
		glm::mat4 m_CubeTransformation;
		
		///Grid Xplus
		Grid m_Xplus;
		///Grid Xminus
		Grid m_Xminus;
		///Grid Yplus
		Grid m_Yplus;
		///Grid Yminus
		Grid m_Yminus;
		///Grid Zplus
		Grid m_Zplus;
		///Grid Zminus
		Grid m_Zminus;

		///Rotates the grid-order
		///grid1<-grid2<-grid3<-grid4<-grid1
		void itlSwitchGrids(Grid& grid1, Grid& grid2, Grid& grid3, Grid& grid4);
		
		///Switches two grids
		void itlSwitchGrids(Grid& grid1, Grid& grid2);

		///Updates the position-strings inside the grids
		void itlUpdateGridPositions();
	//@}
};

#endif //__GAMELOGIC_PROJECT_CUBE_HEADER
