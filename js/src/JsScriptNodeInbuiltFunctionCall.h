#ifndef PALSCRIPTNODEINBUILTFUNCTIONCALL_H
#define PALSCRIPTNODEINBUILTFUNCTIONCALL_H

#include "JsScriptDefinitions.h"
#include "JsScriptNode.h"
#include "JsScriptNodeArray.h"

namespace Js
{
   class ScriptNodeInbuiltFunctionCall : public ScriptNode
   {
   public:

      typedef enum
      {
         Base64DecodeE,
         Base64EncodeE,
         CallbackE,
         CharAtE,
         CharCodeAtE,
         ConcatE,
         ContainsE,
         DecodeUriE,
         DecodeUriComponentE,
         DelayedCallbackE,
         EncodeUriE,
         EncodeUriComponentE,
         EndsWithE,
         EqualsE,
         FormatE,
         FromCharCodeE,
         HexDecodeE,
         HexEncodeE,
         IcontainsE,
         IendsWithE,
         IequalsE,
         IindexOfE,
         IlastIndexOfE,
         IndexOfE,
         IreplaceAllE,
         IreplaceFirstE,
         IsNaNE,
         IstartsWithE,
         JsonFormatE,
         JsonUnformatE,
         LastIndexOfE,
         LengthE,
         LogInfoE,
         Md5E,
         ParseIntE,
         RandE,
         ReadFromFileE,
         ReplaceAllE,
         ReplaceFirstE,
         SplitE,
         StartsWithE,
         SubstrE,
         SubstringE,
         ToLowerCaseE,
         ToStringE,
         ToUpperCaseE,
         TrimE,
         UnkownE,
         WriteToFileE,
         PrintE
      } FunctionType;

      ScriptNodeInbuiltFunctionCall(
         const ScriptString* theFile,
         int                 theLine,
         bool                theIsLeftHandSideExpression,
         FunctionType        theFunction,
         ScriptNode*         theObject);

      virtual
      ~ScriptNodeInbuiltFunctionCall();

      /**
       * Adds an parameter.
       */
      void
      addArgument(
         ScriptNode* theArgument);

      /**
       * Execute the node. 
       */
      ScriptValue
      execute(
         ScriptExecutionContext* theContext);

      /**
       * Get the argument list.
       * @return the argument list.
       */
      ScriptNodeArray&
      getArgumentList();

      /**
       * Get the argument list.
       * @return the argument list.
       */
      const ScriptNodeArray&
      getArgumentList() const;

      /**
       * Get the function.
       * @return the function.
       */
      FunctionType
      getFunction() const;

      /**
       * Get the 'is-constructor-flag'.
       * @return 'true'  if the function is called as a constructor.
       *         'false' otherwise.
       */
      bool
      getIsConstructorFlag() const;

      /**
       * Get the number of arguments.
       */
      int
      getNumberOfArguments() const;

      /**
       * Get the object to which the function is applied.
       * @return the object, if the function is applied to an object.
       *         0,          if the function is a global function.
       */
      ScriptNode*
      getObject();

      /**
       * Get the object to which the function is applied.
       * @return the object, if the function is applied to an object.
       *         0,          if the function is a global function.
       */
      const ScriptNode*
      getObject() const;

      /**
       * Gets the type of the node.
       * @return the type of the node. 
       */
      Type
      getType() const;

      /**
       * Get the value of the node. 
       * @return the value of the node evaluation. 
       */
      ScriptValue
      getValue() const;

      /**
       * Print a human-readable description of the node to a string.
       * This method is mainly used to debug the parser.
       * @param theOutputString the string to which the description is
       *                        appended.
       * @param theLinePrefix   the prefix that should to be added to
       *                        each line of the description except of
       *                        the first one.
       */
      void
      print(
         std::string&       theOutputString,
         const std::string& theLinePrefix = "",
         size_t        theMaxFilenameLength = 30) const;

      /**
       * Set the flag that indicates that this function is used as 
       * a constructor.
       */
      void
      setIsConstructorFlag();

   private:

      ScriptValue
      numberToString(
         Number                  theNumber,
         ScriptExecutionContext* theContext);

      ScriptNodeArray argumentListM;
      FunctionType    functionM;
      bool            isConstructorM;
      ScriptNode*     objectMd;
   };


   // -------------------------------------------------------------------------

   inline
   ScriptNodeArray&
   ScriptNodeInbuiltFunctionCall::getArgumentList()
   {
      return argumentListM;
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptNodeArray&
   ScriptNodeInbuiltFunctionCall::getArgumentList() const
   {
      return argumentListM;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptNodeInbuiltFunctionCall::FunctionType
   ScriptNodeInbuiltFunctionCall::getFunction() const
   {
      return functionM;
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptNodeInbuiltFunctionCall::getIsConstructorFlag() const
   {
      return isConstructorM;
   }

   // -------------------------------------------------------------------------

   inline
   int
   ScriptNodeInbuiltFunctionCall::getNumberOfArguments() const
   {
      return argumentListM.size();
   }

   // -------------------------------------------------------------------------

   inline
   ScriptNode*
   ScriptNodeInbuiltFunctionCall::getObject()
   {
      return objectMd;
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptNode*
   ScriptNodeInbuiltFunctionCall::getObject() const
   {
      return objectMd;
   }
}

#endif
