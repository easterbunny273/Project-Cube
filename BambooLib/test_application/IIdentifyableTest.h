#ifndef IIDENTIFYABLETEST_H
#define IIDENTIFYABLETEST_H

#include <iostream>
#include <vector>
#include <typeinfo>
#include <memory>

#include "BambooLib/include/IStreamable.h"
#include "BambooLib/include/ISerializer.h"

namespace BambooLib
{
    class TestClass1 : public IStreamable, virtual public IIdentifyable
    {
    private:
        TestClass1() : IIdentifyable(ClassID()), IStreamable(s_classID) {}
        TestClass1(t_objectID nObjectID) : IIdentifyable(ClassID(), nObjectID), IStreamable(s_classID, nObjectID) {}

        static t_classID s_classID;

        virtual void ItlWriteToStream(std::ostream &outStream, ISerializer *pSerializer) const;
        virtual void ItlReadFromStream(std::istream &inStream, ISerializer *pSerializer);
    public:

        static TestClass1 * Cast(IIdentifyable *pObject) { return dynamic_cast<TestClass1 *>(pObject); }

        static t_classID ClassID() { return s_classID; }

        static TestClass1 * CreateFromStream(std::istream &rInStream, ISerializer *pSerializer, bool bReplaceObjectID = false, bool bSkipClassID = false);

        void SomethingSpecial()
        {
            std::cout << "hallO!" << std::endl;
        }

        static TestClass1 * Create()
        {
            return new TestClass1;
        }
    };

    class TestClass2 : public IStreamable, virtual public IIdentifyable
    {
    private:
        TestClass2() : IStreamable(s_classID), IIdentifyable(ClassID()), m_pOtherObject(nullptr) {}
        TestClass2(t_objectID nObjectID) : IStreamable(s_classID, nObjectID), IIdentifyable(ClassID(), nObjectID), m_pOtherObject(nullptr) {}

        static t_classID s_classID;
        std::string m_sTestString;
        IStreamable *m_pOtherObject;

        virtual void ItlWriteToStream(std::ostream &outStream, ISerializer *pSerializer) const;
        virtual void ItlReadFromStream(std::istream &inStream, ISerializer *pSerializer);
    public:

        void SetOtherObject(IStreamable *pObject);
        static t_classID ClassID() { return s_classID; }
        static TestClass2 * Cast(IIdentifyable *pObject) { return dynamic_cast<TestClass2 *>(pObject); }

        static TestClass2 * CreateFromStream(std::istream &rInStream, ISerializer *pSerializer, bool bReplaceObjectID = false, bool bSkipClassID = false);

        static TestClass2 * Create(std::string sTestString)
        {
            TestClass2 *pObject = new TestClass2();

            pObject->m_sTestString.assign(sTestString);

            return pObject;
        }
    };

}


#endif // IIDENTIFYABLETEST_H
