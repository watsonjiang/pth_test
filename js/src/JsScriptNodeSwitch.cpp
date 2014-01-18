#include "JsScriptNodeSwitch.h"

using namespace Js;

ScriptNodeSwitch::ScriptNodeSwitch(
   const ScriptString* theFile,
   int                 theLine,
   ScriptNode*         theSwitchExpression)
:  ScriptNode(theFile, theLine, false),
   switchExpressionM(theSwitchExpression)
{
   defaultStatementIndexM = INT_MAX;
}

// ----------------------------------------------------------------------------

ScriptNodeSwitch::~ScriptNodeSwitch()
{
   for (CaseStatementList::iterator iter1 = caseStatementsM.begin();
        iter1 != caseStatementsM.end();
        iter1++)
   {
      delete (*iter1).expression;
   }

   for (StatementList::iterator iter2 = statementListM.begin();
        iter2 != statementListM.end();
        iter2++)
   {
      delete *iter2;
   }

   delete switchExpressionM;
   switchExpressionM = 0;
}

// ----------------------------------------------------------------------------

bool
ScriptNodeSwitch::addCaseStatement(
   ScriptNode* theCaseExpression)
{
   CaseStatement caseStatement;
   caseStatement.expression = theCaseExpression;
   caseStatement.statementIndex = statementListM.size();
   caseStatementsM.push_back(caseStatement);
   return true;
}

// ----------------------------------------------------------------------------

bool
ScriptNodeSwitch::addDefaultStatement()
{
   if (defaultStatementIndexM != INT_MAX)
   {
      return false;
   }
   defaultStatementIndexM = statementListM.size(); 
   return true;
}

// ----------------------------------------------------------------------------

void
ScriptNodeSwitch::addStatement(
   ScriptNode* theStatement)
{
   if (theStatement != 0)
   {
      statementListM.push_back(theStatement);
   }
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeSwitch::execute(
   ScriptExecutionContext* theExecutionContext)
{
   theExecutionContext->countNodeExecution(this);

   // evaluate the switch expression 
   ScriptValue switchValue(switchExpressionM->execute(theExecutionContext));
   if (switchValue.getDataType() == ScriptValue::ReferenceE)
   {
      switchValue = switchValue.getReferenceValue();
   }

   unsigned int statementIndex = defaultStatementIndexM;

   // evaluate the case expressions until you find one that is equal
   // to the switch expression
   for (CaseStatementList::iterator iter1 = caseStatementsM.begin();
        iter1 != caseStatementsM.end();
        iter1++)
   {
      ScriptValue caseValue((*iter1).expression->execute(theExecutionContext));
      if (caseValue.getDataType() == ScriptValue::ReferenceE)
      {
         caseValue = caseValue.getReferenceValue();
      }
      if (switchValue.getDataType() == caseValue.getDataType())
      {
         ScriptValue isEqual(caseValue == switchValue); 
         if (isEqual.toBoolean() == true)
         {
            statementIndex = (*iter1).statementIndex;
            break;
         }
      }
   }

   // Execute the statements    
   ScriptValue result;
   while (statementIndex < statementListM.size())
   {
      result = statementListM[statementIndex]->execute(theExecutionContext);
      ScriptValue::Command command = result.getCommand();
      if (command == ScriptValue::BreakE)
      {
         break;
      }
      else
      if (command != ScriptValue::NoneE)
      {
         return result;
      }
      statementIndex++;
   }
   result.resetCommand();
   return result;

}

// ----------------------------------------------------------------------------

ScriptNode::Type
ScriptNodeSwitch::getType() const
{
   return ScriptNode::SwitchE;
}

// ----------------------------------------------------------------------------

void
ScriptNodeSwitch::print(
   std::string&       theOutputString,
   const std::string& theLinePrefix,
   size_t        theMaxFilenameLength) const
{
   if (theLinePrefix.empty() == true)
   { 
      printFileAndLine(theOutputString, theMaxFilenameLength);
   }
   theOutputString += "Switch statement\n";
   printFileAndLine(theOutputString, theMaxFilenameLength);
   theOutputString += theLinePrefix + " +-exp = op1\n";

   switchExpressionM->printFileAndLine(theOutputString, theMaxFilenameLength);
   theOutputString += theLinePrefix + " |  +-";

   switchExpressionM->print(theOutputString, 
                            theLinePrefix + "    | ",
                            theMaxFilenameLength);



   CaseStatementList::const_iterator next1;
   for (CaseStatementList::const_iterator iter1 = caseStatementsM.begin();
        iter1 != caseStatementsM.end();
        iter1 = next1)
   {
      (*iter1).expression->printFileAndLine(theOutputString, theMaxFilenameLength);
      theOutputString += theLinePrefix + " +-If exp == op1 Goto ";
      char buf[10] = {0};
      snprintf(buf, 9, "%04d", (*iter1).statementIndex);
      //theOutputString.appendNumber((*iter1).statementIndex, 10, 4, true);
      theOutputString.append(buf);
      theOutputString += "\n";
      (*iter1).expression->printFileAndLine(theOutputString, theMaxFilenameLength);
      theOutputString += theLinePrefix + " |  +-";

      next1 = iter1;
      next1++;
      if (next1 != caseStatementsM.end())
      {
         (*iter1).expression->print(theOutputString, 
                                    theLinePrefix + " |  | ",
                                    theMaxFilenameLength);
      }
      else
      {
         (*iter1).expression->print(theOutputString, 
                                    theLinePrefix + " |    ",
                                    theMaxFilenameLength);
      }
   }

   for (unsigned int i = 0; i < statementListM.size(); i++)
   
   {
      statementListM[i]->printFileAndLine(theOutputString, theMaxFilenameLength);
      theOutputString += theLinePrefix + " +-";
      char buf[10] = {0};
      snprintf(buf, 9, "%04d", i);
      //theOutputString.appendNumber(i, 10, 4, true);
      theOutputString.append(buf);
      theOutputString += " ";
      if (i != statementListM.size() - 1)
      {
         statementListM[i]->print(theOutputString, 
                                  theLinePrefix + " |      ",
                                  theMaxFilenameLength);
      }
      else
      {
         statementListM[i]->print(theOutputString, 
                                  theLinePrefix + "        ",
                                  theMaxFilenameLength);
      }
   }
}

