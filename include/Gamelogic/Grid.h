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
		bool AddDoor(glm::vec2 doorPosition);
		bool AddDoor(int iDoorX, int iDoorY);

		void ClearGrid();

		std::vector<glm::vec2> GetDoorPositions();

		bool RotateGrid(const int iFactor);
	//@}


private:
	/*! \name Private members */
    //@{
        /// A vector which holds the door positions of a grid. Positions can go from
		/// (1,1) to (9,9). You have to consider that 2 doors need to have a distance
		/// of at least 1 square (example: (1,1) (3,1) is right; (1,1) (2,1) is wrong)
		std::vector<glm::vec2> m_vDoorPositions;
	//@}

	/*! \name Private methods */
	//@{
		bool CheckDoorInsertion(const int iX, const int iY);
	//@}
};

#endif