#ifndef PALTHREADSAFESMARTPOINTEROBJECT_H
#define PALTHREADSAFESMARTPOINTEROBJECT_H

#include "JsReferenceCounter.h"

namespace Js
{
   /** \class ThreadSafeSmartPointerObject Js.h "Js.h"
    *  \ingroup PALBASE 
    * The class is the base class for all thread-safe smart pointer
    * objects.
    */
   class ThreadSafeSmartPointerObject
   {
   public:

      /**
       * Increment the reference count for the object. 
       * This method should be called for every new copy of a pointer 
       * to the given object. 
       */
      void 
      AddRef() const;

      /**
       * Decrements the reference count for the object. 
       * If the reference count on the object falls 
       * to 0, the object is deleted.
       */
      void 
      Release() const;

   protected:

      /**
       * Creates a reference counted object and sets its
       * reference counter to zero.
       */
      ThreadSafeSmartPointerObject();

      /**
       * copy constructor
       * Exists just because the value of the reference counter must never be copied.
       */
      ThreadSafeSmartPointerObject(const ThreadSafeSmartPointerObject& theOther);

      /**
       * assignment operator
       * default one is fine. Reference Counter is correct.
       */

      /**
       * Destructor.
       */
      virtual
      ~ThreadSafeSmartPointerObject();

   private:

      mutable ReferenceCounter referenceCounterM;

   };

   // ------------------------------------------------------------------------

   inline
   void 
   ThreadSafeSmartPointerObject::AddRef() const
   {
      referenceCounterM.increment();
   }

   // ------------------------------------------------------------------------

   inline
   void 
   ThreadSafeSmartPointerObject::Release() const
   {
      if (referenceCounterM.decrement() == 0)
      {
         delete this;
      }
   }

   // ------------------------------------------------------------------------

   inline
   ThreadSafeSmartPointerObject::ThreadSafeSmartPointerObject()
   :  referenceCounterM(0)
   {
   }

   // ------------------------------------------------------------------------

   inline
   ThreadSafeSmartPointerObject::ThreadSafeSmartPointerObject(const ThreadSafeSmartPointerObject& theOther)
   :  referenceCounterM(0)
   {
   }

   // ------------------------------------------------------------------------

   inline
   ThreadSafeSmartPointerObject::~ThreadSafeSmartPointerObject()
   {
      // empty
   }
}

#endif
