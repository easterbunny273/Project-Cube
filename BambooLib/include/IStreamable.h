#ifndef ISTREAMABLE_H
#define ISTREAMABLE_H

#include "BambooLib/include/IIdentifyable.h"
#include "BambooLib/include/ISerializer.h"

#include <istream>
#include <ostream>
#include <fstream>

#include <memory>
#include <cassert>

namespace BambooLib
{
  class IStreamable : virtual public IIdentifyable
  {
    public:
      IStreamable(t_classID nClassID, t_objectID nObjectID) : IIdentifyable(nClassID, nObjectID) {}
      IStreamable(t_classID nClassID) : IIdentifyable(nClassID) {}

      virtual ~IStreamable() {}

      void Store(std::ostream &outStream, ISerializer *pSerializer) const
      {
          pSerializer->StartObjectWriting();

          pSerializer->Serialize(outStream, GetClassID());
          pSerializer->Serialize(outStream, GetObjectID());

          ItlWriteToStream(outStream, pSerializer);

          pSerializer->StopObjectWriting(outStream);
      }

      bool Restore(std::istream &inStream, ISerializer *pSerializer)
      {
          pSerializer->StartObjectReading();

          t_classID nClassID;
          t_objectID nObjectID;

          pSerializer->Unserialize(inStream, nClassID);
          pSerializer->Unserialize(inStream, nObjectID);

          assert (nClassID == m_nClassID);
          assert (nObjectID == m_nObjectID);

          bool bOk = (nClassID == m_nClassID && m_nObjectID == nObjectID);

          if (bOk)
              ItlReadFromStream(inStream, pSerializer);

          pSerializer->StopObjectReading(inStream, true);

          return bOk;
      }

      static IStreamable * Cast(IIdentifyable *pObject) { return dynamic_cast<IStreamable *>(pObject); }
  protected:

      virtual void ItlWriteToStream(std::ostream &outStream, ISerializer *pSerializer) const = 0;
      virtual void ItlReadFromStream(std::istream &inStream, ISerializer *pSerializer) = 0;
  };
}

#endif // ISTREAMABLE_H
