#ifndef CORESYSTEM_H
#define CORESYSTEM_H

#include "BambooLib/include/GeneralDefinitions.h"

namespace BambooLib
{
    class CoreSystem
    {
    public:
        /*! \name Static methods */
        //@{
            /// Returns the singelton instance of the BambooLib::CoreSystem
            static CoreSystem * GetInstance();
        //@}

        /*! \name Construction / Destruction */
        //@{
            /// destructor
            ~CoreSystem();
        //@}

        /*! \name Public methods for object handling*/
        //@{
            /// Registers a class (not an object!) and returns an unique class id for this class
            t_classID RegisterClass(const char *szClassName, void *pCreateMethod);

            /// Registers an object, gets called by the constructor of IIdentifyable
            bool RegisterObject(IIdentifyable *pObject);

            /// Unregisters an object, gets called by the destructor of IIdentifyable
            bool UnregisterObject(IIdentifyable *pObject);

            /// Returns whether a given object, identified by its object id, is already registered in the core system
            bool IsObjectRegistered(t_objectID nID);

            /// Returns a free object id,
            t_objectID GetNextFreeObjectID();

            /// Returns the wanted object, if it is registered at the core system, else NULL/nullptr is returned
            IIdentifyable * GetObjectForObjectID(t_objectID nID);

            /// Returns a void-pointer to the Create() method of the given class, or NULL/nullptr if the class is not registered
            void * GetCreateMethod(t_classID nClassID);
        //@}
    private:
        /*! \name Construction */
        //@{
            /// private constructor
            CoreSystem();
        //@}

        /*! \name Internal types */
        //@{
            /// forward declaration to internal structure (Pimpl pattern used)
            struct TItlInternalStuff;
        //@}

        /*! \name Private members */
        //@{
            /// Pimpl pattern :)
            TItlInternalStuff * m_pImpl;
        //@}
    };
}


#endif // CORESYSTEM_H
