/*
 * header file for Settings class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 10/2010, Project "BambooIsland"
 * 03/2011, Project "Free Pool"
 * 07/2011, Project "Project Cube"
 */

#ifndef BAMBOOENGINE_SETTINGS
#define BAMBOOENGINE_SETTINGS

// Settings is a IXMLSerializeable-class
#include "IXMLSerializeable.h"

#include <map>
#include <string>
#include <glm/glm.hpp>

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
		TSettingsGroup *GetGroup(std::string sName);

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

                /// this method returns the value if it was found, else the given default value
                /// the template creates an method GetValueOrDefault for each T,
                /// if a fitting GetValue(std::string, T) exists
		template<class T> bool GetValueOrDefault(std::string sName, T tDefault, T &tValue)
		{
		    bool bFound = GetValue(sName, tValue);

		    if (bFound == false)
			tValue = tDefault;

		    return bFound;
		};
	    //@}
	private:
	    /*! \name Construction / Destruction */
	    //@{
		/// private constructor, may only be created by settings class
		TSettingsGroup(std::string sName);
	    //@}

	    /*! \name Private helper methods */
	    //@{
		/// this method checks if the given name is already used for a value
		bool ItlTestIfNameAlreadyUsed(std::string sName);
	    //@}

	    /*! \name Private members */
	    //@{
		std::string				m_sName;

		std::map<std::string, TSettingsGroup *> m_mGroups;
		std::map<std::string, int>		m_mIntEntries;
		std::map<std::string, std::string>	m_mStringEntries;
		std::map<std::string, bool>		m_mBoolEntries;
		std::map<std::string, glm::vec4>	m_mVecEntries;
		std::map<std::string, float>		m_mFloatEntries;
	    //@}
	};
    //@}

    /*! \name methods of the IXMLSerializeable interface */
    //@{
        virtual bool WriteToXMLString(std::string &rsString);
        virtual bool ReadFromXMLString(std::string sString);
    //@}

    /*! \name Public methods */
    //@{
	/// returns the singelton instance
	static Settings *instance();

	TSettingsGroup *GetMainGroup() { return &m_rMainGroup; }

	TSettingsGroup *GetGroup(std::string sName) { return m_rMainGroup.GetGroup(sName); }
    //@}


private:
    /*! \name Construction / Destruction */
    //@{
	/// constructor
	Settings();

	/// destructor
	~Settings();
    //@}

    /*! \name Private members */
    //@{
	TSettingsGroup m_rMainGroup;
    //@}
};

#endif
