#ifndef BAMBOOENGINE_SETTINGS
#define BAMBOOENGINE_SETTINGS

#include <map>
#include <string>
#include <glm/glm.hpp>

class Settings
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

		void SetValue(std::string sName, int iValue);
		void SetValue(std::string sName, float fValue);
		void SetValue(std::string sName, const char *szValue);
		void SetValue(std::string sName, std::string sValue);
		void SetValue(std::string sName, bool bValue);
		void SetValue(std::string sName, glm::vec2 vValue);
		void SetValue(std::string sName, glm::vec3 vValue);
		void SetValue(std::string sName, glm::vec4 vValue);

		bool GetValue(std::string sName, int &iValue);
		bool GetValue(std::string sName, float &fValue);
		bool GetValue(std::string sName, std::string &sValue);
		bool GetValue(std::string sName, bool &bValue);
		bool GetValue(std::string sName, glm::vec2 &vValue);
		bool GetValue(std::string sName, glm::vec3 &vValue);
		bool GetValue(std::string sName, glm::vec4 &vValue);

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
