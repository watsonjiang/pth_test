#include "JsScannerUtilities.h"

using namespace Js;

// ----------------------------------------------------------------------------
 
bool
ScannerUtilities::getDecimalDigit(
   const char*& theCurrentChar,
   int&         theDecimalDigit,
   int&         theLine,
   int&         theColumn)
{
   switch (*theCurrentChar)
   {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      {
         theDecimalDigit = *theCurrentChar - '0';
         theCurrentChar++; // 'eat' current char
         theColumn++;
         return true;
      }
      default:
      {
         // This is no decimal digit
         return false;
      }
   }
}

// ----------------------------------------------------------------------------
 
bool
ScannerUtilities::getDecimalDigit(
   const char*& theCurrentChar,
   int&         theDecimalDigit)
{
   switch (*theCurrentChar)
   {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      {
         theDecimalDigit = *theCurrentChar - '0';
         theCurrentChar++; // 'eat' current char
         return true;
      }
      default:
      {
         // This is no decimal digit
         return false;
      }
   }
}

// ----------------------------------------------------------------------------

bool
ScannerUtilities::getExponentPart(
   const char*& theCurrentChar,
   int&         theExponentValue, 
   int&         theLine,
   int&         theColumn)
{
   //ECMA262 7.8.3
   //ExponentPart ::
   //  ExponentIndicator SignedInteger
   // 
   //ExponentIndicator :: one of
   //  e E
   //
   //SignedInteger ::
   //  DecimalDigits + DecimalDigits - DecimalDigits  
 
   unsigned int exp = 0;
   int exp_sign = 1;
   const char* pos = theCurrentChar;
   int col = theColumn;
   int line = theLine;
   switch(*pos)
   {
   case 'e':
   case 'E':
      {
         pos++; // 'eat' e/E
         col++;
         switch(*pos)
         {
         case '-':
         case '+':
            pos++;  // 'eat' -/+
            col++;
            exp_sign = ',' - (*pos);  // '+' 43 ',' 44 '-' 45
            break;
         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9':
            {
               int digit;
               while(getDecimalDigit(pos,
                                     digit,
                                     line,
                                     col) == true)
               {
                  exp = exp * 10 + digit;
               }
            }
            break;
         default:
            //this is not an ExponentPart
            return false;
         }
         theExponentValue = exp_sign * exp;
         theCurrentChar = pos;
         theColumn = col; 
         theLine = line;
         return true;
      }
      break;
    default:
      //the is not an ExponentPart
      return false;
   }

}

// ----------------------------------------------------------------------------

bool
ScannerUtilities::getExponentPart(
   const char*& theCurrentChar,
   int&         theExponentValue)
{
   //ECMA262 7.8.3
   //ExponentPart ::
   //  ExponentIndicator SignedInteger
   //
   //ExponentIndicator :: one of
   //  e E
   //
   //SignedInteger ::
   //  DecimalDigits + DecimalDigits - DecimalDigits

   unsigned int exp = 0;
   int exp_sign = 1;
   const char* pos = theCurrentChar;
   switch(*pos)
   {
   case 'e':
   case 'E':
      {
         pos++; // 'eat' e/E
         switch(*pos)
         {
         case '-':
         case '+':
            pos++;  // 'eat' -/+
            exp_sign = ',' - (*pos);  // '+' 43 ',' 44 '-' 45
            break;
         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9':
            {
               int digit;
               while(getDecimalDigit(pos,
                                     digit) == true)
               {
                  exp = exp * 10 + digit;
               }
            }
            break;
         default:
            //this is not an ExponentPart
            return false;
         }
         theExponentValue = exp_sign * exp;
         theCurrentChar = pos;
         return true;
      }
      break;
    default:
      //the is not an ExponentPart
      return false;
   }
} 

// ----------------------------------------------------------------------------
 
Number
ScannerUtilities::getDecimalNumber(
   const char*& theCurrentChar,
   int&         theLine,
   int&         theColumn)
{
   //ECMA262 7.8.3
   //DecimalLiteral ::
   //   DecimalIntegerLiteral . [DecimalDigits] [ExponentPartopti]
   //   . DecimalDigits [ExponentPart]
   //   DecimalIntegerLiteral [ExponentPart]
   Number result = 0;
   switch(*theCurrentChar)
   {
   case '.': 
      { 
         theCurrentChar++; // 'eat' .
         int digit; 
         Number index = 0.1;
         while(getDecimalDigit(theCurrentChar,
                               digit,
                               theLine,
                               theColumn) == true)
         {
            result = result + digit * index;
            index = index * 0.1;
         }
         int exp;
         if(getExponentPart(theCurrentChar,
                            exp,
                            theLine,
                            theColumn) == true)
         { 
            result = result * pow(10, exp);
         }
         return result;
      }  
      break;
   case '0':
   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
   case '7':
   case '8':
   case '9':
      { 
         //DecimalIntegerLiteral
         int digit;
         while (getDecimalDigit(theCurrentChar,
                                digit,
                                theLine, 
                                theColumn) == true)
         {
            result = result * 10 + digit;
         }
         switch(*theCurrentChar)
         {
           case 'e':
           case 'E':
             {
                int exp = 0;
                if(getExponentPart(theCurrentChar,
                                   exp,
                                   theLine,
                                   theColumn) == true)
                {
                   result = result * pow(10, exp);
                }
                else
                {
                   //not followed with an exponent part.
                   //just break and return result.
                }
             }
             break;
           case '.':
             {
                const char * pos = theCurrentChar;
                int line = theLine;
                int col = theColumn;
                pos++; // 'eat' .  
                col++;
                switch(*pos)
                {
                 case '0':
                 case '1':
                 case '2':
                 case '3':
                 case '4':
                 case '5':
                 case '6':
                 case '7':
                 case '8':
                 case '9':
                   {
                      int digit;
                      Number index = 0.1;
                      while(getDecimalDigit(pos,
                                            digit,
                                            line,
                                            col) == true)
                      {
                         result = result + index * digit;
                         index = index * 0.1;     
                      }
                      int exp = 0;
                      if(getExponentPart(pos,
                                         exp,
                                         line,
                                         col) == true)
                      {
                         result = result * pow(10, exp);
                      }
                      theCurrentChar = pos;
                      theLine = line;
                      theColumn = col; 
                   }
                   break;
                 default:
                   // not followed with a decimal digits
                   // uneat . and return.
                   break; 
                }
             }
             break; 
           default:
             {
                // number ended.
                break;   
             }
             break;
         }
      }
      break;
   default:
      //this is not a decimal number. should never reach here.
      break;
   }
 
   return result;
}

// ----------------------------------------------------------------------------
 
Number
ScannerUtilities::getDecimalNumber(
   const char*& theCurrentChar)
{
   //ECMA262 7.8.3
   //DecimalLiteral ::
   //   DecimalIntegerLiteral . [DecimalDigits] [ExponentPartopti]
   //   . DecimalDigits [ExponentPart]
   //   DecimalIntegerLiteral [ExponentPart]
   Number result = 0;
   switch(*theCurrentChar)
   {
   case '.':
      {
         theCurrentChar++; // 'eat' .
         int digit;
         Number index = 0.1;
         while(getDecimalDigit(theCurrentChar,
                               digit) == true)
         {
            result = result + digit * index;
            index = index * 0.1;
         }
         int exp;
         if(getExponentPart(theCurrentChar,
                            exp) == true)
         {
            result = result * pow(10, exp);
         }
         return result;
      }
      break;
   case '0':
   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
   case '7':
   case '8':
   case '9':
      {
         //DecimalIntegerLiteral
         int digit;
         while (getDecimalDigit(theCurrentChar,
                                digit) == true)
         {
            result = result * 10 + digit;
         }
         switch(*theCurrentChar)
         {
           case 'e':
           case 'E':
             {
                int exp = 0;
                if(getExponentPart(theCurrentChar,
                                   exp) == true)
                {
                   result = result * pow(10, exp);
                }
                else
                {
                   //not followed with an exponent part.
                   //just break and return result.
                }
             }
             break;
           case '.':
             {
                const char * pos = theCurrentChar;
                pos++; // 'eat' .
                switch(*pos)
                {
                 case '0':
                 case '1':
                 case '2':
                 case '3':
                 case '4':
                 case '5':
                 case '6':
                 case '7':
                 case '8':
                 case '9':
                   {
                      int digit;
                      Number index = 0.1;
                      while(getDecimalDigit(pos,
                                            digit) == true)
                      {
                         result = result + index * digit;
                         index = index * 0.1;
                      }
                      int exp = 0;
                      if(getExponentPart(pos,
                                         exp) == true)
                      {
                         result = result * pow(10, exp);
                      }
                      theCurrentChar = pos;
                   }
                   break;
                 default:
                   // not followed with a decimal digits
                   // uneat . and return.
                   break;
                }
             }
             break;
           default:
             {
                // number ended.
                break;
             }
             break;
         }
      }
      break;
   default:
      //this is not a decimal number. should never reach here.
      break;
   }

   return result;  
}

// ----------------------------------------------------------------------------
 
int
ScannerUtilities::getEscapeCharacter(
   const char*& theCurrentChar,
   int&         theLine,
   int&         theColumn)
{
   int result;
   theCurrentChar++;   // 'eat' '\'
   theColumn++;
   switch (*theCurrentChar)
   {
      case '0':
      {
         // \0
         result = 0x00;
         break;
      }
      case 'b':
      {
         // \b
         result = 0x08;
         break;
      }
      case 't':
      {
         // \t
         result = 0x09;
         break;
      }
      case 'n':
      {
         // \n
         result = 0x0A;
         break;
      }
      case 'v':
      {
         // \v
         result = 0x0B;
         break;
      }
      case 'f':
      {
         // \f
         result = 0x0C;
         break;
      }
      case 'r':
      {
         // \r
         result = 0x0D;
         break;
      }
      case '"':
      {
         // \"
         result = 0x22;
         break;
      }
      case '\'':
      {
         // \'
         result = 0x27;
         break;
      }
      case '\\':
      {
         // '\\'
         result = 0x5C;               
         break;
      }
      case 'x':
      {
         // \xFF
         theCurrentChar++; // 'eat' 'x'
         theColumn++;
         int digit1;
         if (getHexadecimalDigit(theCurrentChar,
                                 digit1,
                                 theLine, 
                                 theColumn) == false)
         {
            // there is some strange stuff; no digit
            throw ScannerException("Hexadecimal digit expected", 
                                   theLine, 
                                   theColumn);
         }
         int digit2;
         if (getHexadecimalDigit(theCurrentChar,
                                 digit2,
                                 theLine, 
                                 theColumn) == false)
         {
            // there is some strange stuff; no digit
            throw ScannerException("Hexadecimal digit expected", 
                                   theLine, 
                                   theColumn);
         }
         return digit1 * 16 + digit2;               
      }
      default:
      {
         result = *theCurrentChar;
         break;
      }
   }
   theCurrentChar++; // 'eat' current char
   theColumn++;
   return result;      
}

// ----------------------------------------------------------------------------
 
bool
ScannerUtilities::getHexadecimalDigit(
   const char*& theCurrentChar,
   int&         theHexadecimalDigit,
   int&         theLine,
   int&         theColumn)
{
   switch (*theCurrentChar)
   {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      {
         theHexadecimalDigit = *theCurrentChar - '0';
         theCurrentChar++; // 'eat' current char
         theColumn++;
         return true;
      }
      case 'a':
      case 'b':
      case 'c':
      case 'd':
      case 'e':
      case 'f':
      {
         theHexadecimalDigit = *theCurrentChar - 'a' + 10;
         theCurrentChar++; // 'eat' current char
         theColumn++;
         return true;
      }
      case 'A':
      case 'B':
      case 'C':
      case 'D':
      case 'E':
      case 'F':
      {
         theHexadecimalDigit = *theCurrentChar - 'A' + 10;
         theCurrentChar++; // 'eat' current char
         theColumn++;
         return true;
      }
      default:
      {
         // This is no hexadecimal digit
         return false;
      }
   }
}

// ----------------------------------------------------------------------------
 
bool
ScannerUtilities::getHexadecimalDigit(
   const char*& theCurrentChar,
   int&         theHexadecimalDigit)
{
   switch (*theCurrentChar)
   {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      {
         theHexadecimalDigit = *theCurrentChar - '0';
         theCurrentChar++; // 'eat' current char
         return true;
      }
      case 'a':
      case 'b':
      case 'c':
      case 'd':
      case 'e':
      case 'f':
      {
         theHexadecimalDigit = *theCurrentChar - 'a' + 10;
         theCurrentChar++; // 'eat' current char
         return true;
      }
      case 'A':
      case 'B':
      case 'C':
      case 'D':
      case 'E':
      case 'F':
      {
         theHexadecimalDigit = *theCurrentChar - 'A' + 10;
         theCurrentChar++; // 'eat' current char
         return true;
      }
      default:
      {
         // This is no hexadecimal digit
         return false;
      }
   }
}

// ----------------------------------------------------------------------------
 
Number
ScannerUtilities::getHexadecimalNumber(
   const char*& theCurrentChar,
   int&         theLine,
   int&         theColumn)
{
   Number result = 0;
   int digit;
   while (getHexadecimalDigit(theCurrentChar,
                              digit,
                              theLine, 
                              theColumn) == true)
   {
      result = result * 16 + digit;
   }
   return result;
}

// ----------------------------------------------------------------------------
 
Number
ScannerUtilities::getHexadecimalNumber(
   const char*& theCurrentChar)
{
   Number result = 0;
   int digit;
   while (getHexadecimalDigit(theCurrentChar,
                              digit) == true)
   {
      result = result * 16 + digit;
   }
   return result;
}

// ----------------------------------------------------------------------------

std::string
ScannerUtilities::getIdentifier(
   const char*& theCurrentChar,
   int&         theLine,
   int&         theColumn)
{
   const char* firstChar = theCurrentChar;

   do
   {
      theCurrentChar++; // 'eat' current char
      theColumn++;
   }
   while (isalpha(*theCurrentChar) ||
          isdigit(*theCurrentChar) ||
          *theCurrentChar == '$' ||
          *theCurrentChar == '_');
   return std::string(firstChar, theCurrentChar);  
}

// ----------------------------------------------------------------------------

Number
ScannerUtilities::getNumber(
   const char*& theCurrentChar,
   int&         theLine,
   int&         theColumn)
{
   switch (*theCurrentChar)
   {
      case '0':
      {
         theCurrentChar++; // 'eat' 0
         theColumn++;
         if (*theCurrentChar == 'x' ||
             *theCurrentChar == 'X')
         {
            theCurrentChar++; // 'eat' x or X
            theColumn++;
            return getHexadecimalNumber(theCurrentChar, 
                                        theLine, 
                                        theColumn);
         }
         else
         {
            theCurrentChar--; // 'un-eat' 0
            theColumn--;
            return getDecimalNumber(theCurrentChar, 
                                    theLine, 
                                    theColumn);
         }
      }
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      {
         return getDecimalNumber(theCurrentChar, 
                                 theLine, 
                                 theColumn);
      }
      default:
      {
         // there is some strange stuff; no digits
         throw ScannerException("Decimal digit expected", 
                                theLine, 
                                theColumn);
      }
   }
}

// ----------------------------------------------------------------------------

Number
ScannerUtilities::getNumber(
   const char*& theCurrentChar)
{
   switch (*theCurrentChar)
   {
      case '0':
      {
         theCurrentChar++; // 'eat' 0
         if (*theCurrentChar == 'x' ||
             *theCurrentChar == 'X')
         {
            theCurrentChar++; // 'eat' x or X
            return getHexadecimalNumber(theCurrentChar);
         }
         else
         {
            theCurrentChar--; // 'un-eat' 0
            return getDecimalNumber(theCurrentChar);
         }
      }
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      {
         return getDecimalNumber(theCurrentChar);
      }
      default:
      {
         // there is some strange stuff; no digits
         throw ScannerException("Decimal digit expected");
      }
   }
}

// ----------------------------------------------------------------------------
 
std::string
ScannerUtilities::getString(
   const char*& theCurrentChar,
   int&         theLine,
   int&         theColumn)
{
   char stringTerminator = *theCurrentChar;

   theCurrentChar++; // Skip '"' or "'"
   theColumn++;
   const char* firstChar = theCurrentChar;
   std::string result;
   while (true)
   {
      switch (*theCurrentChar)
      {
         case 0x0A: // Line Feed
         case 0x0D: // Carriage Return         
         {
            // there is a line terminator in the string
            throw ScannerException("End-of-line in string", 
                                   theLine, 
                                   theColumn);
         }
         case '"':
         case '\'':
         {
            if (*theCurrentChar == stringTerminator)
            {
               // Found end of String
               result.append(firstChar, theCurrentChar);
               theCurrentChar++;
               theColumn++;
               return result;
            }
            else
            {
               theCurrentChar++;
               theColumn++;
            }
            break;
         }
         case '\\':
         {
            // Found escape sequence
            result.append(firstChar, theCurrentChar);
            result += getEscapeCharacter(theCurrentChar, 
                                         theLine, 
                                         theColumn);
            firstChar = theCurrentChar;
            break;
         }
         default:
         {
            theCurrentChar++;
            theColumn++;
            break;
         }
      }
   }
      
}

// ----------------------------------------------------------------------------

void
ScannerUtilities::skipMultiLineComment(
   const char*& theCurrentChar,
   int&         theLine,
   int&         theColumn)
{
   while (true)
   {
      switch (*theCurrentChar)
      {
         case 0:
         {
            // end-of-input reached
            return;
         }
         case 0x0A: // Line Feed
         {
            theCurrentChar++;
            theLine++;
            theColumn = 1;
            break;
         }
         case 0x0D: // Carriage Return
         {
            theCurrentChar++;
            theColumn++;
            break;
         }
         case '*':
         {
            // end-of-comment reached
            theCurrentChar++;
            theColumn++;
            if (*theCurrentChar == '/')
            {
               // end-of-comment reached
               theCurrentChar++;
               theColumn++;
               return;
            }
            break;
         }         
         default:
         {
            theCurrentChar++;
            theColumn++;
            break;
         }
      }
   }
}

// ----------------------------------------------------------------------------

void
ScannerUtilities::skipSingleLineComment(
   const char*& theCurrentChar,
   int&         theLine,
   int&         theColumn)
{
   while (true)
   {
      switch (*theCurrentChar)
      {
         case 0:
         {
            // end-of-input reached
            return;
         }
         case 0x0A: // Line Feed
         {
            // end-of-comment reached
            theCurrentChar++;
            theLine++;
            theColumn = 1;
            return;
         }
         case 0x0D: // Carriage Return
         {
            // end-of-comment reached
            theCurrentChar++;
            theColumn++;
            return;
         }
         default:
         {
            theCurrentChar++;
            theColumn++;
            break;
         }
      }
   }
}

// ----------------------------------------------------------------------------
 
void
ScannerUtilities::skipStringWhiteSpace(
   const char*& theCurrentChar,
   int&         theLine,
   int&         theColumn)
{
   while (true)
   {
      switch ((unsigned char)*theCurrentChar)
      {
         case 0x09: // Tab
         case 0x0B: // Vertical Tab
         case 0x0C: // Form Feed
         case 0x0D: // Carriage Return         
         case 0x20: // Space
         case 0xA0: // No-break space
         {
            theCurrentChar++;
            theColumn++;
            break;
         }
         case 0x0A: // Line Feed
         {
            theCurrentChar++;
            theLine++;
            theColumn = 1;
            break;
         }
         default: // Found some non-whitespace character
         {
            return;
         }
      }
   }
}

// ----------------------------------------------------------------------------
 
void
ScannerUtilities::skipStringWhiteSpace(
   const char*& theCurrentChar)
{
   while (true)
   {
      switch ((unsigned int)*theCurrentChar)
      {
         case 0x09: // Tab
         case 0x0A: // Line Feed
         case 0x0B: // Vertical Tab
         case 0x0C: // Form Feed
         case 0x0D: // Carriage Return         
         case 0x20: // Space
         case 0xA0: // No-break space
         {
            theCurrentChar++;
            break;
         }
         default: // Found some non-whitespace character
         {
            return;
         }
      }
   }
}

// ----------------------------------------------------------------------------
 
void
ScannerUtilities::skipWhiteSpaceAndComments(
   const char*& theCurrentChar,
   int&         theLine,
   int&         theColumn)
{
   // loop until a real character is found or until end of input is reached
   while (true)
   {
      // skip all white space and line terminator
      skipStringWhiteSpace(theCurrentChar, theLine, theColumn);
  
      // ckeck if there is a comment
      if (*theCurrentChar == '/')
      {
         theCurrentChar++;
         theColumn++;
         if (*theCurrentChar == '/')
         {
            // single line comment - skip until end of line
            theCurrentChar++;
            theColumn++;
            skipSingleLineComment(theCurrentChar, theLine, theColumn);
         }
         else
         if (*theCurrentChar == '*')
         {
            // multi line comment - skip until end of comment
            theCurrentChar++;
            theColumn++;
            skipMultiLineComment(theCurrentChar, theLine, theColumn);
         }
         else
         {
            // previous '/' was a real character - return 
            theCurrentChar--;
            theColumn--;
            return;
         }
      }
      else
      {
         // this is real character
         return;
      }
   }
}

        
        
