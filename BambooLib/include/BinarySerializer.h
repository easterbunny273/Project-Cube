#ifndef BINARYSERIALIZER_H
#define BINARYSERIALIZER_H

#include "BambooLib/include/ISerializer.h"

#include <ostream>
#include <istream>
#include <string>
#include <list>
#include <set>
#include <map>
#include <vector>

namespace BambooLib
{
    class BinarySerializer : public ISerializer
    {
    private:
        unsigned int m_nObjectWriteLevels;
        unsigned int m_nObjectReadLevels;

        std::list<t_objectID> m_lAdditionalObjectsToSerialize;
        std::set<t_objectID> m_sAdditionalObjectsToSerialize;

        std::list<t_objectID> m_lAdditionalObjectsToRead;
        std::set<t_objectID> m_sAdditionalObjectsToRead;
        std::map<t_objectID, t_objectID> m_mObjectIDReplaceTable;
        std::vector<IStreamable **> m_vDummyPointersToReplace;

    public:
        BinarySerializer() : m_nObjectWriteLevels(0), m_nObjectReadLevels(0) {}
        ~BinarySerializer() {}

        virtual void StartObjectWriting();
        virtual void StopObjectWriting(std::ostream &rOutStream);

        virtual void StartObjectReading();
        virtual void StopObjectReading(std::istream &rInStream, bool bUseObjectID = true);

        virtual void Serialize(std::ostream &rOutStream, const IStreamable *rValue);
        virtual void Serialize(std::ostream &rOutStream, const unsigned int &rValue);
        virtual void Serialize(std::ostream &rOutStream, const int &rValue);
        virtual void Serialize(std::ostream &rOutStream, const long int &rValue);
        virtual void Serialize(std::ostream &rOutStream, const long long int &rValue);
        virtual void Serialize(std::ostream &rOutStream, const unsigned long long &rValue);
        virtual void Serialize(std::ostream &rOutStream, const float &rValue);
        virtual void Serialize(std::ostream &rOutStream, const double &rValue);
        virtual void Serialize(std::ostream &rOutStream, const char &rValue);
        virtual void Serialize(std::ostream &rOutStream, const bool &rValue);
        virtual void Serialize(std::ostream &rOutStream, const std::string & rsValue);

        virtual void Unserialize(std::istream &rInStream, IStreamable ** ppObject);
        virtual void Unserialize(std::istream &rInStream, unsigned int &rValue);
        virtual void Unserialize(std::istream &rInStream, int &rValue);
        virtual void Unserialize(std::istream &rInStream, long int &rValue);
        virtual void Unserialize(std::istream &rInStream, long long int &rValue);
        virtual void Unserialize(std::istream &rInStream, unsigned long long &rValue);
        virtual void Unserialize(std::istream &rInStream, float &rValue);
        virtual void Unserialize(std::istream &rInStream, double &rValue);
        virtual void Unserialize(std::istream &rInStream, char &rValue);
        virtual void Unserialize(std::istream &rInStream, bool &rValue);
        virtual void Unserialize(std::istream &rInStream, std::string & rsValue);
    };

}

#endif // BINARYSERIALIZER_H
