/*
 * header file for Grid class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * written by: florian spechtenhauser <florian.spechtenhauser@gmail.com>
 * 07/2011, Project Cube
 */

#pragma once
#ifndef __GRID_PROJECT_CUBE_HEADER
#define __GRID_PROJECT_CUBE_HEADER

#include <glm/glm.hpp>
#include <vector>

class Grid
{
public:
	/*! \name Construction / Destruction */
    //@{
        /// constructor
        Grid();

        /// destructor
        ~Grid();
    //@}

	/*! name Public methods */
	//@{
		///Adds a door at the given ivec2 position
		///returns true if successful, else false
		bool AddDoor(glm::ivec2 doorPosition);

		///Adds a door at the given position
		///returns true if successful, else false
		bool AddDoor(int iDoorX, int iDoorY);

		///Deletes all doors in the grid
		void ClearGrid();

		///Returns the doorpositions inside the grid
		std::vector<glm::ivec2> GetDoorPositions();

		///Rotates the grid about a factor
		///allowed are only 90, 180, 270,...
		bool RotateGrid(const int iFactor);

		///Mirrors a grid about its vertical axis
		void MirrorGridVertical();

		///Mirrors a grid about its horizontal axis
		void MirrorGridHorizontal();
	//@}


private:
	/*! \name Private members */
    //@{
        /// A vector which holds the door positions of a grid. Positions can go from
		/// (1,1) to (9,9). You have to consider that 2 doors need to have a distance
		/// of at least 1 square (example: (1,1) (3,1) is right; (1,1) (2,1) is wrong)
		std::vector<glm::ivec2> m_vDoorPositions;
	//@}

	/*! \name Private methods */
	//@{
		///Checks if a door can be inserted to the grid
		bool itlCheckDoorInsertion(const int iX, const int iY);
	//@}
};

#endif