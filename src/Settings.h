#include <map>
#include <string>

class Settings
{
public:
	static Settings& instance();


private:
	Settings();
	~Settings();
	Settings(const Settings&);

	void LoadSettings(std::string sFile);
};