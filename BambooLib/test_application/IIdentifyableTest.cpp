#include "IIdentifyableTest.h"
#include <cassert>

namespace BambooLib
{

    t_classID TestClass1::s_classID = CoreSystem::GetInstance()->RegisterClass("TESTCLA1", (void *) &TestClass1::CreateFromStream);
    t_classID TestClass2::s_classID = CoreSystem::GetInstance()->RegisterClass("TDSTCLA2", (void *) &TestClass2::CreateFromStream);

    void TestClass2::ItlWriteToStream(std::ostream &outStream, ISerializer *pSerializer) const
    {
        pSerializer->Serialize(outStream, m_sTestString);
        pSerializer->Serialize(outStream, (const IStreamable *) m_pOtherObject);
    }

    void TestClass2::ItlReadFromStream(std::istream &rInStream, ISerializer *pSerializer)
    {
        pSerializer->Unserialize(rInStream, m_sTestString);
        pSerializer->Unserialize(rInStream, (IStreamable **) &m_pOtherObject);
    }

    void TestClass2::SetOtherObject(IStreamable *pObject)
    {
        m_pOtherObject = pObject;
    }

    void TestClass1::ItlWriteToStream(std::ostream &outStream, ISerializer *pSerializer) const
    {
        pSerializer->Serialize(outStream, std::string("INSIDEMAN"));
    }

    void TestClass1::ItlReadFromStream(std::istream &inStream, ISerializer *pSerializer)
    {
        std::string sReadTest;

        pSerializer->Unserialize(inStream, sReadTest);

        assert (sReadTest == "INSIDEMAN");
    }

    TestClass1 *TestClass1::CreateFromStream(std::istream &rInStream, ISerializer *pSerializer, bool bReplaceObjectID /* = false */, bool bSkipClassID /*= false*/)
    {
        pSerializer->StartObjectReading();

        if (bSkipClassID == false)
        {
            t_classID nClassID;
            pSerializer->Unserialize(rInStream, (long long int &) nClassID);

            assert (nClassID == ClassID());
        }

        t_objectID nObjectID;
        pSerializer->Unserialize(rInStream, nObjectID);

        // overwrite object id with next free, if we should ignore the read object id
        if (bReplaceObjectID)
            nObjectID = CoreSystem::GetInstance()->GetNextFreeObjectID();

        TestClass1 *pNewObject = new TestClass1(nObjectID);
        pNewObject->ItlReadFromStream(rInStream, pSerializer);

        pSerializer->StopObjectReading(rInStream, bReplaceObjectID);

        return pNewObject;
    }

    TestClass2 *TestClass2::CreateFromStream(std::istream &rInStream, ISerializer *pSerializer, bool bReplaceObjectID /* = false */, bool bSkipClassID /*= false*/)
    {
        pSerializer->StartObjectReading();

        if (bSkipClassID == false)
        {
            t_classID nClassID;
            pSerializer->Unserialize(rInStream, (long long int &) nClassID);

            assert (nClassID == ClassID());
        }

        t_objectID nObjectID;
        pSerializer->Unserialize(rInStream, nObjectID);

        // overwrite object id with next free objectID
        if (bReplaceObjectID)
            nObjectID = CoreSystem::GetInstance()->GetNextFreeObjectID();

        TestClass2 *pNewObject = new TestClass2(nObjectID);
        pNewObject->ItlReadFromStream(rInStream, pSerializer);

        pSerializer->StopObjectReading(rInStream, bReplaceObjectID);

        return pNewObject;
    }

}
