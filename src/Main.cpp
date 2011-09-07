#include "MainApp.h"
#include "TestMain.h"
#include "Gamelogic/Level.h"

int main()
{
    TestMain::GetInstance()->Run();
    MainApp::GetInstance()->Run();
}
