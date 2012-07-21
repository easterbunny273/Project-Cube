#ifndef GENERALDEFINITIONS_H
#define GENERALDEFINITIONS_H

#define BAMBOOLIB_VERSION "0.1"

namespace BambooLib
{
    /*! \name Some general forward declarations */
    //@{
        class IIdentifyable;
        class ISerializer;
    //@}

    /*! \name Type declarations */
    //@{
        /// Each object gets an unique object id
        typedef unsigned long long t_objectID;

        /// Each class gets an unique class id
        typedef unsigned long long t_classID;
    //@}

    /*! \name Some constant variables */
    //@{
        const t_objectID INVALID_OBJECTID = 0;
    //@}
}

#endif // GENERALDEFINITIONS_H
