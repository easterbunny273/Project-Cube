/*
 * header file for Cube class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * written by: florian spechtenhauser <florian.spechtenhauser@gmail.com>
 * 07/2011, Project Cube
 */

#pragma once
#ifndef __CUBE_PROJECT_CUBE_HEADER
#define __CUBE_PROJECT_CUBE_HEADER

#include "GeometryData.h"

#include "Gamelogic/ICube.h"
#include "Gamelogic/Grid.h"

class Cube : public ICube
{
public:
    /*! \name Construction / Destruction */
    //@{
        /// default constructor
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

        /// Rotates the cube around its x-axis for the given factor
        bool RotateX(const int iFactor);

        /// Rotates the cube around its y-axis for the given factor
        bool RotateY(const int iFactor);

        /// Rotates the cube around its z-axis for the given factor
        bool RotateZ(const int iFactor);

        /// Setter for all 6 grids
        void SetGrids(Grid& xplus, Grid& xminus, Grid& yplus, Grid& yminus, Grid& zplus, Grid& zminus);

        /// Setter for m_Xplus
        void SetXplus(Grid& xplus);
        /// Setter for m_Xminus
        void SetXminus(Grid& xminus);
        /// Setter for m_Yplus
        void SetYplus(Grid& yplus);
        /// Setter for m_Yminus
        void SetYminus(Grid& yminus);
        /// Setter for m_Zplus
        void SetZplus(Grid& zplus);
        /// Setter for m_Zminus
        void SetZminus(Grid& zminus);
		
        /// Returns the grid with the given id
        /// 1 = Xplus, 2 = Xminus, 3 = Yplus, 4 = Yminus, 5 = Zplus, 6 = Zminus
        /// TODO: find a better solution
        Grid GetGrid(const unsigned int nID);

		void BuildCubeVertices();
		std::vector<glm::vec3> GetVertices();
		std::vector<glm::vec3> GetNormals();
		std::vector<glm::vec2> GetTexCoords();

	std::shared_ptr<GeometryData::GenericObject> GenerateGenericObject();

        /// Getter for the x-coordinate of the position of the cube inside a level
        int GetX();
        /// Getter for the y-coordinate of the position of the cube inside a level
        int GetY();
        /// Getter for the z-coordinate of the position of the cube inside a level
        int GetZ();

        /// Setter for the x-coordinate of the position of the cube inside a level
        void SetX(int iX);
        /// Setter for the y-coordinate of the position of the cube inside a level
        void SetY(int iY);
        /// Setter for the z-coordinate of the position of the cube inside a level
        void SetZ(int iZ);
    //@}

private:
    /*! \name Private members */
    //@{
        /// Cube-ID
        unsigned int m_nCubeID;
		
        /// Relative position of the cube inside a level
        glm::ivec3 m_CubePosition;

        /// Transformation of a cube; not used yet
        glm::mat4 m_CubeTransformation;
		
		std::vector<glm::vec3> m_CubeVertices;
		std::vector<glm::vec3> m_CubeNormals;
		std::vector<glm::vec2> m_CubeTexCoords;
				
        /// Grid Xplus
        Grid m_Xplus;
        /// Grid Xminus
        Grid m_Xminus;
        /// Grid Yplus
        Grid m_Yplus;
        /// Grid Yminus
        Grid m_Yminus;
        /// Grid Zplus
        Grid m_Zplus;
        /// Grid Zminus
        Grid m_Zminus;

        /// Rotates the grid-order
        /// grid1<-grid2<-grid3<-grid4<-grid1
        void itlSwitchGrids(Grid& grid1, Grid& grid2, Grid& grid3, Grid& grid4);
		
        /// Switches two grids
        void itlSwitchGrids(Grid& grid1, Grid& grid2);

        /// Updates the position-strings inside the grids
        void itlUpdateGridPositions();
    //@}
};

#endif //__GAMELOGIC_PROJECT_CUBE_HEADER
