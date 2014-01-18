#ifndef PALSCRIPTEXECUTIONCONTEXTIDENTIFIER_H
#define PALSCRIPTEXECUTIONCONTEXTIDENTIFIER_H

#include "JsThreadSafeSmartPointerObject.h"
#include "stdio.h"
#include "string.h"
#include "assert.h"
namespace Js
{
   class ScriptExecutionContextIdentifier : public ThreadSafeSmartPointerObject
   {
   public:

      virtual
      std::string
      toString() const;
      
      ScriptExecutionContextIdentifier();

      ScriptExecutionContextIdentifier(
         const ScriptExecutionContextIdentifier& theOther);
      
      ScriptExecutionContextIdentifier(
         unsigned int theIdentifier0);
      
      ScriptExecutionContextIdentifier(
         unsigned int theIdentifier0,
         unsigned int theIdentifier1);
      
      ScriptExecutionContextIdentifier(
         unsigned int theIdentifier0,
         unsigned int theIdentifier1,
         unsigned int theIdentifier2);
      
       ScriptExecutionContextIdentifier(
         unsigned int theIdentifier0,
         unsigned int theIdentifier1,
         unsigned int theIdentifier2,
         unsigned int theIdentifier3);
      
       ScriptExecutionContextIdentifier(
         unsigned int theIdentifier0,
         unsigned int theIdentifier1,
         unsigned int theIdentifier2,
         unsigned int theIdentifier3,
         unsigned int theIdentifier4);
      
      bool
      operator==(
            const ScriptExecutionContextIdentifier& theOther) const;
         
      bool
      operator!=(
            const ScriptExecutionContextIdentifier& theOther) const;
         
      bool
      operator<(
            const ScriptExecutionContextIdentifier& theOther) const;
         
      int
      compare(
         const ScriptExecutionContextIdentifier& theOther) const;
            
      unsigned int
      getIdentifier(
         unsigned int theLevel) const;
      
      unsigned int
      getNumberOfIdentifiers() const;
      
      void
      setIdentifier(
         unsigned int theLevel,
         unsigned int theValue);
      
      unsigned int
      setNextIdentifier(
         unsigned int theValue);
      
   private:
   
      enum { MaxNoOfIdentifiersE = 7 };
   
      unsigned int numberOfIdentifiersM;
      unsigned int identifiersM[MaxNoOfIdentifiersE];
   };
   
   typedef Js::SmartPointer<ScriptExecutionContextIdentifier> ScriptExecutionContextIdentifierPtr;

   // -------------------------------------------------------------------------
   
   inline
   std::string
   ScriptExecutionContextIdentifier::toString() const
   {
      std::string result;
      for (unsigned int i = 0; i < numberOfIdentifiersM; i++)
      {
         if (i != 0)
         {
            result.append(".");
         }
         char buf[50] = {0};
         snprintf(buf, 49, "%d", identifiersM[i]);
         result.append(buf);
      }
      
      return result;
   }
   
   // -------------------------------------------------------------------------
   
   inline
   ScriptExecutionContextIdentifier::ScriptExecutionContextIdentifier()
   {
      numberOfIdentifiersM = 0;
   }

   // -------------------------------------------------------------------------
   
   inline
   ScriptExecutionContextIdentifier::ScriptExecutionContextIdentifier(
      const ScriptExecutionContextIdentifier& theOther)
   :  numberOfIdentifiersM(theOther.numberOfIdentifiersM)
   {
      for (unsigned int i = 0; i < numberOfIdentifiersM; i++)
      {
         identifiersM[i] = theOther.identifiersM[i];
      }
   }
      
   // -------------------------------------------------------------------------
   
   inline
   ScriptExecutionContextIdentifier::ScriptExecutionContextIdentifier(
      unsigned int theIdentifier0)
   :  numberOfIdentifiersM(1)
   {
      identifiersM[0] = theIdentifier0;
   }

   // -------------------------------------------------------------------------
      
   inline
   ScriptExecutionContextIdentifier::ScriptExecutionContextIdentifier(
      unsigned int theIdentifier0,
      unsigned int theIdentifier1)
   :  numberOfIdentifiersM(2)
   {
      identifiersM[0] = theIdentifier0;
      identifiersM[1] = theIdentifier1;
   }
      
   // -------------------------------------------------------------------------
      
   inline
   ScriptExecutionContextIdentifier::ScriptExecutionContextIdentifier(
      unsigned int theIdentifier0,
      unsigned int theIdentifier1,
      unsigned int theIdentifier2)
   :  numberOfIdentifiersM(3)
   {
      identifiersM[0] = theIdentifier0;
      identifiersM[1] = theIdentifier1;
      identifiersM[2] = theIdentifier2;
   }
      
   // -------------------------------------------------------------------------
      
   inline
   ScriptExecutionContextIdentifier::ScriptExecutionContextIdentifier(
      unsigned int theIdentifier0,
      unsigned int theIdentifier1,
      unsigned int theIdentifier2,
      unsigned int theIdentifier3)
   :  numberOfIdentifiersM(4)
   {
      identifiersM[0] = theIdentifier0;
      identifiersM[1] = theIdentifier1;
      identifiersM[2] = theIdentifier2;
      identifiersM[3] = theIdentifier3;
   }
      
   // -------------------------------------------------------------------------
      
   inline
   ScriptExecutionContextIdentifier::ScriptExecutionContextIdentifier(
      unsigned int theIdentifier0,
      unsigned int theIdentifier1,
      unsigned int theIdentifier2,
      unsigned int theIdentifier3,
      unsigned int theIdentifier4)
   :  numberOfIdentifiersM(5)
   {
      identifiersM[0] = theIdentifier0;
      identifiersM[1] = theIdentifier1;
      identifiersM[2] = theIdentifier2;
      identifiersM[3] = theIdentifier3;
      identifiersM[4] = theIdentifier4;
   }
      
   // -------------------------------------------------------------------------
   
   inline
   bool
   ScriptExecutionContextIdentifier::operator==(
      const ScriptExecutionContextIdentifier& theOther) const
   {
      return compare(theOther) == 0;
   }

   // -------------------------------------------------------------------------
   
   inline
   bool
   ScriptExecutionContextIdentifier::operator!=(
      const ScriptExecutionContextIdentifier& theOther) const
   {
      return compare(theOther) != 0;
   }

   // -------------------------------------------------------------------------
   
   inline
   bool
   ScriptExecutionContextIdentifier::operator<(
      const ScriptExecutionContextIdentifier& theOther) const
   {
      return compare(theOther) < 0;
   }

   // -------------------------------------------------------------------------
         
   inline
   int
   ScriptExecutionContextIdentifier::compare(
      const ScriptExecutionContextIdentifier& theOther) const
   {
      if (numberOfIdentifiersM == theOther.numberOfIdentifiersM)
      {
         return memcmp(identifiersM, 
                             theOther.identifiersM, 
                             numberOfIdentifiersM * sizeof(unsigned int));
      }
      else
      if (numberOfIdentifiersM < theOther.numberOfIdentifiersM)
      {
         int result = memcmp(identifiersM, 
                             theOther.identifiersM, 
                             numberOfIdentifiersM * sizeof(unsigned int));
         return (result != 0) ? result : -1;
      }
      else
      {
         int result = memcmp(identifiersM, 
                             theOther.identifiersM, 
                             theOther.numberOfIdentifiersM * sizeof(unsigned int));
         return (result != 0) ? result : 1;
      }
   }
         
   // -------------------------------------------------------------------------
   
   inline
   unsigned int
   ScriptExecutionContextIdentifier::getIdentifier(
      unsigned int theLevel) const
   {
      return (theLevel < numberOfIdentifiersM) ? identifiersM[theLevel] : 0;
   }
      
   // -------------------------------------------------------------------------
   
   inline
   unsigned int   
   ScriptExecutionContextIdentifier::getNumberOfIdentifiers() const
   {
      return numberOfIdentifiersM;
   }
      
   // -------------------------------------------------------------------------
   
   inline
   void
   ScriptExecutionContextIdentifier::setIdentifier(
      unsigned int theLevel,
      unsigned int theValue)
   {
      assert(theLevel < MaxNoOfIdentifiersE);
      identifiersM[theLevel] = theValue;
      if (theLevel >= numberOfIdentifiersM)
      {
         if (theLevel > numberOfIdentifiersM)
         {
            for (unsigned int i = numberOfIdentifiersM; i < theLevel; i++)
            {
               identifiersM[i] = 0;
            }
         }
         numberOfIdentifiersM = theLevel + 1;
      }
   }

   // -------------------------------------------------------------------------
   
   inline
   unsigned int
   ScriptExecutionContextIdentifier::setNextIdentifier(
      unsigned int theValue)
   {
      assert(numberOfIdentifiersM < MaxNoOfIdentifiersE);
      identifiersM[numberOfIdentifiersM++] = theValue;
      return numberOfIdentifiersM - 1;
   }
}

#endif
