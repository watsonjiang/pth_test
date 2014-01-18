#include "JsScannerException.h"
#include "JsScannerUtilities.h"
#include "JsScriptScanner.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

using namespace Js;

struct KeywordTableEntry
{
   const char*          keyword;
   ScriptScanner::Token token;      
};

// Keyword Table:
// IMPORTANT NOTE: must be alphabetical ordered!!!
static KeywordTableEntry keywordTable[] =
{
   { "$line",      ScriptScanner::LineE       },
   { "break",      ScriptScanner::BreakE      },
   { "case",       ScriptScanner::CaseE       },
   { "catch",      ScriptScanner::CatchE      },
   { "continue",   ScriptScanner::ContinueE   },
   { "default",    ScriptScanner::DefaultE    },
   { "delete",     ScriptScanner::DeleteE     },
   { "do",         ScriptScanner::DoE         },
   { "else",       ScriptScanner::ElseE       },
   { "false",      ScriptScanner::FalseE      },
   { "finally",    ScriptScanner::FinallyE    },
   { "for",        ScriptScanner::ForE        },
   { "function",   ScriptScanner::FunctionE   },
   { "if",         ScriptScanner::IfE         },
   { "in",         ScriptScanner::InE         },
   { "instanceof", ScriptScanner::InstanceofE },
   { "new",        ScriptScanner::NewE        },
   { "null",       ScriptScanner::NullE       },
   { "return",     ScriptScanner::ReturnE     },
   { "switch",     ScriptScanner::SwitchE     },
   { "this",       ScriptScanner::ThisE       },
   { "throw",      ScriptScanner::ThrowE      },
   { "true",       ScriptScanner::TrueE       },
   { "try",        ScriptScanner::TryE        },
   { "typeof",     ScriptScanner::TypeofE     },
   { "undefined",  ScriptScanner::UndefinedE  },
   { "var",        ScriptScanner::VarE        },
   { "void",       ScriptScanner::VoidE       },
   { "while",      ScriptScanner::WhileE      },
   { "with",       ScriptScanner::WithE       }
};

// routine to compare two nodes based on an  
// alphabetical ordering of the keyword field
static 
int
key_compare(
   const void* theKeyword, 
   const void* theKeywordTableEntry) 
{
   return strcmp((const char*)theKeyword,
                 ((const KeywordTableEntry*)theKeywordTableEntry)->keyword);
}

// ----------------------------------------------------------------------------

ScriptScanner::ScriptScanner()
:  columnM(1),
   currentCharM(0),
   filenameM(new Js::ScriptString("<memory>")),
   lineM(1)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptScanner::Token
ScriptScanner::scanNextToken()
{
   while (true)
   {
      ScannerUtilities::skipWhiteSpaceAndComments(currentCharM, 
                                                  lineM, 
                                                  columnM);
      if (*currentCharM != '#')
      {
         break;
      }

      scanPreprocessorDirective();
   }

   if (isalpha(*currentCharM))
   {
      return scanKeywordOrIdentifier();
   }
   else
   if (isdigit(*currentCharM))
   {
      try
      {
         numberValueM = ScannerUtilities::getNumber(currentCharM, 
                                                    lineM, 
                                                    columnM);
      }
      catch (ScannerException& e)
      {
         e.setFile(filenameM);
         throw e;
      }
      return NumberE;
   }
   else
   {
      switch (*currentCharM)
      {
         case 0:
         {
            return EofE;
         }
         case '!':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            if (*currentCharM == '=')
            {
               // !=
               ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
               if (*currentCharM == '=')
               {
                  // !==
                  ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
                  return StrictInequalityE;
               }
               return InequalityE;
            }
            // !
            return LogicalNotE;
         }
         case '"':
         case '\'':
         {
            try
            {
               stringValueM = ScannerUtilities::getString(currentCharM, 
                                                          lineM, 
                                                          columnM);
            }
            catch (ScannerException& e)
            {
               e.setFile(filenameM);
               throw e;
            }
            return StringE;  
         }
         case '$':
         {
            return scanKeywordOrIdentifier();
         }
         case '%':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            if (*currentCharM == '=')
            {
               // %=
               ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
               return ModulusAssignmentE;
            }
            // %
            return ModulusE;
         }
         case '&':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            if (*currentCharM == '&')
            {
               // &&
               ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
               return LogicalAndE;
            }
            else
            if (*currentCharM == '=')
            {
               // &=
               ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
               return BitwiseAndAssignmentE;
            }
            // &
            return BitwiseAndE;
         }
         case '(':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            return LeftParenthesisE;
         }
         case ')':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            return RightParenthesisE;
         }
         case '*':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            if (*currentCharM == '=')
            {
               // *=
               ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
               return MultiplicationAssignmentE;
            }
            // *
            return MultiplicationE;
         }
         case '+':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            if (*currentCharM == '+')
            {
               // ++
               ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
               return IncrementE;
            }
            else
            if (*currentCharM == '=')
            {
               // +=
               ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
               return AdditionAssignmentE;
            }
            // +
            return AdditionE;
         }
         case ',':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            return CommaE;
         }
         case '-':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            if (*currentCharM == '-')
            {
               // --
               ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
               return DecrementE;
            }
            else
            if (*currentCharM == '=')
            {
               // -=
               ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
               return SubtractionAssignmentE;
            }
            // -
            return SubtractionE;
         }
         case '.':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            return DotE;
         }
         case '/':
         {                  
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            if (*currentCharM == '=')
            {
               // /=
               ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
               return DivisionAssignmentE;
            }
            // /
            return DivisionE;
         }
         case ':':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            return ColonE;
         }
         case ';':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            return SemicolonE;
         }
         case '<':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            if (*currentCharM == '=')
            {
               // <=
               ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
               return LessThanOrEqualToE;
            }
            else
            if (*currentCharM == '<')
            {
               ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
               if (*currentCharM == '=')
               {
                  // <<=
                  ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
                  return BitwiseLeftShiftAssignmentE;
               }
               // <<
               return BitwiseLeftShiftE;
            }
            // <
            return LessThanE;
         }
         case '=':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            if (*currentCharM == '=')
            {
               // ==
               ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
               if (*currentCharM == '=')
               {
                  // ===
                  ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
                  return StrictEqualityE;
               }
               return EqualityE;
            }
            // =
            return AssignmentE;
         }
         case '>':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            if (*currentCharM == '=')
            {
               // >=
               ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
               return GreaterThanOrEqualToE;
            }
            else
            if (*currentCharM == '>')
            {
               ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
               if (*currentCharM == '=')
               {
                  // >>=
                  ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
                  return BitwiseRightShiftAssignmentE;
               }
               else
               if (*currentCharM == '>')
               {
                  // >>>
                  ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
                  if (*currentCharM == '=')
                  {
                     // >>>=
                     ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
                     return BitwiseRightShiftWithZeroFillAssignmentE;
                  }
                  return BitwiseRightShiftWithZeroFillE;
               }
               
               return BitwiseRightShiftE;
            }       
            // >
            return GreaterThanE;
         }
         case '?':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            return QuestionmarkE;
         }
         case '[':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            return LeftBracketE;
         }
         case ']':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            return RightBracketE;
         }
         case '^':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            if (*currentCharM == '=')
            {
               // ^=
               ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
               return BitwiseXorAssignmentE;
            }
            // ^
            return BitwiseXorE;
         }
         case '_':
         {
            stringValueM = ScannerUtilities::getIdentifier(currentCharM, 
                                                           lineM, 
                                                           columnM);
            return IdentifierE;  
         }
         case '{':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            return LeftBraceE;
         }
         case '|':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            if (*currentCharM == '|')
            {
               // ||
               ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
               return LogicalOrE;
            }
            else
            if (*currentCharM == '=')
            {
               // |=
               ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
               return BitwiseOrAssignmentE;
            }
            // &
            return BitwiseOrE;
         }
         case '}':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            return RightBraceE;
         }
         case '~':
         {
            ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
            return BitwiseNotE;
         }
         default:
         {
            std::string message("Illegal character '");
            message += *currentCharM;
            message += ".";
            throw ScannerException(message,
                                   filenameM,
                                   lineM, 
                                   columnM);
         }
      }
   }      
}

// ----------------------------------------------------------------------------

void
ScriptScanner::setInput(
   const std::string& theInput)
{
   currentCharM = theInput.c_str();
   columnM = 1;
   lineM = 1;   
}

// ----------------------------------------------------------------------------
// PRIVATE METHODS:
// ----------------------------------------------------------------------------

ScriptScanner::Token
ScriptScanner::getNextToken()
{
   currentTokenM = scanNextToken();
   return currentTokenM;
}

// ----------------------------------------------------------------------------

ScriptScanner::Token
ScriptScanner::scanKeywordOrIdentifier()
{
   stringValueM = ScannerUtilities::getIdentifier(currentCharM, 
                                                  lineM, 
                                                  columnM);

   // use keyword table to check if this is a keyword
   KeywordTableEntry* tableEntry;
   tableEntry = (KeywordTableEntry*)
                bsearch(stringValueM.c_str(),
                        keywordTable, 
                        sizeof(keywordTable) / sizeof(KeywordTableEntry),
                        sizeof(KeywordTableEntry), 
                        key_compare);
   if (tableEntry == 0)
   {
      // this is no keyword, so it must be an identifier
      return IdentifierE;  
   }
   else
   {
      // this is a keyword - return its token
      return tableEntry->token;
   }  
}

// ----------------------------------------------------------------------------

void
ScriptScanner::scanPreprocessorDirective()
{
   // Skip the #
   ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
   
   // Skip spaces between # and directive
   ScannerUtilities::skipSpaces(currentCharM, lineM, columnM);

   // Get the pre-processor directive
   if (!isalpha(*currentCharM))
   {
      ScannerUtilities::skipSingleLineComment(currentCharM, 
                                              lineM, 
                                              columnM);
      return;
   }

   std::string directive = ScannerUtilities::getIdentifier(currentCharM, 
                                                           lineM, 
                                                           columnM);
   if (directive != "line")
   {
      throw Js::ScannerException(
         "Bad pre-processor directive. Expecting '#line'.", 
         filenameM,
         lineM, 
         columnM);
   }

   // Skip spaces between directive and line number and get line number
   ScannerUtilities::skipSpaces(currentCharM, lineM, columnM);

   unsigned int newLine = ScannerUtilities::getDecimalNumber(currentCharM, 
                                                             lineM, 
                                                             columnM);
   // Skip spaces between line number and filename
   ScannerUtilities::skipSpaces(currentCharM, lineM, columnM);

   // Get the filename
   std::string includeFilename;
   if (*currentCharM == '"')
   {
      ScannerUtilities::getNextChar(currentCharM, lineM, columnM);

      while (*currentCharM != '"' &&
             *currentCharM != '\n' &&
             *currentCharM != 0)
      {
         includeFilename += *currentCharM;
         ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
      }

      if (*currentCharM != '"')
      {
         throw Js::ScannerException(
            "Bad pre-processor directive. Expecting \"filename\".", 
            filenameM,
            lineM, 
            columnM);
      }
      ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
   }
   Js::ScannerUtilities::skipSingleLineComment(currentCharM, lineM, columnM);
   lineM = newLine;
   filenameM = new ScriptString(includeFilename);
}
