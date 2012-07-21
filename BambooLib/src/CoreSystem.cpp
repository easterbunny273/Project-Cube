#include "BambooLib/include/CoreSystem.h"
#include "BambooLib/include/IIdentifyable.h"

#include <cassert>
#include <cstring>
#include <map>

// internal structure for PIMPL
struct BambooLib::CoreSystem::TItlInternalStuff
{
    std::map<t_objectID, IIdentifyable *>                   m_mRegisteredObjects;
    std::map<t_classID, const char *>                       m_mRegisteredClasses;
    std::map<t_classID, void *>     m_mRegisteredCreateMethods;

    t_objectID m_nNextFreeObjectCode;
};

BambooLib::CoreSystem::CoreSystem()
{
    // create struct for internal data
    m_pImpl = new TItlInternalStuff;

    m_pImpl->m_nNextFreeObjectCode = 1;
}

BambooLib::CoreSystem::~CoreSystem()
{
    // delete internal data struct
    delete m_pImpl;
}

BambooLib::CoreSystem *BambooLib::CoreSystem::GetInstance()
{
    static CoreSystem sInstance;

    return &sInstance;
}


BambooLib::t_classID BambooLib::CoreSystem::RegisterClass(const char *szClassName, void * pCreateMethod)
{
    assert (strlen(szClassName) == 8);

    long long int nClassID = 0;

    assert (sizeof(nClassID) >= 8);

    for (unsigned int i=0; i < 8; i++)
        nClassID += ((unsigned short int) szClassName[i]) << i;

    auto iter = m_pImpl->m_mRegisteredClasses.find(nClassID);
    assert (iter == m_pImpl->m_mRegisteredClasses.end());

    m_pImpl->m_mRegisteredClasses[nClassID] = szClassName;
    m_pImpl->m_mRegisteredCreateMethods[nClassID] = pCreateMethod;

    return (t_classID) (nClassID);
}


bool BambooLib::CoreSystem::RegisterObject(BambooLib::IIdentifyable *pObject)
{
    // get object id
    t_objectID nObjectID = pObject->GetObjectID();
    assert (nObjectID != INVALID_OBJECTID);

    // make sure that we don't return an already used object ID as free object ID (can happen due object (un)serialization)
    if (nObjectID >= m_pImpl->m_nNextFreeObjectCode)
        m_pImpl->m_nNextFreeObjectCode = nObjectID+1;

    // make sure object is not registered yet
    bool bOk = IsObjectRegistered(nObjectID) == false;
    assert (bOk);

    // insert object into map
    m_pImpl->m_mRegisteredObjects[nObjectID] = pObject;

    return bOk;
}

bool BambooLib::CoreSystem::UnregisterObject(BambooLib::IIdentifyable *pObject)
{
    // get object id
    t_objectID nObjectID = pObject->GetObjectID();

    assert (nObjectID != INVALID_OBJECTID);

    // make sure object is registered
    bool bOk = IsObjectRegistered(nObjectID);
    assert (bOk);

    // erase object
    m_pImpl->m_mRegisteredObjects.erase(nObjectID);

    return bOk;
}

bool BambooLib::CoreSystem::IsObjectRegistered(BambooLib::t_objectID nID)
{
    auto iterator = m_pImpl->m_mRegisteredObjects.find(nID);

    return (iterator != m_pImpl->m_mRegisteredObjects.end());
}

BambooLib::t_objectID BambooLib::CoreSystem::GetNextFreeObjectID()
{
    t_objectID nNextFree = m_pImpl->m_nNextFreeObjectCode++;

    return nNextFree;

}

BambooLib::IIdentifyable *BambooLib::CoreSystem::GetObjectForObjectID(BambooLib::t_objectID nID)
{
    assert (IsObjectRegistered(nID));

    auto iterator = m_pImpl->m_mRegisteredObjects.find(nID);

    if (iterator != m_pImpl->m_mRegisteredObjects.end())
        return iterator->second;
    else
        return nullptr;
}

void *BambooLib::CoreSystem::GetCreateMethod(BambooLib::t_classID nClassID)
{
    assert (m_pImpl->m_mRegisteredClasses.find(nClassID) != m_pImpl->m_mRegisteredClasses.end());

    auto iterator = m_pImpl->m_mRegisteredCreateMethods.find(nClassID);

    if (iterator != m_pImpl->m_mRegisteredCreateMethods.end())
        return iterator->second;
    else
        return nullptr;
}
