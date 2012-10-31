/*
 * source file for BambooLib::KeyValueMap class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 10/2012, BambooLib
 */


#include <map>
#include <cassert>

#include "KeyValueMap.h"

namespace BambooLib
{

    class KeyValueMap::TItlPimpl
    {
    public:
        std::map<TKey, TValueType>      m_mValueTypes;
        std::map<TKey, float>           m_mFloatValues;
        std::map<TKey, double>          m_mDoubleValues;
        std::map<TKey, bool>            m_mBooleanValues;
        std::map<TKey, int>             m_mIntegerValues;
        std::map<TKey, long>            m_mLongValues;
        std::map<TKey, std::string>     m_mStringValues;
    };

    /******************************************************************************************************
     *****************************************************************************************************/
    KeyValueMap::KeyValueMap()
    {
        // create pimpl
        m_pImpl = new TItlPimpl();
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    KeyValueMap::KeyValueMap(const KeyValueMap &rOtherMap)
    {
        // create pimpl
        m_pImpl = new TItlPimpl();

        // copy value maps
        m_pImpl->m_mValueTypes      = rOtherMap.m_pImpl->m_mValueTypes;
        m_pImpl->m_mFloatValues     = rOtherMap.m_pImpl->m_mFloatValues;
        m_pImpl->m_mDoubleValues    = rOtherMap.m_pImpl->m_mDoubleValues;
        m_pImpl->m_mBooleanValues   = rOtherMap.m_pImpl->m_mBooleanValues;
        m_pImpl->m_mIntegerValues   = rOtherMap.m_pImpl->m_mIntegerValues;
        m_pImpl->m_mLongValues      = rOtherMap.m_pImpl->m_mLongValues;
        m_pImpl->m_mStringValues    = rOtherMap.m_pImpl->m_mStringValues;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    KeyValueMap &KeyValueMap::operator =(const KeyValueMap &rOtherMap)
    {
        // copy value maps
        m_pImpl->m_mValueTypes      = rOtherMap.m_pImpl->m_mValueTypes;
        m_pImpl->m_mFloatValues     = rOtherMap.m_pImpl->m_mFloatValues;
        m_pImpl->m_mDoubleValues    = rOtherMap.m_pImpl->m_mDoubleValues;
        m_pImpl->m_mBooleanValues   = rOtherMap.m_pImpl->m_mBooleanValues;
        m_pImpl->m_mIntegerValues   = rOtherMap.m_pImpl->m_mIntegerValues;
        m_pImpl->m_mLongValues      = rOtherMap.m_pImpl->m_mLongValues;
        m_pImpl->m_mStringValues    = rOtherMap.m_pImpl->m_mStringValues;

        // return this instance
        return *this;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    KeyValueMap::~KeyValueMap()
    {
        // clean up
        delete m_pImpl;
        m_pImpl = NULL;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    KeyValueMap::TValueType KeyValueMap::GetType(KeyValueMap::TKey sKey)
    {
        std::map<TKey, TValueType>::iterator iter = m_pImpl->m_mValueTypes.find(sKey);

        if (iter != m_pImpl->m_mValueTypes.end())
            return iter->second;
        else
            return VALUE_UNSET;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    std::vector<std::pair<KeyValueMap::TKey, KeyValueMap::TValueType> > KeyValueMap::GetKeysAndTypes()
    {
        std::vector<std::pair<KeyValueMap::TKey, KeyValueMap::TValueType> > vResultVector;

        for (std::map<TKey, TValueType>::iterator iter = m_pImpl->m_mValueTypes.begin(); iter != m_pImpl->m_mValueTypes.end(); iter++)
            vResultVector.push_back(*iter);

        return vResultVector;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool KeyValueMap::SetValue(KeyValueMap::TKey sKey, double dValue)
    {
        TValueType eType = GetType(sKey);
        bool bOk = (eType == VALUE_UNSET || eType == VALUE_DOUBLE);

        if (bOk)
        {
            m_pImpl->m_mValueTypes[sKey]    = VALUE_DOUBLE;
            m_pImpl->m_mDoubleValues[sKey]  = dValue;
        }

        return bOk;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool KeyValueMap::SetValue(KeyValueMap::TKey sKey, float fValue)
    {
        TValueType eType = GetType(sKey);
        bool bOk = (eType == VALUE_UNSET || eType == VALUE_FLOAT);

        if (bOk)
        {
            m_pImpl->m_mValueTypes[sKey]    = VALUE_FLOAT;
            m_pImpl->m_mFloatValues[sKey]   = fValue;
        }

        return bOk;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool KeyValueMap::SetValue(KeyValueMap::TKey sKey, bool bValue)
    {
        TValueType eType = GetType(sKey);
        bool bOk = (eType == VALUE_UNSET || eType == VALUE_BOOL);

        if (bOk)
        {
            m_pImpl->m_mValueTypes[sKey]    = VALUE_BOOL;
            m_pImpl->m_mBooleanValues[sKey] = bValue;
        }

        return bOk;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool KeyValueMap::SetValue(KeyValueMap::TKey sKey, int iValue)
    {
        TValueType eType = GetType(sKey);
        bool bOk = (eType == VALUE_UNSET || eType == VALUE_INTEGER);

        if (bOk)
        {
            m_pImpl->m_mValueTypes[sKey]    = VALUE_INTEGER;
            m_pImpl->m_mIntegerValues[sKey] = iValue;
        }

        return bOk;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool KeyValueMap::SetValue(KeyValueMap::TKey sKey, long lValue)
    {
        TValueType eType = GetType(sKey);
        bool bOk = (eType == VALUE_UNSET || eType == VALUE_LONG);

        if (bOk)
        {
            m_pImpl->m_mValueTypes[sKey]    = VALUE_LONG;
            m_pImpl->m_mLongValues[sKey]    = lValue;
        }

        return bOk;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool KeyValueMap::SetValue(KeyValueMap::TKey sKey, const std::string &rsValue)
    {
        TValueType eType = GetType(sKey);
        bool bOk = (eType == VALUE_UNSET || eType == VALUE_STRING);

        if (bOk)
        {
            m_pImpl->m_mValueTypes[sKey]    = VALUE_STRING;
            m_pImpl->m_mStringValues[sKey]  = rsValue;
        }

        return bOk;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool KeyValueMap::GetValue(KeyValueMap::TKey sKey, double &rdValue)
    {
        // check matching type
        TValueType eType = GetType(sKey);
        bool bOk = (eType == VALUE_DOUBLE);

        if (bOk)
        {
            // find value
            std::map<TKey, double>::iterator iter = m_pImpl->m_mDoubleValues.find(sKey);

            // make sure that value was found
            assert(iter != m_pImpl->m_mDoubleValues.end());

            // set return value
            rdValue = iter->second;
        }

        return bOk;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool KeyValueMap::GetValue(KeyValueMap::TKey sKey, float &rfValue)
    {
        // check matching type
        TValueType eType = GetType(sKey);
        bool bOk = (eType == VALUE_FLOAT);

        if (bOk)
        {
            // find value
            std::map<TKey, float>::iterator iter = m_pImpl->m_mFloatValues.find(sKey);

            // make sure that value was found
            assert(iter != m_pImpl->m_mFloatValues.end());

            // set return value
            rfValue = iter->second;
        }

        return bOk;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool KeyValueMap::GetValue(KeyValueMap::TKey sKey, bool &rbValue)
    {
        // check matching type
        TValueType eType = GetType(sKey);
        bool bOk = (eType == VALUE_BOOL);

        if (bOk)
        {
            // find value
            std::map<TKey, bool>::iterator iter = m_pImpl->m_mBooleanValues.find(sKey);

            // make sure that value was found
            assert(iter != m_pImpl->m_mBooleanValues.end());

            // set return value
            rbValue = iter->second;
        }

        return bOk;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool KeyValueMap::GetValue(KeyValueMap::TKey sKey, int &riValue)
    {
        // check matching type
        TValueType eType = GetType(sKey);
        bool bOk = (eType == VALUE_INTEGER);

        if (bOk)
        {
            // find value
            std::map<TKey, int>::iterator iter = m_pImpl->m_mIntegerValues.find(sKey);

            // make sure that value was found
            assert(iter != m_pImpl->m_mIntegerValues.end());

            // set return value
            riValue = iter->second;
        }

        return bOk;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool KeyValueMap::GetValue(KeyValueMap::TKey sKey, long &rlValue)
    {
        // check matching type
        TValueType eType = GetType(sKey);
        bool bOk = (eType == VALUE_LONG);

        if (bOk)
        {
            // find value
            std::map<TKey, long>::iterator iter = m_pImpl->m_mLongValues.find(sKey);

            // make sure that value was found
            assert(iter != m_pImpl->m_mLongValues.end());

            // set return value
            rlValue = iter->second;
        }

        return bOk;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool KeyValueMap::GetValue(KeyValueMap::TKey sKey, std::string &rsValue)
    {
        // check matching type
        TValueType eType = GetType(sKey);
        bool bOk = (eType == VALUE_STRING);

        if (bOk)
        {
            // find value
            std::map<TKey, std::string>::iterator iter = m_pImpl->m_mStringValues.find(sKey);

            // make sure that value was found
            assert(iter != m_pImpl->m_mStringValues.end());

            // set return value
            rsValue = iter->second;
        }

        return bOk;
    }

}


