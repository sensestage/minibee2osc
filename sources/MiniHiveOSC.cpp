
#include <iostream>
#include <string>
// #include <cstring>

#include "MiniHiveOSC.h"

#include "MiniHive.h"

// #include <iostream>
// #include <string>

using namespace std;

namespace libminibee {


int HiveOscServer::minibeeOutputHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug ){
    cout << "[HiveOscServer::minibeeOutputHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";
  }
  
  if ( argc < 1 ){
      std::cout << "minibeeOutputHandler: too few arguments" << std::endl;
      return 0;
  }

  vector<int> mydata;
  int id = argv[0]->i;
  for ( int i=1; i<argc; i++ ){
      mydata.push_back( argv[i]->i );
  }
  server->handle_minibee_output( id, &mydata, 0 );
  return 0;
}

int HiveOscServer::minibeeCustomHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug ){
    cout << "[HiveOscServer::minibeeCustomHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";
  }
  if ( argc < 1 ){
      std::cout << "minibeeCustomHandler: too few arguments" << std::endl;
      return 0;
  }
  vector<int> mydata;
  int id = argv[0]->i;
  for ( int i=1; i<argc; i++ ){
      mydata.push_back( argv[i]->i );
  }
  server->handle_minibee_custom( id, &mydata, 0 );
  return 0;
}


int HiveOscServer::minibeeOutputNoAckHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug ){
    cout << "[HiveOscServer::minibeeOutputNoAckHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";
  }
  
  if ( argc < 1 ){
      std::cout << "minibeeOutputNoAckHandler: too few arguments" << std::endl;
      return 0;
  }

  vector<int> mydata;
  int id = argv[0]->i;
  for ( int i=1; i<argc; i++ ){
      mydata.push_back( argv[i]->i );
  }
  server->handle_minibee_output( id, &mydata, 1 );
  return 0;
}

int HiveOscServer::minibeeCustomNoAckHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug ){
    cout << "[HiveOscServer::minibeeCustomNoAckHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";
  }
  if ( argc < 1 ){
      std::cout << "minibeeCustomNoAckHandler: too few arguments" << std::endl;
      return 0;
  }
  vector<int> mydata;
  int id = argv[0]->i;
  for ( int i=1; i<argc; i++ ){
      mydata.push_back( argv[i]->i );
  }
  server->handle_minibee_custom( id, &mydata, 1 );
  return 0;
}


int HiveOscServer::minibeeConfigHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug ){
    cout << "[HiveOscServer::minibeeConfigHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";
  }
//   possible return messages:
// /minibee/configuration/done node id, config id, serial number (optional)
// /minibee/configuration/error node id, config id, serial number (optional)

  if ( argc < 2 ){
      std::cout << "MinibeeConfigMessage: too few arguments" << std::endl;
      return 0;
  }
  int id = argv[0]->i;
  int configid = argv[1]->i;
  server->handle_minibee_config( id, configid );
  return 0;
}

int HiveOscServer::minibeeRunHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug ){
    cout << "[HiveOscServer::minibeeRunHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";
  }
  if ( argc < 2 ){
      std::cout << "MinibeeRunMessage: too few arguments" << std::endl;
      return 0;
  }
  int id = argv[0]->i;
  int onoff = argv[1]->i;
  server->handle_minibee_run( id, onoff );
  return 0;
}

int HiveOscServer::minibeeLoopbackHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug ){
    cout << "[HiveOscServer::minibeeLoopbackHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";
  }
  if ( argc < 2 ){
      std::cout << "MinibeeLoopBackMessage: too few arguments" << std::endl;
      return 0;
  }
  int id = argv[0]->i;
  int onoff = argv[1]->i;
  server->handle_minibee_loopback( id, onoff );
  return 0;
}


int HiveOscServer::minibeeResetHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug ){
    cout << "[HiveOscServer::minibeeResetHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";
  }
  
  if ( argc < 1 ){
      std::cout << "MinibeeResetMessage: too few arguments" << std::endl;
      return 0;
  }
  int id = argv[0]->i;
  server->handle_minibee_reset( id );
  return 0;
}

int HiveOscServer::minibeeAnnounceHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug ){
    cout << "[HiveOscServer::minibeeAnnounceHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";
  }
  if ( argc < 1 ){
      std::cout << "MinibeeAnnounceMessage: too few arguments" << std::endl;
      return 0;
  }
  int id = argv[0]->i;
  server->handle_minibee_announce( id );
  return 0;
}

int HiveOscServer::minibeeSaveIDHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug ){
    cout << "[HiveOscServer::minibeeSaveIDHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";
  }
  if ( argc < 1 ){
      std::cout << "MinibeeSaveIDMessage: too few arguments" << std::endl;
      return 0;
  }
  int id = argv[0]->i;
  server->handle_minibee_saveid( id );
  return 0;
}

int HiveOscServer::minihiveResetHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug ){
    cout << "[HiveOscServer::minibeeResetHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";
  }
  if ( argc < 1 ){
      std::cout << "MinibeeResetMessage: too few arguments" << std::endl;
      return 0;
  }
  int id = argv[0]->i;
  server->handle_minibee_reset( id );
  return 0;
}


int HiveOscServer::minihiveSaveConfigHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug )
    cout << "[HiveOscServer::minihiveSaveConfigHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";
  
  // TODO
  return 0;
}

int HiveOscServer::minihiveLoadConfigHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug )
    cout << "[HiveOscServer::minihiveLoadConfigHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";
  
  // TODO
  return 0;
}

int HiveOscServer::minihiveAnnounceHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug )
    cout << "[HiveOscServer::minihiveAnnounceHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";
  
  // TODO
  return 0;
}

int HiveOscServer::minihiveSaveIDHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug )
    cout << "[HiveOscServer::minihiveSaveIDHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";
  
  // TODO
  return 0;
}

int HiveOscServer::minihiveQuitHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug )
    cout << "[HiveOscServer::minihiveQuitHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";
  
  server->handle_quit();
  return 0;
}

int HiveOscServer::minihiveTickHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug )
    cout << "[HiveOscServer::minihiveTickHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";
  
  server->handle_tick();
  return 0;
}

int HiveOscServer::minihivePingHandler( handlerArgs )
{ 
  lo_message msg = (lo_message) data;
  lo_address addr = lo_message_get_source( msg );
  HiveOscServer* server = ( HiveOscServer* ) user_data;

  if ( server->postDebug )
    cout << "[HiveOscServer::minihivePingHandler] " + server->getContent( path, types, argv, argc, addr ) << "\n";

  int id = argv[0]->i;  
  server->handle_ping( id );
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

// ---------------- handling incoming osc messages ---------

void HiveOscServer::handle_quit()
{
  hive->quit();
}

void HiveOscServer::handle_tick()
{
  hive->tick();
  // send tock back
}

void HiveOscServer::handle_ping( int id )
{
  hive->tick();
  // send tock back
  lo_message msg = lo_message_new();
  lo_message_add_int32( msg, id );
  sendMessage( targetAddress, "/minihive/pong", msg );
  lo_message_free( msg );
}

void HiveOscServer::handle_minibee_output(int minibeeID, vector< int >* data, unsigned char noAck )
{
  if ( hive->send_output_to_minibee( minibeeID, data, noAck ) < 0 ){
      // error message
    sendOutputErrorMessage( minibeeID, data );
//     std::cout << "Error sending output message to minibee: " << minibeeID << std::endl;
  } else {
    sendOutputSuccessMessage( minibeeID, data );
  }
}

void HiveOscServer::handle_minibee_custom(int minibeeID, vector< int >* data, unsigned char noAck )
{
  if ( hive->send_custom_to_minibee( minibeeID, data, noAck ) < 0 ){
      // error message
    sendCustomErrorMessage( minibeeID, data );
//     std::cout << "Error sending custom message to minibee: " << minibeeID << std::endl;
  } else {
    sendCustomSuccessMessage( minibeeID, data );
  }
}

void HiveOscServer::handle_minibee_config(int minibeeID, int configID)
{
  //TODO
//   if ( hive->send_config_to_minibee( minibeeID, configID ) < 0 ){
//       // error message
//     std::cout << "Error sending config message to minibee: " << minibeeID << std::endl;
//   }
}

void HiveOscServer::handle_minibee_loopback(int minibeeID, int onoff)
{
  if ( hive->send_loopback_to_minibee( minibeeID, onoff ) < 0 ){
      // error message
    std::cout << "Error sending loopback message to minibee: " << minibeeID << std::endl;
  }
}

void HiveOscServer::handle_minibee_run(int minibeeID, int onoff)
{
  if ( hive->send_running_to_minibee( minibeeID, onoff ) < 0 ){
    sendRunErrorMessage( minibeeID, onoff );
      // error message
//     std::cout << "Error sending running message to minibee: " << minibeeID << std::endl;
  } else {
    sendRunSuccessMessage( minibeeID, onoff );
  }

}

void HiveOscServer::handle_minibee_reset(int minibeeID)
{
  if ( hive->send_reset_to_minibee( minibeeID ) < 0 ){
      // error message
    std::cout << "Error sending reset message to minibee: " << minibeeID << std::endl;
  }

}

void HiveOscServer::handle_minibee_saveid(int minibeeID)
{
  if ( hive->send_save_id_to_minibee( minibeeID ) < 0 ){
      // error message
    std::cout << "Error sending save id message to minibee: " << minibeeID << std::endl;
  }
}

void HiveOscServer::handle_minibee_announce(int minibeeID)
{
  if ( hive->send_announce_to_minibee( minibeeID ) < 0 ){
      // error message
    std::cout << "Error sending announce message to minibee: " << minibeeID << std::endl;
  }
}

// void HiveOscServer::handle_minihive_saveconfig(string filename)
// {
// 
// }
// 
// void HiveOscServer::handle_minihive_loadconfig(string filename)
// {
// 
// }

void HiveOscServer::handle_minihive_reset()
{
//TODO
}

void HiveOscServer::handle_minihive_saveid()
{
//TODO
}

void HiveOscServer::handle_minihive_announce()
{
//TODO
}

// ----------- sending messages to client program ----------

void HiveOscServer::sendStatusMessage( int minibeeID, string status, int statusid ){
  lo_message msg = lo_message_new();
  lo_message_add_int32( msg, minibeeID );
  lo_message_add_string( msg, status.c_str() );
  lo_message_add_int32( msg, statusid );
  sendMessage( targetAddress, "/minibee/status", msg );
  lo_message_free( msg );
}

void HiveOscServer::sendInfoMessage( int minibeeID, string serialnumber, int noin, int noout ){
  lo_message msg = lo_message_new();
  lo_message_add_string( msg, serialnumber.c_str() );
  lo_message_add_int32( msg, minibeeID );
  lo_message_add_int32( msg, noin );
  lo_message_add_int32( msg, noout );
  sendMessage( targetAddress, "/minibee/info", msg );
  lo_message_free( msg );
}


void HiveOscServer::sendOutputErrorMessage( int minibeeID, std::vector<int> * data ){
  lo_message msg = lo_message_new();
  lo_message_add_int32( msg, minibeeID );
  for (auto n : *data) {
    lo_message_add_int32( msg, n );
  }
  sendMessage( targetAddress, "/minibee/output/error", msg );
  lo_message_free( msg );
}

void HiveOscServer::sendRunErrorMessage( int minibeeID, int onoff ){
  lo_message msg = lo_message_new();
  lo_message_add_int32( msg, minibeeID );
  lo_message_add_int32( msg, onoff );
  sendMessage( targetAddress, "/minibee/run/error", msg );
  lo_message_free( msg );
}

void HiveOscServer::sendCustomErrorMessage( int minibeeID, std::vector<int> * data ){
  lo_message msg = lo_message_new();
  lo_message_add_int32( msg, minibeeID );
  for (auto n : *data) {
    lo_message_add_int32( msg, n );
  }
  sendMessage( targetAddress, "/minibee/custom/error", msg );
  lo_message_free( msg );
}

void HiveOscServer::sendOutputSuccessMessage( int minibeeID, std::vector<int> * data ){
  lo_message msg = lo_message_new();
  lo_message_add_int32( msg, minibeeID );
  for (auto n : *data) {
    lo_message_add_int32( msg, n );
  }
  sendMessage( targetAddress, "/minibee/output/success", msg );
  lo_message_free( msg );
}

void HiveOscServer::sendRunSuccessMessage( int minibeeID, int onoff ){
  lo_message msg = lo_message_new();
  lo_message_add_int32( msg, minibeeID );
  lo_message_add_int32( msg, onoff );
  sendMessage( targetAddress, "/minibee/run/success", msg );
  lo_message_free( msg );
}

void HiveOscServer::sendCustomSuccessMessage( int minibeeID, std::vector<int> * data ){
  lo_message msg = lo_message_new();
  lo_message_add_int32( msg, minibeeID );
  for (auto n : *data) {
    lo_message_add_int32( msg, n );
  }
  sendMessage( targetAddress, "/minibee/custom/success", msg );
  lo_message_free( msg );
}

void HiveOscServer::sendOutputMessage( int minibeeID, std::vector<float> * data ){
  lo_message msg = lo_message_new();
  lo_message_add_int32( msg, minibeeID );
  for (auto n : *data) {
    lo_message_add_float( msg, n );
  }
  sendMessage( targetAddress, "/minibee/data", msg );
  lo_message_free( msg );
}

void HiveOscServer::sendOutputBlockMessage( int minibeeID, int noSamples, std::vector<float> * data ){
  lo_message msg = lo_message_new();
  lo_message_add_int32( msg, minibeeID );
  lo_message_add_int32( msg, noSamples );
  for (auto n : *data) {
    lo_message_add_float( msg, n );
  }
  sendMessage( targetAddress, "/minibee/data/block", msg );
  lo_message_free( msg );
}

void HiveOscServer::sendPrivateMessage( int minibeeID, std::vector<float> * data ){
  lo_message msg = lo_message_new();
  lo_message_add_int32( msg, minibeeID );
  for (auto n : *data) {
    lo_message_add_float( msg, n );
  }
  sendMessage( targetAddress, "/minibee/private", msg );
  lo_message_free( msg );  
}

void HiveOscServer::sendTriggerMessage( int minibeeID, std::vector<float> * data ){
  lo_message msg = lo_message_new();
  lo_message_add_int32( msg, minibeeID );
  for (auto n : *data) {
    lo_message_add_float( msg, n );
  }
  sendMessage( targetAddress, "/minibee/trigger", msg );
  lo_message_free( msg );    
}

void HiveOscServer::sendPrivateMessage( int minibeeID, std::vector<unsigned char> * data ){
  lo_message msg = lo_message_new();
  lo_message_add_int32( msg, minibeeID );
  for (auto n : *data) {
    lo_message_add_int32( msg, n );
  }
  sendMessage( targetAddress, "/minibee/private", msg );
  lo_message_free( msg );  
}

void HiveOscServer::sendTriggerMessage( int minibeeID, std::vector<unsigned char> * data ){
  lo_message msg = lo_message_new();
  lo_message_add_int32( msg, minibeeID );
  for (auto n : *data) {
    lo_message_add_int32( msg, n );
  }
  sendMessage( targetAddress, "/minibee/trigger", msg );
  lo_message_free( msg );    
}

// ------------- creation methods and setting parameters ------------------

HiveOscServer::HiveOscServer( const char *port ) : BlockingOSCServer( port )
{
  postDebug = false;
}

HiveOscServer::HiveOscServer( HiveOscServer * orig ) : BlockingOSCServer( orig ){
  postDebug = orig->postDebug;
}

void HiveOscServer::setHive(MiniXHive* inhive)
{
  hive = inhive;
}

void HiveOscServer::setTargetAddress(const char* host, int port){
  targetAddress = lo_address_create_from(host, port);
}

void HiveOscServer::setTargetAddress(const char* host, const char * port){
  targetAddress = lo_address_new(host, port);
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
//   	addMethod( NULL, NULL, genericHandler, this );

  	addMethod( "/minihive/tick",  NULL, minihiveTickHandler, this );    // port, name
  	addMethod( "/minihive/ping",  NULL, minihivePingHandler, this );    // port, name
  	addMethod( "/minihive/quit",  NULL, minihiveQuitHandler, this );    // port, name

	addMethod( "/minibee/output",  NULL, minibeeOutputHandler, this );    // port, name
	addMethod( "/minibee/custom",  NULL, minibeeCustomHandler, this );    // port, name
	addMethod( "/minibee/output/noack",  NULL, minibeeOutputNoAckHandler, this );    // port, name
	addMethod( "/minibee/custom/noack",  NULL, minibeeCustomNoAckHandler, this );    // port, name
	addMethod( "/minibee/configuration",  "iis", minibeeConfigHandler, this );    // port, name
	addMethod( "/minibee/configuration",  "ii", minibeeConfigHandler, this );    // port, name

// 	addMethod( "/minihive/configuration",  "s", minihiveLoadConfigHandler, this );    // port, name
// 	addMethod( "/minihive/configuration",  "s", minihiveSaveConfigHandler, this );    // port, name
	
// --- load and save config ---
// /minihive/configuration/save filename
// /minihive/configuration/load filename

	addMethod( "/minibee/run",  "ii", minibeeRunHandler, this );    // port, name
	addMethod( "/minibee/reset",  "i", minibeeResetHandler, this );    // port, name
	addMethod( "/minibee/saveid",  "i", minibeeSaveIDHandler, this );    // port, name
	addMethod( "/minibee/announce", "i", minibeeAnnounceHandler, this );    // port, name
// 
// 	addMethod( "/minihive/reset",  "", minihiveResetHandler, this );    // port, name
// 	addMethod( "/minihive/ids/save",  "", minihiveSaveIDHandler, this );    // port, name
// 	addMethod( "/minihive/announce", "", minihiveAnnounceHandler, this );    // port, name
// 
	addMethod( "/minibee/loopback", "ii", minibeeLoopbackHandler, this );    // port, name

	// The generic handler must be added last. 
	// Otherwise it would be called instead of the handlers. 
	addMethod( NULL, NULL, genericHandler, this );
} // end class

} // end namespace


/*
--- set configuration ---
/minihive/configuration/create

Format:
i config id
s config name
i samples per message
i message interval
i number of pins defined (N)
i number of TWI devices defined (M)

then N times:
  s      - pin id (e.g. A0)
  s or i - pin function (e.g. 3, or 'AnalogIn')
  s      - pin label (e.g. light)

then M times:
  i      - twi id (e.g. 0)
  s or i - twi function (e.g. 10, or 'ADXL345')
  s      - twi label (e.g. accelero)

/minihive/configuration/short (as above but without separate pin definitions; those are done separately by the message that follow)
/minihive/configuration/pin config id, pinid, pinconfig
/minihive/configuration/twi config id, twiid, twiconfig

--- query configuration ---
/minihive/configuration/query config id

/minihive/configuration/pin/query config id, pinid
/minihive/configuration/twi/query config id, twiid



--- delete a configuration
/minihive/configuration/delete config id

  possible return messages:
    /minihive/configuration/error config id
    /minihive/configuration/delete/done config id
*/	
