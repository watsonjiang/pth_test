inline
ScriptTraceContext::ScriptTraceContext()
:  levelOfDetailM(0)
   //traceLogM(TraceLog::instance())
{
   setDefaultTraceMode();
}

// ----------------------------------------------------------------------------

inline
void
ScriptTraceContext::disableTraceLog()
{
   identifierM.erase();
   setDefaultTraceMode();
   levelOfDetailM = 0;
}

// ----------------------------------------------------------------------------

inline
bool
ScriptTraceContext::enableTraceLog(
   const std::string& theIdentifier)
{
   /*levelOfDetailM = traceLogM->getLevelOfDetail(theIdentifier);
   if (levelOfDetailM > 0)
   {
      identifierM = theIdentifier;
      setTraceMode();
      return true;
   }
   else
   {
      identifierM.erase();
      setDefaultTraceMode();
      return false;
   }*/
   return true;
}
      
// ----------------------------------------------------------------------------

inline
const std::string&
ScriptTraceContext::getIdentifier() const
{
   return identifierM;
}

// ----------------------------------------------------------------------------

inline
int
ScriptTraceContext::getLevelOfDetail() const
{
   return levelOfDetailM;
}

// ----------------------------------------------------------------------------

inline
std::string
ScriptTraceContext::getOption(
   const std::string& theOptionName) const
{
   //return traceLogM->getOption(identifierM, theOptionName);
   return "";
}

// ----------------------------------------------------------------------------

inline
ScriptTraceContext::ScriptTraceMode
ScriptTraceContext::getScriptTraceMode() const
{
   return scriptTraceModeM;
}

// ----------------------------------------------------------------------------

inline
bool
ScriptTraceContext::isEnabled() const
{
   return levelOfDetailM > 0;
}

// ----------------------------------------------------------------------------

inline
bool
ScriptTraceContext::isEnabled(
   int theLevelOfDetail) const
{
   return levelOfDetailM >= theLevelOfDetail;
}

// ----------------------------------------------------------------------------

inline
bool
ScriptTraceContext::optionExists(
   const std::string& theOptionName) const
{
   //return traceLogM->optionExists(identifierM, theOptionName);
   return false;
}

// ----------------------------------------------------------------------------

inline
void
ScriptTraceContext::writeLogRecord(
   TraceLogRecord* theLogRecord)
{
   //levelOfDetailM = traceLogM->log(identifierM, theLogRecord);
   if (levelOfDetailM == 0)
   {
      identifierM.erase();
      setDefaultTraceMode();
   }
   else
   {
      setTraceMode();
   }
}

