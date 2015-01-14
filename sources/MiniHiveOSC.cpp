
#include <iostream>
#include <string>
// #include <cstring>

#include "MiniHiveOSC.h"

// #include <iostream>
// #include <string>

using namespace std;

namespace libminibee {


int HiveOscServer::minibeeOutputHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug )
    cout << "[HiveOscServer::minibeeOutputHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";
  
  // TODO
  return 0;
}

int HiveOscServer::minibeeCustomHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug )
    cout << "[HiveOscServer::minibeeCustomHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";
  
  // TODO
  return 0;
}

int HiveOscServer::minibeeConfigHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug )
    cout << "[HiveOscServer::minibeeConfigHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";
  
  // TODO
  return 0;
}

int HiveOscServer::genericHandler( handlerArgs )
{
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  lo_address newaddr = lo_address_copy( addr );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  string tag = string( path );
  
  if ( server->postDebug )
    cout << "[HiveOscServer::genericHandler] arbitrary osc tag: " + server->getContent( path, types, argv, argc, addr ) << "\n";
  
  return 1;
}

void HiveOscServer::setTargetAddress(const char* host, int port){
  targetAddress = lo_address_create_from(host, port);
}

void HiveOscServer::setTargetAddress(const char* host, const char * port){
  targetAddress = lo_address_new(host, port);
}


void HiveOscServer::sendOutputMessage( int minibeeID, vector<float> * data ){
  lo_message msg = lo_message_new();
  lo_message_add_int32( msg, minibeeID );
  for (auto n : *data) {
    lo_message_add_float( msg, n );
  }
  sendMessage( targetAddress, "/minibee/output", msg );
  lo_message_free( msg );
}

void HiveOscServer::sendPrivateMessage( int minibeeID, vector<float> * data ){
  lo_message msg = lo_message_new();
  lo_message_add_int32( msg, minibeeID );
  for (auto n : *data) {
    lo_message_add_float( msg, n );
  }
  sendMessage( targetAddress, "/minibee/private", msg );
  lo_message_free( msg );  
}

void HiveOscServer::sendTriggerMessage( int minibeeID, vector<float> * data ){
  lo_message msg = lo_message_new();
  lo_message_add_int32( msg, minibeeID );
  for (auto n : *data) {
    lo_message_add_float( msg, n );
  }
  sendMessage( targetAddress, "/minibee/trigger", msg );
  lo_message_free( msg );    
}

HiveOscServer::HiveOscServer( const char *port ) : NonBlockOSCServer( port )
{
    postDebug = false;
}

void HiveOscServer::debug( bool onoff ){
     postDebug = onoff;
     if ( postDebug ){
      cout << "debug posting on\n";
     } else {
      cout << "debug posting off\n";
     }
}

HiveOscServer::~HiveOscServer()
{
}

//-------------- basic message interface -----------------

void HiveOscServer::addBasicMethods()
{
	addMethod( "/minibee/output",  NULL, minibeeOutputHandler, this );    // port, name
	addMethod( "/minibee/custom",  NULL, minibeeCustomHandler, this );    // port, name
	addMethod( "/minibee/configuration",  NULL, minibeeConfigHandler, this );    // port, name
	
	// The generic handler must be added last. 
	// Otherwise it would be called instead of the handlers. 
	addMethod( NULL, NULL, genericHandler, this );
}

}
