/*
 * header file for BambooLib::KeyValueMap class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 10/2012, BambooLib
 */

#ifndef _BAMBOOLIB_KEYVALUEMAP_H
#define _BAMBOOLIB_KEYVALUEMAP_H

#include <string>
#include <vector>

namespace BambooLib
{
    class KeyValueMap
    {
    public:
        /*! \name Some types */
        //@{
        /// use std::string as keys
        typedef std::string TKey;

        enum TValueType
        {
            VALUE_UNSET,

            VALUE_FLOAT,
            VALUE_DOUBLE,
            VALUE_BOOL,
            VALUE_INTEGER,
            VALUE_LONG,
            VALUE_STRING
        };
        //@}

        /*! \name Construction / Destruction */
        //@{
        /// simple constructor
        KeyValueMap();

        /// constructor which copies another map
        KeyValueMap(const KeyValueMap &rOtherMap);

        /// copy constructor
        KeyValueMap &operator=(const KeyValueMap &rOtherMap);

        /// destructor
        ~KeyValueMap();
        //@}

        /*! \name Add or get keys / values */
        //@{

        /// Sets a double value if no associated value for sKey exists, or if the associated value
        /// is a double. Returns true on success and false if sKey is already associated with another type of value.
        bool SetValue(TKey sKey, double dValue);

        /// Sets a float value if no associated value for sKey exists, or if the associated value
        /// is a float. Returns true on success and false if sKey is already associated with another type of value.
        bool SetValue(TKey sKey, float fValue);

        /// Sets a bool value if no associated value for sKey exists, or if the associated value
        /// is a bool. Returns true on success and false if sKey is already associated with another type of value.
        bool SetValue(TKey sKey, bool bValue);

        /// Sets a int value if no associated value for sKey exists, or if the associated value
        /// is a int. Returns true on success and false if sKey is already associated with another type of value.
        bool SetValue(TKey sKey, int iValue);

        /// Sets a long value if no associated value for sKey exists, or if the associated value
        /// is a long. Returns true on success and false if sKey is already associated with another type of value.
        bool SetValue(TKey sKey, long lValue);

        /// Sets a std::string value if no associated value for sKey exists, or if the associated value
        /// is a std::string. Returns true on success and false if sKey is already associated with another type of value.
        bool SetValue(TKey sKey, const std::string &rsValue);

        /// Sets rdValue to the associated value, if the stored value is a double. Returns true on success and false
        /// if the searched key/value has another type or does not exist.
        bool GetValue(TKey sKey, double &rdValue);

        /// Sets rfValue to the associated value, if the stored value is a float. Returns true on success and false
        /// if the searched key/value has another type or does not exist.
        bool GetValue(TKey sKey, float &rfValue);

        /// Sets rbValue to the associated value, if the stored value is a bool. Returns true on success and false
        /// if the searched key/value has another type or does not exist.
        bool GetValue(TKey sKey, bool &rbValue);

        /// Sets riValue to the associated value, if the stored value is an int. Returns true on success and false
        /// if the searched key/value has another type or does not exist.
        bool GetValue(TKey sKey, int &riValue);

        /// Sets rlValue to the associated value, if the stored value is a long. Returns true on success and false
        /// if the searched key/value has another type or does not exist.
        bool GetValue(TKey sKey, long &rlValue);

        /// Sets rsValue to the associated value, if the stored value is a std::string. Returns true on success and false
        /// if the searched key/value has another type or does not exist.
        bool GetValue(TKey sKey, std::string &rsValue);

        /// Sets rValue to the stored value, if the types are matching. If the value is not found or the types not match,
        /// rValue is set to defaultValue. Returns FALSE if rValue is set to the defaultValue.
        template<class T> bool GetValueOrDefault(TKey sKey, T defaultValue, T &rValue)
        {
            bool bValueFound = GetValue(sKey, rValue);

            if (bValueFound == false)
                rValue = defaultValue;

            return bValueFound;
        }

        /*! \name Get Keys / Types */
        //@{
        /// Returns the type of the stored value for the given key and returns VALUE_UNSET if key does not exist
        TValueType GetType(TKey sKey);

        /// Returns two arrays which describe the current stored key/value pairs (key_names + types)
        std::vector<std::pair<TKey, TValueType> > GetKeysAndTypes();
        //@}

    private:
        /*! \name Internal types */
        //@{
            class TItlPimpl;
        //@}

        /*! \name Internal members */
        //@{
            TItlPimpl   * m_pImpl;
        //@}

    };
}
#endif // KEYVALUEMAP_H
