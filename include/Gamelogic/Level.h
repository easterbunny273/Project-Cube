/*
 * header file for Level class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 *             florian spechtenhauser <florian.spechtenhauser@gmail.com>
 *
 * 07/2011, Project "Project Cube"
 */

#pragma once
#ifndef __LEVEL_PROJECTCUBE_HEADER
#define __LEVEL_PROJECTCUBE_HEADER

#include "tinyxml.h"
#include "tinystr.h"
#include "Gamelogic/ILevel.h"
#include "IXMLSerializeable.h"
#include "Gamelogic/Cube.h"
#include <vector>
#include "SemanticSceneNodes/ISemanticSceneNode.h"
#include "Camera.h"
#include "Gamelogic/IObject.h"
#include "Gamelogic/Objects/LightObject.h"
#include "Gamelogic/Objects/Object.h"


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

    /*! \name Public methods */
    //@{
        /// Returns the unique ID of the Level
        unsigned int GetLevelID();

        /// Returns the levelname of the level
        std::string GetLevelName();

        /// Returns the number of cubes that are in this level
        int GetNumCubes();

        /// Clears the level. Resets all data, i.e. deletes the cubes, resets level-id and name
        void Clear();

        /// Rotates a level around its x-axis
        /// allowed factors are 90, 180, 270, ...
        bool RotateX(const int iFactor);

        /// Rotates a level around its y-axis
        /// allowed factors are 90, 180, 270, ...
        bool RotateY(const int iFactor);

        /// Rotates a level around its z-axis
        /// allowed factors are 90, 180, 270, ...
        bool RotateZ(const int iFactor);

        /// Writes the level to the given file
        bool StoreLevelAsXMLFile(std::string sFilename);

        /// Loads the level from the given file
        bool LoadLevelFromXMLFile(std::string sFilename);

        /// Returns the vector of cubes
        std::vector<Cube*> GetCubes();

        /// Returns a cube by its givenposition inside the level
        /// Returns NULL if not no cube is at the given position
        Cube* GetCubeByPosition(glm::ivec3 iv3Position);

        /// Returns a cube by its given position inside the level
        /// Returns NULL if not no cube is at the given position
        Cube* GetCubeByPosition(int iX, int iY, int iZ);

		/// Adds a object to the level - and its semantic scene node to
		/// the level's node
		Object* CreateObject(std::string sName, std::string sFileName);

		/// Adds a light to the level - and its semantic scene node to the
		/// level's node
		LightObject* CreateLight(std::string sName);

		/// returns the object with the specified name
		Object* GetObjectByName(std::string sName);

		/// returns the light with the specified name
		LightObject* GetLightByName(std::string sName);

		/// returns the root semantic scene node of the level
		std::shared_ptr<ISemanticSceneNode> GetSemanticSceneNode()
		{
			return m_spSemanticScene;
		}

		/// returns the camera
		std::shared_ptr<Bamboo::ICamera> GetCamera()
		{
			return m_spCamera;
		}
    //@}
private:
    /*! \name Private members */
    //@{
        /// The level ID
        unsigned int m_nLevelID;

        /// The level name
        std::string m_sLevelName;

        /// Contains all cubes inside the level
        std::vector<Cube*> m_Cubes;

		/// Contains all objects inside the level
		std::vector<Object*> m_Objects;

		/// Contains all lights of the level
		std::vector<LightObject*> m_Lights;

        /// Number of cubes of the level
        int m_iNumCubes;

		/// Camera of this level
		std::shared_ptr<Bamboo::ICamera> m_spCamera;

		/// Pointer to the root of the semantic scene nodes
		std::shared_ptr<ISemanticSceneNode> m_spSemanticScene;
    //@}

    /*! \name Private methods */
    //@{
		/// Reads a level from an xml-string
        bool ReadFromXMLString(std::string sString);

        /// Writes the current level state to an xml-string
        bool WriteToXMLString(std::string &sString);

        /// Reads a group from xml
        bool itlReadGroupFromXML(TiXmlElement *pGroup);

        /// Reads all attributes that come from the group named attributes
        bool itlLoadAttributesFromXML(TiXmlElement *pAttribGroup);

        /// Loads all cubes from the cube-group of the xml string
        bool itlLoadCubesFromXML(TiXmlElement *pCubeGroup);

        /// Loads a Grid from a given xml element
        bool itlLoadGridFromXML(TiXmlElement *pGrid, Grid& grid);

        /// Adds a cube to the level (to the cube-vector)
        void itlAddCube(Cube* cube);

		/// Initializes the semantic scene root node - TODO initialize like now
		/// with camera, but specify a starting position for the actor - and 
		/// set camera parameters with that values
		void itlInitSemanticSceneNode();

    //@}
};

#endif //__LEVEL_PROJECTCUBE_HEADER
