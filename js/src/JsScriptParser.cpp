#include "JsScannerException.h"
#include "JsScriptNodeArray.h"
#include "JsScriptNodeArrayInitialiser.h"
#include "JsScriptNodeBinaryOperator.h"
#include "JsScriptNodeConstructorCall.h"
#include "JsScriptNodeFunction.h"
#include "JsScriptNodeFunctionCall.h"
#include "JsScriptNodeIdDotLiteral.h"
#include "JsScriptNodeIdentifier.h"
#include "JsScriptNodeInbuiltFunctionCall.h"
#include "JsScriptNodeLiteral.h"
#include "JsScriptNodeNoArgOperation.h"
#include "JsScriptNodeObjectInitialiser.h"
#include "JsScriptNodeQuadrupleOperation.h"
#include "JsScriptNodeStatementBlock.h"
#include "JsScriptNodeSwitch.h"
#include "JsScriptNodeTernaryOperator.h"
#include "JsScriptNodeUnaryOperator.h"
#include "JsScriptNodeVarDeclaration.h"
#include "JsScriptParser.h"
#include <fstream>
#include <string>
#include <unistd.h>

using namespace Js;

ScriptParser::ScriptParser(
   const ScriptPredefinedNameTable& thePredefinedNameTable)
:  insideFunctionM(false),
   insideLoopM(0),
   insideSwitchM(0),
   predefinedNameTableM(thePredefinedNameTable)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptParser::~ScriptParser()
{
   // Empty
}

// ----------------------------------------------------------------------------

const ScriptPreprocessor::FileMap&
ScriptParser::getIncludedFiles() const
{
   return preprocessorM.getIncludedFiles();
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parse(
   const std::string& theInput)
{
   std::string output;
   preprocessorM.preprocess(filenameM, theInput, output);

   scannerM.setInput(output);
   scannerM.getNextToken();

   ScriptNodeStatementBlock* node1 = 
      new ScriptNodeStatementBlock(scannerM.getFilename(),
                                   scannerM.getLine(), 
                                   false, 
                                   ScriptScanner::LeftBraceE);
   while (scannerM.getCurrentToken() != ScriptScanner::EofE)
   {
      if (scannerM.getCurrentToken() == ScriptScanner::FunctionE)
      {
         insideFunctionM = true;
         node1->addStatement(parseFunctionDeclaration());
         insideFunctionM = false;
      }
      else
      {
         node1->addStatement(parseStatement());
      }
   }
   if (node1->size() < 2)
   {
      // remove statement block node when it is empty or contains
      // only one statement
      ScriptNode* result = node1->removeFirstStatement();
      delete node1;
      return result;
   }
   return node1;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseFile(
   const std::string& theFileName)
{
   filenameM = theFileName;
   scannerM.setFilename(filenameM);

   if (access(filenameM.c_str(), F_OK) == -1)
   {
     throw ScannerException( "File does not exist.",
                             scannerM.getFilename(),
                             -1,
                             0);
   }
   if (access(filenameM.c_str(), R_OK) == -1)
   {
     throw ScannerException( "Can not read file, Permission denied.",
                             scannerM.getFilename(),
                             -1,
                             0);
   }
   std::ifstream ifs(filenameM.c_str());
   std::string input((std::istreambuf_iterator<char>(ifs)), 
                     (std::istreambuf_iterator<char>()));
   return parse(input);
}

// ----------------------------------------------------------------------------
// PRIVATE METHODS:
// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseAdditiveExpression()
{
   // ECMA-262 11.6
   // AdditiveExpression:
   //    MultiplicativeExpression
   //    AdditiveExpression + MultiplicativeExpression
   //    AdditiveExpression - MultiplicativeExpression
   ScriptNode* node1 = parseMultiplicativeExpression();
   while (true)
   {
      switch (scannerM.getCurrentToken())
      {
         case ScriptScanner::AdditionE:
         case ScriptScanner::SubtractionE:
         {
            ScriptScanner::Token operation = scannerM.getCurrentToken();
            ScriptStringPtr file = scannerM.getFilename();
            int line = scannerM.getLine();
            scannerM.getNextToken(); // 'eat' operator
            ScriptNode* node2 = parseMultiplicativeExpression();
            node1 = new ScriptNodeBinaryOperator(file,
                                                 line,
                                                 false,
                                                 operation,
                                                 node1,
                                                 node2);  
            break;
         }
         default:
         {
            return node1;
         }       
      }
   }
   // We should never reach this point
   return node1;   
}

// ----------------------------------------------------------------------------

void
ScriptParser::parseArgumentList(
   ScriptNodeArray& theArgumentList,
   const std::string&    theErrorMessage)
{
   theArgumentList.erase(theArgumentList.begin(), theArgumentList.end());

   if (scannerM.getCurrentToken() != ScriptScanner::LeftParenthesisE)
   {
      throw ScannerException(theErrorMessage + " '(' expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
   }
   scannerM.getNextToken(); // 'eat' (

   if (scannerM.getCurrentToken() != ScriptScanner::RightParenthesisE)
   {
      while (true)
      {
         ScriptNode* node1 = parseAssignmentExpression(false);
         theArgumentList.push_back(node1);
         
         if (scannerM.getCurrentToken() != ScriptScanner::CommaE)
         {
            break;
         }

         scannerM.getNextToken(); // 'eat' ,
      }               
   }

   if (scannerM.getCurrentToken() != ScriptScanner::RightParenthesisE)
   {
      throw ScannerException(theErrorMessage + " ')' expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
   }
   scannerM.getNextToken(); // 'eat' )
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseArrayInitialiser()
{
   ScriptNodeArrayInitialiser* arrayInitialiser = 
      new ScriptNodeArrayInitialiser(scannerM.getFilename(),
                                     scannerM.getLine(),
                                     false);
   scannerM.getNextToken();  // 'eat' [

   int index = 0;
   while (true)
   {
      if (scannerM.getCurrentToken() == ScriptScanner::RightBracketE)
      {
         scannerM.getNextToken(); // 'eat' ]
         return arrayInitialiser;
      }
      if (scannerM.getCurrentToken() == ScriptScanner::CommaE)
      {
         scannerM.getNextToken(); // 'eat' ,
         index++;
      }
      else
      {
         ScriptNode* node = parseAssignmentExpression(false);
         arrayInitialiser->addElement(index, node);
         if (scannerM.getCurrentToken() != ScriptScanner::CommaE)
         {
            break;
         }
         scannerM.getNextToken(); // 'eat' ,
         index++;
      }
   }
   if (scannerM.getCurrentToken() != ScriptScanner::RightBracketE)
   {
      throw ScannerException("Syntax error in array initialiser:"
                             " ',' or ']' expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());      
   }
   
   scannerM.getNextToken(); // 'eat' ]
   return arrayInitialiser;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseAssignmentExpression(bool theNoInInd)
{
   // ECMA-262 11.13
   // AssignmentExpression:
   //    ConditionalExpression
   //    LeftHandSideExpression = AssignmentExpression
   //    LeftHandSideExpression *= AssignmentExpression
   //    LeftHandSideExpression /= AssignmentExpression
   //    LeftHandSideExpression %= AssignmentExpression
   //    LeftHandSideExpression += AssignmentExpression
   //    LeftHandSideExpression -= AssignmentExpression
   //    LeftHandSideExpression <<= AssignmentExpression
   //    LeftHandSideExpression >>= AssignmentExpression
   //    LeftHandSideExpression >>>= AssignmentExpression
   //    LeftHandSideExpression &= AssignmentExpression
   //    LeftHandSideExpression ^= AssignmentExpression
   //    LeftHandSideExpression |= AssignmentExpression
   ScriptNode* node1 = parseConditionalExpression(theNoInInd);
   switch (scannerM.getCurrentToken())
   {
      case ScriptScanner::AdditionAssignmentE:
      case ScriptScanner::AssignmentE:
      case ScriptScanner::BitwiseAndAssignmentE:
      case ScriptScanner::BitwiseLeftShiftAssignmentE:
      case ScriptScanner::BitwiseOrAssignmentE:
      case ScriptScanner::BitwiseRightShiftAssignmentE:
      case ScriptScanner::BitwiseRightShiftWithZeroFillAssignmentE:
      case ScriptScanner::BitwiseXorAssignmentE:
      case ScriptScanner::DivisionAssignmentE:
      case ScriptScanner::MultiplicationAssignmentE:
      case ScriptScanner::ModulusAssignmentE:
      case ScriptScanner::SubtractionAssignmentE:
      {
         ScriptScanner::Token operation = scannerM.getCurrentToken();
         
         ScriptStringPtr file = scannerM.getFilename();
         int line = scannerM.getLine();
         scannerM.getNextToken(); // 'eat' assignment operator
         ScriptNode* node2 = parseAssignmentExpression(theNoInInd);
         node1 = new ScriptNodeBinaryOperator(file,
                                              line,
                                              false,
                                              operation,
                                              node1,
                                              node2);         
      }
      default:
      {
         // empty
      }
   }
   
   return node1;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseBitwiseAndExpression(bool theNoInInd)
{
   // ECMA-262 11.10
   // BitwiseANDExpression:
   //    EqualityExpression:
   //    BitwiseANDExpression & EqualityExpression
   ScriptNode* node1 = parseEqualityExpression(theNoInInd);
   while  (scannerM.getCurrentToken() == ScriptScanner::BitwiseAndE)
   {
      ScriptStringPtr file = scannerM.getFilename();
      int line = scannerM.getLine();
      scannerM.getNextToken(); // 'eat' &
      ScriptNode* node2 = parseEqualityExpression(theNoInInd);
      node1 = new ScriptNodeBinaryOperator(file,
                                           line,
                                           false,
                                           ScriptScanner::BitwiseAndE,
                                           node1,
                                           node2);         
   }
   
   return node1;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseBitwiseOrExpression(bool theNoInInd)
{
   // ECMA-262 11.10
   // BitwiseORExpression:
   //    BitwiseXORExpression
   //    BitwiseORExpression | BitwiseXORExpression
   ScriptNode* node1 = parseBitwiseXorExpression(theNoInInd);
   while  (scannerM.getCurrentToken() == ScriptScanner::BitwiseOrE)
   {
      ScriptStringPtr file = scannerM.getFilename();
      int line = scannerM.getLine();
      scannerM.getNextToken(); // 'eat' |
      ScriptNode* node2 = parseBitwiseXorExpression(theNoInInd);
      node1 = new ScriptNodeBinaryOperator(file,
                                           line,
                                           false,
                                           ScriptScanner::BitwiseOrE,
                                           node1,
                                           node2);         
   }
   
   return node1;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseBitwiseXorExpression(bool theNoInInd)
{
   // ECMA-262 11.10
   // BitwiseXORExpression:
   //    BitwiseANDExpression
   //    BitwiseXORExpression ^ BitwiseANDExpression
   ScriptNode* node1 = parseBitwiseAndExpression(theNoInInd);
   while  (scannerM.getCurrentToken() == ScriptScanner::BitwiseXorE)
   {
      ScriptStringPtr file = scannerM.getFilename();
      int line = scannerM.getLine();
      scannerM.getNextToken(); // 'eat' ^
      ScriptNode* node2 = parseBitwiseAndExpression(theNoInInd);
      node1 = new ScriptNodeBinaryOperator(file,
                                           line,
                                           false,
                                           ScriptScanner::BitwiseXorE,
                                           node1,
                                           node2);         
   }
   
   return node1;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseBlock()
{
   // ECMA-262 12.1
   // Block:
   //    { StatementList(opt) }
   ScriptNodeStatementBlock* node1 = 
      new ScriptNodeStatementBlock(scannerM.getFilename(),
                                   scannerM.getLine(),
                                   false,
                                   ScriptScanner::LeftBraceE);
   scannerM.getNextToken(); // 'eat' {
   while (scannerM.getCurrentToken() != ScriptScanner::RightBraceE)
   {
      node1->addStatement(parseStatement());
   }
   scannerM.getNextToken(); // 'eat' }
   if (node1->size() < 2)
   {
      // remove statement block node when it is empty or contains
      // only one statement
      ScriptNode* result = node1->removeFirstStatement();
      delete node1;
      return result;
   }
   return node1;
}       

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseBreakStatement()
{
   if (insideLoopM == 0 && insideSwitchM == 0)
   {
      throw ScannerException(
               "Syntax error in 'break' statement:"
               " 'break' is only allowed within a loop or a switch.", 
               scannerM.getFilename(),
               scannerM.getLine(), 
               scannerM.getColumn());
   }

   ScriptStringPtr file = scannerM.getFilename();
   int line = scannerM.getLine();
   scannerM.getNextToken(); // 'eat' break
   if (scannerM.getCurrentToken() == ScriptScanner::IdentifierE)
   {
      throw ScannerException("Syntax error in 'break' statement:"
                             " 'break Identifier;' is not yet implemented.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
      
   }
   
   if (scannerM.getCurrentToken() != ScriptScanner::SemicolonE)
   {
      throw ScannerException("Syntax error in 'break' statement:"
                             " ';' expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
      
   }
   scannerM.getNextToken(); // 'eat' ;
   ScriptNode* node1 = new ScriptNodeNoArgOperation(file,
                                                    line, 
                                                    false,
                                                    ScriptScanner::BreakE);
   return node1;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseConditionalExpression(bool theNoInInd)
{
   // ECMA-262 11.12
   // ConditionalExpression:
   //    LogicalORExpression
   //    LogicalORExpression ? AssignmentExpression : AssignmentExpression
   ScriptNode* node1 = parseLogicalOrExpression(theNoInInd);
   
   if (scannerM.getCurrentToken() == ScriptScanner::QuestionmarkE)
   {
      ScriptStringPtr file = scannerM.getFilename();
      int line = scannerM.getLine();
      scannerM.getNextToken(); // 'eat' ?
      ScriptNode* node2 = parseAssignmentExpression(theNoInInd);
      if (scannerM.getCurrentToken() != ScriptScanner::ColonE)
      {
         throw ScannerException("Syntax error in conditional expression:"
                                " ':' expected.", 
                                scannerM.getFilename(),
                                scannerM.getLine(), 
                                scannerM.getColumn());
      }
      scannerM.getNextToken(); // 'eat' :
      ScriptNode* node3 = parseAssignmentExpression(theNoInInd);
      node1 = new ScriptNodeTernaryOperator(file,
                                            line,
                                            false,
                                            ScriptScanner::QuestionmarkE,
                                            node1,
                                            node2,
                                            node3);         
   }
   
   return node1;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseContinueStatement()
{
   if (insideLoopM == 0)
   {
      throw ScannerException(
               "Syntax error in 'continue' statement:"
               " 'continue' is only allowed within a loop.", 
               scannerM.getFilename(),
               scannerM.getLine(), 
               scannerM.getColumn());
   }
   ScriptStringPtr file = scannerM.getFilename();
   int line = scannerM.getLine();
   scannerM.getNextToken(); // 'eat' continue
   if (scannerM.getCurrentToken() == ScriptScanner::IdentifierE)
   {
      throw ScannerException("Syntax error in 'continue' statement:"
                             " 'continue Identifier;' is not yet implemented.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
      
   }
   
   if (scannerM.getCurrentToken() != ScriptScanner::SemicolonE)
   {
      throw ScannerException("Syntax error in 'continue' statement:"
                             " ';' expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
      
   }
   scannerM.getNextToken(); // 'eat' ;
   ScriptNode* node1 = new ScriptNodeNoArgOperation(file,
                                                    line, 
                                                    false,
                                                    ScriptScanner::ContinueE);
   return node1;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseEqualityExpression(bool theNoInInd)
{
   // ECMA-262 11.9
   // EqualityExpression:
   //    RelationalExpression
   //    EqualityExpression == RelationalExpression
   //    EqualityExpression != RelationalExpression
   //    EqualityExpression === RelationalExpression
   //    EqualityExpression !== RelationalExpression
   ScriptNode* node1 = parseRelationalExpression(theNoInInd);
   while (true)
   {
      switch (scannerM.getCurrentToken())
      {
         case ScriptScanner::EqualityE:
         case ScriptScanner::InequalityE:
         case ScriptScanner::StrictEqualityE:
         case ScriptScanner::StrictInequalityE:
         {
            ScriptScanner::Token operation = scannerM.getCurrentToken();
            ScriptStringPtr file = scannerM.getFilename();
            int line = scannerM.getLine();
            scannerM.getNextToken(); // 'eat' equality operator
            ScriptNode* node2 = parseRelationalExpression(theNoInInd);
            node1 = new ScriptNodeBinaryOperator(file,
                                                 line,
                                                 false,
                                                 operation,
                                                 node1,
                                                 node2);  
            break;
         }       
         default:
         {
            return node1;
         }       
      }
   }
   // We should never reach this point
   return node1;   
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseExpression(bool theNoInInd)
{
   // ECMA-262 11.14
   // Expression:
   //    Expression , AssignmentExpression
   ScriptNode* node1 = parseAssignmentExpression(theNoInInd);
   if (scannerM.getCurrentToken() != ScriptScanner::CommaE)
   {
      return node1;
   }

   ScriptNodeStatementBlock* statementList = 
      new ScriptNodeStatementBlock(scannerM.getFilename(),
                                   scannerM.getLine(),
                                   false,
                                   ScriptScanner::CommaE);
   statementList->addStatement(node1);

   do
   {
      scannerM.getNextToken(); // 'eat' ,
      ScriptNode* node2 = parseAssignmentExpression(theNoInInd);
      statementList->addStatement(node2);
   }
   while (scannerM.getCurrentToken() == ScriptScanner::CommaE);
   return statementList;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseForVarStatement()
{
   // ECMA-262 12.6
   // IterationStatement:
   //    for ( var VariableDeclarationListNoIn ; Expression(opt) ; Expression(opt) ) Statement
   //    for ( LeftHandSideExpression in Expression ) Statement
   //    for ( var VariableDeclarationNoIn in Expression ) Statement
   //    
   // VariableDeclarationListNoIn
   //    VariableDeclarationNoIn
   //    VariableDeclarationListNoIn , VariableDeclarationNoIn
   //   
   // VariableDeclarationNoIn
   //    Identifier InitialiserNoIn(opt)
   // 
   // InitialiserNoIn:
   //    = AssignmentExpressionNoIn
   ScriptStringPtr file = scannerM.getFilename();
   int line = scannerM.getLine();
   scannerM.getNextToken(); // 'eat' var

   ScriptNodeStatementBlock* statementBlock = 
      new ScriptNodeStatementBlock(file, line, false, ScriptScanner::CommaE);

   while (true)
   {
      if (scannerM.getCurrentToken() != ScriptScanner::IdentifierE)
      {
         throw ScannerException("Syntax error in 'for' 'var' statement:"
                                " Identifier expected.", 
                                scannerM.getFilename(),
                                scannerM.getLine(), 
                                scannerM.getColumn());
      }

      std::string identifierName = scannerM.getStringValue();
      scannerM.getNextToken(); // 'eat' Identifier
      
      ScriptNode* initialValue;
      if (scannerM.getCurrentToken() == ScriptScanner::AssignmentE)
      {
         // There is an initialiser
         scannerM.getNextToken(); // 'eat' =
         initialValue = parseAssignmentExpression(true);
      }
      else
      {
         // There is no initialiser
         initialValue = 0;
      }
      
      ScriptNode* declaration = new ScriptNodeVarDeclaration(
                                       scannerM.getFilename(),
                                       scannerM.getLine(),
                                       ScriptScanner::ForVarE, 
                                       identifierName, 
                                       initialValue);
      statementBlock->addStatement(declaration);
      if (scannerM.getCurrentToken() != ScriptScanner::CommaE)
      {
         if (statementBlock->size() < 2)
         {
            ScriptNode* result = statementBlock->removeFirstStatement();
            delete statementBlock;
            return result;
         }
         return statementBlock;
      }
      scannerM.getNextToken(); // 'eat' ,
   }
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseFunctionCall(
   ScriptNode* theNode)
{
   ScriptNode::Type type = theNode->getType();
   if (type == ScriptNode::IdentifierE)
   {
      std::string identifier = 
         ((ScriptNodeIdentifier*)theNode)->getValue().toString();
      if (identifier == "callback")
      {
         return parseInbuiltFunctionCall(identifier, 0);
      }
      else
      if (identifier == "decodeURI")
      {
         return parseInbuiltFunctionCall(identifier, 0);
      }
      else
      if (identifier == "decodeURIComponent")
      {
         return parseInbuiltFunctionCall(identifier, 0);
      }
      else
      if (identifier == "delayedCallback")
      {
         return parseInbuiltFunctionCall(identifier, 0);
      }
      else
      if (identifier == "encodeURI")
      {
         return parseInbuiltFunctionCall(identifier, 0);
      }
      else
      if (identifier == "encodeURIComponent")
      {
         return parseInbuiltFunctionCall(identifier, 0);
      }
      else
      if (identifier == "format")
      {
         return parseInbuiltFunctionCall(identifier, 0);
      }
      else
      if (identifier == "fromCharCode")
      {
         return parseInbuiltFunctionCall(identifier, 0);
      }
      else
      if (identifier == "isNaN")
      {
         return parseInbuiltFunctionCall(identifier, 0);
      }
      else
      if (identifier == "logInfo")
      {
         return parseInbuiltFunctionCall(identifier, 0);
      }
      else
      if (identifier == "parseInt")
      {
         return parseInbuiltFunctionCall(identifier, 0);
      }
      else
      if (identifier == "rand")
      {
         return parseInbuiltFunctionCall(identifier, 0);
      }
      else
      if (identifier == "readFromFile")
      {
         return parseInbuiltFunctionCall(identifier, 0);
      }
      else
      if (identifier == "jsonFormat")
      {
         return parseInbuiltFunctionCall(identifier, 0);
      }
      else
      if (identifier == "jsonUnformat")
      {
         return parseInbuiltFunctionCall(identifier, 0);
      }
      else
      if (identifier == "writeToFile")
      {
         return parseInbuiltFunctionCall(identifier, 0);
      }
      else
      if (identifier == "print")
      {
         return parseInbuiltFunctionCall(identifier, 0);
      }
   }

   ScriptStringPtr file = scannerM.getFilename();
   int line = scannerM.getLine();
   scannerM.getNextToken(); // 'eat' (

   ScriptNodeFunctionCall* fctCall = 
      new ScriptNodeFunctionCall(file, line, true, theNode);

   // Parse arguments
   if (scannerM.getCurrentToken() != ScriptScanner::RightParenthesisE)
   {
      while (true)
      {
         ScriptNode* node2 = parseAssignmentExpression(false);
         fctCall->addArgument(node2);
            
         if (scannerM.getCurrentToken() != ScriptScanner::CommaE)
         {
            break;
         }

         scannerM.getNextToken(); // 'eat' ,
      }               
   }

   if (scannerM.getCurrentToken() != ScriptScanner::RightParenthesisE)
   {
      throw ScannerException("Syntax error in function call:"
                             " ')' expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
   }
   scannerM.getNextToken(); // 'eat' )
   return fctCall;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseFunctionDeclaration()
{
   // ECMA-262 13
   // FunctionDeclaration:
   //    function Identifier ( FormalParameterList(opt) ) { FunctionBody }
   ScriptStringPtr file = scannerM.getFilename();
   int line = scannerM.getLine();
   scannerM.getNextToken(); // 'eat' function

   if (scannerM.getCurrentToken() != ScriptScanner::IdentifierE)
   {
      throw ScannerException("Syntax error in function declaration:"
                             " Function name expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
      
   }

   ScriptNodeFunction* function = 
      new ScriptNodeFunction(file,
                             line, 
                             false, 
                             scannerM.getStringValue(),
                             true);

   scannerM.getNextToken(); // 'eat' identifier

   if (scannerM.getCurrentToken() != ScriptScanner::LeftParenthesisE)
   {
      throw ScannerException("Syntax error in function declaration:"
                             " Begin of formal parameter list ('(') expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());      
   }

   scannerM.getNextToken(); // 'eat' (
   
   if (scannerM.getCurrentToken() == ScriptScanner::IdentifierE)
   {
      while (true)
      {
         function->addParameter(scannerM.getStringValue());

         scannerM.getNextToken(); // 'eat' Identifier

         if (scannerM.getCurrentToken() != ScriptScanner::CommaE)
         {
            break;
         }
         scannerM.getNextToken(); // 'eat' ,
         if (scannerM.getCurrentToken() != ScriptScanner::IdentifierE)
         {
            throw ScannerException("Syntax error in function declaration:"
                                   " Formal parameter name expected.", 
                                   scannerM.getFilename(),
                                   scannerM.getLine(), 
                                   scannerM.getColumn());
         }
      }
      
   }

   if (scannerM.getCurrentToken() != ScriptScanner::RightParenthesisE)
   {
      throw ScannerException("Syntax error in function declaration:"
                             " End of formal parameter list (')') expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
   }

   scannerM.getNextToken(); // 'eat' )

   if (scannerM.getCurrentToken() != ScriptScanner::LeftBraceE)
   {
      throw ScannerException("Syntax error in function declaration:"
                             " Begin of function body ('{') expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());      
   }
   
   scannerM.getNextToken(); // 'eat' {

   while (scannerM.getCurrentToken() != ScriptScanner::RightBraceE)
   {
      function->addStatement(parseStatement());
   }
   scannerM.getNextToken(); // 'eat' }

   return function;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseFunctionExpression()
{
   // ECMA-262 13
   // FunctionExpression:
   //    function Identifier(opt) ( FormalParameterList(opt) ) { FunctionBody }
   ScriptStringPtr file = scannerM.getFilename();
   int line = scannerM.getLine();
   scannerM.getNextToken(); // 'eat' function

   std::string functionName;
   if (scannerM.getCurrentToken() == ScriptScanner::IdentifierE)
   {
      functionName = scannerM.getStringValue();
      scannerM.getNextToken(); // 'eat' identifier
   }

   ScriptNodeFunction* function = new ScriptNodeFunction(file,
                                                         line, 
                                                         false, 
                                                         functionName,
                                                         false);


   if (scannerM.getCurrentToken() != ScriptScanner::LeftParenthesisE)
   {
      throw ScannerException("Syntax error in function expression:"
                             " Begin of formal parameter list ('(') expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());      
   }

   scannerM.getNextToken(); // 'eat' (
   
   if (scannerM.getCurrentToken() == ScriptScanner::IdentifierE)
   {
      while (true)
      {
         function->addParameter(scannerM.getStringValue());

         scannerM.getNextToken(); // 'eat' Identifier

         if (scannerM.getCurrentToken() != ScriptScanner::CommaE)
         {
            break;
         }
         scannerM.getNextToken(); // 'eat' ,
         if (scannerM.getCurrentToken() != ScriptScanner::IdentifierE)
         {
            throw ScannerException("Syntax error in function expression:"
                                   " Formal parameter name expected.", 
                                   scannerM.getFilename(),
                                   scannerM.getLine(), 
                                   scannerM.getColumn());
         }
      }
      
   }

   if (scannerM.getCurrentToken() != ScriptScanner::RightParenthesisE)
   {
      throw ScannerException("Syntax error in function expression:"
                             " End of formal parameter list (')') expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
   }

   scannerM.getNextToken(); // 'eat' )

   if (scannerM.getCurrentToken() != ScriptScanner::LeftBraceE)
   {
      throw ScannerException("Syntax error in function expression:"
                             " Begin of function body ('{') expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());      
   }
   
   scannerM.getNextToken(); // 'eat' {

   while (scannerM.getCurrentToken() != ScriptScanner::RightBraceE)
   {
      function->addStatement(parseStatement());
   }
   scannerM.getNextToken(); // 'eat' }

   return function;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseInbuiltFunctionCall(
   const std::string& theIdentifier,
   ScriptNode*   theObject)
{
   if (scannerM.getCurrentToken() != ScriptScanner::LeftParenthesisE)
   {
      // This is no function call, but it might be an inbuilt property
      if (theIdentifier == "length")
      {
         ScriptNodeInbuiltFunctionCall* fctCall = 
            new ScriptNodeInbuiltFunctionCall(
                   scannerM.getFilename(),
                   scannerM.getLine(),
                   true,
                   ScriptNodeInbuiltFunctionCall::LengthE,
                   theObject);
         return fctCall;
      }
      return 0;
   }

   // This might be an inbuilt function
   ScriptNodeInbuiltFunctionCall::FunctionType function;

   if (theIdentifier == "base64Decode")
   {
      function = ScriptNodeInbuiltFunctionCall::Base64DecodeE;      
   }
   else
   if (theIdentifier == "base64Encode")
   {
      function = ScriptNodeInbuiltFunctionCall::Base64EncodeE;      
   }
   else
   if (theIdentifier == "callback")
   {
      function = ScriptNodeInbuiltFunctionCall::CallbackE;      
   }
   else
   if (theIdentifier == "charAt")
   {
      function = ScriptNodeInbuiltFunctionCall::CharAtE;
   }
   else
   if (theIdentifier == "charCodeAt")
   {
      function = ScriptNodeInbuiltFunctionCall::CharCodeAtE;
   }
   else
   if (theIdentifier == "concat")
   {
      function = ScriptNodeInbuiltFunctionCall::ConcatE;
   }
   else
   if (theIdentifier == "contains")
   {
      function = ScriptNodeInbuiltFunctionCall::ContainsE;
   }
   else
   if (theIdentifier == "decodeURI")
   {
      function = ScriptNodeInbuiltFunctionCall::DecodeUriE;      
   }
   else
   if (theIdentifier == "decodeURIComponent")
   {
      function = ScriptNodeInbuiltFunctionCall::DecodeUriComponentE;      
   }
   else
   if (theIdentifier == "delayedCallback")
   {
      function = ScriptNodeInbuiltFunctionCall::DelayedCallbackE;      
   }
   else
   if (theIdentifier == "encodeURI")
   {
      function = ScriptNodeInbuiltFunctionCall::EncodeUriE;      
   }
   else
   if (theIdentifier == "encodeURIComponent")
   {
      function = ScriptNodeInbuiltFunctionCall::EncodeUriComponentE;      
   }
   else
   if (theIdentifier == "endsWith")
   {
      function = ScriptNodeInbuiltFunctionCall::EndsWithE;      
   }
   else
   if (theIdentifier == "equals")
   {
      function = ScriptNodeInbuiltFunctionCall::EqualsE;      
   }
   else
   if (theIdentifier == "format")
   {
      function = ScriptNodeInbuiltFunctionCall::FormatE;      
   }
   else
   if (theIdentifier == "fromCharCode")
   {
      function = ScriptNodeInbuiltFunctionCall::FromCharCodeE;      
   }
   else
   if (theIdentifier == "hexDecode")
   {
      function = ScriptNodeInbuiltFunctionCall::HexDecodeE;      
   }
   else
   if (theIdentifier == "hexEncode")
   {
      function = ScriptNodeInbuiltFunctionCall::HexEncodeE;      
   }
   else
   if (theIdentifier == "icontains")
   {
      function = ScriptNodeInbuiltFunctionCall::IcontainsE;
   }
   else
   if (theIdentifier == "iendsWith")
   {
      function = ScriptNodeInbuiltFunctionCall::IendsWithE;      
   }
   else
   if (theIdentifier == "iequals")
   {
      function = ScriptNodeInbuiltFunctionCall::IequalsE;      
   }
   else
   if (theIdentifier == "iindexOf")
   {
      function = ScriptNodeInbuiltFunctionCall::IindexOfE;      
   }
   else
   if (theIdentifier == "ilastIndexOf")
   {
      function = ScriptNodeInbuiltFunctionCall::IlastIndexOfE;      
   }
   else
   if (theIdentifier == "indexOf")
   {
      function = ScriptNodeInbuiltFunctionCall::IndexOfE;      
   }
   else
   if (theIdentifier == "ireplaceAll")
   {
      function = ScriptNodeInbuiltFunctionCall::IreplaceAllE;      
   }
   else
   if (theIdentifier == "ireplaceFirst")
   {
      function = ScriptNodeInbuiltFunctionCall::IreplaceFirstE;      
   }
   else
   if (theIdentifier == "isNaN")
   {
      function = ScriptNodeInbuiltFunctionCall::IsNaNE;      
   }
   else
   if (theIdentifier == "istartsWith")
   {
      function = ScriptNodeInbuiltFunctionCall::IstartsWithE;      
   }
   else
   if (theIdentifier == "lastIndexOf")
   {
      function = ScriptNodeInbuiltFunctionCall::LastIndexOfE;      
   }
   else
   if (theIdentifier == "logInfo")
   {
      function = ScriptNodeInbuiltFunctionCall::LogInfoE;      
   }
   else
   if (theIdentifier == "md5")
   {
      function = ScriptNodeInbuiltFunctionCall::Md5E;      
   }
   else
   if (theIdentifier == "parseInt")
   {
      function = ScriptNodeInbuiltFunctionCall::ParseIntE;      
   }
   else
   if (theIdentifier == "rand")
   {
      function = ScriptNodeInbuiltFunctionCall::RandE;      
   }
   else
   if (theIdentifier == "readFromFile")
   {
      function = ScriptNodeInbuiltFunctionCall::ReadFromFileE;      
   }
   else
   if (theIdentifier == "replaceAll")
   {
      function = ScriptNodeInbuiltFunctionCall::ReplaceAllE;      
   }
   else
   if (theIdentifier == "replaceFirst")
   {
      function = ScriptNodeInbuiltFunctionCall::ReplaceFirstE;      
   }
   else
   if (theIdentifier == "split")
   {
      function = ScriptNodeInbuiltFunctionCall::SplitE;      
   }
   else
   if (theIdentifier == "startsWith")
   {
      function = ScriptNodeInbuiltFunctionCall::StartsWithE;      
   }
   else
   if (theIdentifier == "substr")
   {
      function = ScriptNodeInbuiltFunctionCall::SubstrE;      
   }
   else
   if (theIdentifier == "substring")
   {
      function = ScriptNodeInbuiltFunctionCall::SubstringE;      
   }
   else
   if (theIdentifier == "toLowerCase")
   {
      function = ScriptNodeInbuiltFunctionCall::ToLowerCaseE;      
   }
   else
   if (theIdentifier == "toString")
   {
      function = ScriptNodeInbuiltFunctionCall::ToStringE;      
   }
   else
   if (theIdentifier == "toUpperCase")
   {
      function = ScriptNodeInbuiltFunctionCall::ToUpperCaseE;      
   }
   else
   if (theIdentifier == "trim")
   {
      function = ScriptNodeInbuiltFunctionCall::TrimE;      
   }
   else
   if (theIdentifier == "jsonFormat")
   {
      function = ScriptNodeInbuiltFunctionCall::JsonFormatE;
   }
   else
   if (theIdentifier == "jsonUnformat")
   {
      function = ScriptNodeInbuiltFunctionCall::JsonUnformatE;
   }
   else
   if (theIdentifier == "writeToFile")
   {
      function = ScriptNodeInbuiltFunctionCall::WriteToFileE;      
   }
   else
   if (theIdentifier == "print")
   {
      function = ScriptNodeInbuiltFunctionCall::PrintE;
   }   
   else
   {
      // This is no inbuilt function call
      return 0;
   }

   ScriptStringPtr file = scannerM.getFilename();
   int line = scannerM.getLine();
   int column = scannerM.getColumn();
   scannerM.getNextToken(); // 'eat' (
   ScriptNodeInbuiltFunctionCall* fctCall = 
      new ScriptNodeInbuiltFunctionCall(file,
                                        line,
                                        true,
                                        function,
                                        theObject);

   // Parse arguments
   if (scannerM.getCurrentToken() != ScriptScanner::RightParenthesisE)
   {
      while (true)
      {
         ScriptNode* node2 = parseAssignmentExpression(false);
         fctCall->addArgument(node2);
         
         if (scannerM.getCurrentToken() != ScriptScanner::CommaE)
         {
            break;
         }

         scannerM.getNextToken(); // 'eat' ,
      }               
   }

   if (scannerM.getCurrentToken() != ScriptScanner::RightParenthesisE)
   {
      throw ScannerException("Syntax error in function call:"
                              " ')' expected.", 
                              scannerM.getFilename(),
                              scannerM.getLine(), 
                              scannerM.getColumn());
   }
   scannerM.getNextToken(); // 'eat' )

   switch (function)
   {
      case ScriptNodeInbuiltFunctionCall::Base64DecodeE:
      {
         if (fctCall->getNumberOfArguments() != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'base64Decode':"
               " Function allows no arguments.", 
               file,
               line, 
               column);            
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::Base64EncodeE:
      {
         if (fctCall->getNumberOfArguments() != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'base64Encode':"
               " Function allows no arguments.", 
               file,
               line, 
               column);            
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::CallbackE:
      {
         if (fctCall->getNumberOfArguments() < 1)
         {
            throw ScannerException(
               "Syntax error when calling method 'callback':"
               " At least one argument (callback, [arguments, ...]) is required.", 
               file,
               line, 
               column);            
         }
         if (theObject != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'callback':"
               " Can't apply the global function to an object.", 
               file,
               line, 
               column);            
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::CharAtE:
      {
         if (fctCall->getNumberOfArguments() != 1)
         {
            throw ScannerException(
               "Syntax error when calling method 'charAt':"
               " One argument (pos) is required.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::CharCodeAtE:
      {
         if (fctCall->getNumberOfArguments() != 1)
         {
            throw ScannerException(
               "Syntax error when calling method 'charCodeAt':"
               " One argument (pos) is required.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::ConcatE:
      {
         break;
      }
      case ScriptNodeInbuiltFunctionCall::ContainsE:
      {
         if (fctCall->getNumberOfArguments() < 1 ||
             fctCall->getNumberOfArguments() > 2)
         {
            throw ScannerException(
               "Syntax error when calling method 'contains':"
               " One or two arguments (string [, offset]) are required.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::DecodeUriE:
      {
         if (fctCall->getNumberOfArguments() != 1)
         {
            throw ScannerException(
               "Syntax error when calling method 'decodeURI':"
               " One argument (uri) is required.", 
               file,
               line, 
               column);            
         }
         if (theObject != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'decodeURI':"
               " Can't apply the global function to an object.", 
               file,
               line, 
               column);            
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::DecodeUriComponentE:
      {
         if (fctCall->getNumberOfArguments() != 1)
         {
            throw ScannerException(
               "Syntax error when calling method 'decodeURIComponent':"
               " One argument (string) is required.", 
               file,
               line, 
               column);            
         }
         if (theObject != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'decodeURIComponent':"
               " Can't apply the global function to an object.", 
               file,
               line, 
               column);            
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::DelayedCallbackE:
      {
         if (fctCall->getNumberOfArguments() < 2)
         {
            throw ScannerException(
               "Syntax error when calling method 'delayedCallback':"
               " At least two arguments (milliseconds, callback,"
               " [arguments, ...]) are required.", 
               file,
               line, 
               column);            
         }
         if (theObject != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'delayedCallback':"
               " Can't apply the global function to an object.", 
               file,
               line, 
               column);            
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::EncodeUriE:
      {
         if (fctCall->getNumberOfArguments() != 1)
         {
            throw ScannerException(
               "Syntax error when calling method 'EncodeURI':"
               " One argument (uri) is required.", 
               file,
               line, 
               column);            
         }
         if (theObject != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'encodeURI':"
               " Can't apply the global function to an object.", 
               file,
               line, 
               column);            
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::EncodeUriComponentE:
      {
         if (fctCall->getNumberOfArguments() != 1)
         {
            throw ScannerException(
               "Syntax error when calling method 'encodeURIComponent':"
               " One argument (string) is required.", 
               file,
               line, 
               column);            
         }
         if (theObject != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'encodeURIComponent':"
               " Can't apply the global function to an object.", 
               file,
               line, 
               column);            
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::EndsWithE:
      {
         if (fctCall->getNumberOfArguments() != 1)
         {
            throw ScannerException(
               "Syntax error when calling method 'endsWith':"
               " One argument (suffix) is required.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::EqualsE:
      {
         if (fctCall->getNumberOfArguments() != 1)
         {
            throw ScannerException(
               "Syntax error when calling method 'equals':"
               " One argument (string) is required.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::FormatE:
      {
         if (fctCall->getNumberOfArguments() < 1 ||
             fctCall->getNumberOfArguments() > 2 )
         {
            throw ScannerException(
               "Syntax error when calling method 'format':"
               " One or two arguments (value [, line-prefix]) are required.", 
               file,
               line, 
               column);            
         }
         if (theObject != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'format':"
               " Can't apply the global function to an object.", 
               file,
               line, 
               column);            
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::FromCharCodeE:
      {
         if (theObject != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'fromCharCode':"
               " Can't apply the global function to an object.", 
               file,
               line, 
               column);            
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::HexDecodeE:
      {
         if (fctCall->getNumberOfArguments() != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'hexDecode':"
               " Function allows no arguments.", 
               file,
               line, 
               column);            
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::HexEncodeE:
      {
         if (fctCall->getNumberOfArguments() != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'hexEncode':"
               " Function allows no arguments.", 
               file,
               line, 
               column);            
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::IcontainsE:
      {
         if (fctCall->getNumberOfArguments() < 1 ||
             fctCall->getNumberOfArguments() > 2)
         {
            throw ScannerException(
               "Syntax error when calling method 'icontains':"
               " One or two arguments (string [, offset]) are required.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::IendsWithE:
      {
         if (fctCall->getNumberOfArguments() != 1)
         {
            throw ScannerException(
               "Syntax error when calling method 'iendsWith':"
               " One argument (suffix) is required.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::IequalsE:
      {
         if (fctCall->getNumberOfArguments() != 1)
         {
            throw ScannerException(
               "Syntax error when calling method 'iequals':"
               " One argument (string) is required.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::IindexOfE:
      {
         if (fctCall->getNumberOfArguments() < 1 ||
             fctCall->getNumberOfArguments() > 2)
         {
            throw ScannerException(
               "Syntax error when calling method 'iindexOf':"
               " One or two arguments (substring [, offset]) are required.", 
               file,
               line, 
               column);
         }
         break;
      }      
      case ScriptNodeInbuiltFunctionCall::IlastIndexOfE:
      {
         if (fctCall->getNumberOfArguments() < 1 ||
             fctCall->getNumberOfArguments() > 2)
         {
            throw ScannerException(
               "Syntax error when calling method 'ilastIndexOf':"
               " One or two arguments (substring [, offset]) are required.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::IndexOfE:
      {
         if (fctCall->getNumberOfArguments() < 1 ||
             fctCall->getNumberOfArguments() > 2)
         {
            throw ScannerException(
               "Syntax error when calling method 'indexOf':"
               " One or two arguments (substring [, offset]) are required.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::IreplaceAllE:
      {
         if (fctCall->getNumberOfArguments() != 2)
         {
            throw ScannerException(
               "Syntax error when calling method 'ireplaceAll':"
               " Two arguments (searchValue, replaceValue) are required.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::IreplaceFirstE:
      {
         if (fctCall->getNumberOfArguments() != 2)
         {
            throw ScannerException(
               "Syntax error when calling method 'ireplaceFirst':"
               " Two arguments (searchValue, replaceValue) are required.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::IsNaNE:
      {
         if (fctCall->getNumberOfArguments() != 1)
         {
            throw ScannerException(
               "Syntax error when calling method 'isNaN':"
               " One argument ( number ) is required.", 
               file,
               line, 
               column);
         }
         if (theObject != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'isNaN':"
               " Can't apply the global function to an object.", 
               file,
               line, 
               column);            
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::IstartsWithE:
      {
         if (fctCall->getNumberOfArguments() < 1 ||
             fctCall->getNumberOfArguments() > 2)
         {
            throw ScannerException(
               "Syntax error when calling method 'istartsWith':"
               " One or two arguments (prefix [, offset]) are required.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::LastIndexOfE:
      {
         if (fctCall->getNumberOfArguments() < 1 ||
             fctCall->getNumberOfArguments() > 2)
         {
            throw ScannerException(
               "Syntax error when calling method 'lastIndexOf':"
               " One or two arguments (substring [, offset]) are required.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::LogInfoE:
      {
         if (fctCall->getNumberOfArguments() != 2)
         {
            throw ScannerException(
               "Syntax error when calling method 'logInfo':"
               " Two arguments (groupId, freeText) are required.", 
               file,
               line, 
               column);            
         }
         if (theObject != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'logInfo':"
               " Can't apply the global function to an object.", 
               file,
               line, 
               column);            
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::Md5E:
      {
         if (fctCall->getNumberOfArguments() > 1)
         {
            throw ScannerException(
               "Syntax error when calling method 'md5':"
               " Zero or one arguments allowed ([encoding]).", 
               file,
               line, 
               column);            
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::ParseIntE:
      {
         if (fctCall->getNumberOfArguments() < 1 ||
             fctCall->getNumberOfArguments() > 2 )
         {
            throw ScannerException(
               "Syntax error when calling method 'parseInt':"
               " One or two arguments (string [, radix]) are required.", 
               file,
               line, 
               column);            
         }
         if (theObject != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'parseInt':"
               " Can't apply the global function to an object.", 
               file,
               line, 
               column);            
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::RandE:
      {
         if (fctCall->getNumberOfArguments() == 1 ||
             fctCall->getNumberOfArguments() > 2 )
         {
            throw ScannerException(
               "Syntax error when calling method 'rand':"
               " Zero or two arguments ([startValue, endValue]) are required.", 
               file,
               line, 
               column);            
         }
         if (theObject != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'rand':"
               " Can't apply the global function to an object.", 
               file,
               line, 
               column);            
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::ReadFromFileE:
      {
         if (fctCall->getNumberOfArguments() != 1)
         {
            throw ScannerException(
               "Syntax error when calling method 'readFromFile':"
               " One argument (filename) is required.", 
               file,
               line, 
               column);            
         }
         if (theObject != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'readFromFile':"
               " Can't apply the global function to an object.", 
               file,
               line, 
               column);            
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::ReplaceAllE:
      {
         if (fctCall->getNumberOfArguments() != 2)
         {
            throw ScannerException(
               "Syntax error when calling method 'replaceAll':"
               " Two arguments (searchValue, replaceValue) are required.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::ReplaceFirstE:
      {
         if (fctCall->getNumberOfArguments() != 2)
         {
            throw ScannerException(
               "Syntax error when calling method 'replaceFirst':"
               " Two arguments (searchValue, replaceValue) are required.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::SplitE:
      {
         if (fctCall->getNumberOfArguments() < 1 ||
             fctCall->getNumberOfArguments() > 2)
         {
            throw ScannerException(
               "Syntax error when calling method 'split':"
               " One or two arguments (separator [, limit]) are required.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::StartsWithE:
      {
         if (fctCall->getNumberOfArguments() < 1 ||
             fctCall->getNumberOfArguments() > 2)
         {
            throw ScannerException(
               "Syntax error when calling method 'startsWith':"
               " One or two arguments (prefix [, offset]) are required.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::SubstrE:
      {
         if (fctCall->getNumberOfArguments() < 1 ||
             fctCall->getNumberOfArguments() > 2)
         {
            throw ScannerException(
               "Syntax error when calling method 'substr':"
               " One or two arguments (start [, length]) are required.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::SubstringE:
      {
         if (fctCall->getNumberOfArguments() < 1 ||
             fctCall->getNumberOfArguments() > 2)
         {
            throw ScannerException(
               "Syntax error when calling method 'substring':"
               " One or two arguments (start [, end]) are required.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::ToLowerCaseE:
      {
         if (fctCall->getNumberOfArguments() != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'toLowerCase':"
               " Method allows no arguments.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::ToStringE:
      {
         if (fctCall->getNumberOfArguments() > 2)
         {
            throw ScannerException(
               "Syntax error when calling method 'toString':"
               " Not more than two arguments allowed ([radix [, width]]).", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::ToUpperCaseE:
      {
         if (fctCall->getNumberOfArguments() != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'toUpperCase':"
               " Method allows no arguments.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::TrimE:
      {
         if (fctCall->getNumberOfArguments() != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'trim':"
               " Method allows no arguments.", 
               file,
               line, 
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::JsonFormatE:
      {
         if (fctCall->getNumberOfArguments() < 1 || fctCall->getNumberOfArguments() > 2)
         {
            throw ScannerException(
               "Syntax error when calling method 'jsonFormat':"
               " One or two arguments (value [, string]) are required.",
               file,
               line,
               column);
         }
         if (theObject != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'jsonFormat':"
               " Can't apply the global function to an object.",
               file,
               line,
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::JsonUnformatE:
      {
         if (fctCall->getNumberOfArguments() != 1 )
         {
            throw ScannerException(
               "Syntax error when calling method 'jsonUnformat':"
               " One argument (value ) is required.",
               file,
               line,
               column);
         }
         if (theObject != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'jsonUnformat':"
               " Can't apply the global function to an object.",
               file,
               line,
               column);
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::WriteToFileE:
      {
         if (fctCall->getNumberOfArguments() != 2)
         {
            throw ScannerException(
               "Syntax error when calling method 'writeToFile':"
               " Two arguments (filename, content) are required.", 
               file,
               line, 
               column);            
         }
         if (theObject != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'writeToFile':"
               " Can't apply the global function to an object.", 
               file,
               line, 
               column);            
         }
         break;
      }
      case ScriptNodeInbuiltFunctionCall::PrintE:
      {
         if (fctCall->getNumberOfArguments() != 1)
         {
            throw ScannerException(
               "Syntax error when calling method 'printf':"
               " One argument (content) is required..",
               file,
               line,
               column); 
         }
         if (theObject != 0)
         {
            throw ScannerException(
               "Syntax error when calling method 'printf':"
               " Can't apply the global function to an object.",
               file,
               line,
               column);
         }
         break;
      }
      default:
      {
         // empty
      }
   }
   return fctCall;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseIterationStatementDo()
{
   // IterationStatement
   //    ECMA-262 12.6.1
   //    do Statement while ( Expression ) ;
   ScriptStringPtr file = scannerM.getFilename();
   int line = scannerM.getLine();
   scannerM.getNextToken(); // 'eat' do

   insideLoopM++;
   ScriptNode* statement = parseStatement();
   insideLoopM--;

   if (scannerM.getCurrentToken() != ScriptScanner::WhileE)
   {
      throw ScannerException("Syntax error in 'do' ... 'while'  expression:"
                             " 'while' expected.",
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
   }
   scannerM.getNextToken(); // 'eat' while

   if (scannerM.getCurrentToken() != ScriptScanner::LeftParenthesisE)
   {
      throw ScannerException("Syntax error in 'do' ... 'while'  expression:"
                             " '(' expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
   }
   scannerM.getNextToken(); // 'eat' (

   ScriptNode* exp1 = parseExpression(false);

   if (scannerM.getCurrentToken() != ScriptScanner::RightParenthesisE)
   {
      throw ScannerException("Syntax error in 'do' ... 'while'  expression:"
                             " ')' expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
   }
   scannerM.getNextToken(); // 'eat' )

   if (scannerM.getCurrentToken() != ScriptScanner::SemicolonE)
   {
      throw ScannerException("Syntax error in 'do' ... 'while'  expression:"
                             " Terminating ';' expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
   }
   scannerM.getNextToken(); // 'eat' ;

   if (statement != 0)
   {
      return new ScriptNodeBinaryOperator(file,
                                          line, 
                                          false, 
                                          ScriptScanner::DoE, 
                                          statement,
                                          exp1);
   }
   else
   {
      return new ScriptNodeUnaryOperator(file,
                                         line, 
                                         false, 
                                         ScriptScanner::DoE, 
                                         exp1);
   }
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseIterationStatementFor()
{
   // IterationStatement
   //    ECMA-262 12.6
   //    for ( ExpressionNoIn(opt) ; Expression(opt) ; Expression(opt) ) Statement
   //    for ( var VariableDeclarationListNoIn ; Expression(opt) ; Expression(opt) ) Statement
   //    for ( LeftHandSideExpression in Expression ) Statement
   //    for ( var VariableDeclarationNoIn in Expression ) Statement
   ScriptStringPtr file = scannerM.getFilename();
   int line = scannerM.getLine();
   scannerM.getNextToken(); // 'eat' for

   if (scannerM.getCurrentToken() != ScriptScanner::LeftParenthesisE)
   {
      throw ScannerException("Syntax error in 'for' statement:"
                             " '(' expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
   }
   scannerM.getNextToken(); // 'eat' (

   ScriptNode* exp1 = 0;
   bool hasMoreThanOneVariableDeclaration = false; 
   if (scannerM.getCurrentToken() != ScriptScanner::SemicolonE)
   {
      if (scannerM.getCurrentToken() == ScriptScanner::VarE)
      {
         // Parse variable declaration 
         exp1 = parseForVarStatement();
         if (exp1->getType() == ScriptNode::StatementBlockE)
         {
            hasMoreThanOneVariableDeclaration = true;
         }
      }
      else
      {
         // Parse first expression
         exp1 = parseExpression(true);
      }

      // Check if there is an 'in'
      if (scannerM.getCurrentToken() == ScriptScanner::InE)
      {
         if (hasMoreThanOneVariableDeclaration == true)
         {
            throw ScannerException(
               "Syntax error in 'for' ... 'in' statement:"
               " Only one variable declaration in 'var' statement allowed.", 
               scannerM.getFilename(),
               scannerM.getLine(), 
               scannerM.getColumn());
         }
         scannerM.getNextToken(); // 'eat' in
         ScriptNode* exp2 = parseExpression(false);

         if (scannerM.getCurrentToken() != ScriptScanner::RightParenthesisE)
         {
            throw ScannerException("Syntax error in 'for' ... 'in' statement:"
                                   " ')' expected.", 
                                   scannerM.getFilename(),
                                   scannerM.getLine(), 
                                   scannerM.getColumn());
         }
         scannerM.getNextToken(); // 'eat' )

         insideLoopM++;
         ScriptNode* statement = parseStatement();
         insideLoopM--;

         if (statement != 0)
         {
            return new ScriptNodeTernaryOperator(file,
                                                 line, 
                                                 false, 
                                                 ScriptScanner::ForInE, 
                                                 exp1,
                                                 exp2,
                                                 statement);
         }
         else
         {
            return new ScriptNodeBinaryOperator(file,
                                                line, 
                                                false, 
                                                ScriptScanner::ForInE, 
                                                exp1,
                                                exp2);
         }
      }

      // There is no 'in' -> there must be a semicolon
      if (scannerM.getCurrentToken() != ScriptScanner::SemicolonE)
      {
         throw ScannerException("Syntax error in 'for' statement:"
                                " ';' expected.", 
                                scannerM.getFilename(),
                                scannerM.getLine(), 
                                scannerM.getColumn());
      }
   }
   scannerM.getNextToken(); // 'eat' ;

   ScriptNode* exp2 = 0;
   if (scannerM.getCurrentToken() != ScriptScanner::SemicolonE)
   {
      // Parse second expression
      exp2 = parseExpression(false);
      if (scannerM.getCurrentToken() != ScriptScanner::SemicolonE)
      {
         throw ScannerException("Syntax error in 'for' statement:"
                                " ';' expected.", 
                                scannerM.getFilename(),
                                scannerM.getLine(), 
                                scannerM.getColumn());
      }
   }
   scannerM.getNextToken(); // 'eat' ;

   ScriptNode* exp3 = 0;
   if (scannerM.getCurrentToken() != ScriptScanner::RightParenthesisE)
   {
      // Parse third expression
      exp3 = parseExpression(false);
      if (scannerM.getCurrentToken() != ScriptScanner::RightParenthesisE)
      {
         throw ScannerException("Syntax error in 'for' statement:"
                                " ')' expected.", 
                                scannerM.getFilename(),
                                scannerM.getLine(), 
                                scannerM.getColumn());
      }
   }
   scannerM.getNextToken(); // 'eat' )

   insideLoopM++;
   ScriptNode* statement = parseStatement();
   insideLoopM--;

   if (exp1 == 0 && exp2 == 0 && exp3 == 0)
   {
      if (statement != 0)
      {
         return new ScriptNodeUnaryOperator(file,
                                            line, 
                                            false, 
                                            ScriptScanner::For000E, 
                                            statement);
      }
      else
      {
         throw ScannerException(
                  "Syntax error in 'for' statement:\n"
                  "Endless loop. Neither test expression, nor statement present.",
                                scannerM.getFilename(), 
                                scannerM.getLine(), 
                                scannerM.getColumn());
      }
   }
   else
   if (exp1 == 0 && exp2 == 0 && exp3 != 0)
   {
      if (statement != 0)
      {
         return new ScriptNodeBinaryOperator(file,
                                             line, 
                                             false, 
                                             ScriptScanner::For001E, 
                                             exp3,
                                             statement);
      }
      else
      {
         throw ScannerException(
                  "Syntax error in 'for' statement:\n"
                  "Endless loop. Neither test expression, nor statement present.",
                  scannerM.getFilename(),
                  scannerM.getLine(), 
                  scannerM.getColumn());
      }
   }
   else
   if (exp1 == 0 && exp2 != 0 && exp3 == 0)
   {
      if (statement != 0)
      {
         return new ScriptNodeBinaryOperator(file,
                                             line, 
                                             false, 
                                             ScriptScanner::For010E, 
                                             exp2,
                                             statement);
      }
      else
      {
         return new ScriptNodeUnaryOperator(file,
                                            line, 
                                            false, 
                                            ScriptScanner::For010E, 
                                            exp2);
      }
   }
   else
   if (exp1 == 0 && exp2 != 0 && exp3 != 0)
   {
      if (statement != 0)
      {
         return new ScriptNodeTernaryOperator(file,
                                              line, 
                                              false, 
                                              ScriptScanner::For011E, 
                                              exp2,
                                              exp3,
                                              statement);
      }
      else
      {
         return new ScriptNodeBinaryOperator(file,
                                             line, 
                                             false, 
                                             ScriptScanner::For011E, 
                                             exp2,
                                             exp3);
      }
   }
   else
   if (exp1 != 0 && exp2 == 0 && exp3 == 0)
   {
      if (statement != 0)
      {
         return new ScriptNodeBinaryOperator(file,
                                             line, 
                                             false, 
                                             ScriptScanner::For100E, 
                                             exp1,
                                             statement);
      }
      else
      {
         throw ScannerException(
                  "Syntax error in 'for' statement:\n"
                  "Endless loop. Neither test expression, nor statement present.",
                  scannerM.getFilename(),
                  scannerM.getLine(), 
                  scannerM.getColumn());
      }
   }
   else
   if (exp1 != 0 && exp2 == 0 && exp3 != 0)
   {
      if (statement != 0)
      {
         return new ScriptNodeTernaryOperator(file,
                                              line, 
                                              false, 
                                              ScriptScanner::For101E, 
                                              exp1,
                                              exp3,
                                              statement);
      }
      else
      {
         throw ScannerException(
                  "Syntax error in 'for' statement:\n"
                  "Endless loop. No test expression and no statement present.",
                  scannerM.getFilename(),
                  scannerM.getLine(), 
                  scannerM.getColumn());
      }
   }
   else
   if (exp1 != 0 && exp2 != 0 && exp3 == 0)
   {
      if (statement != 0)
      {
         return new ScriptNodeTernaryOperator(file,
                                              line, 
                                              false, 
                                              ScriptScanner::For110E, 
                                              exp1,
                                              exp2,
                                              statement);
      }
      else
      {
         return new ScriptNodeBinaryOperator(file,
                                             line, 
                                             false, 
                                             ScriptScanner::For110E, 
                                             exp1,
                                             exp2);
      }
   }
   else  // (exp1 != 0 && exp2 != 0 && exp3 != 0)
   {
      if (statement != 0)
      {
         return new ScriptNodeQuadrupleOperation(file,
                                                 line, 
                                                 false, 
                                                 ScriptScanner::For111E, 
                                                 exp1,
                                                 exp2,
                                                 exp3,
                                                 statement);
      }
      else
      {
         return new ScriptNodeTernaryOperator(file,
                                              line, 
                                              false, 
                                              ScriptScanner::For111E, 
                                              exp1,
                                              exp2,
                                              exp3);
      }
   }
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseIterationStatementWhile()
{
   // IterationStatement
   //    ECMA-262 12.6.2
   //    while ( Expression ) Statement
   ScriptStringPtr file = scannerM.getFilename();
   int line = scannerM.getLine();
   scannerM.getNextToken(); // 'eat' while

   if (scannerM.getCurrentToken() != ScriptScanner::LeftParenthesisE)
   {
      throw ScannerException("Syntax error in 'while' statement:"
                             " '(' expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
   }
   scannerM.getNextToken(); // 'eat' (

   ScriptNode* exp1 = parseExpression(false);

   if (scannerM.getCurrentToken() != ScriptScanner::RightParenthesisE)
   {
      throw ScannerException("Syntax error in 'while' statement:"
                             " ')' expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
   }
   scannerM.getNextToken(); // 'eat' )

   insideLoopM++;
   ScriptNode* statement = parseStatement();
   insideLoopM--;

   if (statement != 0)
   {
      return new ScriptNodeBinaryOperator(file,
                                          line, 
                                          false, 
                                          ScriptScanner::WhileE, 
                                          exp1,
                                          statement);
   }
   else
   {
      return new ScriptNodeUnaryOperator(file,
                                         line, 
                                         false, 
                                         ScriptScanner::WhileE, 
                                         exp1);
   }
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseIfStatement()
{
   // ECMA-262 12.5
   // IfStatement:
   //       if ( Expression ) Statement else Statement
   //       if ( Expression ) Statement
   ScriptStringPtr file = scannerM.getFilename();
   int line = scannerM.getLine();
   scannerM.getNextToken(); // 'eat' if

   if (scannerM.getCurrentToken() != ScriptScanner::LeftParenthesisE)
   {
      throw ScannerException("Syntax error in 'if' statement:"
                             " '(' expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
   }
   scannerM.getNextToken(); // 'eat' (

   ScriptNode* expression = parseExpression(false);

   if (scannerM.getCurrentToken() != ScriptScanner::RightParenthesisE)
   {
      throw ScannerException("Syntax error in 'if' statement:"
                             " ')' expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
   }
   scannerM.getNextToken(); // 'eat' )

   ScriptNode* statement1 = parseStatement();

   if (scannerM.getCurrentToken() != ScriptScanner::ElseE)
   {
      if (statement1 != 0)
      {
         // if expression statement1
         return new ScriptNodeBinaryOperator(file,
                                             line,
                                             false,
                                             ScriptScanner::IfE,
                                             expression,
                                             statement1);
      }
      else
      {
         // if expression { ; }
         return expression;
      }
   }
   scannerM.getNextToken(); // 'eat' else

   ScriptNode* statement2 = parseStatement();
   if (statement2 != 0)
   {
      if (statement1 != 0)
      {
         // if expression statement1 else statement2
         return new ScriptNodeTernaryOperator(file,
                                              line,
                                              false,
                                              ScriptScanner::IfE,
                                              expression,
                                              statement1,
                                              statement2);
      }
      else
      {
         // if expression { ; } else statement2
         return new ScriptNodeBinaryOperator(file,
                                             line,
                                             false,
                                             ScriptScanner::IfNotE,
                                             expression,
                                             statement2);
      }
   }
   else
   {
      if (statement1 != 0)
      {
         // if expression statement1
         return new ScriptNodeBinaryOperator(file,
                                             line,
                                             false,
                                             ScriptScanner::IfE,
                                             expression,
                                             statement1);
      }
      else
      {
         // if expression { ; } 
         return expression;
      }
   }
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseLogicalAndExpression(bool theNoInInd)
{
   // ECMA-262 11.11
   // LogicalANDExpression:
   //    BitwiseORExpression
   //    LogicalANDExpression && BitwiseORExpression
   ScriptNode* node1 = parseBitwiseOrExpression(theNoInInd);
   while  (scannerM.getCurrentToken() == ScriptScanner::LogicalAndE)
   {
      ScriptStringPtr file = scannerM.getFilename();
      int line = scannerM.getLine();
      scannerM.getNextToken(); // 'eat' &&
      ScriptNode* node2 = parseBitwiseOrExpression(theNoInInd);
      node1 = new ScriptNodeBinaryOperator(file,
                                           line,
                                           false,
                                           ScriptScanner::LogicalAndE,
                                           node1,
                                           node2);         
   }
   
   return node1;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseLogicalOrExpression(bool theNoInInd)
{
   // ECMA-262 11.11
   // LogicalORExpression:
   //    LogicalANDExpression
   //    LogicalORExpression || LogicalANDExpression
   ScriptNode* node1 = parseLogicalAndExpression(theNoInInd);
   while  (scannerM.getCurrentToken() == ScriptScanner::LogicalOrE)
   {
      ScriptStringPtr file = scannerM.getFilename();
      int line = scannerM.getLine();
      scannerM.getNextToken(); // 'eat' ||
      ScriptNode* node2 = parseLogicalAndExpression(theNoInInd);
      node1 = new ScriptNodeBinaryOperator(file,
                                           line,
                                           false,
                                           ScriptScanner::LogicalOrE,
                                           node1,
                                           node2);         
   }
   
   return node1;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseMemberExpression()
{
   // ECMA-262 11.2
   // MemberExpression:
   //    PrimaryExpression
   //    FunctionExpression
   //    MemberExpression [ Expression ]
   //    MemberExpression . Identifier
   //    new MemberExpression 
   ScriptNode* node1;
   switch (scannerM.getCurrentToken())
   {
      case ScriptScanner::FunctionE:
      {
         insideFunctionM = true;
         ScriptNode* node1 = parseFunctionExpression();
         insideFunctionM = false;
         return node1;
      }
      case ScriptScanner::NewE:
      {
         return parseNewMemberExpression();
      }
      default:
      {
         // empty
      }
   }

   // This must be a PrimaryExpression
   node1 = parsePrimaryExpression();

   while (true)
   {
      switch (scannerM.getCurrentToken())
      {
         case ScriptScanner::DotE:
         {
            scannerM.getNextToken();
            if (scannerM.getCurrentToken() != ScriptScanner::IdentifierE)
            {
               throw ScannerException(
                  "Syntax error in member expression:"
                  " Identifier after '.' operator expected.",
                                      scannerM.getFilename(),
                                      scannerM.getLine(), 
                                      scannerM.getColumn());
            }
            ScriptStringPtr file = scannerM.getFilename();
            int line = scannerM.getLine();
            std::string identifier = scannerM.getStringValue();

            scannerM.getNextToken(); // 'eat' Identifier
            ScriptNode* fctCall = parseInbuiltFunctionCall(identifier, node1);
            if (fctCall != 0)
            {
               // This was an inbuilt function 
               node1 = fctCall;
            }
            else
            {
               // No inbuilt function
               if (node1->getType() == ScriptNode::IdentifierE)
               {
                  std::string id = ((ScriptNodeIdentifier*)node1)->getIdentifier();
                  delete node1;
                  node1 = new ScriptNodeIdDotLiteral(file,
                                                     line,
                                                     id,
                                                     identifier);
               }
               else
               {
                  ScriptNode* node2 = new ScriptNodeLiteral(file, 
                                                            line, 
                                                            identifier);
                  
                  node1 = new ScriptNodeBinaryOperator(file,
                                                       line,
                                                       true,
                                                       ScriptScanner::DotE,
                                                       node1,
                                                       node2);
               }
            }
            break;
         }
         case ScriptScanner::LeftBracketE:
         {
            ScriptStringPtr file = scannerM.getFilename();
            int line = scannerM.getLine();
            scannerM.getNextToken(); // 'eat' [
            ScriptNode* node2 = parseExpression(false);
            if (scannerM.getCurrentToken() != ScriptScanner::RightBracketE)
            {
               throw ScannerException("Syntax error in member expression:"
                                      " ']' expected.", 
                                      scannerM.getFilename(),
                                      scannerM.getLine(), 
                                      scannerM.getColumn());
            }
            scannerM.getNextToken(); // 'eat' ]
            node1 = new ScriptNodeBinaryOperator(file,
                                                 line,
                                                 true,
                                                 ScriptScanner::LeftBracketE,
                                                 node1,
                                                 node2);
            break;
         }
         case ScriptScanner::LeftParenthesisE:
         {
            node1 = parseFunctionCall(node1);
            break;
         }
         default:
         {
            return node1;
         }       
      }
   }
   // We should never reach this point
   return node1;   
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseMultiplicativeExpression()
{
   // ECMA-262 11.5
   // MultiplicativeExpression:
   //    UnaryExpression
   //    MultiplicativeExpression * UnaryExpression
   //    MultiplicativeExpression / UnaryExpression
   //    MultiplicativeExpression % UnaryExpression
   ScriptNode* node1 = parseUnaryExpression();
   while (true)
   {
      switch (scannerM.getCurrentToken())
      {
         case ScriptScanner::MultiplicationE:
         case ScriptScanner::DivisionE:
         case ScriptScanner::ModulusE:
         {
            ScriptScanner::Token operation = scannerM.getCurrentToken();
            ScriptStringPtr file = scannerM.getFilename();
            int line = scannerM.getLine();
            scannerM.getNextToken(); // 'eat' shift operator
            ScriptNode* node2 = parseUnaryExpression();
            node1 = new ScriptNodeBinaryOperator(file,
                                                 line,
                                                 false,
                                                 operation,
                                                 node1,
                                                 node2);  
            break;
         }       
         default:
         {
            return node1;
         }       
      }
   }
   // We should never reach this point
   return node1;   
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseNewInbuiltObject(
   const ScriptString* theFile,
   int                 theLine)
{
   std::string constructorName = scannerM.getStringValue();
   ScriptConstructorFct constructorFct;
   int numberOfArguments;
   if (predefinedNameTableM.getConstructorFct(
          constructorName,
          constructorFct,
          numberOfArguments) == false)
   {
      return 0;
   }
   scannerM.getNextToken(); // 'eat' constructor name

   ScriptNodeConstructorCall* node1 = 
      new ScriptNodeConstructorCall(theFile,
                                    theLine,
                                    true,
                                    constructorFct,
                                    constructorName);

   // Parse the arguments
   std::string message("Syntax error in 'new " + constructorName +"' expression:");
   ScriptNodeArray& arguments = node1->getArguments();
   parseArgumentList(arguments, message);
   
   if (numberOfArguments != -1)
   {
      // Fixed number of attributes required
      if (arguments.size() != numberOfArguments)
      {
         // Wrong number of attributes
         if (numberOfArguments == 0)
         {
            message += " Constructor allows no arguments.";
         }
         else
         {
            message += " Constructor requires ";
            char buf[10] = {0};
            snprintf(buf, 10, "%d", numberOfArguments);
            message.append(buf);
            //message.appendNumber(numberOfArguments);
            message += " argument(s).";
         }
         delete node1;
         throw ScannerException(message, 
                                scannerM.getFilename(),
                                scannerM.getLine(), 
                                scannerM.getColumn());
      }
   }

   return node1;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseNewMemberExpression()
{
   // ECMA-262 11.2
   // MemberExpression:
   //    new MemberExpression 
   ScriptStringPtr file = scannerM.getFilename();
   int line = scannerM.getLine();
   scannerM.getNextToken(); // 'eat' new

   if (scannerM.getCurrentToken() == ScriptScanner::IdentifierE)
   {
      // Handle some inbuild types
      ScriptNode* node1 = parseNewInbuiltObject(file, line);
      if (node1 != 0)
      {
         return node1;
      }
   }

   // This is no inbuild type
   ScriptNode* node1 = parseMemberExpression();
   if (node1->getType() != ScriptNode::FunctionCallE)
   {
      throw ScannerException("Syntax error in 'new' expression:"
                             " Call of constructor function expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
   }
   dynamic_cast<ScriptNodeFunctionCall*>(node1)->setIsConstructorFlag();
   return node1;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseObjectInitialiser()
{
   ScriptNodeObjectInitialiser* objectInitialiser = 
      new ScriptNodeObjectInitialiser(scannerM.getFilename(),
                                      scannerM.getLine(),
                                      false);
   scannerM.getNextToken();  // 'eat' {
   
   if (scannerM.getCurrentToken() == ScriptScanner::RightBraceE)
   {
      scannerM.getNextToken(); // 'eat' }
      return objectInitialiser;      
   }
   
   while (true)
   {
      std::string propertyName;
      switch (scannerM.getCurrentToken())
      {
         case ScriptScanner::IdentifierE:
         case ScriptScanner::StringE:
         {
            propertyName = scannerM.getStringValue();
            break;
         }
         case ScriptScanner::NumberE:
         {
            char buf[10] = {0};
            snprintf(buf, 10, "%d", scannerM.getNumberValue());
            propertyName.append(buf);
            //propertyName.appendNumber(scannerM.getNumberValue());
            break;
         }
         default:
         {
            throw ScannerException("Syntax error in object initialiser:"
                                   " Property name expected.", 
                                   scannerM.getFilename(),
                                   scannerM.getLine(), 
                                   scannerM.getColumn());
         }
      }

      scannerM.getNextToken();  // 'eat' property name  
      
      if (scannerM.getCurrentToken() != ScriptScanner::ColonE)
      {
         throw ScannerException("Syntax error in object initialiser:"
                                " ':' expected.", 
                                scannerM.getFilename(),
                                scannerM.getLine(), 
                                scannerM.getColumn());
      }

      scannerM.getNextToken();  // 'eat' : 
       
      ScriptNode* node = parseAssignmentExpression(false);
      objectInitialiser->addElement(propertyName, node);         
         
      if (scannerM.getCurrentToken() != ScriptScanner::CommaE)
      {
         break;
      }
      scannerM.getNextToken(); // 'eat' ,
   }
   if (scannerM.getCurrentToken() != ScriptScanner::RightBraceE)
   {
      throw ScannerException("Syntax error in object initialiser:"
                             " ',' or '}' expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
   }
   
   scannerM.getNextToken(); // 'eat' }
   return objectInitialiser;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parsePostfixExpression()
{
   // ECMA-262 11.3
   // PostfixExpression:
   //    LeftHandSideExpression
   //    LeftHandSideExpression ++
   //    LeftHandSideExpression --
   ScriptNode* node1 = parseMemberExpression(); // FIXME!!!
   switch (scannerM.getCurrentToken())
   {
      case ScriptScanner::DecrementE:
      {
         ScriptStringPtr file = scannerM.getFilename();
         int line = scannerM.getLine();
         scannerM.getNextToken(); // 'eat' unary operator
         
         node1 = new ScriptNodeUnaryOperator(file,
                                             line, 
                                             false, 
                                             ScriptScanner::PostDecrementE, 
                                             node1);
         return node1;
      }
      case ScriptScanner::IncrementE:
      {
         ScriptStringPtr file = scannerM.getFilename();
         int line = scannerM.getLine();
         scannerM.getNextToken(); // 'eat' unary operator
         
         node1 = new ScriptNodeUnaryOperator(file,
                                             line, 
                                             false, 
                                             ScriptScanner::PostIncrementE, 
                                             node1);
         return node1;
      }
      default:
      {
         // empty
      }
   }
   return node1;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parsePrimaryExpression()
{
   // ECMA-262 11.1
   // PrimaryExpression:
   //    this
   //    Identifier
   //    Literal
   //    ArrayLiteral
   //    ObjectLiteral
   //    ( Expression )
   switch (scannerM.getCurrentToken())
   {
      case ScriptScanner::FalseE:
      {
         ScriptNode* node1 = new ScriptNodeLiteral(scannerM.getFilename(),
                                                   scannerM.getLine(), 
                                                   false);
         scannerM.getNextToken(); // 'eat' false
         return node1;
      }
      case ScriptScanner::IdentifierE:
      {
         ScriptNode* node1 = new ScriptNodeIdentifier(scannerM.getFilename(),
                                                      scannerM.getLine(), 
                                                      scannerM.getStringValue());
         scannerM.getNextToken(); // 'eat' identifier
         return node1;
      }
      case ScriptScanner::LeftBraceE: // {
      {
         return parseObjectInitialiser();
      }
      case ScriptScanner::LeftBracketE: // [
      {
         return parseArrayInitialiser();
      }
      case ScriptScanner::LeftParenthesisE: // (
      {
         scannerM.getNextToken();  // 'eat' (
         ScriptNode* node1 = parseExpression(false);
         if (scannerM.getCurrentToken() != ScriptScanner::RightParenthesisE)
         {
            throw ScannerException("Syntax error in primary expression:"
                                   " Terminating ')' expected.", 
                                   scannerM.getFilename(),
                                   scannerM.getLine(), 
                                   scannerM.getColumn());
         }
         scannerM.getNextToken(); // 'eat' )
         return node1;
      }
      case ScriptScanner::LineE:
      {
         ScriptNode* node1 = new ScriptNodeLiteral(scannerM.getFilename(),
                                                   scannerM.getLine(), 
                                                   (Number)scannerM.getLine());
         scannerM.getNextToken(); // 'eat' $line
         return node1;
      }
      case ScriptScanner::NullE:
      {
         ScriptNode* node1 = new ScriptNodeLiteral(scannerM.getFilename(),
                                                   scannerM.getLine());
         scannerM.getNextToken(); // 'eat' null
         return node1;
      }
      case ScriptScanner::NumberE:
      {
         ScriptNode* node1 = new ScriptNodeLiteral(scannerM.getFilename(),
                                                   scannerM.getLine(), 
                                                   (Number)scannerM.getNumberValue());
         scannerM.getNextToken(); // 'eat' number
         return node1;
      }
      case ScriptScanner::StringE:
      {
         ScriptNode* node1 = new ScriptNodeLiteral(scannerM.getFilename(),
                                                   scannerM.getLine(), 
                                                   scannerM.getStringValue());
         scannerM.getNextToken(); // 'eat' string
         return node1;
      }
      case ScriptScanner::ThisE:
      {
      {
         ScriptNode* node1 = new ScriptNodeIdentifier(scannerM.getFilename(),
                                                      scannerM.getLine(), 
                                                      "this");
         scannerM.getNextToken(); // 'eat' this
         return node1;
      }
      }
      case ScriptScanner::TrueE:
      {
         ScriptNode* node1 = new ScriptNodeLiteral(scannerM.getFilename(),
                                                   scannerM.getLine(), true);
         scannerM.getNextToken(); // 'eat' true
         return node1;
      }
      case ScriptScanner::UndefinedE:
      {
         ScriptNode* node1 = new ScriptNodeLiteral(scannerM.getFilename(),
                                                   scannerM.getLine(), 
                                                   ScriptValue::UndefinedE);
         scannerM.getNextToken(); // 'eat' true
         return node1;
      }
      default:
      {
         throw ScannerException("Syntax error in primary expression:"
                                " Unexpected token.",
                                scannerM.getFilename(),
                                scannerM.getLine(), 
                                scannerM.getColumn());
      }
   }
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseRelationalExpression(bool theNoInInd)
{
   // ECMA-262 11.8
   // RelationalExpression:
   //    ShiftExpression
   //    RelationalExpression < ShiftExpression
   //    RelationalExpression > ShiftExpression
   //    RelationalExpression <= ShiftExpression
   //    RelationalExpression >= ShiftExpression
   //    RelationalExpression instanceof ShiftExpression
   //    RelationalExpression in ShiftExpression
   ScriptNode* node1 = parseShiftExpression();
   while (true)
   {
      switch (scannerM.getCurrentToken())
      {
         case ScriptScanner::LessThanE:
         case ScriptScanner::GreaterThanE:
         case ScriptScanner::LessThanOrEqualToE:
         case ScriptScanner::GreaterThanOrEqualToE:
         case ScriptScanner::InstanceofE:
         {
            ScriptScanner::Token operation = scannerM.getCurrentToken();
            ScriptStringPtr file = scannerM.getFilename();
            int line = scannerM.getLine();
            scannerM.getNextToken(); // 'eat' relational operator
            ScriptNode* node2 = parseShiftExpression();
            node1 = new ScriptNodeBinaryOperator(file,
                                                 line,
                                                 false,
                                                 operation,
                                                 node1,
                                                 node2);  
            break;
         }       
         case ScriptScanner::InE:
         {
            if (theNoInInd == false)
            {
               ScriptScanner::Token operation = scannerM.getCurrentToken();
               ScriptStringPtr file = scannerM.getFilename();
               int line = scannerM.getLine();
               scannerM.getNextToken(); // 'eat' relational operator
               ScriptNode* node2 = parseShiftExpression();
               node1 = new ScriptNodeBinaryOperator(file,
                                                    line,
                                                    false,
                                                    operation,
                                                    node1,
                                                    node2);  
            }
            else
            {
               return node1;
            }
            break;
         }       
         default:
         {
            return node1;
         }       
      }
   }
   // We should never reach this point
   return node1;   
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseReturnStatement()
{
   // ECMA-262 11.9
   // ReturnStatement:
   //    return Expression(opt) ;
/*
   if (insideFunctionM == false)
   {
      throw ScannerException(
               "Syntax error in 'return' statement:"
               " 'return' is only allowed within a function.", 
               scannerM.getLine(), 
               scannerM.getColumn());
   }
*/
   ScriptStringPtr file = scannerM.getFilename();
   int line = scannerM.getLine();
   scannerM.getNextToken(); // 'eat' return
   if (scannerM.getCurrentToken() == ScriptScanner::SemicolonE)
   {
      // No expression present
      scannerM.getNextToken(); // 'eat' ;
      ScriptNode* node1 = new ScriptNodeNoArgOperation(file,
                                                       line, 
                                                       false,
                                                       ScriptScanner::ReturnE);
      return node1;
   }
   else
   {
      // Expression present
      ScriptNode* node1 = parseExpression(false);

      if (scannerM.getCurrentToken() != ScriptScanner::SemicolonE)
      {
         throw ScannerException("Syntax error in 'return' statement:"
                                " ';' expected.", 
                                scannerM.getFilename(),
                                scannerM.getLine(), 
                                scannerM.getColumn());  
      }
      scannerM.getNextToken(); // 'eat' ;
      ScriptNode* node2 = new ScriptNodeUnaryOperator(file,
                                                      line, 
                                                      false,
                                                      ScriptScanner::ReturnE,
                                                      node1);
      return node2;
   }
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseShiftExpression()
{
   // ECMA-262 11.7
   // ShiftExpression:
   //    ShiftExpression << AdditiveExpression
   //    ShiftExpression >> AdditiveExpression
   //    ShiftExpression >>> AdditiveExpression 
   ScriptNode* node1 = parseAdditiveExpression();
   while (true)
   {
      switch (scannerM.getCurrentToken())
      {
         case ScriptScanner::BitwiseLeftShiftE:
         case ScriptScanner::BitwiseRightShiftE:
         case ScriptScanner::BitwiseRightShiftWithZeroFillE:
         {
            ScriptScanner::Token operation = scannerM.getCurrentToken();
            ScriptStringPtr file = scannerM.getFilename();
            int line = scannerM.getLine();
            scannerM.getNextToken(); // 'eat' shift operator
            ScriptNode* node2 = parseAdditiveExpression();
            node1 = new ScriptNodeBinaryOperator(file,
                                                 line,
                                                 false,
                                                 operation,
                                                 node1,
                                                 node2);  
            break;
         }       
         default:
         {
            return node1;
         }       
      }
   }
   // We should never reach this point
   return node1;   
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseStatement()
{
   // ECMA-262 12
   // Statement:
   //    Block
   //       ECMA-262 12.1
   //       { StatementList(opt) }
   //    VariableStatement
   //       ECMA-262 12.2
   //       var VariableDeclarationList ;
   //    EmptyStatement
   //       ECMA-262 12.3
   //       ;
   //    ExpressionStatement
   //       ECMA-262 12.4
   //       [lookahead != { or function] Expression ;
   //    IfStatement
   //       ECMA-262 12.5
   //       if ( Expression ) Statement else Statement
   //       if ( Expression ) Statement
   //    IterationStatement
   //       ECMA-262 12.6
   //       do Statement while ( Expression) ;
   //       while ( Expression ) Statement
   //       for ( ExpressionNoIn(opt) ; Expression(opt) ; Expression(opt) ) Statement
   //       for ( var VariableDeclarationListNoIn ; Expression(opt) ; Expression(opt) ) Statement
   //       for ( LeftHandSideExpression in Expression ) Statement
   //       for ( var VariableDeclarationNoIn in Expression ) Statement
   //    ContinueStatement
   //       ECMA-262 12.7
   //       continue Identifier(opt) ;
   //    BreakStatement
   //       ECMA-262 12.8
   //       break Identifier(opt) ;
   //    ReturnStatement
   //       ECMA-262 12.9
   //       return Expression(opt) ;
   //    WithStatement
   //       ECMA-262 12.10
   //       with ( Expression ) Statement 
   //    LabelledStatement
   //       ECMA-262 12.12
   //       Identifier : Statement 
   //    SwitchStatement
   //       ECMA-262 12.11
   //       switch ( Expression ) CaseBlock 
   //    ThrowStatement
   //       ECMA-262 12.13
   //       throw Expression ;
   //    TryStatement
   //       ECMA-262 12.14
   //       try Block Catch
   //       try Block Finally
   //       try Block Catch Finally
   switch (scannerM.getCurrentToken())
   {
      case ScriptScanner::LeftBraceE:
      {
         ScriptNodeStatementBlock* node1 = 
            new ScriptNodeStatementBlock(scannerM.getFilename(),
                                         scannerM.getLine(),
                                         false,
                                         ScriptScanner::LeftBraceE);
         scannerM.getNextToken(); // 'eat' {
         while (scannerM.getCurrentToken() != ScriptScanner::RightBraceE)
         {
            node1->addStatement(parseStatement());
         }
         scannerM.getNextToken(); // 'eat' }
         if (node1->size() < 2)
         {
            // remove statement block node when it is empty or contains
            // only one statement
            ScriptNode* result = node1->removeFirstStatement();
            delete node1;
            return result;
         }
         return node1;
      }       
      case ScriptScanner::VarE:
      {
         return parseVarStatement(false);
      }
      case ScriptScanner::SemicolonE:
      {
         scannerM.getNextToken(); // 'eat' ;
         return 0;
      }
      case ScriptScanner::IfE:
      {
         return parseIfStatement();
      }
      case ScriptScanner::DoE:
      {
         return parseIterationStatementDo();
      }
      case ScriptScanner::WhileE:
      {
         return parseIterationStatementWhile();
      }
      case ScriptScanner::ForE:
      {
         return parseIterationStatementFor();
      }
      case ScriptScanner::ContinueE:
      {
         return parseContinueStatement();
      }
      case ScriptScanner::BreakE:
      {
         return parseBreakStatement();
      }
      case ScriptScanner::ReturnE:
      {
         return parseReturnStatement();
      }
      case ScriptScanner::WithE:
      {
         return parseWithStatement();
      }
      case ScriptScanner::SwitchE:
      {
         return parseSwitchStatement();
      }
      case ScriptScanner::ThrowE:
      {
         ScriptStringPtr file = scannerM.getFilename();
         int line = scannerM.getLine();
         scannerM.getNextToken(); // 'eat' throw
         ScriptNode* node1 = parseExpression(false);
         if (scannerM.getCurrentToken() != ScriptScanner::SemicolonE)
         {
            throw ScannerException("Syntax error in 'throw' statement:"
                                   " Terminating ';' expected.", 
                                   scannerM.getFilename(),
                                   scannerM.getLine(), 
                                   scannerM.getColumn());
         }
         scannerM.getNextToken(); // 'eat' ;
         return new ScriptNodeUnaryOperator(file,
                                            line, 
                                            false, 
                                            ScriptScanner::ThrowE, 
                                            node1);
      }
      case ScriptScanner::TryE:
      {
         return parseTryStatement();
      }
      default:
      {
         ScriptNode* node1 = parseExpression(false);
         if (scannerM.getCurrentToken() != ScriptScanner::SemicolonE)
         {
            throw ScannerException("Syntax error in expression statement:"
                                   " Terminating ';' expected.", 
                                   scannerM.getFilename(),
                                   scannerM.getLine(), 
                                   scannerM.getColumn());
         }
         scannerM.getNextToken(); // 'eat' }
         return node1;
      }       
   }
   // We should never reach this point
   return 0;   
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseSwitchStatement()
{
   // ECMA-262 12
   // Statement:
   // ECMA-262 12.11
   // SwitchStatement:
   //    switch ( Expression ) CaseBlock 
   ScriptStringPtr file = scannerM.getFilename();
   int line = scannerM.getLine();
   scannerM.getNextToken(); // 'eat' switch

   if (scannerM.getCurrentToken() != ScriptScanner::LeftParenthesisE)
   {
      throw ScannerException(
         "Syntax error in 'switch' statement: '(' expected.", 
         scannerM.getFilename(),
         scannerM.getLine(), 
         scannerM.getColumn());
   }
   scannerM.getNextToken(); // 'eat' (

   ScriptNode* expression = parseExpression(false);

   if (scannerM.getCurrentToken() != ScriptScanner::RightParenthesisE)
   {
      throw ScannerException(
         "Syntax error in 'switch' statement: ')' expected.", 
         scannerM.getFilename(),
         scannerM.getLine(), 
         scannerM.getColumn());
   }
   scannerM.getNextToken(); // 'eat' )

   if (scannerM.getCurrentToken() != ScriptScanner::LeftBraceE)
   {
      throw ScannerException(
         "Syntax error in 'switch' statement:"
         " Start of 'case' block ('{') expected.", 
         scannerM.getFilename(),
         scannerM.getLine(), 
         scannerM.getColumn());
   }
   scannerM.getNextToken(); // 'eat' {

   ScriptNodeSwitch* switchNode = new ScriptNodeSwitch(file,
                                                       line, 
                                                       expression);

   while (true)
   {
      switch (scannerM.getCurrentToken())
      {
         case ScriptScanner::CaseE:
         {
            scannerM.getNextToken(); // 'eat' case
            ScriptNode* node1 = parseExpression(false);
            if (scannerM.getCurrentToken() != ScriptScanner::ColonE)
            {
               throw ScannerException(
                  "Syntax error in 'case' statement: ':' expected.", 
                  scannerM.getFilename(),
                  scannerM.getLine(), 
                  scannerM.getColumn());
            }
            scannerM.getNextToken(); // 'eat' :
            switchNode->addCaseStatement(node1);
            break;
         }
         case ScriptScanner::DefaultE:
         {
            scannerM.getNextToken(); // 'eat' default
            if (scannerM.getCurrentToken() != ScriptScanner::ColonE)
            {
               throw ScannerException(
                  "Syntax error in 'default' statement: ':' expected.", 
                  scannerM.getFilename(),
                  scannerM.getLine(), 
                  scannerM.getColumn());
            }
            scannerM.getNextToken(); // 'eat' :
            switchNode->addDefaultStatement();
            break;
         }
         case ScriptScanner::RightBraceE:
         {
            scannerM.getNextToken(); // 'eat' }
            return switchNode;
         }
         default:
         {
            insideSwitchM++;
            ScriptNode* statement = parseStatement();
            insideSwitchM--;
            switchNode->addStatement(statement);
         }
      }
   }
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseTryStatement()
{
   // ECMA-262 12.14
   // TryStatement:
   //    try Block Catch 
   //    try Block Finally 
   //    try Block Catch Finally 
   // Catch:
   //    catch ( Identifier ) Block
   // Finally:
   //    finally Block
   ScriptNode* tryBlock = 0;
   ScriptNode* catchBlock = 0;
   bool catchBlockPresent = false;
   ScriptNode* finallyBlock = 0;
   bool finallyBlockPresent = false;
   ScriptNode* catchIdentifier = 0;

   ScriptStringPtr file = scannerM.getFilename();
   int line = scannerM.getLine();
   scannerM.getNextToken(); // 'eat' try
 
   if (scannerM.getCurrentToken() != ScriptScanner::LeftBraceE)
   {
      throw ScannerException(
         "Syntax error in 'try' statement: Statement block ('{') expected.", 
         scannerM.getFilename(),
         scannerM.getLine(), 
         scannerM.getColumn());
   }
   tryBlock = parseBlock();
   
   if (scannerM.getCurrentToken() == ScriptScanner::CatchE)
   {
      scannerM.getNextToken(); // 'eat' catch
      if (scannerM.getCurrentToken() != ScriptScanner::LeftParenthesisE)
      {
         throw ScannerException(
            "Syntax error in 'catch' statement: '(' expected.", 
            scannerM.getFilename(),
            scannerM.getLine(), 
            scannerM.getColumn());
      }
      scannerM.getNextToken(); // 'eat' '('
      if (scannerM.getCurrentToken() != ScriptScanner::IdentifierE)
      {
         throw ScannerException(
            "Syntax error in 'catch' statement: Exception identifier expected.",
            scannerM.getFilename(),
            scannerM.getLine(), 
            scannerM.getColumn());
      }

      catchIdentifier = new ScriptNodeIdentifier(scannerM.getFilename(),
                                                 scannerM.getLine(),
                                                 scannerM.getStringValue());
      scannerM.getNextToken(); // 'eat' identifier

      if (scannerM.getCurrentToken() != ScriptScanner::RightParenthesisE)
      {
         throw ScannerException(
            "Syntax error in 'catch' statement: ')' expected.", 
            scannerM.getFilename(),
            scannerM.getLine(), 
            scannerM.getColumn());
      }
      scannerM.getNextToken(); // 'eat' ')'
      
      if (scannerM.getCurrentToken() != ScriptScanner::LeftBraceE)
      {
         throw ScannerException(
            "Syntax error in 'catch' statement: Statement block ('{') expected.",
            scannerM.getFilename(),
            scannerM.getLine(), 
            scannerM.getColumn());
      }
      catchBlockPresent = true;
      catchBlock = parseBlock();
   }
      
   if (scannerM.getCurrentToken() == ScriptScanner::FinallyE)
   {
      scannerM.getNextToken(); // 'eat' finally      
      if (scannerM.getCurrentToken() != ScriptScanner::LeftBraceE)
      {
         throw ScannerException(
            "Syntax error in 'finally' statement:"
            " Statement block ('{') expected.",
            scannerM.getFilename(),
            scannerM.getLine(), 
            scannerM.getColumn());
      }
      finallyBlockPresent = true;
      finallyBlock = parseBlock();      
   }
   if (catchBlockPresent == false && finallyBlockPresent == false)
   {
      throw ScannerException(
         "Syntax error in 'try' statement:"
         " 'catch' or 'finally' statement expected.", 
         scannerM.getFilename(),
         scannerM.getLine(), 
         scannerM.getColumn());
   }
   
   ScriptNode* node1;
   if (catchBlockPresent == false)
   {
      if (finallyBlockPresent == false)
      {
         // Error: Only 'try' present
         throw ScannerException(
            "Syntax error in 'try' statement:"
             " 'catch' or 'finally' statement expected.", 
            scannerM.getFilename(),
            scannerM.getLine(), 
            scannerM.getColumn());
      }
      else
      {
         // Create try/finally node
         if (tryBlock != 0)
         {
            // try block not empty
            node1 = new ScriptNodeBinaryOperator(file,
                                                 line,
                                                 true,
                                                 ScriptScanner::TryFinallyE,
                                                 tryBlock,
                                                 finallyBlock);
         }
         else
         {
            // Empty try block
            node1 = finallyBlock;
         }
      }
   }
   else
   {
      if (finallyBlockPresent == false)
      {
         // Create try/catch node
         if (tryBlock != 0)
         {
            node1 = new ScriptNodeTernaryOperator(file,
                                                  line,
                                                  false,
                                                  ScriptScanner::TryCatchE,
                                                  tryBlock,
                                                  catchIdentifier,
                                                  catchBlock);
         }
         else
         {
            // Empty try block -> catch block is also obsolete
            return 0;
         }
      }
      else
      {
         // Create try/catch/finally node
         if (tryBlock != 0)
         {
            node1 = new ScriptNodeQuadrupleOperation(
                           file,
                           line, 
                           false, 
                           ScriptScanner::TryCatchFinallyE,
                           tryBlock,
                           catchIdentifier,
                           catchBlock,
                           finallyBlock);      
         }
         else
         {
            // Empty try block -> catch block is also obsolete
            return finallyBlock;
         }
      }
   }
   return node1;
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseUnaryExpression()
{
   // ECMA-262 11.4
   // UnaryExpression:
   //    PostfixExpression
   //    delete UnaryExpression
   //    void UnaryExpression
   //    typeof UnaryExpression
   //    ++ UnaryExpression
   //    -- UnaryExpression
   //    + UnaryExpression
   //    - UnaryExpression
   //    ~ UnaryExpression
   //    ! UnaryExpression
   switch (scannerM.getCurrentToken())
   {
      case ScriptScanner::AdditionE:
      {
         scannerM.getNextToken(); // 'eat' +
         
         return parseUnaryExpression();
      }
      case ScriptScanner::BitwiseNotE:
      case ScriptScanner::DecrementE:
      case ScriptScanner::DeleteE:
      case ScriptScanner::IncrementE:
      case ScriptScanner::LogicalNotE:
      case ScriptScanner::SubtractionE:
      case ScriptScanner::TypeofE:
      case ScriptScanner::VoidE:
      {
         ScriptScanner::Token operation = scannerM.getCurrentToken();
         scannerM.getNextToken(); // 'eat' unary operator
         
         ScriptNode* node1 = parseUnaryExpression();
         node1 = new ScriptNodeUnaryOperator(scannerM.getFilename(),
                                             scannerM.getLine(), 
                                             false, 
                                             operation, 
                                             node1);
         return node1;
      }
      default:
      {
         // empty
      }
   }
   return parsePostfixExpression();
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseVarStatement(bool theNoInInd)
{
   // ECMA-262 12.2
   // VariableStatement:
   // var VariableDeclarationList ;
   //    
   // VariableDeclarationList:
   //    VariableDeclaration
   //    VariableDeclarationList , VariableDeclaration
   //   
   // VariableDeclaration
   //    Identifier Initialiser(opt)
   // 
   // Initialiser:
   //    = AssignmentExpression
   ScriptStringPtr file = scannerM.getFilename();
   int line = scannerM.getLine();
   scannerM.getNextToken(); // 'eat' var

   ScriptNodeStatementBlock* statementBlock = 
      new ScriptNodeStatementBlock(file, line, false, ScriptScanner::CommaE);

   while (true)
   {
      if (scannerM.getCurrentToken() != ScriptScanner::IdentifierE)
      {
         throw ScannerException("Syntax error in 'var' statement:"
                                " Identifier expected.", 
                                scannerM.getFilename(),
                                scannerM.getLine(), 
                                scannerM.getColumn());
      }

      std::string identifierName = scannerM.getStringValue();
      scannerM.getNextToken(); // 'eat' Identifier
      
      ScriptNode* initialValue;
      if (scannerM.getCurrentToken() == ScriptScanner::AssignmentE)
      {
         // There is an initialiser
         scannerM.getNextToken(); // 'eat' =
         initialValue = parseAssignmentExpression(theNoInInd);
      }
      else
      {
         // There is no initialiser
         initialValue = 0;
      }
      
      ScriptNode* declaration = new ScriptNodeVarDeclaration(
                                       scannerM.getFilename(),
                                       scannerM.getLine(), 
                                       ScriptScanner::VarE,
                                       identifierName, 
                                       initialValue);
      statementBlock->addStatement(declaration);
      if (scannerM.getCurrentToken() != ScriptScanner::CommaE)
      {
         if (scannerM.getCurrentToken() != ScriptScanner::SemicolonE)
         {
            throw ScannerException("Syntax error in 'var' statement:"
                                   " Terminating ';' expected.", 
                                   scannerM.getFilename(),
                                   scannerM.getLine(), 
                                   scannerM.getColumn());
         }
         scannerM.getNextToken(); // 'eat' ;

         if (statementBlock->size() < 2)
         {
            ScriptNode* result = statementBlock->removeFirstStatement();
            delete statementBlock;
            return result;
         }
         return statementBlock;
      }
      scannerM.getNextToken(); // 'eat' ,
   }
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptParser::parseWithStatement()
{
   // ECMA-262 12.10
   // WithStatement:
   //       with ( Expression ) Statement
   ScriptStringPtr file = scannerM.getFilename();
   int line = scannerM.getLine();
   scannerM.getNextToken(); // 'eat' with

   if (scannerM.getCurrentToken() != ScriptScanner::LeftParenthesisE)
   {
      throw ScannerException("Syntax error in 'with' statement:"
                             " '(' expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
   }
   scannerM.getNextToken(); // 'eat' (

   ScriptNode* expression = parseExpression(false);

   if (scannerM.getCurrentToken() != ScriptScanner::RightParenthesisE)
   {
      throw ScannerException("Syntax error in 'with' statement:"
                             " ')' expected.", 
                             scannerM.getFilename(),
                             scannerM.getLine(), 
                             scannerM.getColumn());
   }
   scannerM.getNextToken(); // 'eat' )

   ScriptNode* statement = parseStatement();
   if (statement != 0)
   {
      // with ( expression ) statement
      return new ScriptNodeBinaryOperator(file,
                                          line,
                                          false,
                                          ScriptScanner::WithE,
                                          expression,
                                          statement);
   }
   else
   {
      // with ( expression ) { ; }
      return expression;
   }
}

