#ifndef PALSCRIPTOBJECTACTIVATION_H
#define PALSCRIPTOBJECTACTIVATION_H

#include "JsScriptObject.h"

namespace Js
{
   class ScriptObjectActivation : public ScriptObject
   {
   public:

      ScriptObjectActivation();

      virtual
      ~ScriptObjectActivation();

      /**
       * Check if this is an activation object.
       */
      bool
      isActivationObject() const;
   };
}

#endif
