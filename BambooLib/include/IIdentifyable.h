#ifndef IIDENTIFYABLE_H
#define IIDENTIFYABLE_H

#include "BambooLib/include/CoreSystem.h"

namespace BambooLib
{
    class IIdentifyable
    {
    public:
        /*! \name Construction / Destruction */
        //@{

        /// Constructor for a given class ID and a given object ID
        IIdentifyable(t_classID nClassID, t_objectID nObjectID) : m_nClassID(nClassID), m_nObjectID(nObjectID) { CoreSystem::GetInstance()->RegisterObject(this); }

        /// Constructor for a given class ID
        IIdentifyable(t_classID nClassID) : m_nClassID(nClassID), m_nObjectID(CoreSystem::GetInstance()->GetNextFreeObjectID()) { CoreSystem::GetInstance()->RegisterObject(this); }

        /// Destructor
        virtual ~IIdentifyable() { CoreSystem::GetInstance()->UnregisterObject(this); }
        //@}

        /*! \name Public attributes */
        //@{
            /// Returns the object ID
            t_objectID GetObjectID() const { return m_nObjectID; }

            /// Returns the class ID
            t_classID GetClassID() const { return m_nClassID; }
        //@}

        /*! \name Static interface */
        //@{
            // each object which implements IIdentifyable should also implement
            // the following static methods, although this cannot be defined in
            // the interface

            // returns the class ID of the class:
            // static t_classID ClassID() {};

            // try to cast an IIdentifyable to the specific class, returns 0
            // if not possible:
            // static CONCRETE_CLASS * Cast(IIdentifyable *pObject);
        //@}
    protected:
        /*! \name Private members */
        //@{
            const t_classID   m_nClassID;
            const t_objectID  m_nObjectID;
        //@}
    };
}
#endif // IIDENTIFYABLE_H
