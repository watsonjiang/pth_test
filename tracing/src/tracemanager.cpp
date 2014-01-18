#include "tracemanager.h"
#include "traceappender.h"
#include "tracepeer.h"
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <algorithm>
namespace wpr_tracing
{

  TraceManager* TraceManager::_instance = NULL;
  
  TraceManager *
  TraceManager::getInstance()
  {
    if(NULL == _instance){
      _instance = new TraceManager();
    }
    return _instance;
  }
 
  TraceManager::TraceManager()
  :_peerHashTable(WPR_TRACING_PEER_HASHTBL_SIZE)
  {
    _consoleAppender=new ConsoleTraceAppender();
    _fileAppender=new FileTraceAppender("wpr.log");
  }
  
  TracePeer *
  TraceManager::getTracePeer(const unsigned int peerId)
  {
    int slotId = peerId % WPR_TRACING_PEER_HASHTBL_SIZE; 
    for(PEER_HASH_TBL_SLOT::iterator it = _peerHashTable[slotId].begin(); 
        it != _peerHashTable[slotId].end();
        ++it){
      if((*it)->_peerId==peerId){
        return *it;
      }
    } 
    //not exist, create one and link into list.
    TracePeer * newPeer = new TracePeer(peerId);
    _peerHashTable[slotId].push_back(newPeer); 
    return newPeer;
  }

  void
  TraceManager::addConsoleAppender(const unsigned int peerId)
  {
    TracePeer * peer = getTracePeer(peerId);
    if(peer->_appenderList.end()!=find(peer->_appenderList.begin(),
                                       peer->_appenderList.end(),
                                       _consoleAppender)){
      return; //already have console appender, return immediately.
    }
    peer->_appenderList.push_back(_consoleAppender); 
  }
  
  void 
  TraceManager::addFileAppender(const unsigned int peerId, const char * filename)
  {
    TracePeer * peer = getTracePeer(peerId);
    if(peer->_appenderList.end()!=find(peer->_appenderList.begin(),
                                       peer->_appenderList.end(),
                                       _fileAppender)){
      return;
    }
    peer->_appenderList.push_back(_fileAppender);
  }

  TraceManager::~TraceManager()
  {
    //reclaim all peers.
    for(PEER_HASH_TBL::iterator it=_peerHashTable.begin();
        it!=_peerHashTable.end();
        ++it){
      for(PEER_HASH_TBL_SLOT::iterator it1=(*it).begin();
          it1!=(*it).end();
          ++it1){
        delete (*it1);
      }
    }
    //reclaim all appenders;
    delete _consoleAppender;
    delete _fileAppender;
  }
}

