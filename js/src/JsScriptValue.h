#ifndef PALSCRIPTVALUE_H
#define PALSCRIPTVALUE_H

#include "JsScriptDefinitions.h"
#include "JsScriptString.h"
#include <string>
#include <math.h>

namespace Js
{
   class ScriptObject;
   class ScriptReference;

   class ScriptValue
   {
   public:

      typedef enum
      {
         BooleanE         = 0, 
         NullE            = 1,
         NumberE          = 2, 
         ObjectE          = 3,
         ReferenceE       = 4,
         StringE          = 5, 
         UndefinedE       = 6,
         DataTypeMaskE    = 0x000000FF,
         NotDataTypeMaskE = ~DataTypeMaskE
      } DataType;

      typedef enum
      {
         NoneE           = 0 << 8,
         BreakE          = 1 << 8,
         ContinueE       = 2 << 8,
         ReturnE         = 3 << 8,
         CommandMaskE    = 0x0000FF00,
         NotCommandMaskE = ~CommandMaskE
      } Command;

      static const ScriptValue UndefinedC;

      ScriptValue();

      ScriptValue(
         DataType theDataType);

      ScriptValue(
         Command  theCommand,
         DataType theDataType);

      ScriptValue(
         bool theValue);

      ScriptValue(
         Command theCommand,
         bool    theValue);

      ScriptValue(
         Number theValue);
      
      ScriptValue(
         long theValue);

      ScriptValue(
         Command theCommand,
         Number  theValue);

      ScriptValue(
         Command       theCommand,
         const std::string& theValue);

      ScriptValue(
         const std::string& theValue);

      ScriptValue(
         const char* theValue);

      ScriptValue(
         Command     theCommand,
         const char* theValue);

      ScriptValue(
         const ScriptObject* theObject);

      ScriptValue(
         Command             theCommand,
         const ScriptObject* theObject);

      ScriptValue(
         const ScriptObject* theObject,
         const std::string&       thePropertyName);

      ScriptValue(
         Command             theCommand,
         const ScriptObject* theObject,
         const std::string&       thePropertyName);

      ScriptValue(
         const ScriptValue& theOther);

      ScriptValue(
         ScriptValue& theObject,
         ScriptValue& thePropertyName);

      ScriptValue(
         Command      theCommand,
         ScriptValue& theObject,
         ScriptValue& thePropertyName);

      ScriptValue(
         const ScriptString* theString);

      /**
       * Destructor.
       */
      ~ScriptValue();

      /**
       * Assignement operator.
       */
      ScriptValue&
      operator=(
         const ScriptValue& theOther);

      /**
       * Equality operator.
       * @param theOther the other script value.
       * @return 'true' if this value is equal to the other value.
       */
      bool
      operator==(
         bool theOther);

      /**
       * Delete operator.
       * @return 'true'  if this value was a reference and the property
       *                 that it referenced could be deleted,
       *         'false' otherwise.
       */
      ScriptValue
      deleteOperator();

      /**
       * Formats the script value.
       */
      void
      format(
         std::string&       theFormatedValue,
         const std::string& theLinePrefix);

      /**
       * Returns the base object if this is a reference.
       * @return base object if this is a reference, 0 otherwise.
       */
      ScriptObject*
      getBase();

      /**
       * Returns the command that belonngs to the value.
       * @return NoneE, BreakE, ContinueE, ReturnE.
       */
      Command
      getCommand() const;

      /**
       * Returns the data type of the value.
       * @return NumberE, StringE, BooleanE, ObjectE, FunctionE, UndefinedE.
       */
      DataType
      getDataType() const;

      /**
       * Returns the property name if this is a reference.
       * @return property name if this is a reference, an empty string otherwise.
       */
      const std::string&
      getPropertyName() const;

      ScriptValue
      getReferenceValue() const;

      /**
       * Returns the data type of the value as a string.
       * @return the data type of the value as a string.
       */
      ScriptValue
      getTypeof() const;

      /**
       * Returns the value of the property.
       * @return Value of property.
       */
      ScriptValue
      getValue() const;

      /**
       * Checks if the value is a boolean or a reference to a
       * boolean value.
       * @return 'true'  If the value or the referenced value is a boolean, 
       *         'false' otherwise.
       */
      bool
      isBoolean() const;

      /**
       * Checks if the value is null or a reference to a
       * null value.
       * @return 'true'  If the value or the referenced value is null, 
       *         'false' otherwise.
       */
      bool
      isNull() const;

      /**
       * Checks if the value is a number or a reference to a
       * number value.
       * @return 'true'  If the value or the referenced value is a number, 
       *         'false' otherwise.
       */
      bool
      isNumber() const;

      /**
       * Checks if the value is an object or a reference to an
       * object value.
       * @return 'true'  If the value or the referenced value is an object, 
       *         'false' otherwise.
       */
      bool
      isObject() const;

      /**
       * Checks if the value is a reference.
       * @return 'true'  If the value is reference, 
       *         'false' otherwise.
       */
      bool
      isReference() const;

      /**
       * Checks if the value is a string or a reference to a
       * string value.
       * @return 'true'  If the value or the referenced value is a string, 
       *         'false' otherwise.
       */
      bool
      isString() const;

      /**
       * Checks if the value is undefined or a reference to an
       * undefined value.
       * @return 'true'  If the value or the referenced value is undefined, 
       *         'false' otherwise.
       */
      bool
      isUndefined() const;

      /**
       * Formatting according to JSON. The format is compact (i.e. no CRLF)
       * @param theValue The string representation of the ScriptValue.
       */
      void
      jsonFormat(
         std::string&             theFormatedValue,
         const ScriptString* theFile,
         int&                theLine,
         int&                theColumn,
         const std::string&       theFormatOption,
         const std::string&       theLinePrefix);

      /**
       * Sets the value of the property.
       * @param theValue The value of the property.
       */
      void
      putValue(
         const ScriptValue& theValue);

      /**
       * Prints the value to stdout.
       */
      void
      print() const;

      /**
       * Read value from the specified C-string.
       */
      void
      read(
         const char*& theCurrentChar, 
         int&         theLine, 
         int&         theColumn);

      /**
       * Sets the command that belongs to the value to NoneE.
       */
      void
      resetCommand();

      ScriptValue
      shiftRightWithZeroFill(
         ScriptValue& theRhs);

      /**
       * Sets the command that belongs to the value.
       * @param theCommand the command that belongs to the value.
       */
      void
      setCommand(
         Command theCommand);

      /**
       * Convert the value to a value of type Boolean.
       * @return the boolean value.
       */
      bool
      toBoolean();

      /**
       * Convert the value to a value of type Number.
       * @return the number value.
       */
      Number
      toNumber();

      /**
       * Convert the value to a value of type String.
       * @return the string value.
       */
      const std::string&
      toString();

      /**
       * Convert the value to an integral numeric value.
       * @return the integral numeric value.
       */
      Number
      toInteger();

      /**
       * Convert the value to a value of type singed
       * 32 bits integer 
       * @return the Int32 value.
       */
       Int32 
       toInt32();

      /**
       * Convert the value to a value of type unsigned 
       * 32 bits integer 
       * @return the Uint32 value.
       */
       UInt32
       toUInt32();    
 
      /**
       * Convert the value to a value of type String.
       * @return the string value.
       */
      ScriptString*
      toScriptString();

      /**
       * Convert the value to a value of type Object.
       * @return the object value.
       */
      ScriptObject*
      toObject();

      /**
       * Convert the value to a value of type Object.
       * @return the object value.
       */
      const ScriptObject*
      toObject() const;

      ScriptValue
      operator+(
         ScriptValue& theRhs);

      ScriptValue
      operator+(
         Number theRhs);

      ScriptValue
      operator-(
         ScriptValue& theRhs);

      ScriptValue
      operator-(
         Number theRhs);

      ScriptValue
      operator*(
         ScriptValue& theRhs);

      ScriptValue
      operator/(
         ScriptValue& theRhs);

      ScriptValue
      operator%(
         ScriptValue& theRhs);

      /**
       * Equality operator.
       * @param theRhs the right-hand side of the expression.
       * @return 'true'  if this object is equal to the right-hand side,
       *         'false' otherwise.
       */
      ScriptValue
      operator==(
         ScriptValue& theRhs);

      ScriptValue
      operator<(
         ScriptValue& theRhs);

      ScriptValue
      operator<=(
         ScriptValue& theRhs);

      ScriptValue
      operator>(
         ScriptValue& theRhs);

      ScriptValue
      operator>=(
         ScriptValue& theRhs);

      ScriptValue
      operator&(
         ScriptValue& theRhs);

      ScriptValue
      operator^(
         ScriptValue& theRhs);

      ScriptValue
      operator<<(
         ScriptValue& theRhs);

      ScriptValue
      operator|(
         ScriptValue& theRhs);

      ScriptValue
      operator>>(
         ScriptValue& theRhs);

      ScriptValue
      strictlyEqual(
         ScriptValue& theRhs);

      ScriptValue
      operator~();

      ScriptValue
      operator!();

      ScriptValue
      operator-();

   private:

      void
      addRefObjectValue() const;

      void
      addRefReferenceValue() const;

      bool
      convertToBoolean();

      Number
      convertToNumber();

      ScriptObject*
      convertToObject();

      const std::string&
      convertToString();

      ScriptString*
      convertToScriptString();

      void
      releaseObjectValue() const;

      void
      releaseReferenceValue() const;

      unsigned int cmdAndDataTypeM;
      union
      {
         bool             booleanValue;
         Number           numberValue;
         ScriptObject*    objectValue;
         ScriptReference* referenceValue;
         ScriptString*    stringValue;
      } valueM;
   };

   // -------------------------------------------------------------------------

   inline
   ScriptValue::ScriptValue()
   :  cmdAndDataTypeM(UndefinedE)
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue::ScriptValue(
      bool theValue)
   :  cmdAndDataTypeM(BooleanE)   
   {
      valueM.booleanValue = theValue;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue::ScriptValue(
      Command theCommand,
      bool    theValue)
   :  cmdAndDataTypeM(theCommand | BooleanE)   
   {
      valueM.booleanValue = theValue;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue::ScriptValue(
      Number theValue)
   :  cmdAndDataTypeM(NumberE)
   {
      valueM.numberValue = theValue;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue::ScriptValue(
      long theValue)
   :  cmdAndDataTypeM(NumberE)
   {
      valueM.numberValue = (Number)theValue;
   }

   // -------------------------------------------------------------------------


   inline
   ScriptValue::ScriptValue(
      Command theCommand,
      Number  theValue)
   :  cmdAndDataTypeM(theCommand | NumberE)
   {
      valueM.numberValue = theValue;
   }

   // -------------------------------------------------------------------------
   
   inline
   ScriptValue::ScriptValue(
      const std::string& theValue)
   :  cmdAndDataTypeM(StringE)
   {
      valueM.stringValue = new ScriptString(theValue);
      valueM.stringValue->AddRef();
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue::ScriptValue(
      Command       theCommand,
      const std::string& theValue)
   :  cmdAndDataTypeM(theCommand | StringE)
   {
      valueM.stringValue = new ScriptString(theValue);
      valueM.stringValue->AddRef();
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue::ScriptValue(
      const char* theValue)
   :  cmdAndDataTypeM(StringE)
   {
      if (theValue != 0)
      {
         valueM.stringValue = new ScriptString(theValue);
         valueM.stringValue->AddRef();
      }
      else
      {
         cmdAndDataTypeM = NullE;
      }
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue::ScriptValue(
      Command     theCommand,
      const char* theValue)
   :  cmdAndDataTypeM(theCommand | StringE)
   {
      if (theValue != 0)
      {
         valueM.stringValue = new ScriptString(theValue);
         valueM.stringValue->AddRef();
      }
      else
      {
         cmdAndDataTypeM = theCommand | NullE;
      }
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue::ScriptValue(
      DataType theDataType)
   :  cmdAndDataTypeM(theDataType)   
   {
      switch (cmdAndDataTypeM & DataTypeMaskE)
      {
         case BooleanE:
         {
            valueM.booleanValue = false;
            break;
         }
         case NullE:
         {
            break;
         }
         case NumberE:
         {
            valueM.numberValue = 0;
            break;
         }
         case ObjectE:
         {
            valueM.objectValue = 0;
            break;
         }
         case ReferenceE:
         {
            valueM.referenceValue = 0;
            break;
         }
         case StringE:
         {
            valueM.stringValue = 0;
            break;
         }
         case UndefinedE:
         {
            break;
         }
      }
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue::ScriptValue(
      Command  theCommand,
      DataType theDataType)
   :  cmdAndDataTypeM(theCommand | theDataType)   
   {
      switch (theDataType)
      {
         case BooleanE:
         {
            valueM.booleanValue = false;
            break;
         }
         case NullE:
         {
            break;
         }
         case NumberE:
         {
            valueM.numberValue = 0;
            break;
         }
         case ObjectE:
         {
            valueM.objectValue = 0;
            break;
         }
         case ReferenceE:
         {
            valueM.referenceValue = 0;
            break;
         }
         case StringE:
         {
            valueM.stringValue = 0;
            break;
         }
         case UndefinedE:
         {
            break;
         }
         default:
         {
            // empty
         }
      }
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue::ScriptValue(
      const ScriptObject* theObject)
   :  cmdAndDataTypeM(ObjectE)
   {
      if (theObject != 0)
      {
         valueM.objectValue = const_cast<ScriptObject*>(theObject);
         addRefObjectValue();
      }
      else
      {
         cmdAndDataTypeM = NullE;
      }
   }
   
   // -------------------------------------------------------------------------

   inline
   ScriptValue::ScriptValue(
      Command             theCommand,
      const ScriptObject* theObject)
   :  cmdAndDataTypeM(theCommand | ObjectE)
   {
      if (theObject != 0)
      {
         valueM.objectValue = const_cast<ScriptObject*>(theObject);
         addRefObjectValue();
      }
      else
      {
         cmdAndDataTypeM = theCommand | NullE;
      }
   }
   
   // -------------------------------------------------------------------------

   inline
   ScriptValue::ScriptValue(
      const ScriptString* theString)
   :  cmdAndDataTypeM(StringE)
   {
      if (theString != 0)
      {
         valueM.stringValue = const_cast<ScriptString*>(theString);
         valueM.stringValue->AddRef();
      }
      else
      {
         cmdAndDataTypeM = NullE;
      }
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue::ScriptValue(
      const ScriptValue& theOther)
   {
      cmdAndDataTypeM = theOther.cmdAndDataTypeM;
      switch (cmdAndDataTypeM & DataTypeMaskE)
      {
         case BooleanE:
         {
            valueM.booleanValue = theOther.valueM.booleanValue;
            break;
         }
         case NumberE:
         {
            valueM.numberValue = theOther.valueM.numberValue;
            break;
         }
         case ObjectE:
         {
            if (theOther.valueM.objectValue != 0)
            {
               valueM.objectValue = theOther.valueM.objectValue;
               addRefObjectValue();
            }
            else
            {
               cmdAndDataTypeM &= NotDataTypeMaskE;
               cmdAndDataTypeM |= NullE;
            }
            break;
         }
         case ReferenceE:
         {
            if (theOther.valueM.referenceValue != 0)
            {
               valueM.referenceValue = theOther.valueM.referenceValue;
               addRefReferenceValue();
            }
            else
            {
               cmdAndDataTypeM &= NotDataTypeMaskE;
               cmdAndDataTypeM |= NullE;
            }
            break;
         }
         case StringE:
         {
            if (theOther.valueM.stringValue != 0)
            {
               valueM.stringValue = theOther.valueM.stringValue;
               valueM.stringValue->AddRef();
            }
            else
            {
               cmdAndDataTypeM &= NotDataTypeMaskE;
               cmdAndDataTypeM |= NullE;
            }
            break;
         }
      }
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptValue::operator==(
      bool theOther)
   {
      return toBoolean() == theOther;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue::Command
   ScriptValue::getCommand() const
   {
      return static_cast<Command>(cmdAndDataTypeM & CommandMaskE); 
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue::DataType
   ScriptValue::getDataType() const
   {
      return static_cast<DataType>(cmdAndDataTypeM & DataTypeMaskE);
   }

   // -------------------------------------------------------------------------

   inline
   void
   ScriptValue::resetCommand()
   {
      cmdAndDataTypeM &= NotCommandMaskE;
   }

   // -------------------------------------------------------------------------

   inline
   void
   ScriptValue::setCommand(
      Command theCommand)
   {
      cmdAndDataTypeM = (cmdAndDataTypeM & NotCommandMaskE) | theCommand;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptValue::operator+(
      Number theRhs)
   {
      return ScriptValue(toNumber() + theRhs);
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptValue::operator-(
      ScriptValue& theRhs)
   {
      return ScriptValue(toNumber() - theRhs.toNumber());
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptValue::operator-(
      Number theRhs)
   {
      return ScriptValue(toNumber() - theRhs);
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptValue::operator*(
      ScriptValue& theRhs)
   {
      return ScriptValue(toNumber() * theRhs.toNumber());
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptValue::operator/(
      ScriptValue& theRhs)
   {
      if (theRhs.toNumber() == 0)
      {
         return ScriptValue(InfinityC);
      }
      else
      {
         return ScriptValue(toNumber() / theRhs.toNumber());
      }
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptValue::operator%(
      ScriptValue& theRhs)
   {
      if (theRhs.toNumber() == 0)
      {
         return ScriptValue(NaNC);
      }
      else
      {
         return ScriptValue(fmod(toNumber(), theRhs.toNumber()));
      }
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptValue::operator&(
      ScriptValue& theRhs)
   {
      //ECMA262 11.10
      return ScriptValue(toInt32() & theRhs.toInt32());
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptValue::operator|(
      ScriptValue& theRhs)
   {
      //ECMA262 11.10
      return ScriptValue(toInt32() | theRhs.toInt32());
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptValue::operator^(
      ScriptValue& theRhs)
   {
      //ECMA252 11.10
      return ScriptValue(toInt32() ^ theRhs.toInt32());
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptValue::operator<<(
      ScriptValue& theRhs)
   {
      //ECMA262 11.7.1
      Int32 lnum = toInt32();
      UInt32 rnum = theRhs.toUInt32();
   
      return ScriptValue(lnum << 
                         (rnum & 0x1F));
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptValue::operator>>(
      ScriptValue& theRhs)
   {
      // Shift right with sign
      Int32 result;
      Int32 lnum = toInt32();
      UInt32 rnum = toUInt32();
      UInt32 shiftCount = rnum & 0x1F;
      
      if (lnum < 0)
      {
         result = (lnum >> shiftCount | 0x80000000);
      }
      else
      {
         result = (lnum >> shiftCount & 0x7FFFFFFF);
      }
      return ScriptValue((Number)result);
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptValue::shiftRightWithZeroFill(
      ScriptValue& theRhs)
   {
      // Shift right with zero fill 
      unsigned int shift = theRhs.toUInt32() & 0x1F;
      UInt32 result = (toUInt32() >> shift);
      return ScriptValue((Number)result);
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptValue::strictlyEqual(
      ScriptValue& theRhs)
   {
      if ((cmdAndDataTypeM & DataTypeMaskE) == 
          (theRhs.cmdAndDataTypeM & DataTypeMaskE))
      {
         // Operands have same data type
         return *this == theRhs;
      }
      return ScriptValue(false);
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptValue::operator~()
   {
      return ScriptValue(~toInt32());
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptValue::operator!()
   {
      return ScriptValue(!toBoolean());
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptValue::operator-()
   {
      return ScriptValue(-toNumber());
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptValue::getValue() const
   {
      if ((cmdAndDataTypeM & DataTypeMaskE) == ReferenceE)
      {
         return getReferenceValue();
      }
      else
      {
         return *this;
      }
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptValue::isBoolean() const
   {
      DataType dataType = getDataType();
      if (dataType == BooleanE)
      {
         return true;
      }
      else
      if (dataType == ReferenceE)
      {
         ScriptValue value = getValue();
         return value.getDataType() == BooleanE;
      }
      return false;
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptValue::isNull() const
   {
      DataType dataType = getDataType();
      if (dataType == NullE)
      {
         return true;
      }
      else
      if (dataType == ReferenceE)
      {
         ScriptValue value = getValue();
         return value.getDataType() == NullE;
      }
      return false;
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptValue::isNumber() const
   {
      DataType dataType = getDataType();
      if (dataType == NumberE)
      {
         return true;
      }
      else
      if (dataType == ReferenceE)
      {
         ScriptValue value = getValue();
         return value.getDataType() == NumberE;
      }
      return false;
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptValue::isObject() const
   {
      DataType dataType = getDataType();
      if (dataType == ObjectE)
      {
         return true;
      }
      else
      if (dataType == ReferenceE)
      {
         ScriptValue value = getValue();
         return value.getDataType() == ObjectE;
      }
      return false;
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptValue::isReference() const
   {
      return getDataType() == ReferenceE;
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptValue::isString() const
   {
      DataType dataType = getDataType();
      if (dataType == StringE)
      {
         return true;
      }
      else
      if (dataType == ReferenceE)
      {
         ScriptValue value = getValue();
         return value.getDataType() == StringE;
      }
      return false;
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptValue::isUndefined() const
   {
      DataType dataType = getDataType();
      if (dataType == UndefinedE)
      {
         return true;
      }
      else
      if (dataType == ReferenceE)
      {
         ScriptValue value = getValue();
         return value.getDataType() == UndefinedE;
      }
      return false;
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptValue::toBoolean()
   {
      if ((cmdAndDataTypeM & DataTypeMaskE) == BooleanE)
      {
         // See ECMA-262 9.2
         return valueM.booleanValue;
      }
      else
      {
         return convertToBoolean();
      }
   }

   // -------------------------------------------------------------------------

   inline
   Number
   ScriptValue::toNumber()
   {
      if ((cmdAndDataTypeM & DataTypeMaskE) == NumberE)
      {
         // See ECMA-262 9.3
         return valueM.numberValue;
      }
      else
      {
         return convertToNumber();
      }
   }

   // -------------------------------------------------------------------------

   inline
   ScriptObject*
   ScriptValue::toObject()
   {
      if ((cmdAndDataTypeM & DataTypeMaskE) == ObjectE)
      {
         return valueM.objectValue;
      }
      else
      {
         return convertToObject();
      }
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptObject*
   ScriptValue::toObject() const
   {
      if ((cmdAndDataTypeM & DataTypeMaskE) == ObjectE)
      {
         return valueM.objectValue;
      }
      else
      {
         return (const_cast<ScriptValue*>(this))->convertToObject();
      }
   }

   // -------------------------------------------------------------------------

   inline
   ScriptString*
   ScriptValue::toScriptString()
   {
      if ((cmdAndDataTypeM & DataTypeMaskE) == StringE)
      {
         // See ECMA-262 9.8
         return valueM.stringValue;
      }
      else
      {
         return convertToScriptString();
      }
   }

   // -------------------------------------------------------------------------

   inline
   const std::string&
   ScriptValue::toString()
   {
      if ((cmdAndDataTypeM & DataTypeMaskE) == StringE)
      {
         // See ECMA-262 9.8
         return *valueM.stringValue;
      }
      else
      {
         return convertToString();
      }
   }
  
   // -------------------------------------------------------------------------
   
   inline
   Number
   ScriptValue::toInteger()
   {
      //ECMA262 9.4
      Number tmp = toNumber();
      if (tmp == NaNC)
      {
         return 0.0;
      }
      if (tmp == InfinityC)
      {
         return tmp;
      }
      int sign = (tmp > 0) ? 1 : -1;
      tmp = sign * tmp;
      return sign * floor(tmp);    
   }

   // -------------------------------------------------------------------------
   
   inline
   Int32
   ScriptValue::toInt32()
   {
      //ECMA262 9.5
      Number tmp = toNumber();
      if (tmp == NaNC
          ||tmp == InfinityC)
      {
         return 0.0; 
      } 
      Number posInt = ((tmp > 0)? 1 : -1) * floor((tmp > 0) ? tmp : -tmp);
      Number int32bit = fmod(posInt, pow(2, 32));
      if (int32bit >= pow(2, 31))
      {
         return (Int32) (int32bit - pow(2, 31));
      }
      return (Int32) int32bit;
  
   }

   // -------------------------------------------------------------------------

   inline
   UInt32
   ScriptValue::toUInt32()
   {
      //ECMA262 9.6
      Number tmp = toNumber();
      if (tmp == NaNC
          || tmp == InfinityC)
      {
         return 0.0;
      }
      Number posInt = ((tmp > 0)? 1 : -1) * floor((tmp > 0) ? tmp : -tmp);
      Number int32bit = fmod(posInt, pow(2, 32));
      return (UInt32) int32bit;
   } 
}

#endif
