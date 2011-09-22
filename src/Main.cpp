#include "MainApp.h"
#include "TestMain.h"
#include "Gamelogic/Level.h"

#include "EventManager.h"

int main()
{
    //EventManager test;
    //test.Test();

    TestMain::GetInstance()->Run();
    MainApp::GetInstance()->Run();
}
