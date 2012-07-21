#include <cassert>

#include "BambooLib/include/IStreamable.h"
#include "BambooLib/include/BinarySerializer.h"

#include <iostream>

namespace BambooLib
{

/***************** BOOL with signature * ************************/
   /* void BinarySerializer::Serialize(std::ostream &rOutStream, bool &rValue)
    {
        char cSizeOfBool = (unsigned char) sizeof(bool);
        char cBoolSignature = 'b';

        rOutStream.write(reinterpret_cast<const char *>(&cBoolSignature), 1);
        rOutStream.write(reinterpret_cast<const char *>(&cSizeOfBool), 1);

        rOutStream.write(reinterpret_cast<const char *>(&rValue), sizeof(bool));
    }

    void BinarySerializer::Unserialize(std::istream &rInStream, bool &rValue)
    {
        char cSizeOfBool = 0;
        char cBoolSignature = 0;

        rInStream.read(reinterpret_cast<char *>(&cBoolSignature), 1);
        rInStream.read(reinterpret_cast<char *>(&cSizeOfBool), 1);

        assert (cSizeOfBool == (unsigned char) sizeof(bool));
        assert (cBoolSignature == 'b');

        rInStream.read(reinterpret_cast<char *>(&rValue), sizeof(bool));
    }*/

/***************** Objects  * ************************/
    void BinarySerializer::StartObjectWriting()
    {
        m_nObjectWriteLevels++;
    }

    void BinarySerializer::StopObjectWriting(std::ostream &rOutStream)
    {
        assert (m_nObjectWriteLevels > 0);

        m_nObjectWriteLevels--;

        if (m_nObjectWriteLevels == 0)
        {
            while (m_lAdditionalObjectsToSerialize.empty() == false)
            {
                m_nObjectWriteLevels++;

                const t_objectID nObjectToSerialize = m_lAdditionalObjectsToSerialize.front();

                assert (m_sAdditionalObjectsToSerialize.find(nObjectToSerialize) != m_sAdditionalObjectsToSerialize.end());

                IStreamable *pObject = IStreamable::Cast(CoreSystem::GetInstance()->GetObjectForObjectID(nObjectToSerialize));
                assert (pObject != nullptr);

                pObject->Store(rOutStream, this);

                m_lAdditionalObjectsToSerialize.pop_front();
                m_sAdditionalObjectsToSerialize.erase(nObjectToSerialize);

                m_nObjectWriteLevels--;
            }
        }
    }

    void BinarySerializer::StartObjectReading()
    {
        m_nObjectReadLevels++;
    }

    void BinarySerializer::StopObjectReading(std::istream &rInStream, bool bReplaceObjectID)
    {
        assert (m_nObjectReadLevels > 0);

        m_nObjectReadLevels--;

        if (m_nObjectReadLevels == 0)
        {
            while (m_lAdditionalObjectsToRead.empty() == false)
            {
                m_nObjectReadLevels++;

                t_classID nClassID;
                Unserialize(rInStream, nClassID);

                void * pCreateMethod = CoreSystem::GetInstance()->GetCreateMethod(nClassID);
                assert (pCreateMethod != nullptr);

                IIdentifyable * (*pMethod) (std::istream &, ISerializer *, bool, bool) = (IIdentifyable * (*) (std::istream &, ISerializer *, bool, bool)) (pCreateMethod);

                IIdentifyable *pNewObject = pMethod(rInStream, this, bReplaceObjectID, true);
                assert (pNewObject != nullptr);

                if (bReplaceObjectID)
                {
                    t_objectID nObjectIDInStream = m_lAdditionalObjectsToRead.front();
                    t_objectID nObjectIDOfNewObject = pNewObject->GetObjectID();

                    m_mObjectIDReplaceTable[nObjectIDInStream] = nObjectIDOfNewObject;
                }

                //assert (pNewObject->GetObjectID() == m_lAdditionalObjectsToRead.front());

                m_lAdditionalObjectsToRead.pop_front();

                m_nObjectReadLevels--;
            }

            for (auto iter = m_vDummyPointersToReplace.begin(); iter != m_vDummyPointersToReplace.end(); iter++)
            {
                IStreamable ** ppObject = *iter;

                t_objectID nObjectID = reinterpret_cast<t_objectID> (*ppObject);

                if (bReplaceObjectID)
                {
                    auto iter = m_mObjectIDReplaceTable.find(nObjectID);
                    assert (iter != m_mObjectIDReplaceTable.end());

                    t_objectID nNewObjectID = iter->second;

                    *ppObject = IStreamable::Cast(CoreSystem::GetInstance()->GetObjectForObjectID(nNewObjectID));
                    assert (*ppObject != nullptr);
                }
                else
                {
                    *ppObject = IStreamable::Cast(CoreSystem::GetInstance()->GetObjectForObjectID(nObjectID));
                    assert (*ppObject != nullptr);
                }

                assert (*ppObject != nullptr);
            }

            m_mObjectIDReplaceTable.clear();
            m_sAdditionalObjectsToRead.clear();
            assert (m_lAdditionalObjectsToRead.empty());
        }
    }

/***************** objects  * ************************/
   void BinarySerializer::Serialize(std::ostream &rOutStream, const IStreamable * rpObject)
    {
       if (rpObject != nullptr)
       {
           if (m_nObjectWriteLevels > 0)
           {
               t_objectID nObjectID = rpObject->GetObjectID();
               assert (nObjectID != INVALID_OBJECTID);

               if (m_sAdditionalObjectsToSerialize.find(nObjectID) == m_sAdditionalObjectsToSerialize.end())
               {
                    m_sAdditionalObjectsToSerialize.insert(nObjectID);
                    m_lAdditionalObjectsToSerialize.push_back(nObjectID);
               }


               Serialize(rOutStream, nObjectID);
           }
           else
           {
               rpObject->Store(rOutStream, this);
           }
       }
       else
           Serialize(rOutStream, INVALID_OBJECTID);

    }

   void BinarySerializer::Unserialize(std::istream &rInStream, IStreamable ** ppObject)
    {
       if (m_nObjectReadLevels > 0)
       {
           t_objectID nObjectID;
           Unserialize(rInStream, nObjectID);

           *ppObject = reinterpret_cast<IStreamable *> (nObjectID);

           if (nObjectID != INVALID_OBJECTID)
           {
               if (m_sAdditionalObjectsToRead.find(nObjectID) == m_sAdditionalObjectsToRead.end())
               {
                    m_sAdditionalObjectsToRead.insert(nObjectID);
                    m_lAdditionalObjectsToRead.push_back(nObjectID);
               }

               m_vDummyPointersToReplace.push_back(ppObject);

               std::cout << "Plan to read " << nObjectID << std::endl;
           }
       }
       else
       {
           std::cout << "UNKNOWN" << std::endl;
           //rpObject->Store(rOutStream, this);
       }
    }


/***************** BOOL  * ************************/
   void BinarySerializer::Serialize(std::ostream &rOutStream, const bool &rValue)
    {
        rOutStream.write(reinterpret_cast<const char *>(&rValue), sizeof(bool));
    }

    void BinarySerializer::Unserialize(std::istream &rInStream, bool &rValue)
    {
        rInStream.read(reinterpret_cast<char *>(&rValue), sizeof(bool));
    }

/***************** UNSIGNED INT * ************************/
    void BinarySerializer::Serialize(std::ostream &rOutStream, const unsigned int &rValue)
    {
        rOutStream.write(reinterpret_cast<const char *>(&rValue), sizeof(unsigned int));
        std::cout << "n" << rValue << std::endl;
    }

    void BinarySerializer::Unserialize(std::istream &rInStream, unsigned int &rValue)
    {
        rInStream.read(reinterpret_cast<char *>(&rValue), sizeof(unsigned int));
    }

/***************** INT * ************************/
    void BinarySerializer::Serialize(std::ostream &rOutStream, const int &rValue)
    {
        rOutStream.write(reinterpret_cast<const char *>(&rValue), sizeof(int));
        std::cout << "n" << rValue << std::endl;
    }

    void BinarySerializer::Unserialize(std::istream &rInStream, int &rValue)
    {
        rInStream.read(reinterpret_cast<char *>(&rValue), sizeof(int));
    }

/***************** LONG INT * ************************/
    void BinarySerializer::Serialize(std::ostream &rOutStream, const long int &rValue)
    {
        rOutStream.write(reinterpret_cast<const char *>(&rValue), sizeof(long int));
    }

    void BinarySerializer::Unserialize(std::istream &rInStream, long int &rValue)
    {
        rInStream.read(reinterpret_cast<char *>(&rValue), sizeof(long int));
    }

/***************** LONG LONG INT * ************************/
    void BinarySerializer::Serialize(std::ostream &rOutStream, const long long int &rValue)
    {
        std::cout << "n" << rValue << std::endl;
        rOutStream.write(reinterpret_cast<const char *>(&rValue), sizeof(long long int));
    }

    void BinarySerializer::Unserialize(std::istream &rInStream, long long int &rValue)
    {
        rInStream.read(reinterpret_cast<char *>(&rValue), sizeof(long long int));
    }

/***************** UNSIGNED LONG LONG * ************************/
    void BinarySerializer::Serialize(std::ostream &rOutStream, const unsigned long long &rValue)
    {
        std::cout << "n" << rValue << std::endl;
        rOutStream.write(reinterpret_cast<const char *>(&rValue), sizeof(unsigned long long));
    }

    void BinarySerializer::Unserialize(std::istream &rInStream, unsigned long long &rValue)
    {
        rInStream.read(reinterpret_cast<char *>(&rValue), sizeof(unsigned long long));
    }


/***************** DOUBLE * ************************/
    void BinarySerializer::Serialize(std::ostream &rOutStream, const double &rValue)
    {
        rOutStream.write(reinterpret_cast<const char *>(&rValue), sizeof(double));
    }

    void BinarySerializer::Unserialize(std::istream &rInStream, double &rValue)
    {
        rInStream.read(reinterpret_cast<char *>(&rValue), sizeof(double));
    }

/***************** FLOAT * ************************/
    void BinarySerializer::Serialize(std::ostream &rOutStream, const float &rValue)
    {
        rOutStream.write(reinterpret_cast<const char *>(&rValue), sizeof(float));
    }

    void BinarySerializer::Unserialize(std::istream &rInStream, float &rValue)
    {
        rInStream.read(reinterpret_cast<char *>(&rValue), sizeof(float));
    }


/***************** CHAR * ************************/
    void BinarySerializer::Serialize(std::ostream &rOutStream, const char &rValue)
    {
        rOutStream.write(reinterpret_cast<const char *>(&rValue), sizeof(char));
    }

    void BinarySerializer::Unserialize(std::istream &rInStream, char &rValue)
    {
        rInStream.read(reinterpret_cast<char *>(&rValue), sizeof(char));
    }

/***************** STRING * ************************/
    void BinarySerializer::Serialize(std::ostream &rOutStream, const std::string &rsValue)
    {
        unsigned int nLength = rsValue.length();

        Serialize(rOutStream, nLength);

        for (unsigned int nChar = 0; nChar < nLength; nChar++)
            rOutStream.write(reinterpret_cast<const char *>(&rsValue.at(nChar)), sizeof(char));

        std::cout << "s" << rsValue << std::endl;
    }

    void BinarySerializer::Unserialize(std::istream &rInStream, std::string &rsValue)
    {
        unsigned int nLength = 0;

        Unserialize(rInStream, nLength);

        rsValue.resize(nLength);

        for (unsigned int nChar = 0; nChar < nLength; nChar++)
            rInStream.read(reinterpret_cast<char *>(&rsValue.at(nChar)), sizeof(char));
    }
}
