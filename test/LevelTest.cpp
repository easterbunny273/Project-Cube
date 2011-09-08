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
