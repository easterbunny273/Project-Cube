/*
 * header file for Settings class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 *             florian spechtenhauser <florian.spechtenhauser@gmail.com>
 *
 * 10/2010, Project "BambooIsland"
 * 03/2011, Project "Free Pool"
 * 07/2011, Project "Project Cube"
 */

#ifndef BAMBOOENGINE_SETTINGS
#define BAMBOOENGINE_SETTINGS

// Settings is a IXMLSerializeable-class
#include "IXMLSerializeable.h"

// stl includes
#include <map>
#include <string>

// other includes
#include <glm/glm.hpp>
#include <memory>

// forward declerations
class TiXmlElement;

class Settings : public IXMLSerializeable
{
public:
    /*! \name Forward declerations */
    //@{
	class TSettingsGroup;
    //@}

    /*! \name Nested classes */
    //@{
	class TSettingsGroup
	{
	    friend class Settings;

	public:
	    /*! \name Public methods */
	    //@{
                /// destructor
                ~TSettingsGroup();

		TSettingsGroup *GetGroup(std::string sName);

                void Clear();

                /// stores the given value
                void SetValue(std::string sName, int iValue);
		void SetValue(std::string sName, float fValue);
                void SetValue(std::string sName, const char *szValue);
                void SetValue(std::string sName, std::string sValue);
                void SetValue(std::string sName, bool bValue);
                void SetValue(std::string sName, glm::vec2 vValue);
                void SetValue(std::string sName, glm::vec3 vValue);
		void SetValue(std::string sName, glm::vec4 vValue);

                /// reads the wanted value to the given variable and returns true if successfull (if value was found)
                bool GetValue(std::string sName, int &iValue);
		bool GetValue(std::string sName, float &fValue);
		bool GetValue(std::string sName, std::string &sValue);
		bool GetValue(std::string sName, bool &bValue);
		bool GetValue(std::string sName, glm::vec2 &vValue);
		bool GetValue(std::string sName, glm::vec3 &vValue);
		bool GetValue(std::string sName, glm::vec4 &vValue);

		/// this method returns the value if it was found by reference, else the given default value
                /// the template creates an method GetValueOrDefault for each T,
                /// if a fitting GetValue(std::string, T) exists
		template<class T> bool GetValueOrDefault(std::string sName, T tDefault, T &tValue, bool bSetIfNotFound=true)
		{
		    bool bFound = GetValue(sName, tValue);

		    if (bFound == false)
		    {
			tValue = tDefault;

			if (bSetIfNotFound)
			    SetValue(sName, tDefault);
		    }

		    return bFound;
		}

		/// this method returns the value if it was found, else the given default value
		/// the template creates an method GetValueOrDefault for each T,
		/// if a fitting GetValue(std::string, T) exists
		template<class T> T GetValueOrDefault(std::string sName, T tDefault, bool bSetIfNotFound=true)
		{
		    T tValue;
		    bool bFound = GetValue(sName, tValue);

		    if (bFound == false)
		    {
			tValue = tDefault;

			if (bSetIfNotFound)
			    SetValue(sName, tDefault);
		    }

		    return tValue;
		}

	    //@}
	private:
	    /*! \name Construction / Destruction */
	    //@{
		/// private constructor, may only be created by settings class
		TSettingsGroup(std::string sName);
	    //@}

	    /*! \name Private helper methods */
	    //@{
                /// adds the xml node of one group to the given output stream
                void ItlWriteGroupAsXMLToStringStream(std::ostringstream &rssStream);

                /// reads a group from the given xml element
                bool ItlReadGroupFromXML(TiXmlElement *pElement);

                /// parses a value
                bool ItlParseValue(std::string sName, std::string sType, std::string sValue);

		/// this method checks if the given name is already used for a value
		bool ItlTestIfNameAlreadyUsed(std::string sName);
	    //@}

	    /*! \name Private members */
	    //@{
		std::string				m_sName;

                std::map<std::string, std::shared_ptr<TSettingsGroup> > m_mGroups;
		std::map<std::string, int>		m_mIntEntries;
		std::map<std::string, std::string>	m_mStringEntries;
		std::map<std::string, bool>		m_mBoolEntries;
		std::map<std::string, glm::vec4>	m_mVecEntries;
		std::map<std::string, float>		m_mFloatEntries;
	    //@}
	};
    //@}

    /*! \name Construction / Destruction */
    //@{
        /// constructor
        Settings();

        /// destructor
        ~Settings();
    //@}

    /*! \name methods of the IXMLSerializeable interface */
    //@{
        virtual bool WriteToXMLString(std::string &rsString);
        virtual bool ReadFromXMLString(std::string sString);
    //@}

    /*! \name Public methods */
    //@{
        /// clears all settings
        void Clear();

        /// writes the settings to the given file
        void StoreSettingsAsXMLFile(std::string sFilename);

        /// reads the settings from the given file
        bool RestoreSettingsFromXMLFile(std::string sFilename);

        /// returns a ptr to the given group and creates it, if it does not exist
        TSettingsGroup *GetGroup(std::string sName);
    //@}


private:
    /*! \name Internal methods */
    //@{
            TSettingsGroup *ItlGetMainGroup();
    //@}

    /*! \name Private members */
    //@{
        std::shared_ptr<TSettingsGroup> m_spMainGroup;
    //@}
};

#endif
