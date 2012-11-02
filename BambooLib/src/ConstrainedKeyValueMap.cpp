/*
 * source file for BambooLib::ConstrainedKeyValueMap class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 11/2012, BambooLib
 */


#include "ConstrainedKeyValueMap.h"
#include <algorithm>

namespace BambooLib
{
    /******************************************************************************************************
     *****************************************************************************************************/
    ConstrainedKeyValueMap::ConstrainedKeyValueMap(std::map<KeyValueMap::TKey, TConstraint> mConstraints)
    {
        m_pKeyValueMap  = new KeyValueMap();
        m_mConstraints  = mConstraints;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    ConstrainedKeyValueMap::~ConstrainedKeyValueMap()
    {
        delete m_pKeyValueMap;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool ConstrainedKeyValueMap::ItlTestTypeConstraint(KeyValueMap::TValueType eType, float fValue)
    {
        if (eType == KeyValueMap::VALUE_FLOAT)
            return true;
        else
            return false;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool ConstrainedKeyValueMap::ItlTestTypeConstraint(KeyValueMap::TValueType eType, double dValue)
    {
        if (eType == KeyValueMap::VALUE_DOUBLE)
            return true;
        else
            return false;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool ConstrainedKeyValueMap::ItlTestTypeConstraint(KeyValueMap::TValueType eType, int iValue)
    {
        if (eType == KeyValueMap::VALUE_INTEGER)
            return true;
        else
            return false;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool ConstrainedKeyValueMap::ItlTestTypeConstraint(KeyValueMap::TValueType eType, long lValue)
    {
        if (eType == KeyValueMap::VALUE_LONG)
            return true;
        else
            return false;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool ConstrainedKeyValueMap::ItlTestTypeConstraint(KeyValueMap::TValueType eType, bool bValue)
    {
        if (eType == KeyValueMap::VALUE_BOOL)
            return true;
        else
            return false;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool ConstrainedKeyValueMap::ItlTestTypeConstraint(KeyValueMap::TValueType eType, const std::string &rsValue)
    {
        if (eType == KeyValueMap::VALUE_STRING)
            return true;
        else
            return false;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    ConstrainedKeyValueMap::TValueRangeConstraint::TValueRangeConstraint(double dMinimum, double dMaximum, double dStep)
    {
        m_dMinimum = dMinimum;
        m_dMaximum = dMaximum;
        m_dStep    = dStep;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool ConstrainedKeyValueMap::TValueRangeConstraint::MatchesForType(KeyValueMap::TValueType eType) const
    {
        switch (eType)
        {
        case KeyValueMap::VALUE_FLOAT:
        case KeyValueMap::VALUE_DOUBLE:
        case KeyValueMap::VALUE_INTEGER:
        case KeyValueMap::VALUE_LONG:
            return true;
        default:
            return false;
        }
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    void ConstrainedKeyValueMap::TValueRangeConstraint::GetRange(double &rdMinimum, double &rdMaximum, double &rdStep) const
    {
        rdMinimum = m_dMinimum;
        rdMaximum = m_dMaximum;
        rdStep    = m_dStep;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    void ConstrainedKeyValueMap::TValueRangeConstraint::SetRange(double dMinimum, double dMaximum, double dStep)
    {
        m_dMinimum = dMinimum;
        m_dMaximum = dMaximum;
        m_dStep    = dStep;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool ConstrainedKeyValueMap::TValueRangeConstraint::TestValue(float fValue) const
    {
        return (fValue <= m_dMaximum && fValue >= m_dMinimum);
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool ConstrainedKeyValueMap::TValueRangeConstraint::TestValue(double dValue) const
    {
        return (dValue <= m_dMaximum && dValue >= m_dMinimum);
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool ConstrainedKeyValueMap::TValueRangeConstraint::TestValue(int iValue) const
    {
        return (iValue <= m_dMaximum && iValue >= m_dMinimum);
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool ConstrainedKeyValueMap::TValueRangeConstraint::TestValue(long lValue) const
    {
        return (lValue <= m_dMaximum && lValue >= m_dMinimum);
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    ConstrainedKeyValueMap::TRestrictedValuesConstraint::TRestrictedValuesConstraint(const std::vector<float> &vfPossibleValues)
    {
        m_eValueType = KeyValueMap::VALUE_FLOAT;
        m_vfValues   = vfPossibleValues;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    ConstrainedKeyValueMap::TRestrictedValuesConstraint::TRestrictedValuesConstraint(const std::vector<double> &vdPossibleValues)
    {
        m_eValueType = KeyValueMap::VALUE_DOUBLE;
        m_vdValues   = vdPossibleValues;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    ConstrainedKeyValueMap::TRestrictedValuesConstraint::TRestrictedValuesConstraint(const std::vector<int> &viPossibleValues)
    {
        m_eValueType = KeyValueMap::VALUE_INTEGER;
        m_viValues   = viPossibleValues;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    ConstrainedKeyValueMap::TRestrictedValuesConstraint::TRestrictedValuesConstraint(const std::vector<long> &vlPossibleValues)
    {
        m_eValueType = KeyValueMap::VALUE_LONG;
        m_vlValues   = vlPossibleValues;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    ConstrainedKeyValueMap::TRestrictedValuesConstraint::TRestrictedValuesConstraint(const std::vector<std::string> &vsPossibleValues)
    {
        m_eValueType = KeyValueMap::VALUE_STRING;
        m_vsValues   = vsPossibleValues;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool ConstrainedKeyValueMap::TRestrictedValuesConstraint::MatchesForType(KeyValueMap::TValueType eType) const
    {
        return (eType == m_eValueType);
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool ConstrainedKeyValueMap::TRestrictedValuesConstraint::TestValue(float fValue) const
    {
        bool bValueFound = false;

        if (m_eValueType == KeyValueMap::VALUE_FLOAT)
            for_each (m_vfValues.begin(), m_vfValues.end(), [&] (float _value) { bValueFound |= (fValue == _value); } );

        return bValueFound;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool ConstrainedKeyValueMap::TRestrictedValuesConstraint::TestValue(double dValue) const
    {
        bool bValueFound = false;

        if (m_eValueType == KeyValueMap::VALUE_DOUBLE)
            for_each (m_vdValues.begin(), m_vdValues.end(), [&] (double _value) { bValueFound |= (dValue == _value); } );

        return bValueFound;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool ConstrainedKeyValueMap::TRestrictedValuesConstraint::TestValue(int iValue) const
    {
        bool bValueFound = false;

        if (m_eValueType == KeyValueMap::VALUE_INTEGER)
            for_each (m_viValues.begin(), m_viValues.end(), [&] (int _value) { bValueFound |= (iValue == _value); } );

        return bValueFound;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool ConstrainedKeyValueMap::TRestrictedValuesConstraint::TestValue(long lValue) const
    {
        bool bValueFound = false;

        if (m_eValueType == KeyValueMap::VALUE_LONG)
            for_each (m_vlValues.begin(), m_vlValues.end(), [&] (long _value) { bValueFound |= (lValue == _value); } );

        return bValueFound;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    bool ConstrainedKeyValueMap::TRestrictedValuesConstraint::TestValue(std::string rsValue) const
    {
        bool bValueFound = false;

        if (m_eValueType == KeyValueMap::VALUE_STRING)
            for_each (m_vsValues.begin(), m_vsValues.end(), [&] (const std::string &_rsValue) { bValueFound |= (_rsValue.compare(rsValue) == 0); } );

        return bValueFound;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    BambooLib::ConstrainedKeyValueMap::TConstraint BambooLib::ConstrainedKeyValueMap::CreateConstraint(std::vector<std::string> vsAllowedValues)
    {
        ConstrainedKeyValueMap::TConstraint NewConstraint;

        NewConstraint.eTypeConstraint   = KeyValueMap::VALUE_STRING;
        NewConstraint.spValueConstraint = std::shared_ptr<IValueConstraint>(new TRestrictedValuesConstraint(vsAllowedValues));

        return NewConstraint;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    BambooLib::ConstrainedKeyValueMap::TConstraint BambooLib::ConstrainedKeyValueMap::CreateConstraint(std::vector<float> vfAllowedValues)
    {
        ConstrainedKeyValueMap::TConstraint NewConstraint;

        NewConstraint.eTypeConstraint   = KeyValueMap::VALUE_FLOAT;
        NewConstraint.spValueConstraint = std::shared_ptr<IValueConstraint>(new TRestrictedValuesConstraint(vfAllowedValues));

        return NewConstraint;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    BambooLib::ConstrainedKeyValueMap::TConstraint BambooLib::ConstrainedKeyValueMap::CreateConstraint(std::vector<double> vdAllowedValues)
    {
        ConstrainedKeyValueMap::TConstraint NewConstraint;

        NewConstraint.eTypeConstraint   = KeyValueMap::VALUE_DOUBLE;
        NewConstraint.spValueConstraint = std::shared_ptr<IValueConstraint>(new TRestrictedValuesConstraint(vdAllowedValues));

        return NewConstraint;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    BambooLib::ConstrainedKeyValueMap::TConstraint BambooLib::ConstrainedKeyValueMap::CreateConstraint(std::vector<int> viAllowedValues)
    {
        ConstrainedKeyValueMap::TConstraint NewConstraint;

        NewConstraint.eTypeConstraint   = KeyValueMap::VALUE_INTEGER;
        NewConstraint.spValueConstraint = std::shared_ptr<IValueConstraint>(new TRestrictedValuesConstraint(viAllowedValues));

        return NewConstraint;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    BambooLib::ConstrainedKeyValueMap::TConstraint BambooLib::ConstrainedKeyValueMap::CreateConstraint(std::vector<long> vlAllowedValues)
    {
        ConstrainedKeyValueMap::TConstraint NewConstraint;

        NewConstraint.eTypeConstraint   = KeyValueMap::VALUE_LONG;
        NewConstraint.spValueConstraint = std::shared_ptr<IValueConstraint>(new TRestrictedValuesConstraint(vlAllowedValues));

        return NewConstraint;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    BambooLib::ConstrainedKeyValueMap::TConstraint BambooLib::ConstrainedKeyValueMap::CreateConstraint(KeyValueMap::TValueType eValueType)
    {
        ConstrainedKeyValueMap::TConstraint NewConstraint;

        NewConstraint.eTypeConstraint   = eValueType;
        NewConstraint.spValueConstraint = std::shared_ptr<IValueConstraint>();

        return NewConstraint;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    ConstrainedKeyValueMap::TConstraint ConstrainedKeyValueMap::CreateConstraint(KeyValueMap::TValueType eValueType, double dMin, double dMax, double dStep)
    {
        ConstrainedKeyValueMap::TConstraint NewConstraint;

        NewConstraint.eTypeConstraint   = eValueType;
        NewConstraint.spValueConstraint = std::shared_ptr<IValueConstraint>(new TValueRangeConstraint(dMin, dMax, dStep));

        return NewConstraint;
    }

    /******************************************************************************************************
     *****************************************************************************************************/
    const ConstrainedKeyValueMap::TConstraint *ConstrainedKeyValueMap::GetConstraint(KeyValueMap::TKey sKey)
    {
        return &m_mConstraints[sKey];
    }

}
