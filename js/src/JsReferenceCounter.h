#ifndef PALREFERENCECOUNTER_H
#define PALREFERENCECOUNTER_H

#ifdef WIN32
#   include "JsWinInclude.h"
#endif

#ifdef SOLARIS
#   include "JsAtomic.h"
#endif

#ifdef LINUX
#   include <errno.h>
#   include <pthread.h>
#   include <stdint.h>
#endif

namespace Js
{
#  ifdef WIN32

   typedef long RefCntValueType;
   typedef long RefCntDeltaValueType;

#  else // UNIX

   typedef uint32_t RefCntValueType;
   typedef int32_t  RefCntDeltaValueType;

#  endif

   /** \class ReferenceCounter Js.h "Js.h"
	 * \ingroup PALSYNC
    * Reference counters can thread-safe be incremented and decremented.
    */
   class ReferenceCounter
   {
   public:

      /**
       * Default constructor.
       * @param theInitialValue The initial reference counter value.
       */
      ReferenceCounter(
         RefCntValueType theInitialValue = 0);

      /**
       * Destructor.
       */
      ~ReferenceCounter();

      /**
       * Copy constructor.
       * @param theOther The reference counter to be copied.
       */
      ReferenceCounter(
         const ReferenceCounter& theOther);

	  /**
       * Add a value to the reference counter.
       * @param theValueToAdd The value to add.
       */
      void
      add(
         RefCntDeltaValueType theValueToAdd);

      /**
       * Decrement the reference counter by one.
       * @return The new value.
       */ 
      RefCntValueType
      decrement();

      /**
       * Get the value of the reference counter.
       * @return The value of the counter.
       */
      RefCntValueType
      get() const;

      /**
       * Increment the reference counter by one.
       */ 
      void
      increment();

      /**
       * Set the value of the reference counter.
       * @param  theNewValue The new value.
       * @return The previous value.
       */
      RefCntValueType
      set(
         RefCntValueType theNewValue);

   private:
      RefCntValueType counterM;
   };

   // ------------------------------------------------------------------------

   inline
   ReferenceCounter::ReferenceCounter(
      RefCntValueType theInitialValue)
   :  counterM(theInitialValue)
   {
      // empty
   }

   // ------------------------------------------------------------------------

   inline
   ReferenceCounter::~ReferenceCounter()
   {
      // empty
   }

   // ------------------------------------------------------------------------

   inline
   ReferenceCounter::ReferenceCounter(
      const ReferenceCounter& theOther) 
   {
      counterM = theOther.counterM;
   }

   // ------------------------------------------------------------------------

   inline
   void
   ReferenceCounter::add(
      RefCntDeltaValueType theValueToAdd)
   {
#ifdef WIN32
      InterlockedExchangeAdd(&counterM, theValueToAdd);
#endif
#ifdef SOLARIS
      pal_atomic_add_32(&counterM,  theValueToAdd);
#endif
#ifdef LINUX
      counterM += theValueToAdd;
#endif
   }

   // ------------------------------------------------------------------------

   inline
   RefCntValueType
   ReferenceCounter::decrement()
   {
#ifdef WIN32
      return InterlockedDecrement(&counterM);
#endif
#ifdef SOLARIS
      return pal_atomic_dec_32(&counterM);
#endif
#ifdef LINUX
      counterM -= 1;
      RefCntValueType tmp = counterM;
      return tmp;
#endif
   }

   // ------------------------------------------------------------------------

   inline
   RefCntValueType
   ReferenceCounter::get() const
   {
      return counterM;
   }

   // ------------------------------------------------------------------------

   inline
   void
   ReferenceCounter::increment()
   {
#ifdef WIN32
      InterlockedIncrement(&counterM);
#endif
#ifdef SOLARIS
      pal_atomic_inc_32(&counterM);
#endif
#ifdef LINUX
      counterM += 1;
#endif
   }

   // ------------------------------------------------------------------------

   inline
   RefCntValueType
   ReferenceCounter::set(
      RefCntValueType theNewValue)
   {
#ifdef WIN32
      return InterlockedExchange(&counterM, theNewValue);
#endif
#ifdef SOLARIS
      return pal_atomic_swap_32(&counterM,  theNewValue);
#endif
#ifdef LINUX
      RefCntValueType tmp = counterM;
      counterM = theNewValue;
      return tmp;
#endif
   }
}

#endif
