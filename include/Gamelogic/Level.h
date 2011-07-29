/*
 * header file for Level class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 *             florian spechtenhauser <florian.spechtenhauser@gmail.com>
 *
 * 07/2011, Project "Project Cube"
 */


#include "Gamelogic/ILevel.h"
#include "IXMLSerializeable.h"
#include "Gamelogic/Cube.h"
#include <vector>

class Level : public ILevel, public IXMLSerializeable
{
public:
	/*! \name Construction / Destruction */
    //@{
        /// constructor
		Level(std::string sFilename);
		/// destructor
		~Level();
	//@}

	/* \name Public methods */
	//@{
		///Rotates a level around its x-axis
		///allowed factors are 90, 180, 270, ...
		bool RotateX(const int iFactor);

		///Rotates a level around its y-axis
		///allowed factors are 90, 180, 270, ...
		bool RotateY(const int iFactor);

		///Rotates a level around its z-axis
		///allowed factors are 90, 180, 270, ...
		bool RotateZ(const int iFactor);

		///Writes the current level state to an xml-file
		virtual bool WriteToXMLString(std::string &rsString);

		///Reads a level from a xml-file
	    virtual bool ReadFromXMLString(std::string sString);
	//@}
private:
	/* \name Private members */
	//@{
		///Contains all cubes inside the level
		std::vector<Cube> m_Cubes;
	//@}
};