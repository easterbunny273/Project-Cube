#include "LevelTest.h"

void LevelTest::setup()
{

}

void LevelTest::tear_down()
{
}

void LevelTest::Test_loadXML_LevelAttributes()
{
        bool bRead = testlevel.LoadLevelFromXMLFile("test/config/levels/testlevel1.xml");

        TEST_ASSERT(bRead)
        TEST_ASSERT(testlevel.GetLevelID() == 1)
        TEST_ASSERT(testlevel.GetLevelName().compare("testlevel")==0)
        TEST_ASSERT(testlevel.GetNumCubes() == 2)
}

void LevelTest::Test_loadXML_Cubes()
{
    bool bRead = testlevel.LoadLevelFromXMLFile("test/config/levels/testlevel1.xml");

    Cube* cube1 = testlevel.GetCubeByPosition(0,0,0);
    Cube* cube2 = testlevel.GetCubeByPosition(0,1,0);

    TEST_ASSERT(bRead);

    //Test cube1
    TEST_ASSERT(cube1!=NULL);
    TEST_ASSERT(cube1->GetCubeID()==1);
    //Test grid Xplus
    TEST_ASSERT(cube1->GetGrid(1).GetDoorPositions().size()==1);
    TEST_ASSERT(cube1->GetGrid(1).GetDoorPositions().at(0).x==1);
    TEST_ASSERT(cube1->GetGrid(1).GetDoorPositions().at(0).y==1);
    //Test grid Xminus
    TEST_ASSERT(cube1->GetGrid(2).GetDoorPositions().size()==2);
    TEST_ASSERT(cube1->GetGrid(2).GetDoorPositions().at(0).x==2);
    TEST_ASSERT(cube1->GetGrid(2).GetDoorPositions().at(0).y==2);
    TEST_ASSERT(cube1->GetGrid(2).GetDoorPositions().at(1).x==2);
    TEST_ASSERT(cube1->GetGrid(2).GetDoorPositions().at(1).y==5);
    //Test grid Yplus
    TEST_ASSERT(cube1->GetGrid(3).GetDoorPositions().size()==3);
    TEST_ASSERT(cube1->GetGrid(3).GetDoorPositions().at(0).x==3);
    TEST_ASSERT(cube1->GetGrid(3).GetDoorPositions().at(0).y==3);
    TEST_ASSERT(cube1->GetGrid(3).GetDoorPositions().at(1).x==5);
    TEST_ASSERT(cube1->GetGrid(3).GetDoorPositions().at(1).y==5);
    TEST_ASSERT(cube1->GetGrid(3).GetDoorPositions().at(2).x==2);
    TEST_ASSERT(cube1->GetGrid(3).GetDoorPositions().at(2).y==7);
    //Test grid Yminus
    TEST_ASSERT(cube1->GetGrid(4).GetDoorPositions().size()==1);
    TEST_ASSERT(cube1->GetGrid(4).GetDoorPositions().at(0).x==3);
    TEST_ASSERT(cube1->GetGrid(4).GetDoorPositions().at(0).y==9);
    //Test grid Zplus
    TEST_ASSERT(cube1->GetGrid(5).GetDoorPositions().size()==2);
    TEST_ASSERT(cube1->GetGrid(5).GetDoorPositions().at(0).x==6);
    TEST_ASSERT(cube1->GetGrid(5).GetDoorPositions().at(0).y==8);
    TEST_ASSERT(cube1->GetGrid(5).GetDoorPositions().at(1).x==9);
    TEST_ASSERT(cube1->GetGrid(5).GetDoorPositions().at(1).y==1);
    //Test grid Zminus
    TEST_ASSERT(cube1->GetGrid(6).GetDoorPositions().size()==1);
    TEST_ASSERT(cube1->GetGrid(6).GetDoorPositions().at(0).x==6);
    TEST_ASSERT(cube1->GetGrid(6).GetDoorPositions().at(0).y==7);

    //Test cube2
    TEST_ASSERT(cube2!=NULL)
    TEST_ASSERT(cube2->GetCubeID()==2);
    //Test grid Xplus
    TEST_ASSERT(cube2->GetGrid(1).GetDoorPositions().size()==1);
    TEST_ASSERT(cube2->GetGrid(1).GetDoorPositions().at(0).x==5);
    TEST_ASSERT(cube2->GetGrid(1).GetDoorPositions().at(0).y==5);
    //Test grid Xminus
    TEST_ASSERT(cube2->GetGrid(2).GetDoorPositions().size()==0);
    //Test grid Yplus
    TEST_ASSERT(cube2->GetGrid(3).GetDoorPositions().size()==0);
    //Test grid Yminus
    TEST_ASSERT(cube2->GetGrid(4).GetDoorPositions().size()==1);
    TEST_ASSERT(cube2->GetGrid(4).GetDoorPositions().at(0).x==1);
    TEST_ASSERT(cube2->GetGrid(4).GetDoorPositions().at(0).y==1);
    //Test grid Zplus
    TEST_ASSERT(cube2->GetGrid(5).GetDoorPositions().size()==2);
    TEST_ASSERT(cube2->GetGrid(5).GetDoorPositions().at(0).x==7);
    TEST_ASSERT(cube2->GetGrid(5).GetDoorPositions().at(0).y==7);
    TEST_ASSERT(cube2->GetGrid(5).GetDoorPositions().at(1).x==2);
    TEST_ASSERT(cube2->GetGrid(5).GetDoorPositions().at(1).y==5);
    //Test grid Zminus
    TEST_ASSERT(cube2->GetGrid(6).GetDoorPositions().size()==1);
    TEST_ASSERT(cube2->GetGrid(6).GetDoorPositions().at(0).x==9);
    TEST_ASSERT(cube2->GetGrid(6).GetDoorPositions().at(0).y==9);
}

void LevelTest::Test_RotateLevelX90()
{
    bool bRead = testlevel.LoadLevelFromXMLFile("test/config/levels/testlevel2.xml");
    bool bOk = testlevel.RotateX(90);

    Cube* cube1 = testlevel.GetCubeByPosition(0,0,0);
    Cube* cube2 = testlevel.GetCubeByPosition(-1,0,2);
    Cube* cube3 = testlevel.GetCubeByPosition(-1,0,3);
    Cube* cube4 = testlevel.GetCubeByPosition(-1,1,3);
    Cube* cube5 = testlevel.GetCubeByPosition(0,0,2);
    Cube* cube6 = testlevel.GetCubeByPosition(0,0,1);
    Cube* cube7 = testlevel.GetCubeByPosition(-2,1,3);
    Cube* cube8 = testlevel.GetCubeByPosition(-2,2,3);
    Cube* cube9 = testlevel.GetCubeByPosition(2,5,4);

    TEST_ASSERT(bRead);
    TEST_ASSERT(bOk);
    TEST_ASSERT(cube1!=NULL);
    TEST_ASSERT(cube2!=NULL);
    TEST_ASSERT(cube3!=NULL);
    TEST_ASSERT(cube4!=NULL);
    TEST_ASSERT(cube5!=NULL);
    TEST_ASSERT(cube6!=NULL);
    TEST_ASSERT(cube7!=NULL);
    TEST_ASSERT(cube8!=NULL);
    TEST_ASSERT(cube9!=NULL);
}

void LevelTest::Test_RotateLevelX180()
{
    bool bRead = testlevel.LoadLevelFromXMLFile("test/config/levels/testlevel2.xml");
    bool bOk = testlevel.RotateX(180);
    Cube* cube1 = testlevel.GetCubeByPosition(0,0,0);
    Cube* cube2 = testlevel.GetCubeByPosition(-1,-2,0);
    Cube* cube3 = testlevel.GetCubeByPosition(-1,-3,0);
    Cube* cube4 = testlevel.GetCubeByPosition(-1,-3,1);
    Cube* cube5 = testlevel.GetCubeByPosition(0,-2,0);
    Cube* cube6 = testlevel.GetCubeByPosition(0,-1,0);
    Cube* cube7 = testlevel.GetCubeByPosition(-2,-3,1);
    Cube* cube8 = testlevel.GetCubeByPosition(-2,-3,2);
    Cube* cube9 = testlevel.GetCubeByPosition(2,-4,5);

    TEST_ASSERT(bRead);
    TEST_ASSERT(bOk);
    TEST_ASSERT(cube1!=NULL);
    TEST_ASSERT(cube2!=NULL);
    TEST_ASSERT(cube3!=NULL);
    TEST_ASSERT(cube4!=NULL);
    TEST_ASSERT(cube5!=NULL);
    TEST_ASSERT(cube6!=NULL);
    TEST_ASSERT(cube7!=NULL);
    TEST_ASSERT(cube8!=NULL);
    TEST_ASSERT(cube9!=NULL);
}

void LevelTest::Test_RotateLevelX270()
{
    bool bRead = testlevel.LoadLevelFromXMLFile("test/config/levels/testlevel2.xml");
    bool bOk = testlevel.RotateX(270);
    Cube* cube1 = testlevel.GetCubeByPosition(0,0,0);
    Cube* cube2 = testlevel.GetCubeByPosition(-1,0,-2);
    Cube* cube3 = testlevel.GetCubeByPosition(-1,0,-3);
    Cube* cube4 = testlevel.GetCubeByPosition(-1,-1,-3);
    Cube* cube5 = testlevel.GetCubeByPosition(0,0,-2);
    Cube* cube6 = testlevel.GetCubeByPosition(0,0,-1);
    Cube* cube7 = testlevel.GetCubeByPosition(-2,-1,-3);
    Cube* cube8 = testlevel.GetCubeByPosition(-2,-2,-3);
    Cube* cube9 = testlevel.GetCubeByPosition(2,-5,-4);

    TEST_ASSERT(bRead);
    TEST_ASSERT(bOk);
    TEST_ASSERT(cube1!=NULL);
    TEST_ASSERT(cube2!=NULL);
    TEST_ASSERT(cube3!=NULL);
    TEST_ASSERT(cube4!=NULL);
    TEST_ASSERT(cube5!=NULL);
    TEST_ASSERT(cube6!=NULL);
    TEST_ASSERT(cube7!=NULL);
    TEST_ASSERT(cube8!=NULL);
    TEST_ASSERT(cube9!=NULL);
}

void LevelTest::Test_RotateLevelY90()
{
    bool bRead = testlevel.LoadLevelFromXMLFile("test/config/levels/testlevel2.xml");
    bool bOk = testlevel.RotateY(90);
    Cube* cube1 = testlevel.GetCubeByPosition(0,0,0);
    Cube* cube2 = testlevel.GetCubeByPosition(0,2,1);
    Cube* cube3 = testlevel.GetCubeByPosition(0,3,1);
    Cube* cube4 = testlevel.GetCubeByPosition(-1,3,1);
    Cube* cube5 = testlevel.GetCubeByPosition(0,2,0);
    Cube* cube6 = testlevel.GetCubeByPosition(0,1,0);
    Cube* cube7 = testlevel.GetCubeByPosition(-1,3,2);
    Cube* cube8 = testlevel.GetCubeByPosition(-2,3,2);
    Cube* cube9 = testlevel.GetCubeByPosition(-5,4,-2);

    TEST_ASSERT(bRead);
    TEST_ASSERT(bOk);
    TEST_ASSERT(cube1!=NULL);
    TEST_ASSERT(cube2!=NULL);
    TEST_ASSERT(cube3!=NULL);
    TEST_ASSERT(cube4!=NULL);
    TEST_ASSERT(cube5!=NULL);
    TEST_ASSERT(cube6!=NULL);
    TEST_ASSERT(cube7!=NULL);
    TEST_ASSERT(cube8!=NULL);
    TEST_ASSERT(cube9!=NULL);
}

void LevelTest::Test_RotateLevelY180()
{
    bool bRead = testlevel.LoadLevelFromXMLFile("test/config/levels/testlevel2.xml");
    bool bOk = testlevel.RotateY(180);
    Cube* cube1 = testlevel.GetCubeByPosition(0,0,0);
    Cube* cube2 = testlevel.GetCubeByPosition(1,2,0);
    Cube* cube3 = testlevel.GetCubeByPosition(1,3,0);
    Cube* cube4 = testlevel.GetCubeByPosition(1,3,1);
    Cube* cube5 = testlevel.GetCubeByPosition(0,2,0);
    Cube* cube6 = testlevel.GetCubeByPosition(0,1,0);
    Cube* cube7 = testlevel.GetCubeByPosition(2,3,1);
    Cube* cube8 = testlevel.GetCubeByPosition(2,3,2);
    Cube* cube9 = testlevel.GetCubeByPosition(-2,4,5);

    TEST_ASSERT(bRead);
    TEST_ASSERT(bOk);
    TEST_ASSERT(cube1!=NULL);
    TEST_ASSERT(cube2!=NULL);
    TEST_ASSERT(cube3!=NULL);
    TEST_ASSERT(cube4!=NULL);
    TEST_ASSERT(cube5!=NULL);
    TEST_ASSERT(cube6!=NULL);
    TEST_ASSERT(cube7!=NULL);
    TEST_ASSERT(cube8!=NULL);
    TEST_ASSERT(cube9!=NULL);
}

void LevelTest::Test_RotateLevelY270()
{
    bool bRead = testlevel.LoadLevelFromXMLFile("test/config/levels/testlevel2.xml");
    bool bOk = testlevel.RotateY(270);
    Cube* cube1 = testlevel.GetCubeByPosition(0,0,0);
    Cube* cube2 = testlevel.GetCubeByPosition(0,2,-1);
    Cube* cube3 = testlevel.GetCubeByPosition(0,3,-1);
    Cube* cube4 = testlevel.GetCubeByPosition(1,3,-1);
    Cube* cube5 = testlevel.GetCubeByPosition(0,2,0);
    Cube* cube6 = testlevel.GetCubeByPosition(0,1,0);
    Cube* cube7 = testlevel.GetCubeByPosition(1,3,-2);
    Cube* cube8 = testlevel.GetCubeByPosition(2,3,-2);
    Cube* cube9 = testlevel.GetCubeByPosition(5,4,2);

    TEST_ASSERT(bRead);
    TEST_ASSERT(bOk);
    TEST_ASSERT(cube1!=NULL);
    TEST_ASSERT(cube2!=NULL);
    TEST_ASSERT(cube3!=NULL);
    TEST_ASSERT(cube4!=NULL);
    TEST_ASSERT(cube5!=NULL);
    TEST_ASSERT(cube6!=NULL);
    TEST_ASSERT(cube7!=NULL);
    TEST_ASSERT(cube8!=NULL);
    TEST_ASSERT(cube9!=NULL);
}

void LevelTest::Test_RotateLevelZ90()
{
    bool bRead = testlevel.LoadLevelFromXMLFile("test/config/levels/testlevel2.xml");
    bool bOk = testlevel.RotateZ(90);
    Cube* cube1 = testlevel.GetCubeByPosition(0,0,0);
    Cube* cube2 = testlevel.GetCubeByPosition(-2,-1,0);
    Cube* cube3 = testlevel.GetCubeByPosition(-3,-1,0);
    Cube* cube4 = testlevel.GetCubeByPosition(-3,-1,-1);
    Cube* cube5 = testlevel.GetCubeByPosition(-2,0,0);
    Cube* cube6 = testlevel.GetCubeByPosition(-1,0,0);
    Cube* cube7 = testlevel.GetCubeByPosition(-3,-2,-1);
    Cube* cube8 = testlevel.GetCubeByPosition(-3,-2,-2);
    Cube* cube9 = testlevel.GetCubeByPosition(-4,2,-5);

    TEST_ASSERT(bRead);
    TEST_ASSERT(bOk);
    TEST_ASSERT(cube1!=NULL);
    TEST_ASSERT(cube2!=NULL);
    TEST_ASSERT(cube3!=NULL);
    TEST_ASSERT(cube4!=NULL);
    TEST_ASSERT(cube5!=NULL);
    TEST_ASSERT(cube6!=NULL);
    TEST_ASSERT(cube7!=NULL);
    TEST_ASSERT(cube8!=NULL);
    TEST_ASSERT(cube9!=NULL);
}

void LevelTest::Test_RotateLevelZ180()
{
    bool bRead = testlevel.LoadLevelFromXMLFile("test/config/levels/testlevel2.xml");
    bool bOk = testlevel.RotateZ(180);
    Cube* cube1 = testlevel.GetCubeByPosition(0,0,0);
    Cube* cube2 = testlevel.GetCubeByPosition(1,-2,0);
    Cube* cube3 = testlevel.GetCubeByPosition(1,-3,0);
    Cube* cube4 = testlevel.GetCubeByPosition(1,-3,-1);
    Cube* cube5 = testlevel.GetCubeByPosition(0,-2,0);
    Cube* cube6 = testlevel.GetCubeByPosition(0,-1,0);
    Cube* cube7 = testlevel.GetCubeByPosition(2,-3,-1);
    Cube* cube8 = testlevel.GetCubeByPosition(2,-3,-2);
    Cube* cube9 = testlevel.GetCubeByPosition(-2,-4,-5);

    TEST_ASSERT(bRead);
    TEST_ASSERT(bOk);
    TEST_ASSERT(cube1!=NULL);
    TEST_ASSERT(cube2!=NULL);
    TEST_ASSERT(cube3!=NULL);
    TEST_ASSERT(cube4!=NULL);
    TEST_ASSERT(cube5!=NULL);
    TEST_ASSERT(cube6!=NULL);
    TEST_ASSERT(cube7!=NULL);
    TEST_ASSERT(cube8!=NULL);
    TEST_ASSERT(cube9!=NULL);
}

void LevelTest::Test_RotateLevelZ270()
{
    bool bRead = testlevel.LoadLevelFromXMLFile("test/config/levels/testlevel2.xml");
    bool bOk = testlevel.RotateZ(270);
    Cube* cube1 = testlevel.GetCubeByPosition(0,0,0);
    Cube* cube2 = testlevel.GetCubeByPosition(2,1,0);
    Cube* cube3 = testlevel.GetCubeByPosition(3,1,0);
    Cube* cube4 = testlevel.GetCubeByPosition(3,1,-1);
    Cube* cube5 = testlevel.GetCubeByPosition(2,0,0);
    Cube* cube6 = testlevel.GetCubeByPosition(1,0,0);
    Cube* cube7 = testlevel.GetCubeByPosition(3,2,-1);
    Cube* cube8 = testlevel.GetCubeByPosition(3,2,-2);
    Cube* cube9 = testlevel.GetCubeByPosition(4,-2,-5);

    TEST_ASSERT(bRead);
    TEST_ASSERT(bOk);
    TEST_ASSERT(cube1!=NULL);
    TEST_ASSERT(cube2!=NULL);
    TEST_ASSERT(cube3!=NULL);
    TEST_ASSERT(cube4!=NULL);
    TEST_ASSERT(cube5!=NULL);
    TEST_ASSERT(cube6!=NULL);
    TEST_ASSERT(cube7!=NULL);
    TEST_ASSERT(cube8!=NULL);
    TEST_ASSERT(cube9!=NULL);
}

void LevelTest::Test_writeXML()
{
        bool bRead = testlevel.LoadLevelFromXMLFile("test/config/levels/testlevel1.xml");
	std::string sFilename("test/config/levels/testlevel1_w.xml");
        bool bWrite = testlevel.StoreLevelAsXMLFile(sFilename);

        TEST_ASSERT(bRead==true)
        TEST_ASSERT(bWrite==true)
}

void LevelTest::Test_loadXML_NotExistingLevel()
{
        bool bRead = testlevel.LoadLevelFromXMLFile("test/config/levels/abasobaba.xml");
	TEST_ASSERT(bRead==false)
}

void LevelTest::Test_loadXML_WrongGroupTag()
{
        bool bRead = testlevel.LoadLevelFromXMLFile("test/config/levels/LevelWrongGroupTag.xml");
	TEST_ASSERT(bRead==false)
}

void LevelTest::Test_loadXML_WrongCubeTag()
{
        bool bRead = testlevel.LoadLevelFromXMLFile("test/config/levels/LevelWrongCubeTag.xml");
	TEST_ASSERT(bRead==false)
}

void LevelTest::Test_loadXML_WrongGridTag()
{
        bool bRead = testlevel.LoadLevelFromXMLFile("test/config/levels/LevelWrongGridTag.xml");
	TEST_ASSERT(bRead==false)
}

void LevelTest::Test_loadXML_WrongDoorTag()
{
        bool bRead = testlevel.LoadLevelFromXMLFile("test/config/levels/LevelWrongDoorTag.xml");
	TEST_ASSERT(bRead==false)
}

void LevelTest::Test_loadXML_WrongDoorRange()
{
        bool bRead = testlevel.LoadLevelFromXMLFile("test/config/levels/LevelWrongDoorRange.xml");
	TEST_ASSERT(bRead==false)
}

void LevelTest::Test_NotExistingCubeInLevel()
{
    bool bRead = testlevel.LoadLevelFromXMLFile("test/config/levels/testlevel1.xml");
    Cube* cube = testlevel.GetCubeByPosition(5,5,5);

    TEST_ASSERT(bRead==true);
    TEST_ASSERT(cube==NULL);
}
