#ifndef __WPR_TRACEMANAGER_H__
#define __WPR_TRACEMANAGER_H__
#include <list>
#include <vector>
#define WPR_TRACING_PEER_HASHTBL_SIZE 100
namespace wpr_tracing
{
  class TracePeer;
  class TraceAppender;
  class TraceManager
  {
  public:
    typedef std::list<TracePeer *> PEER_HASH_TBL_SLOT;
    typedef std::vector<PEER_HASH_TBL_SLOT> PEER_HASH_TBL; 

    static 
    TraceManager *
    getInstance();
    
    void
    addFileAppender(const unsigned int peerId, const char * filename );

    void
    addConsoleAppender(const unsigned int peerId);
 
    TracePeer *
    getTracePeer(const unsigned int peerId);

    virtual
    ~TraceManager();

  private:
    TraceManager(); //singleton
    static TraceManager * _instance;
    PEER_HASH_TBL _peerHashTable;
    TraceAppender * _consoleAppender;
    TraceAppender * _fileAppender;
  };
}
#endif
