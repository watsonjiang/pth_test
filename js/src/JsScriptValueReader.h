#ifndef PALSCRIPTVALUEREADER_H_
#define PALSCRIPTVALUEREADER_H_

#include "JsScriptValue.h"
#include <vector>

namespace Js
{
   class ScriptValueReader
   /**
    * Sample usage
    * JSON
    * {
    *    "array": [1,2,3],
    *  "string: "test",
    *  "number" 123,
    *  "flag": true
    * }
    *
    * Js::ScriptValue value;
    * bool isValid;
    * if (readParsedCfg("file.json, isValid, value, errorMessage) && isValid)
    * {
    *    ScriptValueReader wrapper(value)
    *      // access "array"[1]
    *    Number number = wrapper.getObject("array", isValid).getNumber(1, isValid);
    *    if (isValid) {// Useful}

    *      // access "flag"
    *    bool flag = wrapper.getBoolean("flag", isValid, true);
    *    if (isValid) {// Useful}
    * }
    *
    */
   {
   public:

      /**
       * Constructor.
       *
       * @param theValue the ScriptValue returned from PaCfgReader::readParsedCfg()
       */
      ScriptValueReader(Js::ScriptValue theValue);

      // Copy constructor
      ScriptValueReader(const ScriptValueReader& theOther);

      /**
       * Assignment
       */
      ScriptValueReader&
      operator=(const ScriptValueReader& theOther);

      /**
       * Check if the specified array element is a string.
       * @param theIndex of the aray element
       *
       * @return 'true'  if script value is an array and contains the specified element
       *                 and the type of that element is string.
       *         'false' otherwise.
       */
      bool
      arrayElementIsString(
            unsigned theIndex) const;

      /**
      * @param attributeName the name of the attribute having an array value
      *
      * @param isValid. Out parameter assigned to
      *          true  if attribute exists and is an array
      *          false if not
      *
      * @return a ScriptValueReader for the array
      */
      ScriptValueReader
      getArray(
             const std::string& attributeName,
             bool&              isValid) const;

      /**
      * @param index the index of the array element having an array value
      *
      * @param isValid. Out parameter assigned to
      *          true  if element exists and is an array
      *          false if not
      *
      * @return a ScriptValueReader for the array
      */
      ScriptValueReader
      getArray(
             unsigned index,
             bool&    isValid) const;

      /**
       * @param attributeName the name of the attribute
       *
       * @param isValid. Out parameter assigned to
       *          true if attribute was found
       *          false if (boolean) attribute was not found
       *
       * @param defaultValue. Value returned if isValid = false
       *
       * @return value of attribute or defaultValue if not found
       */
      bool
      getBoolean(
            const std::string& attributeName,
            bool&              isValid,
            bool               defaultValue = false) const;

      /**
       * @param index the index of an array element
       *
       * @param isValid. Out parameter assigned to
       *          true  if element was found
       *          false if (boolean) element was not found.
       *                This includes out of index.
       *
       * @param defaultValue. Value returned if isValid = false
       *
       * @return value of attribute or defaultValue if not found
       */
      bool
      getBoolean(
            unsigned index,
            bool&    isValid,
            bool     defaultValue = false) const;

      /**
       * @param isValid. Out parameter assigned to
       *          true  if object is an array
       *          false if not an array
       *
       * @return number of element in the array if isValid, else 0.
       */
      unsigned
      getLength(
            bool& isValid) const;

      /**
       * @param attributeName the name of the attribute
       *
       * @param isValid. Out parameter assigned to
       *          true if attribute was found
       *          false if (number) attribute was not found
       *
       * @param defaultValue. Value returned if isValid = false
       *
       * @return value of attribute or defaultValue if not found
       */
      Js::Number
      getNumber(
            const std::string& attributeName,
            bool&              isValid,
            Js::Number        defaultValue = 0) const;

      /**
       * @param index the index of an array element
       *
       * @param isValid. Out parameter assigned to
       *          true  if element was found
       *          false if (number) element was not found.
       *                This includes out of index.
       *
       * @param defaultValue. Value returned if isValid = false
       *
       * @return value of attribute or defaultValue if not found
       */
      Js::Number
      getNumber(
            unsigned    index,
            bool&       isValid,
            Js::Number defaultValue = 0) const;

      /**
       * @param attributeName the name of the attribute having an object value
       *
       * @param isValid. Out parameter assigned to
       *          true  if attribute exists and is an object
       *          false if not
       *
       * @return a ScriptValueReader for the object
       */
      ScriptValueReader
      getObject(
            const std::string& attributeName,
            bool&              isValid) const;

      /**
       * @param index the index of the array element having an object value
       *
       * @param isValid. Out parameter assigned to
       *          true  if element exists and is an object
       *          false if not
       *
       * @return a ScriptValueReader for the object
       */
      ScriptValueReader
      getObject(
            unsigned index,
            bool&    isValid) const;

      /**
       * @param isValid. Out parameter assigned to
       *          true  if object is an object
       *          false if not an object
       *
       * @return number of properties in the object if isValid, else 0.
       */
      unsigned
      getPropertyCount(
            bool& isValid) const;

      /**
       * @param index the index of the property
       * @param isValid. Out parameter assigned to
       *          true  if found
       *          false if not found, including ouf of boundary
       *
       * @return the name of the property.
       */
      std::string
      getPropertyName(
            unsigned index,
            bool& isValid) const;

      /**
       * @param attributeName the name of the attribute
       *
       * @param isValid. Out parameter assigned to
       *          true if attribute was found
       *          false if (string) attribute was not found
       *
       * @param defaultValue. Value returned if isValid = false
       *
       * @return value of attribute or defaultValue if not found
       */
      std::string
      getString(
            const std::string& attributeName,
            bool&              isValid,
            std::string        defaultValue = "") const;

      /**
       * @param index the index of an array element
       *
       * @param isValid. Out parameter assigned to
       *          true  if element was found
       *          false if (string) element was not found.
       *                This includes out of index.
       *
       * @param defaultValue. Value returned if isValid = false
       *
       * @return value of attribute or defaultValue if not found
       */
      std::string
      getString(
            unsigned    index,
            bool&       isValid,
            std::string defaultValue = "") const;

      /**
       * Get the script value, which is read byt the reader.
       * @return the script value
       */
      Js::ScriptValue
      getValue() const;

      /**
       * Check if the specified property exists.
       * @param thePropertyName the name of the property
       *
       * @return 'true'  if script value is an object and has the specified
       *                 property.
       *         'false' otherwise.
       */
      bool
      hasProperty(
            const std::string& thePropertyName) const;

   private:

      Js::ScriptValue valueM;

      mutable std::vector<std::string> propertyNamesM;
   };

   // ------------------------------------------------------------------------

   inline
   Js::ScriptValue
   ScriptValueReader::getValue() const
   {
      return valueM;
   }
}
#endif /* PALSCRIPTVALUEREADER_H_ */
