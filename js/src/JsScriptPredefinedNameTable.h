#ifndef PALSCRIPTPREDEFINEDNAMETABLE_H
#define PALSCRIPTPREDEFINEDNAMETABLE_H

#include "JsScriptDefinitions.h"
#include "JsScriptValueArray.h"
#include <string>
#include <map>

namespace Js
{
   class ScriptExecutionContext;
   class ScriptObjectArray;
   class ScriptObject;

   typedef ScriptObject* (*ScriptConstructorFct)(
      ScriptExecutionContext* theContext,
      const ScriptValueArray& theArguments);

   class ScriptPredefinedNameTable
   {
   public:

      ScriptPredefinedNameTable();

      virtual
      ~ScriptPredefinedNameTable();

      bool
      addConstant(
         const std::string& theName,
         ScriptValue   theValue);

      bool
      addConstructorFct(
         const std::string&        theName,         
         ScriptConstructorFct theFct,
         int                  theNumberOfAttributes);

      bool
      getConstant(
         const std::string& theName,
         ScriptValue&  theValue) const;

      bool
      getConstructorFct(
         const std::string&         theName,
         ScriptConstructorFct& theFct,
         int&                  theNumberOfAttributes) const;

      void
      removeAllConstructorFcts();

      void
      removeConstructorFct(
         const std::string& theName);

   private:

      struct ConstructorEntry
      {
         ScriptConstructorFct constructorFctM;
         int                  numberOfAttributesM;
      };
   
      typedef std::map<std::string, ConstructorEntry> ConstructorTable;
      typedef std::map<std::string, ScriptValue> ConstantTable;


      ConstantTable    constantTableM;
      ConstructorTable constructorTableM;
   };
}

#endif
