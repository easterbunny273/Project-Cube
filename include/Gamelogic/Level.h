/*
 * header file for Level class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 *             florian spechtenhauser <florian.spechtenhauser@gmail.com>
 *
 * 07/2011, Project "Project Cube"
 */

#include "tinyxml.h"
#include "tinystr.h"
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
		Level();
		/// destructor
		~Level();
	//@}

	/* \name Public methods */
	//@{
		unsigned int GetLevelID();

		std::string GetLevelName();

		int GetNumCubes();

		void Clear();

		///Rotates a level around its x-axis
		///allowed factors are 90, 180, 270, ...
		bool RotateX(const int iFactor);

		///Rotates a level around its y-axis
		///allowed factors are 90, 180, 270, ...
		bool RotateY(const int iFactor);

		///Rotates a level around its z-axis
		///allowed factors are 90, 180, 270, ...
		bool RotateZ(const int iFactor);

		///Reads a level from a xml-file
	    virtual bool ReadFromXMLString(std::string sString);

		///Writes the current level state to an xml-file
		virtual bool WriteToXMLString(std::string &rsString);
	//@}
private:
	/* \name Private members */
	//@{
		unsigned int m_nLevelID;
		std::string m_sLevelName;
		std::string m_sFileName;

		///Contains all cubes inside the level
		std::vector<Cube> m_Cubes;

		int m_iNumCubes;
	//@}

	/* \name Private methods */
	//@{
		bool itlReadGroupFromXML(TiXmlElement *pGroup);

		bool itlLoadAttributesFromXML(TiXmlElement *pAttribGroup);
		bool itlLoadCubesFromXML(TiXmlElement *pCubeGroup);
		bool itlLoadGridFromXML(TiXmlElement *pGrid, Grid& grid);

		void itlAddCube(Cube& cube);
		

	//@}
};