#ifndef PALSCANNERUTILITIES_H
#define PALSCANNERUTILITIES_H

#include "JsScannerException.h"
#include "JsScriptDefinitions.h"
#include <ctype.h>

namespace Js
{
   class ScannerUtilities
   {
   public:

      static
      bool
      getExponentPart(
         const char*& theCurrentChar,
         int&         theExponentValue,
         int&         theLine,
         int&         theColumn);

      static
      bool
      getExponentPart(
         const char*& theCurrentChar,
         int&         theExponentValue); 

      static
      bool
      getDecimalDigit(
         const char*& theCurrentChar,
         int&         theDecimalDigit,
         int&         theLine,
         int&         theColumn);

      static
      bool
      getDecimalDigit(
         const char*& theCurrentChar,
         int&         theDecimalDigit);

      static
      Number 
      getDecimalNumber(
         const char*& theCurrentChar,
         int&         theLine,
         int&         theColumn);

      static
      Number
      getDecimalNumber(
         const char*& theCurrentChar);

      static
      int
      getEscapeCharacter(
         const char*& theCurrentChar,
         int&         theLine,
         int&         theColumn);

      static
      bool
      getHexadecimalDigit(
         const char*& theCurrentChar,
         int&         theHexadecimalDigit,
         int&         theLine,
         int&         theColumn);

      static
      bool
      getHexadecimalDigit(
         const char*& theCurrentChar,
         int&         theHexadecimalDigit);

      static
      Number
      getHexadecimalNumber(
         const char*& theCurrentChar,
         int&         theLine,
         int&         theColumn);
      
      static
      Number 
      getHexadecimalNumber(
         const char*& theCurrentChar);
      
      static
      std::string
      getIdentifier(
         const char*& theCurrentChar,
         int&         theLine,
         int&         theColumn);

      static
      void
      getNextChar(
         const char*& theCurrentChar,
         int&         theLine,
         int&         theColumn);

      static
      Number 
      getNumber(
         const char*& theCurrentChar,
         int&         theLine,
         int&         theColumn);

      static
      Number
      getNumber(
         const char*& theCurrentChar);

      static
      std::string
      getString(
         const char*& theCurrentChar,
         int&         theLine,
         int&         theColumn);

      static
      bool
      matchChar(
         const char*& theCurrentChar,
         int          theExpectedChar,
         int&         theLine,
         int&         theColumn);

      static
      bool
      matchChar(
         const char*& theCurrentChar,
         int          theExpectedChar);

      static
      void
      skipMultiLineComment(
         const char*& theCurrentChar,
         int&         theLine,
         int&         theColumn);

      static
      void
      skipSingleLineComment(
         const char*& theCurrentChar,
         int&         theLine,
         int&         theColumn);

      static
      void
      skipSpaces(
         const char*& theCurrentChar,
         int&         theLine,
         int&         theColumn);

      static
      void
      skipSpaces(
         const char*& theCurrentChar);

      static
      void
      skipStringWhiteSpace(
         const char*& theCurrentChar,
         int&         theLine,
         int&         theColumn);

      static
      void
      skipStringWhiteSpace(
         const char*& theCurrentChar);

      static
      void
      skipWhiteSpaceAndComments(
         const char*& theCurrentChar,
         int&         theLine,
         int&         theColumn);
   };

   // -------------------------------------------------------------------------

   inline
   void
   ScannerUtilities::getNextChar(
      const char*& theCurrentChar,
      int&         theLine,
      int&         theColumn)
   {
      theCurrentChar++; // 'eat' current char
      theColumn++;
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScannerUtilities::matchChar(
      const char*& theCurrentChar,
      int          theExpectedChar,
      int&         theLine,
      int&         theColumn)
   {
      if (*theCurrentChar != theExpectedChar)
      {
         return false;
      }
      if (*theCurrentChar == 0x0A)
      {
         // Line Feed
         theLine++;
         theColumn = 1;
      }
      else
      {
         theColumn++;
      }
      theCurrentChar++;
      return true;
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScannerUtilities::matchChar(
      const char*& theCurrentChar,
      int          theExpectedChar)
   {
      if (*theCurrentChar != theExpectedChar)
      {
         return false;
      }
      theCurrentChar++;
      return true;
   }

   // -------------------------------------------------------------------------

   inline
   void
   ScannerUtilities::skipSpaces(
      const char*& theCurrentChar,
      int&         theLine,
      int&         theColumn)
   {
      while (true)
      {
         switch (*(const unsigned char*)theCurrentChar)
         {
            case 0x09: // Tab
            case 0x20: // Space
            case 0xA0: // No-break space
            {
               theCurrentChar++;
               theColumn++;
               break;
            }
            default: // Found some non-space character
            {
               return;
            }
         }
      }
   }

   // -------------------------------------------------------------------------

   inline
   void
   ScannerUtilities::skipSpaces(
      const char*& theCurrentChar)
   {
      while (true)
      {
         switch (*(const unsigned char*)theCurrentChar)
         {
            case 0x09: // Tab
            case 0x20: // Space
            case 0xA0: // No-break space
            {
               theCurrentChar++;
               break;
            }
            default: // Found some non-space character
            {
               return;
            }
         }
      }
   }

}

#endif
