#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <ostream>
#include <istream>
#include <string>

namespace BambooLib
{
    class IStreamable;

    class ISerializer
    {
    public:
        ISerializer() {}
        virtual ~ISerializer() {}

        virtual void StartObjectWriting() = 0;
        virtual void StopObjectWriting(std::ostream &rOutStream) = 0;

        virtual void StartObjectReading() = 0;
        virtual void StopObjectReading(std::istream &rInStream, bool bUseObjectID = true) = 0;

        virtual void Serialize(std::ostream &rOutStream, const IStreamable * rValue) = 0;
        virtual void Serialize(std::ostream &rOutStream, const unsigned int &rValue) = 0;
        virtual void Serialize(std::ostream &rOutStream, const int &rValue) = 0;
        virtual void Serialize(std::ostream &rOutStream, const long int &rValue) = 0;
        virtual void Serialize(std::ostream &rOutStream, const long long int &rValue) = 0;
        virtual void Serialize(std::ostream &rOutStream, const unsigned long long &rValue) = 0;
        virtual void Serialize(std::ostream &rOutStream, const float &rValue) = 0;
        virtual void Serialize(std::ostream &rOutStream, const double &rValue) = 0;
        virtual void Serialize(std::ostream &rOutStream, const char &rValue) = 0;
        virtual void Serialize(std::ostream &rOutStream, const bool &rValue) = 0;
        virtual void Serialize(std::ostream &rOutStream, const std::string & rsValue) = 0;

        void Serialize(std::ostream &rOutStream, const char * rsValue) { Serialize(rOutStream, std::string(rsValue)); }

        virtual void Unserialize(std::istream &rInStream, IStreamable ** ppObject) = 0;
        virtual void Unserialize(std::istream &rInStream, unsigned int &rValue) = 0;
        virtual void Unserialize(std::istream &rInStream, int &rValue) = 0;
        virtual void Unserialize(std::istream &rInStream, long int &rValue) = 0;
        virtual void Unserialize(std::istream &rInStream, long long int &rValue) = 0;
        virtual void Unserialize(std::istream &rInStream, unsigned long long &rValue) = 0;
        virtual void Unserialize(std::istream &rInStream, float &rValue) = 0;
        virtual void Unserialize(std::istream &rInStream, double &rValue) = 0;
        virtual void Unserialize(std::istream &rInStream, char &rValue) = 0;
        virtual void Unserialize(std::istream &rInStream, bool &rValue) = 0;
        virtual void Unserialize(std::istream &rInStream, std::string & rsValue) = 0;
    };
}

#endif // SERIALIZER_H
