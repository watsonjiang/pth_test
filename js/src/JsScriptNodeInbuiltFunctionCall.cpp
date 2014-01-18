#include "JsScannerException.h"
#include "JsScriptCallback.h"
#include "JsScriptNodeInbuiltFunctionCall.h"
#include "JsScriptObjectActivation.h"
#include "JsScriptObjectArray.h"
#include "JsScriptObjectDate.h"
#include "JsScriptTypeError.h"
#include "JsScriptReferenceError.h"
#include "JsScriptUriError.h"
#include "JsScriptUserDefinedException.h"
#include <string>
#include <exception>
#include <ctype.h>
#include <algorithm>

static char DigitToCharacterArrayC[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

using namespace Js;

ScriptNodeInbuiltFunctionCall::ScriptNodeInbuiltFunctionCall(
   const ScriptString* theFile,
   int                 theLine,
   bool                theIsLeftHandSideExpression,
   FunctionType        theFunction,
   ScriptNode*         theObject)
:  ScriptNode(theFile, theLine, theIsLeftHandSideExpression),
   functionM(theFunction),
   isConstructorM(false),
   objectMd(theObject)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptNodeInbuiltFunctionCall::~ScriptNodeInbuiltFunctionCall()
{
   for (ScriptNodeArray::iterator iter = argumentListM.begin();
        iter != argumentListM.end();
        iter++)
   {
      delete *iter;
   }
   delete objectMd;
   objectMd = 0;
}

// ----------------------------------------------------------------------------

void
ScriptNodeInbuiltFunctionCall::addArgument(
   ScriptNode* theArgument)
{
   argumentListM.push_back(theArgument);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeInbuiltFunctionCall::execute(
   ScriptExecutionContext* theContext)
{
   theContext->countNodeExecution(this);
   switch (functionM)
   {/*
      case Base64DecodeE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const std::string& string = op1.toString();
         ScriptStringPtr result = new ScriptString;
         if (Base64Coder::decode(string, *result) == true)
         {
            return ScriptValue(result);
         }
         else
         {
            return ScriptValue::UndefinedC;
         }
      }
      case Base64EncodeE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const std::string& string = op1.toString();
         ScriptStringPtr result = new ScriptString;
         //Base64Coder::encode(string, *result);
         //return ScriptValue(result);
         return ScriptValue::UndefinedC;   //TODO: fix base64encode
      }
      case CallbackE:
      {
         ScriptValue op1 = argumentListM[0]->execute(theContext).getValue();
         std::string callbackName = op1.toString();

         ScriptValueArray arguments;
         for (size_t i = 1; i < argumentListM.size(); i++)
         {
            arguments.push_back(
               argumentListM[i]->execute(theContext).getValue());
         }

         ScriptCallback callback(theContext->getAsyncRespInterface(), 
                                 theContext->getContextIdentifier(),
                                 callbackName);
         
         callback.call(0, arguments);

         return ScriptValue::UndefinedC;
      }
      case CharAtE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const std::string& string = op1.toString();
         Number index = 
            argumentListM[0]->execute(theContext).getValue().toNumber();
      
         std::string result;
         try
         {
            result += string.at(index);
         }
         catch (std::exception& e)
         {
            // Index is out of bounds
            // Ignore this
         }
         return ScriptValue(result);
      }
      case CharCodeAtE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const std::string& string = op1.toString();
         Number index = 
            argumentListM[0]->execute(theContext).getValue().toNumber();
      
         Number result;
         try
         {
            result = (unsigned char)string.at(index);
         }
         catch (Exception& e)
         {
            // Index is out of bounds
            result = NaNC;
         }
         return ScriptValue(result);
      }
      case ConcatE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const std::string& string = op1.toString();
         
         // Calculate the size of the result string
         ScriptValueArray stringValueArray;
         stringValueArray.reserve(argumentListM.size());
         size_t resultSize = string.size();
         for (size_t i = 0; i < argumentListM.size(); i++)
         {
            ScriptValue op = argumentListM[i]->execute(theContext).getValue();
            resultSize += op.toString().size();
            stringValueArray.push_back(op);
         }

         // Create the result string
         ScriptString* concatenatedString = new ScriptString;
         concatenatedString->reserve(resultSize);
         concatenatedString->append(string);

         for (size_t j = 0; j < argumentListM.size(); j++)
         {
            concatenatedString->append(stringValueArray[j].toString());
         }
         return ScriptValue(concatenatedString);   
      }
      case ContainsE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const std::string& string = op1.toString();

         ScriptValue op2 = argumentListM[0]->execute(theContext).getValue();
         const std::string& searchString = op2.toString();

         if (argumentListM.size() == 1)
         {
            return ScriptValue(string.contains(searchString));
         }
         else
         {
            Number offset = 
               argumentListM[1]->execute(theContext).getValue().toNumber();
            return ScriptValue(string.contains(searchString, offset));
         }
      }
      case DecodeUriE:
      {
         ScriptValue op1 = argumentListM[0]->execute(theContext).getValue();
         const std::string& uri = op1.toString();

         ScriptStringPtr result = new ScriptString;
         if (UriCoder::decodeUri(uri, *result) == false)
         {
            std::string errorMessage("decodeURI(): URI '");
            errorMessage.append(uri);
            errorMessage.append("' contains one or more malformed escape sequences.");
            throw ScriptUriError(
               errorMessage,
               getFile(),
               getLine());
         }

         return ScriptValue(result);
      }
      case DecodeUriComponentE:
      {
         ScriptValue op1 = argumentListM[0]->execute(theContext).getValue();
         const std::string& uri = op1.toString();

         ScriptStringPtr result = new ScriptString;
         if (UriCoder::decodeUriComponent(uri, *result) == false)
         {
            std::string errorMessage("decodeURIComponent(): URI '");
            errorMessage.append(uri);
            errorMessage.append("' contains one or more malformed escape sequences.");
            throw ScriptUriError(
               errorMessage,
               getFile(),
               getLine());
         }

         return ScriptValue(result);
      }
      case DelayedCallbackE:
      {
         Number milliseconds = argumentListM[0]->execute(theContext).getValue().toNumber();
         ScriptValue op2 = argumentListM[1]->execute(theContext).getValue();
         std::string callbackName = op2.toString();

         ScriptValueArray arguments;
         for (size_t i = 2; i < argumentListM.size(); i++)
         {
            arguments.push_back(
               argumentListM[i]->execute(theContext).getValue());
         }

         ScriptCallback callback(theContext->getAsyncRespInterface(), 
                                 theContext->getContextIdentifier(),
                                 callbackName);

         // NOTE: The ScriptDelayedCallback object deletes itself when
         //       the timer expires.
         new ScriptDelayedCallback(milliseconds, callback, arguments);

         return ScriptValue::UndefinedC;
      }
      case EncodeUriE:
      {
         ScriptValue op1 = argumentListM[0]->execute(theContext).getValue();
         const std::string& uri = op1.toString();

         ScriptStringPtr result = new ScriptString;
         UriCoder::encodeUri(uri, *result);
         return ScriptValue(result);
      }
      case EncodeUriComponentE:
      {
         ScriptValue op1 = argumentListM[0]->execute(theContext).getValue();
         const std::string& uri = op1.toString();

         ScriptStringPtr result = new ScriptString;
         UriCoder::encodeUriComponent(uri, *result);
         return ScriptValue(result);
      }
      case EndsWithE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const std::string& string = op1.toString();

         ScriptValue op2 = argumentListM[0]->execute(theContext).getValue();
         const std::string& suffix = op2.toString();
         return ScriptValue(string.endsWith(suffix));
      }
      case EqualsE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const std::string& string = op1.toString();

         ScriptValue op2 = argumentListM[0]->execute(theContext).getValue();
         const std::string& suffix = op2.toString();
         return ScriptValue(string.equals(suffix));
      }
      case FormatE:
      {
         ScriptValue op1 = argumentListM[0]->execute(theContext).getValue();
         std::string linePrefix;
         if (argumentListM.size() == 2)
         {
            linePrefix = argumentListM[1]->execute(theContext).getValue().toString();
         }
         std::string result;
         op1.format(result, linePrefix);
         
         return ScriptValue(result);
      }
      case FromCharCodeE:
      {
         size_t numberOfChars = argumentListM.size();
         std::string result;
         result.reserve(numberOfChars);
         for (size_t i = 0; i < numberOfChars; i++)
         {
            char c = (char)argumentListM[i]->execute(theContext).getValue().toNumber();
            result.append(c);
         }

         return ScriptValue(result);
      }
      case HexDecodeE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const std::string& string = op1.toString();
         ScriptStringPtr result = new ScriptString;
         if (HexadecimalCoder::decode(string, *result) == true)
         {
            return ScriptValue(result);
         }
         else
         {
            return ScriptValue::UndefinedC;
         }
      }
      case HexEncodeE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const std::string& string = op1.toString();
         ScriptStringPtr result = new ScriptString;
         HexadecimalCoder::encode(string, *result);
         return ScriptValue(result);
      }
      case IcontainsE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const std::string& string = op1.toString();

         ScriptValue op2 = argumentListM[0]->execute(theContext).getValue();
         const std::string& searchString = op2.toString();

         if (argumentListM.size() == 1)
         {
            return ScriptValue(string.icontains(searchString));
         }
         else
         {
            Number offset = 
               argumentListM[1]->execute(theContext).getValue().toNumber();
            return ScriptValue(string.icontains(searchString, offset));
         }
      }
      case IendsWithE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const std::string& string = op1.toString();

         ScriptValue op2 = argumentListM[0]->execute(theContext).getValue();
         const std::string& suffix = op2.toString();
         return ScriptValue(string.iendsWith(suffix));
      }
      case IequalsE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const std::string& string = op1.toString();

         ScriptValue op2 = argumentListM[0]->execute(theContext).getValue();
         const std::string& suffix = op2.toString();
         return ScriptValue(string.iequals(suffix));
      }
      case IindexOfE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const std::string& string = op1.toString();

         ScriptValue op2 = argumentListM[0]->execute(theContext).getValue();
         const std::string& substring = op2.toString();

         Number offset = 0;
         if (argumentListM.size() == 2)
         {
            offset = 
               argumentListM[1]->execute(theContext).getValue().toNumber();
         }

         return ScriptValue(string.ifind(substring, offset));
      }
      case IlastIndexOfE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const std::string& string = op1.toString();

         ScriptValue op2 = argumentListM[0]->execute(theContext).getValue();
         const std::string& substring = op2.toString();

         unsigned int stringSize = string.size();
         unsigned int substringSize = substring.size();
         Number offset = stringSize;
         if (argumentListM.size() == 2)
         {
            offset = 
               argumentListM[1]->execute(theContext).getValue().toNumber();

            if (offset < 0)
            {
               offset = 0;
            }
         }
         if (offset > (int)stringSize)
         {
            offset = stringSize;
         }

         for (int i = offset - substringSize; i >= 0; i--)
         {
            if (String::caseInsensitiveCompare(string.c_str() + i, 
                                               substring.c_str(), 
                                               substringSize) == 0)
            {
               return ScriptValue(i);
            }
         }
         
         return ScriptValue(-1);
      }
      case IndexOfE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const std::string& string = op1.toString();

         ScriptValue op2 = argumentListM[0]->execute(theContext).getValue();
         const std::string& substring = op2.toString();

         Number offset = 0;
         if (argumentListM.size() == 2)
         {
            offset = 
               argumentListM[1]->execute(theContext).getValue().toNumber();
         }

         return ScriptValue(string.find(substring, offset));
      }
      case IreplaceAllE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const std::string& string =  op1.toString();

         ScriptValue op2 = argumentListM[0]->execute(theContext).getValue();
         const std::string& searchValue = op2.toString();

         if (searchValue.empty())
         {
            return ScriptValue(string);
         }

         ScriptValue op3 = argumentListM[1]->execute(theContext).getValue();
         const std::string& replaceValue = op3.toString();

         std::string result;
         result.reserve(string.size());
         unsigned int searchValueSize = searchValue.size();
         int offset = 0;
         while (true)
         {
            long pos = string.ifind(searchValue, offset);
            if (pos != String::npos)
            {
               result.append(string.c_str() + offset, pos - offset);
               result.append(replaceValue);
               offset = pos + searchValueSize;
            }
            else
            {
               int numberOfBytes = string.size() - offset;
               if (numberOfBytes > 0)
               {
                  result.append(string.c_str() + offset, numberOfBytes);
               }
               return ScriptValue(result);
            }
         }
      }
      case IreplaceFirstE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const std::string& string =  op1.toString();

         ScriptValue op2 = argumentListM[0]->execute(theContext).getValue();
         const std::string& searchValue = op2.toString();

         ScriptValue op3 = argumentListM[1]->execute(theContext).getValue();
         const String& replaceValue = op3.toString();

         String result;
         result.reserve(string.size());
         unsigned int searchValueSize = searchValue.size();
         int offset = 0;
         long pos = string.ifind(searchValue, offset);
         if (pos != String::npos)
         {
            result.append(string.c_str() + offset, pos - offset);
            result.append(replaceValue);
            offset = pos + searchValueSize;
         }
         int numberOfBytes = string.size() - offset;
         if (numberOfBytes > 0)
         {
            result.append(string.c_str() + offset, numberOfBytes);
         }
         return ScriptValue(result);
      }
      */
      case IsNaNE:
      {
         ScriptValue op1 = argumentListM[0]->execute(theContext).getValue();

         if (op1.toNumber() == NaNC)
         {
            return ScriptValue(true);
         }
         return ScriptValue(false);
      }
      /*
      case IstartsWithE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const String& string = op1.toString();

         ScriptValue op2 = argumentListM[0]->execute(theContext).getValue();
         const String& prefix = op2.toString();

         if (argumentListM.size() == 1)
         {
            return ScriptValue(string.istartsWith(prefix, 0));
         }
         else
         if (argumentListM.size() == 2)
         {
            Number offset = 
               argumentListM[1]->execute(theContext).getValue().toNumber();
            return ScriptValue(string.istartsWith(prefix, offset));
         }
      }
      case LastIndexOfE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const String& string = op1.toString();

         ScriptValue op2 = argumentListM[0]->execute(theContext).getValue();
         const String& substring = op2.toString();

         unsigned int stringSize = string.size();
         unsigned int substringSize = substring.size();
         Number offset = stringSize;
         if (argumentListM.size() == 2)
         {
            offset = 
               argumentListM[1]->execute(theContext).getValue().toNumber();

            if (offset < 0)
            {
               offset = 0;
            }
         }
         if (offset > (int)stringSize)
         {
            offset = stringSize;
         }

         for (int i = offset - substringSize; i >= 0; i--)
         {
            if (strncmp(string.c_str() + i, 
                        substring.c_str(), 
                        substringSize) == 0)
            {
               return ScriptValue(i);
            }
         }
         
         return ScriptValue(-1);
      }
      case LengthE:
      {
         ScriptValue object = objectMd->execute(theContext).getValue();
         if (object.getDataType() == ScriptValue::StringE)
         {
            return ScriptValue((Number)object.toString().size());
         }
         else
         {
            return ScriptValue(object.toObject(), "length");
         }
      }
      case LogInfoE:
      {
         ScriptValue op1 = argumentListM[0]->execute(theContext).getValue();
         const String& groupId = op1.toString();
         ScriptValue op2 = argumentListM[1]->execute(theContext).getValue();
         const String& freeText = op2.toString();
         Log::instance().info(groupId.c_str(), 
                              freeText.c_str(), 
                              getLine(), 
                              getFile()->c_str());
         return ScriptValue::UndefinedC;
      }
      case Md5E:
      {
         ScriptValue obj = objectMd->execute(theContext).getValue();

         Md5 md5(obj.toString());

         ScriptStringPtr result = new ScriptString;
         if (argumentListM.size() == 0)
         {
            md5.finalHexadecimal(*result);
         }
         else
         if (argumentListM.size() == 1)
         {
            ScriptValue op1 = argumentListM[0]->execute(theContext).getValue();
            const String& encoding = op1.toString();
            if (encoding == "hex")
            {
               md5.finalHexadecimal(*result);
            }
            else
            if (encoding == "b64")
            {
               md5.finalBase64(*result);
            }
            else
            if (encoding == "bin")
            {
               unsigned char digest[16];
               md5.final(digest);
               result->assign((const char*)digest, 16);
            }
            else
            {
               throw ScriptTypeError(
                  "String.md5(): "
                  " Value of argument must be \"hex\", \"b64\", or \"bin\".");
            }
         }
         return ScriptValue(result);
      }
      case ParseIntE:
      {
         ScriptValue op1 = argumentListM[0]->execute(theContext).getValue();
         const String& numberAsString = op1.toString();
         int radix = 10;
         if (argumentListM.size() == 2)
         {
            radix = argumentListM[1]->execute(theContext).getValue().toNumber();
         }
         char* dummy;
         Number num = strtol(numberAsString.c_str(), &dummy, radix);
         
         if (num == 0)
         {
            if (!numberAsString.startsWith("0"))
            {
               return ScriptValue(NaNC);
            }
         }
         
         return ScriptValue((Number)num);
      }
      case ReadFromFileE:
      {
         ScriptValue op1 = argumentListM[0]->execute(theContext).getValue();
         const String& filename = op1.toString();    
     
         try
         {
            return ScriptValue(ScriptFileManager::instance()->getFile(filename));
         }
         catch (ScriptReferenceError& e)
         {
            if (e.getLine() == 0)
            {
               e.setLineAndFile(getLine(), getFile());
            }
            throw e;
         }
         catch (ScriptTypeError& e)
         {
            if (e.getLine() == 0)
            {
               e.setLineAndFile(getLine(), getFile());
            }
            throw e;
         }
         catch (ScriptUserDefinedException& e)
         {
            if (e.getLine() == 0)
            {
               e.setLineAndFile(getLine(), getFile());
            }
            throw e;
         }
      }
      case RandE:
      {
#ifndef WIN32
         long randomNumber = labs(lrand48());
#else
         int randomNumber = rand();
#endif
         if (argumentListM.size() == 2)
         {
            Number startValue = 
               argumentListM[0]->execute(theContext).getValue().toInt();
            Number endValue = 
               argumentListM[1]->execute(theContext).getValue().toNumber();
            if (startValue > endValue)
            {
               Number temp = startValue;
               startValue = endValue;
               endValue = temp;
            }
            randomNumber = startValue + 
                           randomNumber % (endValue - startValue + 1);
         }
         return ScriptValue((Number)randomNumber);
      }
      case ReplaceAllE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const String& string =  op1.toString();

         ScriptValue op2 = argumentListM[0]->execute(theContext).getValue();
         const String& searchValue = op2.toString();

         if (searchValue.empty())
         {
            return ScriptValue(string);
         }

         ScriptValue op3 = argumentListM[1]->execute(theContext).getValue();
         const String& replaceValue = op3.toString();

         String result;
         result.reserve(string.size());
         unsigned int searchValueSize = searchValue.size();
         int offset = 0;
         while (true)
         {
            long pos = string.find(searchValue, offset);
            if (pos != String::npos)
            {
               result.append(string.c_str() + offset, pos - offset);
               result.append(replaceValue);
               offset = pos + searchValueSize;
            }
            else
            {
               int numberOfBytes = string.size() - offset;
               if (numberOfBytes > 0)
               {
                  result.append(string.c_str() + offset, numberOfBytes);
               }
               return ScriptValue(result);
            }
         }
      }
      case ReplaceFirstE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const String& string =  op1.toString();

         ScriptValue op2 = argumentListM[0]->execute(theContext).getValue();
         const String& searchValue = op2.toString();

         ScriptValue op3 = argumentListM[1]->execute(theContext).getValue();
         const String& replaceValue = op3.toString();

         String result;
         result.reserve(string.size());
         unsigned int searchValueSize = searchValue.size();
         int offset = 0;
         long pos = string.find(searchValue, offset);
         if (pos != String::npos)
         {
            result.append(string.c_str() + offset, pos - offset);
            result.append(replaceValue);
            offset = pos + searchValueSize;
         }
         int numberOfBytes = string.size() - offset;
         if (numberOfBytes > 0)
         {
            result.append(string.c_str() + offset, numberOfBytes);
         }
         return ScriptValue(result);
      }
      case SplitE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();

         ScriptObjectArray* array = new ScriptObjectArray;
         ScriptValue result(array);

         ScriptValue op2 = argumentListM[0]->execute(theContext).getValue();
         if (op2.isUndefined() == true)
         {
            // The separator is undefined - return complete string in
            // array element 0.
            array->putPropertyNoCheck(0, op1);
            return result;
         }

         const String& string = op1.toString();
         const String& separator = op2.toString();
         unsigned int howMany = 0xFFFFFFFF;
         if (argumentListM.size() == 2)
         {
            howMany = 
               argumentListM[1]->execute(theContext).getValue().toNumber();
         }

         size_t separatorLength = separator.size();
         if (separatorLength == 0)
         {
            // The separator is an empty string - return an array that contains
            // each single caharacter
            unsigned int numberOfCharacters = string.size();
            if (numberOfCharacters > howMany)
            {
               numberOfCharacters = howMany;
            }
            for (unsigned int i = 0; i < numberOfCharacters; i++)
            {
               ScriptString* tempString = new ScriptString;
               tempString->append(string[i]);
               array->putPropertyNoCheck(i, ScriptValue(tempString));
            }
            return result;
         }

         size_t offset = 0;
         for (int currentIndex = 0; currentIndex < howMany; currentIndex++)
         {
            long pos = string.find(separator, offset);
            if (pos != -1)
            {
               // Separator found
               ScriptString* tempString = 
                  new ScriptString(string.substr(offset, pos - offset));
               array->putPropertyNoCheck(currentIndex, 
                                         ScriptValue(tempString));
            }
            else
            {
               // Separator not found
               ScriptString* tempString = 
                  new ScriptString(string.substr(offset));
               array->putPropertyNoCheck(currentIndex, 
                                         ScriptValue(tempString));  
               break;
            }
            offset = pos + separatorLength;
         }
         
         return result;
      }
      case StartsWithE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const String& string = op1.toString();

         ScriptValue op2 = argumentListM[0]->execute(theContext).getValue();
         const String& prefix = op2.toString();

         if (argumentListM.size() == 1)
         {
            return ScriptValue(string.startsWith(prefix));
         }
         else
         if (argumentListM.size() == 2)
         {
            Number offset = 
               argumentListM[1]->execute(theContext).getValue().toNumber();
            return ScriptValue(string.startsWith(prefix, offset));
         }
      }
      case SubstrE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const String& string = op1.toString();
         Number start = 
            argumentListM[0]->execute(theContext).getValue().toNumber();
         if (start < 0)
         {
            start = 0;
         }
         Number length = string.size() - start;
         if (argumentListM.size() == 2)
         {
            length = 
               argumentListM[1]->execute(theContext).getValue().toNumber();
            if (length < 0)
            {
               length = 0;
            }
         }
         return ScriptValue(string.substr(start, length));
      }
      case SubstringE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const String& string = op1.toString();
         Number start = 
            argumentListM[0]->execute(theContext).getValue().toNumber();
         if (start < 0)
         {
            start = 0;
         }
         Number end = string.size();
         if (argumentListM.size() == 2)
         {
            end = 
               argumentListM[1]->execute(theContext).getValue().toNumber();
            if (end < 0)
            {
               end = 0;
            }
         }
         if (start < end)
         {
            return ScriptValue(string.substr(start, end - start));
         }
         else
         {
            return ScriptValue(string.substr(end, start - end));
         }
      }*/
      case ToLowerCaseE:
      {
         std::string result = 
            objectMd->execute(theContext).getValue().toString();
         std::transform(result.begin(), result.end(), result.begin(), ::tolower); 
         return ScriptValue(result);
      }
      case ToStringE:
      {
         ScriptValue value = objectMd->execute(theContext).getValue();
         if (value.isNumber() == true)
         {
            return numberToString(value.toNumber(), theContext);
         }
         else
         {
            std::string result = value.toString();
            return ScriptValue(result);            
         }
      }
      case ToUpperCaseE:
      {
         std::string result = 
            objectMd->execute(theContext).getValue().toString();
      
         std::transform(result.begin(), result.end(), result.begin(), ::toupper);
         return ScriptValue(result);
      }
      case TrimE:
      {
         ScriptValue op1 = objectMd->execute(theContext).getValue();
         const std::string& string = op1.toString();
         const char* firstChar = string.c_str();
         const char* lastChar = firstChar + string.size();
         for (const char* start = firstChar; start < lastChar; start++)
         {
            if (!isspace(*start))
            {
               for (const char* end = lastChar - 1; end >= start; end--)
               {
                  if (!isspace(*end))
                  {
                     return ScriptValue(std::string(start, end + 1));
                  }
               }
            }
         }
      
         return ScriptValue("");
      }
      case JsonFormatE:
      {
         ScriptValue op1 = argumentListM[0]->execute(theContext).getValue();

         std::string formatOption;
         if (argumentListM.size() == 2)
         {
            formatOption = argumentListM[1]->execute(theContext).getValue().toString();
            std::transform(formatOption.begin(), formatOption.end(), formatOption.begin(), ::tolower); 
         }

         std::string result;
         std::string linePrefix;

         int ln = getLine();
         int col = 0;
         op1.jsonFormat(result, getFile(), ln, col, formatOption, linePrefix);

         return ScriptValue(result);
      }
      case JsonUnformatE:
      {
         ScriptValue op1 = argumentListM[0]->execute(theContext).getValue();
         const std::string& str = op1.toString();

         int ln  = 0;
         int col = 0;
         const char* chr = str.c_str();

         ScriptValue result;
         try
         {
            result = Js::ScriptObject::readPropertyValue(chr, ln, col);
         }
         catch(ScannerException& e)
         {
            e.setLineAndFile(getLine(), getFile());
            std::string d = e.getDescription();
            d += "\r\nJSON Input : ";
            char buf[10] = {0};
            snprintf(buf, 10, "%d", ln);  
            d.append(buf); 
            d += ", Column: ";
            snprintf(buf, 10, "%d", col);
            d.append(buf);
            e.setDescription(d);
            throw e;
         }
         return result;
      }
      /*
      case WriteToFileE:
      {
         ScriptValue op1 = argumentListM[0]->execute(theContext).getValue();
         const String& filename = op1.toString();    

         ScriptValue op2 = argumentListM[1]->execute(theContext).getValue();
         const String& content = op2.toString();    
     
         try
         {
            File::writeAll(filename, content);
            return  ScriptValue::UndefinedC;
         }
         catch (Exception& e)
         {
            String errorMessage("Failed to write file '");
            errorMessage += filename;
            errorMessage += "': ";
            errorMessage += e.what();
            ScriptValue exceptionData(new ScriptObjectError(errorMessage));
            ScriptUserDefinedException userDefinedException(exceptionData);
            userDefinedException.setLineAndFile(getLine(), getFile());
            throw userDefinedException;
         }
      }*/
      case PrintE:
      {
         ScriptValue op1 = argumentListM[0]->execute(theContext).getValue();
         theContext->print(op1.toString());
         return ScriptValue(ScriptValue::UndefinedE);
      }
      default:
      {
         // empty
      }
   }
   ScriptValue result;
   return result;      
}

// ----------------------------------------------------------------------------

ScriptNode::Type
ScriptNodeInbuiltFunctionCall::getType() const
{
   return ScriptNode::FunctionCallE;
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeInbuiltFunctionCall::getValue() const
{
   return ScriptValue(ScriptValue::UndefinedE);
}

// ----------------------------------------------------------------------------

void
ScriptNodeInbuiltFunctionCall::print(
   std::string&       theOutputString,
   const std::string& theLinePrefix,
   size_t        theMaxFilenameLength) const
{
   if (theLinePrefix.empty() == true)
   { 
      printFileAndLine(theOutputString, theMaxFilenameLength);
   }
   if (isConstructorM == true)
   {
      theOutputString += "new ";
   }

   switch (functionM)
   {
      case Base64DecodeE:
      {
         theOutputString += "obj . base64Decode ( )\n";
         break;
      }
      case Base64EncodeE:
      {
         theOutputString += "obj . base64Encode ( )\n";
         break;
      }
      case CallbackE:
      {
         switch (argumentListM.size())
         {
            case 1:
            {
               theOutputString += "callback ( callback )\n";
               break;
            }
            default:
            {
               theOutputString += "callback ( callback, argument1, ... )\n";
               break;
            }
         }
         break;
      }
      case CharAtE:
      {
         theOutputString += "obj . charAt ( pos )\n";
         break;
      }
      case CharCodeAtE:
      {
         theOutputString += "obj . charCodeAt ( pos )\n";
         break;
      }
      case ConcatE:
      {
         theOutputString += "obj . concat ( [string1 [, string2 [, ...]]] )\n";
         break;
      }
      case ContainsE:
      {
         if (argumentListM.size() == 1)
         {
            theOutputString += "obj . contains ( string )\n";
         }
         else
         {
            theOutputString += "obj . contains ( string, offset )\n";
         }
         break;
      }
      case DecodeUriE:
      {
         theOutputString += "decodeURI ( uri )\n";
         break;
      }
      case DecodeUriComponentE:
      {
         theOutputString += "decodeURIComponent ( string )\n";
         break;
      }
      case DelayedCallbackE:
      {
         switch (argumentListM.size())
         {
            case 2:
            {
               theOutputString += "delayedCallback ( milliseconds, callback )\n";
               break;
            }
            default:
            {
               theOutputString += "delayedCallback ( milliseconds, callback, argument1, ... )\n";
               break;
            }
         }
         break;
      }
      case EncodeUriE:
      {
         theOutputString += "encodeURI ( uri )\n";
         break;
      }
      case EncodeUriComponentE:
      {
         theOutputString += "encodeURIComponent ( string )\n";
         break;
      }
      case EndsWithE:
      {
         theOutputString += "obj . endsWith ( suffix )\n";
         break;
      }
      case EqualsE:
      {
         theOutputString += "obj . equals ( string )\n";
         break;
      }
      case FormatE:
      {
         switch (argumentListM.size())
         {
            case 1:
            {
               theOutputString += "format ( value )\n";
               break;
            }
            case 2:
            {
               theOutputString += "format ( value, line-prefix )\n";
               break;
            }
         }
         break;
      }
      case FromCharCodeE:
      {
         theOutputString += "fromCharCode ( argument1, ... )\n";
         break;
      }
      case HexDecodeE:
      {
         theOutputString += "obj . hexDecode ( )\n";
         break;
      }
      case HexEncodeE:
      {
         theOutputString += "obj . hexEncode ( )\n";
         break;
      }
      case IcontainsE:
      {
         if (argumentListM.size() == 1)
         {
            theOutputString += "obj . icontains ( string )\n";
         }
         else
         {
            theOutputString += "obj . icontains ( string, offset )\n";
         }
         break;
      }
      case IendsWithE:
      {
         theOutputString += "obj . iendsWith ( suffix )\n";
         break;
      }
      case IequalsE:
      {
         theOutputString += "obj . iequals ( string )\n";
         break;
      }
      case IindexOfE:
      {
         if (argumentListM.size() == 1)
         {
            theOutputString += "obj . iindexOf ( substring )\n";
         }
         else
         {
            theOutputString += "obj . iindexOf ( substring, offset )\n";
         }
         break;
      }
      case IlastIndexOfE:
      {
         if (argumentListM.size() == 1)
         {
            theOutputString += "obj . ilastIndexOf ( substring )\n";
         }
         else
         {
            theOutputString += "obj . ilastIndexOf ( substring, offset )\n";
         }
         break;
      }
      case IndexOfE:
      {
         if (argumentListM.size() == 1)
         {
            theOutputString += "obj . indexOf ( substring )\n";
         }
         else
         {
            theOutputString += "obj . indexOf ( substring, offset )\n";
         }
         break;
      }
      case IreplaceAllE:
      {
         theOutputString += "obj . ireplaceAll ( searchValue, replaceValue )\n";
         break;
      }
      case IreplaceFirstE:
      {
         theOutputString += "obj . ireplaceFirst ( searchValue, replaceValue )\n";
         break;
      }
      case IsNaNE:
      {
         theOutputString += "isNaN( number )\n";
         break;
      }
      case IstartsWithE:
      {
         if (argumentListM.size() == 1)
         {
            theOutputString += "obj . istartsWith ( prefix )\n";
         }
         else
         {
            theOutputString += "obj . istartsWith ( prefix, offset )\n";
         }
         break;
      }
      case LastIndexOfE:
      {
         if (argumentListM.size() == 1)
         {
            theOutputString += "obj . lastIndexOf ( substring )\n";
         }
         else
         {
            theOutputString += "obj . lastIndexOf ( substring, offset )\n";
         }
         break;
      }
      case LengthE:
      {
         theOutputString += "obj . length\n";
         break;
      }
      case JsonFormatE:
      {
         theOutputString += "jsonFormat( value )\n";
         break;
      }
      case JsonUnformatE:
      {
         theOutputString += "jsonUnformat( value )\n";
         break;
      }
      case Md5E:
      {
         if (argumentListM.size() == 1)
         {
            theOutputString += "obj . md5 ( encoding )\n";
         }
         else
         {
            theOutputString += "obj . md5 ( )\n";
         }
         break;
      }
      case ParseIntE:
      {
         switch (argumentListM.size())
         {
            case 1:
            {
               theOutputString += "parseInt ( string )\n";
               break;
            }
            case 2:
            {
               theOutputString += "parseInt ( string, radix )\n";
               break;
            }
         }
         break;
      }
      case ReadFromFileE:
      {
         theOutputString += "readFromFile ( filename )\n";
         break;
      }
      case RandE:
      {
         switch (argumentListM.size())
         {
            case 0:
            {
               theOutputString += "rand ( )\n";
               break;
            }
            case 2:
            {
               theOutputString += "rand ( startValue, endValue )\n";
               break;
            }
         }
         break;
      }
      case ReplaceAllE:
      {
         theOutputString += "obj . replaceAll ( searchValue, replaceValue )\n";
         break;
      }
      case ReplaceFirstE:
      {
         theOutputString += "obj . replaceFirst ( searchValue, replaceValue )\n";
         break;
      }
      case SplitE:
      {
         if (argumentListM.size() == 1)
         {
            theOutputString += "obj . split ( separator )\n";
         }
         else
         {
            theOutputString += "obj . split ( separator, limit )\n";
         }
         break;
      }
      case StartsWithE:
      {
         if (argumentListM.size() == 1)
         {
            theOutputString += "obj . startsWith ( prefix )\n";
         }
         else
         {
            theOutputString += "obj . startsWith ( prefix, offset )\n";
         }
         break;
      }
      case SubstrE:
      {
         if (argumentListM.size() == 1)
         {
            theOutputString += "obj . substring ( start )\n";
         }
         else
         {
            theOutputString += "obj . substring ( start, length )\n";
         }
         break;
      }
      case SubstringE:
      {
         if (argumentListM.size() == 1)
         {
            theOutputString += "obj . substring ( start )\n";
         }
         else
         {
            theOutputString += "obj . substring ( start, end )\n";
         }
         break;
      }
      case ToLowerCaseE:
      {
         theOutputString += "obj . toLowerCase ( )\n";
         break;
      }
      case ToStringE:
      {
         if (argumentListM.size() == 1)
         {
            theOutputString += "obj . toString ( radix )\n";
         }
         else
         {
            theOutputString += "obj . toString ( )\n";
         }
         break;
      }
      case ToUpperCaseE:
      {
         theOutputString += "obj . toUpperCase ( )\n";
         break;
      }
      case TrimE:
      {
         theOutputString += "obj . trim ( )\n";
         break;
      }
      case WriteToFileE:
      {
         theOutputString += "writeToFile ( filename, content )\n";
         break;
      }
      case PrintE:
      {
         theOutputString += "print ( content )\n";
         break;
      }
      default:
      {
         // empty
      }
   }
   if (objectMd != 0)
   {
      objectMd->printFileAndLine(theOutputString, theMaxFilenameLength);
      theOutputString += theLinePrefix + " +-";
      if (argumentListM.empty() == false)
      {
         objectMd->print(theOutputString, 
                         theLinePrefix + " | ",
                         theMaxFilenameLength);
      }
      else
      {
         objectMd->print(theOutputString, 
                         theLinePrefix + "   ",
                         theMaxFilenameLength);
      }
   }
   for (ScriptNodeArray::const_iterator iter = argumentListM.begin();
        iter != argumentListM.end();
        iter++)
   {
      (*iter)->printFileAndLine(theOutputString, theMaxFilenameLength);
      theOutputString += theLinePrefix + " +-";
      ScriptNodeArray::const_iterator nextIter = iter;
      nextIter++;
      if (nextIter != argumentListM.end())
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
      }

   }
}

// ----------------------------------------------------------------------------

void
ScriptNodeInbuiltFunctionCall::setIsConstructorFlag()
{
   isConstructorM = true;
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeInbuiltFunctionCall::numberToString(
   Number                  theNumber,
   ScriptExecutionContext* theContext)
{
   int radix;
   int fieldWidth;
   if (argumentListM.size() == 0)
   {
      radix = 10;
      fieldWidth = 0;
   }
   else
   {
      ScriptValue arg0 = argumentListM[0]->execute(theContext).getValue();
      if (arg0.isUndefined() == true)
      {
         radix = 10;
      }
      else
      {
         radix = arg0.toNumber();
         if (radix < 2 || radix > 36)
         {
            throw ScriptTypeError(
               "Number.toString():"
               " Radix must be between 2 and 36.",
               getFile(),
               getLine());
         }
      }
      if (argumentListM.size() == 1)
      {
         fieldWidth = 0;
      }
      else
      {
         fieldWidth = argumentListM[1]->execute(theContext).getValue().toNumber();
      }
   }
   
   bool appendMinus = false;
   if (theNumber < 0 && radix == 10)
   {
      appendMinus = true;
      theNumber *= -1;
   }
   char buffer[128];
   char* currentChar = buffer + sizeof(buffer) - 1;
   *currentChar-- = 0;
   size_t numberOfChars = 0;
   do
   {
      *currentChar-- = DigitToCharacterArrayC[(UInt32)theNumber % radix];
      numberOfChars++;
      theNumber /= radix;
   }
   while (theNumber != 0);
   if (appendMinus == true)
   {
      *currentChar-- = '-';
      numberOfChars++;
   }

   if (radix == 10)
   {
      while (numberOfChars < fieldWidth)
      {
         *currentChar-- = ' ';      
         numberOfChars++;
      }
   }
   else
   {
      while (numberOfChars < fieldWidth)
      {
         *currentChar-- = '0';      
         numberOfChars++;
      }
   }
   return ScriptValue(currentChar + 1);
}

