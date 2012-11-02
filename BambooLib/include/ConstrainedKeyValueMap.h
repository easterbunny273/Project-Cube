/*
 * header file for BambooLib::ConstrainedKeyValueMap class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 11/2012, BambooLib
 */


#ifndef CONSTRAINEDKEYVALUEMAP_H
#define CONSTRAINEDKEYVALUEMAP_H

#include "KeyValueMap.h"    // contains a KeyValueMap
#include <map>
#include <memory>
#include <cassert>

namespace BambooLib
{
    /**
    * ConstrainedKeyValueMap A KeyValueMap with constraints
    *
    * A ConstrainedKeyValueMap is a KeyValueMap, which allows to set different
    * restrictions or constraints on specified keys. Only allowed values can be set.
    * Internally, the constrainedKeyValueMap wrapps a KeyValueMap but braces the SetValue() calls.
    *
    */
    class ConstrainedKeyValueMap
    {
    public:
        /*! \name Public types*/
        //@{

        /// An Interface for constraints
        class IValueConstraint
        {
        public:
            /// Returns if the constraint can handle the given type
            virtual bool MatchesForType(KeyValueMap::TValueType eType) const = 0;

            /// Returns TRUE if the given value fulfills the constraint, otherwise FALSE
            virtual bool TestValue(double dValue) const { return false; }

            /// Returns TRUE if the given value fulfills the constraint, otherwise FALSE
            virtual bool TestValue(float fValue) const  { return false; }

            /// Returns TRUE if the given value fulfills the constraint, otherwise FALSE
            virtual bool TestValue(bool bValue) const   { return false; }

            /// Returns TRUE if the given value fulfills the constraint, otherwise FALSE
            virtual bool TestValue(int iValue) const    { return false; }

            /// Returns TRUE if the given value fulfills the constraint, otherwise FALSE
            virtual bool TestValue(long lValue) const   { return false; }

            /// Returns TRUE if the given value fulfills the constraint, otherwise FALSE
            virtual bool TestValue(std::string sValue) const  { return false; }
        };

        /// A concrete class which implements a value range constraint
        class TValueRangeConstraint : public IValueConstraint
        {
        public:
            /// Constructor, ATTENTION: dStep is ignored at the moment
            TValueRangeConstraint(double dMinimum, double dMaximum, double dStep);

            /// Returns if the constraint handles the given type
            virtual bool MatchesForType(KeyValueMap::TValueType eType) const;

            /// Returns the range which is specified as constraint
            void GetRange(double &rdMinimum, double &rdMaximum, double &rdStep) const;

            /// Sets the range, ATTENTION: dStep is ignored at the moment
            void SetRange(double dMinimum, double dMaximum, double dStep);

            /// Returns TRUE if the given value fulfills the constraint, otherwise FALSE
            virtual bool TestValue(float fValue) const;

            /// Returns TRUE if the given value fulfills the constraint, otherwise FALSE
            virtual bool TestValue(int iValue) const;

            /// Returns TRUE if the given value fulfills the constraint, otherwise FALSE
            virtual bool TestValue(long iValue) const;

            /// Returns TRUE if the given value fulfills the constraint, otherwise FALSE
            virtual bool TestValue(double dValue) const;

        private:
            double m_dMinimum, m_dMaximum, m_dStep;
        };

        class TRestrictedValuesConstraint : public IValueConstraint
        {
        public:
            /// Creates a constraint which allows only the given values
            TRestrictedValuesConstraint(const std::vector<float> &vfPossibleValues);

            /// Creates a constraint which allows only the given values
            TRestrictedValuesConstraint(const std::vector<double> &vdPossibleValues);

            /// Creates a constraint which allows only the given values
            TRestrictedValuesConstraint(const std::vector<int> &vlPossibleValues);

            /// Creates a constraint which allows only the given values
            TRestrictedValuesConstraint(const std::vector<long> &vlPossibleValues);

            /// Creates a constraint which allows only the given values
            TRestrictedValuesConstraint(const std::vector<std::string> &vlPossibleValues);

            /// Returns true if the constraint handles the given type
            virtual bool MatchesForType(KeyValueMap::TValueType eType) const;

            /// Returns TRUE if the given value fulfills the constraint, otherwise FALSE
            virtual bool TestValue(float fValue) const;

            /// Returns TRUE if the given value fulfills the constraint, otherwise FALSE
            virtual bool TestValue(double dValue) const;

            /// Returns TRUE if the given value fulfills the constraint, otherwise FALSE
            virtual bool TestValue(int iValue) const;

            /// Returns TRUE if the given value fulfills the constraint, otherwise FALSE
            virtual bool TestValue(long lValue) const;

            /// Returns TRUE if the given value fulfills the constraint, otherwise FALSE
            virtual bool TestValue(std::string rsValue) const;

        private:
            KeyValueMap::TValueType m_eValueType;
            std::vector<float>      m_vfValues;
            std::vector<double>     m_vdValues;
            std::vector<int>        m_viValues;
            std::vector<long>       m_vlValues;
            std::vector<std::string> m_vsValues;
        };

        struct TConstraint
        {
            KeyValueMap::TValueType             eTypeConstraint;
            std::shared_ptr<IValueConstraint>   spValueConstraint;

            TConstraint(KeyValueMap::TValueType _eType,
                        std::shared_ptr<IValueConstraint> _spValueConstraint)
                : eTypeConstraint(_eType), spValueConstraint(_spValueConstraint) {}
            TConstraint() : eTypeConstraint(KeyValueMap::VALUE_UNSET) {}
        };
        //@}

        /*! \name Construction / Destruction */
        //@{
        ConstrainedKeyValueMap(std::map<KeyValueMap::TKey, TConstraint> mConstraints);
        virtual ~ConstrainedKeyValueMap();
        //@}

        /*! \name Helper methods to create constraints */
        //@{
        /// Creates a constraint which restricts the value to a list of given values
        static TConstraint CreateConstraint(std::vector<std::string>   vsAllowedValues);

        /// Creates a constraint which restricts the value to a list of given values
        static TConstraint CreateConstraint(std::vector<long>          vlAllowedValues);

        /// Creates a constraint which restricts the value to a list of given values
        static TConstraint CreateConstraint(std::vector<int>           viAllowedValues);

        /// Creates a constraint which restricts the value to a list of given values
        static TConstraint CreateConstraint(std::vector<double>        vdAllowedValues);

        /// Creates a constraint which restricts the value to a list of given values
        static TConstraint CreateConstraint(std::vector<float>         vfAllowedValues);

        /// Creates a constraint which restricts the type of the value
        static TConstraint CreateConstraint(KeyValueMap::TValueType    eAllowedType);

        /// Creates a constraint which restricts the value range, ATTENTION: dStep is ignored at the moment
        static TConstraint CreateConstraint(KeyValueMap::TValueType    eAllowedType, double dMin, double dMax, double dStep);
        //@}

        /*! \name Add or get keys / values */
        //@{
        /// Returns the constraints for the given key
        const TConstraint * GetConstraint(KeyValueMap::TKey sKey);

        /// Redirects the call to KeyValueMap::SetValue(), if it fulfills the constraints (but it can still fail in KeyValueMap::SetValue() !!!)
        template<class T> bool SetValue(KeyValueMap::TKey sKey, T Value)
        {
            auto iter = m_mConstraints.find(sKey);

            bool bFulfillsConstraint = true;

            if (iter != m_mConstraints.end())
            {
                bFulfillsConstraint = ItlTestTypeConstraint(iter->second.eTypeConstraint, Value);

                if (iter->second.spValueConstraint)
                {
                    assert (iter->second.spValueConstraint->MatchesForType(iter->second.eTypeConstraint));
                    bFulfillsConstraint &= iter->second.spValueConstraint->TestValue(Value);
                }
            }

            bool bOk = bFulfillsConstraint;

            if (bOk)
                bOk &= m_pKeyValueMap->SetValue(sKey, Value);

            return bOk;
        }

        /// Redirects the call to KeyValueMap::GetValue()
        template<class T> bool GetValue(KeyValueMap::TKey sKey, T &rValue)
        {
            return m_pKeyValueMap->GetValue(sKey, rValue);
        }

        /// Redirects the call to KeyValueMap::GetValueOrDefault()
        template<class T> bool GetValueOrDefault(KeyValueMap::TKey sKey, T defaultValue, T &rValue)
        {
            return m_pKeyValueMap->GetValueOrDefault(sKey, defaultValue, rValue);
        }
        //@}

    private:
        /*! \name Private members */
        //@{
            bool ItlTestTypeConstraint(KeyValueMap::TValueType eType, float fValue);
            bool ItlTestTypeConstraint(KeyValueMap::TValueType eType, double dValue);
            bool ItlTestTypeConstraint(KeyValueMap::TValueType eType, int iValue);
            bool ItlTestTypeConstraint(KeyValueMap::TValueType eType, long lValue);
            bool ItlTestTypeConstraint(KeyValueMap::TValueType eType, bool bValue);
            bool ItlTestTypeConstraint(KeyValueMap::TValueType eType, const std::string &rsValue);
        //@}

        /*! \name Private members */
        //@{
        KeyValueMap                                  *   m_pKeyValueMap;
        std::map<KeyValueMap::TKey, TConstraint >     m_mConstraints;
        //@}
    };

}

#endif // CONSTRAINEDKEYVALUEMAP_H
