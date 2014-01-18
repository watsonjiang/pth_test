#ifndef PALSCRIPTSCANNER_H
#define PALSCRIPTSCANNER_H

#include "JsScriptDefinitions.h"
#include "JsScriptString.h"

namespace Js
{
   class ScriptScanner
   {
   public:

      enum Token
      {
         AdditionE,
         AdditionAssignmentE,
         AssignmentE,
         BitwiseAndE,
         BitwiseAndAssignmentE,
         BitwiseLeftShiftE,
         BitwiseLeftShiftAssignmentE,
         BitwiseNotE,
         BitwiseOrE,
         BitwiseOrAssignmentE,
         BitwiseRightShiftE,
         BitwiseRightShiftAssignmentE,
         BitwiseRightShiftWithZeroFillE,
         BitwiseRightShiftWithZeroFillAssignmentE,
         BitwiseXorE,
         BitwiseXorAssignmentE,
         BreakE,
         CaseE,
         CatchE,
         ColonE,
         CommaE,
         ContinueE,
         DecrementE,
         DefaultE,
         DeleteE,
         DotE,
         DivisionE,
         DivisionAssignmentE,
         DoE,
         ElseE,
         EofE,
         EqualityE,
         FalseE,
         FinallyE,
         ForE,
         For000E,
         For001E,
         For010E,
         For011E,
         For100E,
         For101E,
         For110E,
         For111E,
         ForInE,
         ForVarE,
         FunctionE,
         GreaterThanE,
         GreaterThanOrEqualToE,
         IdentifierE,
         IfE,
         IfNotE,
         InE,
         IncrementE,
         InequalityE,
         InstanceofE,
         LeftBraceE,
         LeftBracketE,
         LeftParenthesisE,
         LessThanE,
         LessThanOrEqualToE,
         LineE,
         LogicalAndE,
         LogicalNotE,
         LogicalOrE,
         ModulusE,
         ModulusAssignmentE,
         MultiplicationE,
         MultiplicationAssignmentE,
         NewE,
         NewArrayE,
         NewDateE,
         NewErrorE,
         NewObjectE,
         NewReferenceErrorE,
         NewTypeErrorE,
         NullE,
         NumberE,
         PostDecrementE,
         PostIncrementE,
         QuestionmarkE,
         ReturnE,
         RightBraceE,
         RightBracketE,
         RightParenthesisE,
         SemicolonE,
         StrictEqualityE,
         StrictInequalityE,
         StringE,
         SubtractionE,
         SubtractionAssignmentE,
         SwitchE,
         ThisE,
         ThrowE,
         TrueE,
         TryE,
         TryCatchE,
         TryCatchFinallyE,
         TryFinallyE,
         TypeofE,
         UndefinedE,
         VarE,
         VoidE,
         WhileE,
         WithE };

      ScriptScanner();

      /**
       * Get  the current column.
       * @return The current column.
       */
      int
      getColumn() const;

      Token
      getCurrentToken() const;

      /**
       * Get  the current filename.
       * @return The current filename.
       */
      const ScriptString*
      getFilename() const;

      /**
       * Get  the current line number.
       * @return The current line number.
       */
      int
      getLine() const;

      Token
      getNextToken();

      Number 
      getNumberValue() const;

      const std::string&
      getStringValue() const;


      void
      setFilename(
         const std::string& theFilename);

      void
      setInput(
         const std::string& theInput);

   private:

      ScriptScanner(
         const ScriptScanner& theOther);

      Token
      scanKeywordOrIdentifier();

      Token
      scanNextToken();

      void
      scanPreprocessorDirective();

      int                  columnM;
      const char*          currentCharM;
      Token                currentTokenM;
      ScriptStringPtr      filenameM;
      int                  lineM;
      Number               numberValueM;
      std::string          stringValueM;
   };

   // -------------------------------------------------------------------------

   inline
   int
   ScriptScanner::getColumn() const
   {
      return columnM;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptScanner::Token
   ScriptScanner::getCurrentToken() const
   {
      return currentTokenM;
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptString*
   ScriptScanner::getFilename() const
   {
      return filenameM;
   }

   // -------------------------------------------------------------------------

   inline
   int
   ScriptScanner::getLine() const
   {
      return lineM;
   }

   // -------------------------------------------------------------------------

   inline
   Number 
   ScriptScanner::getNumberValue() const
   {
      return numberValueM;
   }

   // -------------------------------------------------------------------------

   inline
   const std::string&
   ScriptScanner::getStringValue() const
   {
      return stringValueM;
   }

   // -------------------------------------------------------------------------

   inline
   void
   ScriptScanner::setFilename(
      const std::string& theFilename)
   {
      filenameM = new ScriptString(theFilename);
   }

}

#endif
