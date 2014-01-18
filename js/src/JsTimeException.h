#ifndef PALTIMEEXCEPTION_H
#define PALTIMEEXCEPTION_H

#include <string>

namespace Js
{
   /** \class TimeException Js.h "Js.h"
	 * \ingroup PALTIME
    *
    */
   class TimeException : public std::exception
   {
   public:

      TimeException(
         const char* theDescription)
         throw ();

      TimeException(
         const TimeException& theOther)
         throw ();

      TimeException& 
      operator= (
         const TimeException& theOther)
         throw ();

      virtual 
      ~TimeException()
         throw ();

      virtual 
      const char* 
      what() const
         throw ();

   private:

      std::string descriptionM;
   };

   // ------------------------------------------------------------------------

   inline
   TimeException::TimeException(
      const char* theDescription)
      throw ()
   :  descriptionM("TimeException: ")
   {
      descriptionM += theDescription;
      // empty
   }

   // ------------------------------------------------------------------------

   inline
   TimeException::TimeException(
      const TimeException& theOther)
      throw ()
   :  descriptionM(theOther.descriptionM)
   {
      // empty
   }

   // ------------------------------------------------------------------------

   inline
   TimeException& 
   TimeException::operator=(
      const TimeException& theOther)
      throw ()
   {
      if (this != &theOther)
      {
         descriptionM = theOther.descriptionM;
      }
      return *this;
   }

   // ------------------------------------------------------------------------

   inline
   TimeException::~TimeException()
      throw ()
   {
      // empty
   }

   // ------------------------------------------------------------------------

   inline
   const char* 
   TimeException::what() const
      throw ()
   {
      return descriptionM.c_str();
   }
}

#endif

