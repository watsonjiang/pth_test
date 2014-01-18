#include "JsScriptPredefinedNameTable.h"

using namespace Js;

ScriptPredefinedNameTable::ScriptPredefinedNameTable()
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptPredefinedNameTable::~ScriptPredefinedNameTable()
{
   // Empty
}

// ----------------------------------------------------------------------------

bool
ScriptPredefinedNameTable::addConstant(
   const std::string& theName,
   ScriptValue   theValue)
{
   if (constantTableM.find(theName) != constantTableM.end())
   {
      // There exists already a constant with the specified name.
      return false;
   }
   constantTableM.insert(ConstantTable::value_type(theName, theValue));
   return true;
}

// ----------------------------------------------------------------------------

bool
ScriptPredefinedNameTable::addConstructorFct(
   const std::string&        theName,
   ScriptConstructorFct theFct,
   int                  theNumberOfAttributes)
{
   if (constructorTableM.find(theName) != constructorTableM.end())
   {
      // There exists already a constructor function with the
      // specified name.
      return false;
   }
   ConstructorEntry entry;
   entry.constructorFctM = theFct;
   entry.numberOfAttributesM = theNumberOfAttributes;
   constructorTableM.insert(ConstructorTable::value_type(theName, entry));
   return true;
}

// ----------------------------------------------------------------------------

bool
ScriptPredefinedNameTable::getConstant(
   const std::string& theName,
   ScriptValue&  theValue) const
{
   ConstantTable::const_iterator iter = constantTableM.find(theName);
   if (iter == constantTableM.end())
   {
      return false;
   }
   theValue = (*iter).second;
   return true;
}

// ----------------------------------------------------------------------------

bool
ScriptPredefinedNameTable::getConstructorFct(
   const std::string&         theName,
   ScriptConstructorFct& theFct,
   int&                  theNumberOfAttributes) const
{
   ConstructorTable::const_iterator iter = constructorTableM.find(theName);
   if (iter == constructorTableM.end())
   {
      return false;
   }
   theFct = (*iter).second.constructorFctM;
   theNumberOfAttributes = (*iter).second.numberOfAttributesM;
   return true;
}

// ----------------------------------------------------------------------------

void
ScriptPredefinedNameTable::removeAllConstructorFcts()
{
   constructorTableM.erase(constructorTableM.begin(), constructorTableM.end());
}

// ----------------------------------------------------------------------------

void
ScriptPredefinedNameTable::removeConstructorFct(
   const std::string& theName)
{
   ConstructorTable::iterator iter = constructorTableM.find(theName);
   if (iter != constructorTableM.end())
   {
      constructorTableM.erase(iter);
   }
}

