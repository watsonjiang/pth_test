#ifndef PALSCRIPTEXCEPTION_H
#define PALSCRIPTEXCEPTION_H

#include "JsScriptDefinitions.h"
#include "JsScriptValue.h"
#include <exception>

namespace Js
{
   class ScriptException : public std::exception
   {
   public:

      enum Type
      {
         RangeErrorE,
         ReferenceErrorE,
         SyntaxErrorE,
         TypeErrorE,
         UserDefinedExceptionE,
         UriErrorE
      };

      ScriptException();

      ScriptException(
         const std::string&  theDescription,
         const ScriptString* theFile,
         int                 theLine = 0,
         int                 theColumn = 0);

      ScriptException(
         const std::string&  theDescription,
         int                 theLine = 0,
         int                 theColumn = 0);

      ScriptException(
         const ScriptException& theOther);

      virtual 
      ~ScriptException()
         throw ();

      int
      getColumn() const;

      const std::string&
      getDescription() const;

      const ScriptString*
      getFile() const;

      int
      getLine() const;

      virtual
      const char*
      getName() const = 0;

      virtual
      Type
      getType() const = 0;

      void
      setColumn(
         int theColumn);

      void
      setDescription(
         const std::string& theDescription);

      void
      setFile(
         const ScriptString* theFile);

      void
      setLineAndFile(
         int                 theLine,
         const ScriptString* theFile);

      virtual
      ScriptValue
      toValue() const = 0;

      /**
       * Returns a textual description of the exception.
       * @return A textual description of the exception.
       */
      virtual 
      const char* 
      what() const
         throw ();

   private:

      int             columnM;
      std::string     descriptionM;
      ScriptStringPtr fileM;
      int             lineM;      
   };

   // -------------------------------------------------------------------------
 
   inline
   ScriptException::ScriptException()
   :  columnM(0),
      fileM(new ScriptString),
      lineM(0)
   {
      // Empty
   }

   // -------------------------------------------------------------------------
 
   inline
   ScriptException::ScriptException(
      const std::string&  theDescription,
      const ScriptString* theFile,
      int                 theLine,
      int                 theColumn)
   :  columnM(theColumn),
      descriptionM(theDescription),
      fileM(theFile),
      lineM(theLine)
   {
      // Empty
   }

   // -------------------------------------------------------------------------
 
   inline
   ScriptException::ScriptException(
      const std::string&  theDescription,
      int                 theLine,
      int                 theColumn)
   :  columnM(theColumn),
      descriptionM(theDescription),
      fileM(new ScriptString),
      lineM(theLine)
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   ScriptException::ScriptException(
      const ScriptException& theOther)
   :  columnM(theOther.columnM),
      descriptionM(theOther.descriptionM),
      fileM(theOther.fileM),
      lineM(theOther.lineM)
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   ScriptException::~ScriptException()
   throw ()
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   int
   ScriptException::getColumn() const
   {
      return columnM;
   }

   // -------------------------------------------------------------------------

   inline
   const std::string&
   ScriptException::getDescription() const
   {
      return descriptionM;
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptString*
   ScriptException::getFile() const
   {
      return fileM;
   }
        
   // -------------------------------------------------------------------------

   inline
   int
   ScriptException::getLine() const
   {
      return lineM;
   }
        
   // -------------------------------------------------------------------------

   inline
   void
   ScriptException::setColumn(
      int theColumn)
   {
      columnM = theColumn;
   }
   
   // -------------------------------------------------------------------------

   inline
   void
   ScriptException::setDescription(
      const std::string& theDescription)
   {
      descriptionM = theDescription;
   }

   // -------------------------------------------------------------------------

   inline
   void
   ScriptException::setFile(
      const ScriptString* theFile)
   {
      fileM = theFile;
   }

   // -------------------------------------------------------------------------

   inline
   void
   ScriptException::setLineAndFile(
      int                 theLine,
      const ScriptString* theFile)
   {
      lineM = theLine;
      fileM = theFile;
   }

   // -------------------------------------------------------------------------

   inline
   const char* 
   ScriptException::what() const
   throw ()
   {
      return descriptionM.c_str();
   }
}

#endif
