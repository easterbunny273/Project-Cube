#include <iostream>

using namespace std;


#include <ostream>
#include <istream>
#include <sstream>
#include <fstream>
#include <memory>

#include "assert.h"

#include "BambooLib/include/CoreSystem.h"
#include "BambooLib/include/IStreamable.h"
#include "BambooLib/include/Logger.h"
#include "BambooLib/include/BinarySerializer.h"
#include "IIdentifyableTest.h"

using namespace BambooLib;

int main()
{
   // std::ofstream Ftest("test.txt");

    TestClass1 *pTest = TestClass1::Create();

    TestClass1 *pTest2 = TestClass1::Cast(CoreSystem::GetInstance()->GetObjectForObjectID(pTest->GetObjectID()));

    assert (pTest2 == pTest);

    /*TestClass1 *pHallo1 = TestClass1::Create();
    TestClass2 *pHallo3 = TestClass2::Create("huhu");
    TestClass2 *pHallo2 = TestClass2::Create("hallo");

    pHallo2->SetOtherObject(pHallo3);
    pHallo3->SetOtherObject(pHallo3);

    t_objectID nObjectID1 = pHallo1->GetObjectID();
    t_objectID nObjectID2 = pHallo2->GetObjectID();

    TestClass1 *pTest1 = TestClass1::Cast(CoreSystem::GetInstance()->GetObjectForObjectID(nObjectID1));
    TestClass1 *pTest2 = TestClass1::Cast(CoreSystem::GetInstance()->GetObjectForObjectID(nObjectID2));*/

  /*  std::ofstream fTest("test1.txt");

    pHallo2->Store(fTest, new BinarySerializer());*/

   /*std::ifstream Ftest("test1.txt");

    TestClass2 *pHallo20 = TestClass2::CreateFromStream(Ftest, new BinarySerializer());

    {
    std::ofstream Ftest2("test2.txt");

    pHallo20->Store(Ftest2, new BinarySerializer());
    }

    std::ifstream FTest3("test2.txt");

    TestClass2 *pHallo30 = TestClass2::CreateFromStream(FTest3, new BinarySerializer(), true);
    return 0;
*/
    /*Test hallo;
    IIdentifyableClass *pTest = new Test();
    IIdentifyableClass *pTest2 = new Test3();
    IIdentifyableClass *pTest3 = new Test3();

    std::cout << hallo.m_tClassID << ":" << hallo.GetObjectCode() << std::endl;
    std::cout << pTest->m_tClassID << ":" << pTest->GetObjectCode() << std::endl;
    std::cout << pTest2->m_tClassID << ":" << pTest2->GetObjectCode() << std::endl;
    std::cout << pTest3->m_tClassID << ":" << pTest3->GetObjectCode() << std::endl;

    Test *pTestTT = Test::Cast(pTest);
    assert (pTestTT != NULL);

    Test2 *pTestTT2 = Test2::Cast(pTest);
    assert (pTestTT2 != NULL);
*/
   /* bool bTest1 = false;
    int iTest = 9;
    std::string sStringTest("hallihallO! das ist ein String\nTest mit Ümlauten");

    ISerializer *pSerializer = new BinarySerializer();

    pSerializer->Serialize(Ftest, bTest1);
    pSerializer->Serialize(Ftest, iTest);
    pSerializer->Serialize(Ftest, sStringTest);

    Ftest.close();

    bool bTest = true;
    int iTest1 = 9;
    std::string sStringTest1;

    std::ifstream Ftest2("test.txt");
    pSerializer->Unserialize(Ftest2, bTest);
    pSerializer->Unserialize(Ftest2, iTest1);
    pSerializer->Unserialize(Ftest2, sStringTest1);

    std::cout << bTest << std::endl;
    std::cout << iTest1 << std::endl;
    std::cout << sStringTest1 << std::endl;

    Ftest2.close();*/

 /* BambooLib::StreamableTest test;
  BambooLib::Logger test_logger;

  std::shared_ptr<BambooLib::Logger> testPtr(&test_logger);


  test.m_fTest1 = 99.9f;
  test.m_dTest2 = 1337.00001337;
  test.m_sTest3 = std::string("hallö!");

  test.WriteToFile("test3.txt");


  int i=4;
  int &test_i = i;

  BambooLib::StreamableTest test2;

  test2.RestoreFromFile("test3.txt");

//  cout << test2.m_dTest2 << ":" << test2.m_fTest1 << ":" << test2.m_sTest3 << endl;

  test_logger.LogMessage(Logger::ERROR, "hallo du!");

  Logger::ConsoleLogWriter *pConsoleLogWriter = Logger::ConsoleLogWriter::Create();
  Logger::FileLogWriter *pFileLogWriter = Logger::FileLogWriter::Create("test.log.txt");

  pConsoleLogWriter->SetIgnoreBelow(Logger::INFO);
  pFileLogWriter->SetIgnoreBelow(Logger::DEBUG);

 // Logger::RegisterLogWriter(pConsoleLogWriter);
  //Logger::RegisterLogWriter(pFileLogWriter);
  Logger::RegisterLogWriter(Logger::HTMLLogWriter::Create("test.log.html"));

  Logger::debug() << "hallo! - " << 5 << "#" << 3.4f << Logger::endl;
  Logger::error() << "error-test" << Logger::endl;
  Logger::warning() << "warning-test" << Logger::endl;
  Logger::warning() << "Hallo! Das ist eine ganz lange Fehlermeldung blablabalbalb" << Logger::endl;
  Logger::info() << "info-test" << Logger::endl;
  Logger::error() << "error-test 2!" << Logger::endl;
  Logger::fatal() << "fatal-test 2!" << Logger::endl;

  KeyValueMap test4;

  test4.SetValue("test", 9.9);
  test4.SetValue("test2", std::string("hallihällö!"));
  test4.SetValue("test5", std::string("äöü gugug blablabla \nhihihi hohoho Toll"));
  test4.SetValue("test3", 4.32f);

  int iTestValue = 0;
  float fTestValue = 0.0f;
  std::string sTestString = "";
  std::string sTestString2 = "";
  std::vector<int> vTestVector = { 2, 9, 1, 8, 8, 2, 7, 0, 3 };
  std::vector<int> vTestVector2;
  std::vector<std::string> vTestVectorString = { std::string("hallo!"), std::string("uhu!") };
  std::vector<std::string> vTestVectorString2;
  std::map<int, double> mTestMap = { std::pair<int, double>(99, 3.1415),
                                     std::pair<int, double>(91, 3.1416) };

  std::map<int, double> mTestMap2;

  test4.SetValue("test4", vTestVectorString);
  test4.SetValue("test9", mTestMap);
  test4.GetValue("test4", vTestVectorString2);
  test4.GetValue("test9", mTestMap2);

  test4.GetValue("test", iTestValue);
  test4.GetValue("test2", sTestString);
  test4.GetValue("test5", sTestString2);
  test4.GetValue("test3", fTestValue);


  std::cout << iTestValue << std::endl;
  std::cout << sTestString << std::endl;
  std::cout << sTestString2 << std::endl;*/

  return 0;
}

