#include "JsScannerException.h"
#include "JsScannerUtilities.h"
#include "JsScriptPreprocessor.h"
#include <ctype.h>
#include <fstream>
#include <string>
#include <unistd.h>

using namespace Js;

// ---------------------------------------------------------------------------

ScriptPreprocessor::~ScriptPreprocessor()
{
   // Empty
}

// ---------------------------------------------------------------------------

void
ScriptPreprocessor::preprocess(
   const std::string&  theFilename,
   const std::string&  theFileContent,
   std::string&        theOutput)
{
   columnM = 1;
   currentCharM = theFileContent.c_str();
   filenameM = theFilename;
   includedFilesM.clear();
   lineM = 1;
   parentM = 0;
   preprocess(theOutput);
}

// ---------------------------------------------------------------------------
// PRIVATE METHODS:
// ---------------------------------------------------------------------------

ScriptPreprocessor::ScriptPreprocessor(
   const std::string&  theFilename,
   const std::string&  theFileContent,
   ScriptPreprocessor* theParent)
:  columnM(1),
   currentCharM(theFileContent.c_str()),
   filenameM(theFilename),
   lineM(1),
   parentM(theParent)
{
   // Empty
}

// ---------------------------------------------------------------------------

void
ScriptPreprocessor::parsePreprocessorDirective(
   std::string& theOutput)
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
   if (directive != "include")
   {
      throw ScannerException(
         "Bad pre-processor directive. Expecting '#include'.", 
         new ScriptString(filenameM),
         lineM, 
         columnM);
   }

   // Skip spaces between directive and filename
   ScannerUtilities::skipSpaces(currentCharM, lineM, columnM);

   // Get the filename
   if (*currentCharM != '"')
   {
      throw ScannerException(
         "Bad pre-processor directive. Expecting \"filename\".", 
         new ScriptString(filenameM),
         lineM, 
         columnM);
   }

   ScannerUtilities::getNextChar(currentCharM, lineM, columnM);

   std::string includeFilename;
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
         new ScriptString(filenameM),
         lineM, 
         columnM);
   }
   ScannerUtilities::getNextChar(currentCharM, lineM, columnM);
   
   // Check if it is OK to inlude the file
   if (okToIncludeFile(includeFilename) == false)
   {
      std::string errorMessage("Can not include \"");
      errorMessage += includeFilename + "\". Circular dependency.";
      throw Js::ScannerException(
         errorMessage, 
         new ScriptString(filenameM),
         lineM, 
         columnM);
   }

   ScannerUtilities::skipWhiteSpaceAndComments(currentCharM, 
                                               lineM, 
                                               columnM);
   Time modificationTime;

   if (access(includeFilename.c_str(), F_OK))
   {
      std::string errorMessage("Bad pre-processor directive. Include file \"");
      errorMessage.append(includeFilename);
      errorMessage.append("\" does not exist.");

      throw Js::ScannerException(
         errorMessage,
         new ScriptString(filenameM),
         lineM,
         columnM);
   }
   if (access(includeFilename.c_str(), R_OK))
   {
      std::string errorMessage("Bad pre-processor directive. Can not read include file \"");
      errorMessage.append(includeFilename);
      errorMessage.append("\", permission denied.");

      throw Js::ScannerException(
         errorMessage,
         new ScriptString(filenameM),
         lineM,
         columnM);
   }
   std::ifstream ifs(includeFilename.c_str());
   std::string includeFileContent( (std::istreambuf_iterator<char>(ifs)),
                                   (std::istreambuf_iterator<char>())); 

   theOutput.append("#line 1 \"");
   theOutput.append(includeFilename);
   theOutput.append("\"\n");

   includedFilesM.insert(FileMap::value_type(includeFilename, 
                                             modificationTime));
   ScriptPreprocessor preprocessor(includeFilename, 
                                   includeFileContent, 
                                   this);
   preprocessor.preprocess(theOutput);

   const FileMap& includeFiles = preprocessor.getIncludedFiles();
   for (FileMap::const_iterator iter = includeFiles.begin();
        iter != includeFiles.end();
        iter++)
   {
      includedFilesM.insert(*iter);
   }

   //if (theOutput.endsWith("\n") == false)
   if (theOutput[theOutput.length()-1] != '\n')
   {
      theOutput.append("\n");
   }
   theOutput.append("#line ");
   char buf[10] = {0};
   snprintf(buf, 10, "%d", lineM);
   theOutput.append(buf);
   if (filenameM.size() > 0)
   {
      theOutput.append(" \"");
      theOutput.append(filenameM);
      theOutput.append("\"");
   }
   theOutput.append("\n");
}

// ---------------------------------------------------------------------------

void
ScriptPreprocessor::preprocess(
   std::string& theOutput)
{
   if (parentM == 0)
   {
      theOutput.erase();
   }

   const char* firstCharToCopy = currentCharM;

   while (true)
   {
      Js::ScannerUtilities::skipWhiteSpaceAndComments(currentCharM, lineM, columnM);
      switch (*currentCharM)
      {
         case '#':
         {
            theOutput.append(firstCharToCopy, currentCharM);

            parsePreprocessorDirective(theOutput);
            firstCharToCopy = currentCharM;
            break;
         }

         case 0:
         {
            // End-of-file
            theOutput.append(firstCharToCopy, currentCharM);
            return;
         }

         default:
         {
            ScannerUtilities::skipSingleLineComment(currentCharM, lineM, columnM);
            break;
         }
      }
   }
}

