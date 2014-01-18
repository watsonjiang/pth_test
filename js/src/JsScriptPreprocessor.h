#ifndef PALSCRIPTPREPROCESSOR_H
#define PALSCRIPTPREPROCESSOR_H
#include "JsTime.h"
#include <map>

namespace Js
{
   class FileMap;
   class ScriptPreprocessor
   {
   public:

      typedef std::map<std::string, Js::Time> FileMap;

      ScriptPreprocessor();

      virtual
      ~ScriptPreprocessor();

      const std::string&
      getFilename() const;

      const FileMap&
      getIncludedFiles() const;

      void
      preprocess(
         const std::string&  theFilename,
         const std::string&  theFileContent,
         std::string&        theOutput);

   private:

      ScriptPreprocessor(
         const std::string&  theFilename,
         const std::string&  theFileContent,
         ScriptPreprocessor* theParent);

      const ScriptPreprocessor*
      getParent() const;

      ScriptPreprocessor*
      getParent();

      bool
      okToIncludeFile(
         const std::string& theFilename) const;

      void
      parsePreprocessorDirective(
         std::string& theOutput);

      void
      preprocess(
         std::string& theOutput);

      int                 columnM;
      const char*         currentCharM;
      std::string         filenameM;
      FileMap             includedFilesM;
      int                 lineM;
      ScriptPreprocessor* parentM;
   };

   // ------------------------------------------------------------------------

   inline
   ScriptPreprocessor::ScriptPreprocessor()
   :  columnM(1),
      currentCharM(0),
      lineM(1),
      parentM(0)
   {
      // Empty
   }

   // ------------------------------------------------------------------------

   inline
   const std::string&
   ScriptPreprocessor::getFilename() const
   {
      return filenameM;
   }

   // ------------------------------------------------------------------------

   inline
   const ScriptPreprocessor::FileMap&
   ScriptPreprocessor::getIncludedFiles() const
   {
      return includedFilesM;
   }

   // ------------------------------------------------------------------------

   inline
   const ScriptPreprocessor*
   ScriptPreprocessor::getParent() const
   {
      return parentM;
   }

   // ------------------------------------------------------------------------

   inline
   ScriptPreprocessor*
   ScriptPreprocessor::getParent()
   {
      return parentM;
   }

   // ------------------------------------------------------------------------

   inline
   bool
   ScriptPreprocessor::okToIncludeFile(
      const std::string& theFilename) const
   {
      if (theFilename == filenameM)
      {
         return false;
      }
      if (parentM == 0)
      {
         return true;
      }
      return parentM->okToIncludeFile(theFilename);

   }
}

#endif
