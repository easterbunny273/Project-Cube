#include "MainApp.h"
#include "Gamelogic/Level.h"

int main()
{
	Level level("config/levels/level1.xml");
    MainApp::GetInstance()->Run();
}
