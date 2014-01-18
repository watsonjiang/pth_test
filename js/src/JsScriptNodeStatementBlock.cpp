#include "JsScriptNodeStatementBlock.h"
#include "JsScriptScanner.h"
#include "JsScriptTypeError.h"

using namespace Js;

ScriptNodeStatementBlock::ScriptNodeStatementBlock(
   const ScriptString*  theFile,
   int                  theLine,
   bool                 theIsLeftHandSideExpression,
   ScriptScanner::Token theOperator)
:  ScriptNode(theFile, theLine, theIsLeftHandSideExpression),
   operatorM(theOperator)
{
   // Empty
}


// ----------------------------------------------------------------------------

ScriptNodeStatementBlock::~ScriptNodeStatementBlock()
{
   for (StatementList::iterator iter = statementListM.begin();
        iter != statementListM.end();
        iter++)
   {
      delete *iter;
   }
}

// ----------------------------------------------------------------------------

void
ScriptNodeStatementBlock::addStatement(
   ScriptNode* theStatement)
{
   if (theStatement != 0)
   {
      statementListM.push_back(theStatement);
   }
}

// ----------------------------------------------------------------------------

bool
ScriptNodeStatementBlock::empty() const
{
   return statementListM.empty();
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeStatementBlock::execute(
   ScriptExecutionContext* theContext)
{
   theContext->countNodeExecution(this);
   ScriptValue result;
   for (StatementList::iterator iter = statementListM.begin();
        iter != statementListM.end();
        iter++)
   {
      result = (*iter)->execute(theContext);
      if (result.getCommand() != ScriptValue::NoneE)
      {
         return result;
      }
   }
   return result;
}

// ----------------------------------------------------------------------------

ScriptNode::Type
ScriptNodeStatementBlock::getType() const
{
   return ScriptNode::StatementBlockE;
}

// ----------------------------------------------------------------------------

void
ScriptNodeStatementBlock::print(
   std::string&       theOutputString,
   const std::string& theLinePrefix,
   size_t        theMaxFilenameLength) const
{
   if (theLinePrefix.empty() == true)
   { 
      printFileAndLine(theOutputString, theMaxFilenameLength);
   }
   switch (operatorM)
   {
      case ScriptScanner::CommaE:
      {
         theOutputString += "Comma operator\n";
         break;
      }
      case ScriptScanner::LeftBraceE:
      {
         theOutputString += "Statement block\n";
         break;
      }
      default:
      {
         // empty
      }
   }
   StatementList::const_iterator next;
   for (StatementList::const_iterator iter = statementListM.begin();
        iter != statementListM.end();
        iter = next)
   {
      (*iter)->printFileAndLine(theOutputString, theMaxFilenameLength);
      theOutputString += theLinePrefix + " +-";
      next = iter;
      next++;
      if (next != statementListM.end())
      {
         (*iter)->print(theOutputString, 
                        theLinePrefix + " | ",
                        theMaxFilenameLength);
      }
      else
      {
         (*iter)->print(theOutputString, 
                        theLinePrefix + "   ",
                        theMaxFilenameLength);
         break;
      }
   }
}

// ----------------------------------------------------------------------------

ScriptNode*
ScriptNodeStatementBlock::removeFirstStatement()
{
   if (statementListM.begin() != statementListM.end())
   {
      ScriptNode* result = *statementListM.begin();
      statementListM.erase(statementListM.begin());
      return result;
   }
   else
   {
      return 0;
   }
}

// ----------------------------------------------------------------------------

int
ScriptNodeStatementBlock::size() const
{
   return statementListM.size();
}
