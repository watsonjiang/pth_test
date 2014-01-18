#ifndef PALSCRIPTPARSER_H
#define PALSCRIPTPARSER_H

#include "JsScriptDefinitions.h"
#include "JsScriptNode.h"
#include "JsScriptNodeArray.h"
#include "JsScriptPredefinedNameTable.h"
#include "JsScriptPreprocessor.h"
#include "JsScriptScanner.h"
#include "JsTime.h"

namespace Js
{
   class ScriptParser
   {
   public:

      ScriptParser(
         const ScriptPredefinedNameTable& thePredefinedNameTable);

      virtual
      ~ScriptParser();

      ScriptNode*
      parse(
         const std::string& theInput);

      ScriptNode*
      parseFile(
         const std::string& theFileName);

      Time
      getFileModificationTime() const;

      const std::string&
      getFilename() const;

      const ScriptPreprocessor::FileMap&
      getIncludedFiles() const;

   private:

      ScriptNode*
      parseAdditiveExpression();

      void
      parseArgumentList(
         ScriptNodeArray& theArgumentList,
         const std::string&    theErrorMessage);

      ScriptNode*
      parseArrayInitialiser();

      ScriptNode*
      parseAssignmentExpression(bool theNoInInd);

      ScriptNode*
      parseBitwiseAndExpression(bool theNoInInd);

      ScriptNode*
      parseBitwiseOrExpression(bool theNoInInd);

      ScriptNode*
      parseBitwiseXorExpression(bool theNoInInd);

      ScriptNode*
      parseBlock();

      ScriptNode*
      parseBreakStatement();

      ScriptNode*
      parseConditionalExpression(bool theNoInInd);

      ScriptNode*
      parseContinueStatement();

      ScriptNode*
      parseEqualityExpression(bool theNoInInd);

      ScriptNode*
      parseExpression(bool theNoInInd);

      ScriptNode*
      parseForVarStatement();

      ScriptNode*
      parseFunctionCall(
         ScriptNode* theNode);

      ScriptNode*
      parseFunctionDeclaration();

      ScriptNode*
      parseFunctionExpression();

      ScriptNode*
      parseIfStatement();

      ScriptNode*
      parseInbuiltFunctionCall(
         const std::string& theIdentifier,
         ScriptNode*   theObject = 0);

      ScriptNode*
      parseIterationStatementDo();

      ScriptNode*
      parseIterationStatementFor();

      ScriptNode*
      parseIterationStatementWhile();

      ScriptNode*
      parseLogicalAndExpression(bool theNoInInd);

      ScriptNode*
      parseLogicalOrExpression(bool theNoInInd);

      ScriptNode*
      parseMemberExpression();

      ScriptNode*
      parseMultiplicativeExpression();

      ScriptNode*
      parseNewInbuiltObject(
         const ScriptString* theFile,
         int                 theLine);

      ScriptNode*
      parseNewMemberExpression();

      ScriptNode*
      parseObjectInitialiser();

      ScriptNode*
      parsePostfixExpression();

      ScriptNode*
      parsePrimaryExpression();

      ScriptNode*
      parseRelationalExpression(bool theNoInInd);

      ScriptNode*
      parseReturnStatement();

      ScriptNode*
      parseShiftExpression();

      ScriptNode*
      parseStatement();

      ScriptNode*
      parseSwitchStatement();

      ScriptNode*
      parseTryStatement();

      ScriptNode*
      parseUnaryExpression();

      ScriptNode*
      parseVarStatement(bool theNoInInd);

      ScriptNode*
      parseWithStatement();

      std::string                      filenameM;
      Time                             fileModificationTimeM;
      bool                             insideFunctionM;
      int                              insideLoopM;
      int                              insideSwitchM;
      const ScriptPredefinedNameTable& predefinedNameTableM;
      ScriptPreprocessor               preprocessorM;
      ScriptScanner                    scannerM;
   };

   // ------------------------------------------------------------------------

   inline
   Time
   ScriptParser::getFileModificationTime() const
   {
      return fileModificationTimeM;
   }

   // ------------------------------------------------------------------------

   inline
   const std::string&
   ScriptParser::getFilename() const
   {
      return filenameM;
   }
}

#endif
