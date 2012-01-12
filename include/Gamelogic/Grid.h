/*
 * header file for Grid class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * written by: florian spechtenhauser <florian.spechtenhauser@gmail.com>
 * 07/2011, Project Cube
 */

#ifndef __GRID_PROJECTCUBE_HEADER
#define __GRID_PROJECTCUBE_HEADER

#include <glm/glm.hpp>
#include <vector>
#include <string>

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
        /// Adds a door at the given ivec2 position
        /// Allowed are only values between 1 and 9
        /// returns true if successful, else false
        bool AddDoor(glm::ivec2 doorPosition);

        /// Adds a door at the given position
        /// Allowed are only values between 1 and 9
        /// returns true if successful, else false
        bool AddDoor(int iDoorX, int iDoorY);

        /// Deletes all doors in the grid
        void ClearGrid();

        /// Returns the doorpositions inside the grid
        std::vector<glm::ivec2> GetDoorPositions();

        /// Rotates the grid about a factor
        /// allowed are only 90, 180, 270,...
        bool RotateGrid(const int iFactor);

        /// Mirrors a grid about its vertical axis
        void MirrorGridVertical();

        /// Mirrors a grid about its horizontal axis
        void MirrorGridHorizontal();

        /// Sets the position of the grid inside a cube as string
        void SetPosition(std::string sPosition);

        /// Returns the position inside a cube as string
        std::string GetPosition();

        /// Prints the doors of the grid to the output
        void PrintGrid();

		/// Calculates vertices of the grid for drawing
		std::vector<glm::ivec2> CalcVertices();
    //@}


private:
    /*! \name Private members */
    //@{
        /// A vector which holds the door positions of a grid. Positions can go from
        /// (1,1) to (9,9). You have to consider that 2 doors need to have a distance
        /// of at least 1 square (example: (1,1) (3,1) is right; (1,1) (2,1) is wrong)
        std::vector<glm::ivec2> m_vDoorPositions;

        /// The position of the Grid inside its Cube
        std::string m_sPosition;
    //@}

    /*! \name Private methods */
    //@{
        /// Checks if a given door can be inserted to the grid
        bool ItlCheckDoorInsertion(const int iX, const int iY);

		std::vector<glm::ivec2> ItlCalcVerticesRec(std::vector<glm::ivec2> doors, std::vector<glm::ivec2> vertices);
    //@}
};

#endif //__GRID_PROJECTCUBE_HEADER
