#ifndef PALSCRIPTSTRING_H
#define PALSCRIPTSTRING_H

#include "JsScriptDefinitions.h"
#include "JsSmartPointer.h"
#include "JsThreadSafeSmartPointerObject.h"
#include <string>

namespace Js
{
   class ScriptString : public std::string,
                        public ThreadSafeSmartPointerObject
   {
   public:

      ScriptString();

      ScriptString(
         const std::string& theOther);

      ScriptString(
         const char* theFirst,
         const char* theLast);

      ScriptString(
         const ScriptString* theOther);

      ScriptString(
         const char* theString);

      ScriptString(const char*  theString,
                   size_t       theNumberOfCharacters);

      virtual
      ~ScriptString();

      /**
       * Read string from the specified C-string.
       */
      void
      read(
         const char*& theCurrentChar, 
         int&         theLine, 
         int&         theColumn);

      /**
       * Convert the string to a value of type Number.
       * @return the Number value.
       */
      Number
      toNumber() const;
   private:

      // --- DISABLE ---

      ScriptString(const ScriptString& theOther);

      const ScriptString&
      operator=(const ScriptString& theRhs);
   };

   typedef SmartPointer<ScriptString> ScriptStringPtr;
   typedef SmartPointer<const ScriptString> ScriptStringConstPtr;
}


#endif
