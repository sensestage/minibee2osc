

#include "MiniHive.h"

#include <iostream>
#include <string.h>
#include <unistd.h>

#include <sstream>

#include "minibee_utils.h"

using namespace libxbee;
using namespace libminibee;

// miniXHiveConnection::miniXHiveConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address): libxbee::ConCallback(parent, type, address)
// {};
// miniXHiveTXConnection::miniXHiveTXConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address): libxbee::ConCallback(parent, type, address)
// {};
// miniXHiveModemConnection::miniXHiveModemConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address): libxbee::ConCallback(parent, type, address)
// {};

// void miniXHiveConnection::xbee_conCallback(libxbee::Pkt **pkt) {
// 	std::cout << "MiniXBee Data Callback!!\n";
// 	printXBeePacket( *pkt );
// 	
// 	if ((*pkt)->size() > 0) {
// 	  if ((*pkt)->size() > 2) { // type and msg id
// 	    char type = (**pkt)[0];
// 	    int msgid = (int) (**pkt)[1];
// 	    int msgsize = (*pkt)->size();
// 	    std::vector<unsigned char> data = (*pkt)->getVector();
// 	    minihive->parseDataPacket( type, msgid, (*pkt)->size(), (*pkt)->getVector() );
// 	  }
// 	}
// }
// 
// void miniXHiveTXConnection::xbee_conCallback(libxbee::Pkt **pkt) {
// 	std::cout << "MiniXBee Tx Callback!!\n";
// 	printXBeePacket( *pkt );
// }
// 
// void miniXHiveModemConnection::xbee_conCallback(libxbee::Pkt **pkt) {
// 	std::cout << "MiniXBee Modem Callback!!\n";
// 	printXBeePacket( *pkt );
// }


MiniXHive::MiniXHive(void)
{
  numberOfBees = 0;
  mymsgid = 0;
}

MiniXHive::~MiniXHive(void)
{
  delete xbee;
//   free(&addr);
}

int MiniXHive::createXBee(std::string serialport, int loglevel)
{
  try {
      xbee = new XBee("xbee1", serialport, 57600);
      setLogLevel(loglevel);
    //   xbee.setLogTarget();
      
      struct xbee_conAddress addr;
      /* make connections for the first data */
      memset(&addr, 0, sizeof(addr));
      addr.addr16_enabled = 1;
      addr.addr16[0] = 0xFF;
      addr.addr16[1] = 0xFA;
      
    //   conData16 = new miniXHiveConnection( *xbee, "16-bit Data", &addr );
    //   conData16->minihive = this;
      con = new libxbee::Con( *xbee, "16-bit Data", &addr );
      
      std::ostringstream oss;
      oss << "created 16-bit data connection for Hive";
      writeToLog(10, oss.str() );
      
      struct xbee_conAddress addrCatchall;
      /* make connections for the first data */
      memset(&addrCatchall, 0, sizeof(addrCatchall));
      addrCatchall.addr16_enabled = 1;
      addrCatchall.addr16[0] = 0xFF;
      addrCatchall.addr16[1] = 0xFF;

      struct xbee_conSettings settings;
      conCatchAll = new libxbee::Con( *xbee, "16-bit Data", &addrCatchall );
      
      std::ostringstream oss2;
      oss2 << "created 16-bit data catchall connection";
      writeToLog(10, oss2.str() );
      
      conCatchAll->getSettings(&settings);
      settings.catchAll = 1;
      conCatchAll->setSettings(&settings);


      struct xbee_conAddress addrCatchall64;
      /* make connections for the first data */
      memset(&addrCatchall64, 0, sizeof(addrCatchall64));
      addrCatchall64.addr64_enabled = 1;
      addrCatchall64.addr64[0] = 0x00;
      addrCatchall64.addr64[1] = 0x00;
      addrCatchall64.addr64[2] = 0x00;
      addrCatchall64.addr64[3] = 0x00;
      addrCatchall64.addr64[4] = 0x00;
      addrCatchall64.addr64[5] = 0x00;
      addrCatchall64.addr64[6] = 0xFF;
      addrCatchall64.addr64[7] = 0xFF;


      conCatchAll64 = new libxbee::Con( *xbee, "64-bit Data", &addrCatchall64 );
      
      std::ostringstream oss3;
      oss3 << "created 64-bit data catchall connection";
      writeToLog( 10, oss3.str() );
      
      conCatchAll64->getSettings(&settings);
      settings.catchAll = 1;
      conCatchAll64->setSettings(&settings);
      
      return 0;
    } catch (xbee_err err) {
      std::ostringstream oss;
      oss << "Could not create XBee connection: " << err << "\n";
      writeToLog( 1, oss.str() );
      return 1;
    }

}

int MiniXHive::waitForOSC()
{
  return oscServer->receive( 0 );
}


int MiniXHive::waitForPacket(){
  libxbee::Pkt pkt;
  struct xbee_pkt *pkthandle;

  if ( con->RxAvailable() > 0 ){
    try {
	    //con >> pkt; /* like this */
	    pkt << *con;   /* or this */
    } catch (xbee_err err) {
      std::ostringstream oss;
      oss << "FFFA: Error on Rx! " << err << "\n";
      writeToLog( 1, oss.str() );
      return 1;
    }
    try {
      std::ostringstream oss;
      streamXBeePacket( &pkt, &oss );
      writeToLog( 10, oss.str() );
// 	if ( this->getLogLevel() > 0 ){
// 	  printXBeePacket( &pkt );
// 	}
	if ( pkt.size() > 2) { // type and msg id
	    char type = pkt[0];
	    int msgid = (int) pkt[1];
	    int msgsize = pkt.size();
	    std::vector<unsigned char> data = pkt.getVector();
	    this->parseDataPacket( type, msgid, pkt.size(), pkt.getVector() );
	}
    } catch (xbee_err err) {
      std::ostringstream oss;
      oss << "FFFA: Error accessing packet! " << err << "\n";
      writeToLog(1, oss.str() );
      return 1;
    }
  }

  if ( conCatchAll->RxAvailable() > 0 ){
    try {
	    //con >> pkt; /* like this */
	    pkt << *conCatchAll;   /* or this */
    } catch (xbee_err err) {
      std::ostringstream oss;
      oss << "Catchall: Error on Rx! " << err << "\n";
      writeToLog(1, oss.str() );
      return 1;
    }
    try {
 	pkthandle = pkt.getHnd();
	if ( this->getLogLevel() > 0 ){
	  printXBeePacket( &pkt );
	}
	if ( pkt.size() > 2) { // type and msg id
	    char type = pkt[0];
	    int msgid = (int) pkt[1];
	    int msgsize = pkt.size();
	    std::vector<unsigned char> data = pkt.getVector();
	    this->parseDataPacketCatchall( type, msgid, pkt.size(), pkt.getVector(), &(pkthandle->address) );
	}
    } catch (xbee_err err) {
      std::ostringstream oss;
      oss << "Catchall: Error accessing packet! " << err << "\n";
      writeToLog( 1, oss.str() );
      return 1;
    }
  }

    if ( conCatchAll64->RxAvailable() > 0 ){
    try {
	    //con >> pkt; /* like this */
	    pkt << *conCatchAll64;   /* or this */
    } catch (xbee_err err) {
      std::ostringstream oss;
      oss << "Catchall64: Error on Rx! " << err << "\n";
      writeToLog( 1, oss.str() );
      return 1;
    }
    try {
 	pkthandle = pkt.getHnd();
	std::ostringstream oss;
	streamXBeePacket( &pkt, &oss );
	writeToLog( 10, oss.str() );
// 	if ( this->getLogLevel() > 0 ){
// 	  printXBeePacket( &pkt );
// 	}
	if ( pkt.size() > 2) { // type and msg id
	    char type = pkt[0];
	    int msgid = (int) pkt[1];
	    int msgsize = pkt.size();
	    std::vector<unsigned char> data = pkt.getVector();
	    this->parseDataPacketCatchall( type, msgid, pkt.size(), pkt.getVector(), &(pkthandle->address) );
	}
    } catch (xbee_err err) {
      std::ostringstream oss;
      oss << "Catchall64: Error accessing packet! " << err << "\n";
      writeToLog( 1, oss.str() );
      return 1;
    }
  }

//   if ( conTXStatus->RxAvailable() > 0 ){
//     try {
// 	    pkt << *con;   /* or this */
//     } catch (xbee_err err) {
// 	    std::cout << "Error on Rx Tx status! " << err << "\n";
// 	    return 1;
//     }
//     try {
// 	printXBeePacket( &pkt );      
//     } catch (xbee_err err) {
// 	    std::cout << "Error accessing TX status packet! " << err << "\n";
// 	    return 1;
//     }
//   }

  this->waitForBeePackets();
}

void MiniXHive::waitForBeePackets(){
  std::map<int,MiniXBee*>::iterator beeIt;
//   std::cout << "packets from minibees: " << minibees.size() << std::endl;
  for ( beeIt = minibees.begin(); beeIt != minibees.end(); ++beeIt ){
    MiniXBee * minibee = beeIt->second;
    if ( minibee != NULL ){
      minibee->waitForPacket();
    }
  }  
}

void MiniXHive::parseDataPacket( char type, int msgid, int msgsize, std::vector<unsigned char> data ){
  struct xbee_conAddress newAddress;

  std::ostringstream oss;
  switch ( type ){
    case MINIBEE_N_SER: // serial number
      // Serial High (SH) + Serial Low (SL) + library version + board revision + capabilities
      oss << "FFFA: minibee serial message " << type << ", " << msgid << ", " << msgsize <<  std::endl;
      if ( msgsize > 10 ){
	  memset(&newAddress, 0, sizeof(newAddress));
	  newAddress.addr64_enabled = 1;
	  std::vector<unsigned char>::iterator it = data.begin();
	  it++; it++; // start at index 2
	  for ( int i=0; i<8; i++ ){
	    newAddress.addr64[i] = *it;
	    ++it;
	  }
	  streamXBeeAddress( newAddress, &oss );
// 	  printXBeeAddress( newAddress );
	  MiniXBee * minibee = findMiniBeeByAddress( newAddress );
	  if ( minibee == NULL ){
	     // create a new minibee
	    minibee = createNewMiniBee(newAddress);
	  }
        minibee->parse_serial_message_noaddress(msgsize, data);
      }
      break;
    case MINIBEE_N_INFO:
      oss << "FFFA: minibee info message " << type << ", " << msgid << ", " << msgsize << std::endl;
      break;
    default:
      oss << "WARNING: minibee message " << type << ", " << msgid << ", " << msgsize << std::endl;
      break;
//    case MINIBEE_N_DATA:
//       std::cout << "FIXME: minibee data message " << type << ", " << msgid << ", " << msgsize << std::endl;
//       // send back announce message
//       break;
   case MINIBEE_N_WAIT: // serial number
//       // Serial High (SH) + Serial Low (SL) + library version + board revision + capabilities
      send_announce_message_toFFFA();
      std::cout << "FIXME: minibee waiting message " << type << ", " << msgid << ", " << msgsize <<  std::endl;
      break;
//    case MINIBEE_N_ACTIVE:
//       std::cout << "FIXME: minibee active message " << type << ", " << msgid << ", " << msgsize << std::endl;
//       break;
//    case MINIBEE_N_PAUSED:
//       std::cout << "FIXME: minibee paused message " << type << ", " << msgid << ", " << msgsize << std::endl;
//       break;
  }
  writeToLog( 10, oss.str() );
}


int MiniXHive::send_announce_message_toFFFA(){
  int retval;
  std::vector<unsigned char> mydata;
  mydata.push_back( 'A' );
//   std::cout << "announce message: " << std::endl;
  unsigned char frameid = mymsgid;
  try{
    if ( con->Tx( &frameid, mydata ) != XBEE_ENONE ) {
      std::ostringstream oss;
      oss << "MiniBee: error transmitting 16bit" << std::endl;
      writeToLog(2, oss.str() );
      retval = -1;
    } else {
      retval = 0;
    }
  } catch (xbee_err err) {
      std::ostringstream oss;
      oss << "MiniBee: error transmitting via connection 16bit" << err << "\n";
      writeToLog(1, oss.str() );
      retval = -1;
  } catch (libxbee::xbee_etx etx ){
      std::ostringstream oss;
      oss << "MiniBee: etx error transmitting via connection 16bit" << etx.ret << "\n";
      writeToLog(1, oss.str() );
      retval = -1;   
  }
  mymsgid++;
  mymsgid = mymsgid%256;
  return retval;
}

void MiniXHive::parseDataPacketCatchall( char type, int msgid, int msgsize, std::vector<unsigned char> data, struct xbee_conAddress *address ){
  struct xbee_conAddress newAddress;
  MiniXBee * minibee;
  std::ostringstream oss;

  switch ( type ){
    case MINIBEE_N_SER: // serial number
      // Serial High (SH) + Serial Low (SL) + library version + board revision + capabilities
      oss << "Catchall: minibee serial message " << type << ", " << msgid << ", " << msgsize <<  std::endl;
      if ( msgsize > 10 ){
	  memset(&newAddress, 0, sizeof(newAddress));
	  newAddress.addr64_enabled = 1;
	  std::vector<unsigned char>::iterator it = data.begin();
	  it++; it++; // start at index 2
	  for ( int i=0; i<8; i++ ){
	    newAddress.addr64[i] = *it;
	    ++it;
	  }
	  streamXBeeAddress( newAddress, &oss );
// 	  if ( this->getLogLevel() > 0 ){
// 	    printXBeeAddress( newAddress );
// 	  }
	  minibee = findMiniBeeByAddress( newAddress );
	  if ( minibee == NULL ){
	     // create a new minibee
	    minibee = createNewMiniBee(newAddress);
	    minibee->setHive( this );
	  }
	  minibee->parse_serial_message_noaddress(msgsize, data);
      }
      break;
    case MINIBEE_N_INFO:
      oss << "Catchall: minibee info message " << type << ", " << msgid << ", " << msgsize << std::endl;
      break;
    default:
      oss << "Catchall: minibee message " << type << ", " << msgid << ", " << msgsize << std::endl;
      memset(&newAddress, 0, sizeof(newAddress));
      newAddress.addr16_enabled = address->addr16_enabled;
      newAddress.addr16[0] = address->addr16[0];
      newAddress.addr16[1] = address->addr16[1];
      streamXBeeAddress( newAddress, &oss );
//       printXBeeAddress( newAddress );
      minibee = findMiniBeeByAddress( newAddress );
      if ( minibee == NULL ){
	  // create a new minibee
	minibee = createNewMiniBeeWithID(newAddress);
      }
      minibee->send_announce_message();
      break;
  }
  writeToLog( 10, oss.str() );
}

MiniXBee * MiniXHive::createNewMiniBee( struct xbee_conAddress beeAddress ){
 numberOfBees++;
 MiniXBee * minibee = new MiniXBee( numberOfBees );
 minibee->setHive( this );
 minibee->set64bitAddress( beeAddress, xbee );
 
 beeAddress.addr16_enabled = 1;
 beeAddress.addr16[0] = (unsigned char) (numberOfBees/256);
 beeAddress.addr16[1] = (unsigned char) numberOfBees%256;
 minibee->set16bitAddress( beeAddress, xbee );
//  std::cout << "minibees before insert: " << minibees.size() << std::endl; 
 minibees[numberOfBees] = minibee;
//  std::cout << "minibees after insert: " << minibees.size() << std::endl;
 minibee->createConnections(xbee);
 
 oscServer->sendInfoMessage(numberOfBees, xbeeAddress64AsString( beeAddress ) );

 return minibee;
}

MiniXBee * MiniXHive::createNewMiniBeeWithID( struct xbee_conAddress beeAddress ){
//  numberOfBees++;
 int id = beeAddress.addr16[0]*256 + beeAddress.addr16[1];
 MiniXBee * minibee = new MiniXBee( id );
 minibee->setHive( this );
 minibee->set16bitAddress( beeAddress, xbee );
//  std::cout << "minibees before insert: " << minibees.size() << std::endl; 
 minibees[id] = minibee;
//  std::cout << "minibees after insert: " << minibees.size() << std::endl;
 minibee->createConnections(xbee);
  
 oscServer->sendInfoMessage(id, xbeeAddress64AsString( beeAddress ) );

 return minibee;
}

MiniXBee * MiniXHive::findMiniBeeByAddress( struct xbee_conAddress beeAddress ){
  std::map<int,MiniXBee*>::iterator beeIt;
//   std::cout << "minibees: " << minibees.size() << std::endl;
  for ( beeIt = minibees.begin(); beeIt != minibees.end(); ++beeIt ){
    MiniXBee * minibee = beeIt->second;
    if ( minibee != NULL ){
      if ( beeIt->second->matchAddress(beeAddress) ){
	// match found
	return beeIt->second;
      }
    }
  }
  // no minibees found
  return NULL;
}

void MiniXHive::writeToLog( int level, const char * logstring )
{
  struct xbee * myXBee = xbee->getHnd();
  xbee_log( myXBee, level, logstring );
}

void MiniXHive::writeToLog( int level, std::string logstring )
{
  writeToLog( level, logstring.c_str() );
}

int MiniXHive::getLogLevel()
{
  return xbee->getLogLevel();
}

void MiniXHive::setLogLevel(int level)
{
  xbee->setLogLevel( level );
}

// ---------- OSC ----------

int MiniXHive::createOSCServer(const char* port)
{
  oscServer = new HiveOscServer(port);
  std::ostringstream oss;
  oss << "Created OSC server at port " << oscServer->getPort();
  writeToLog(10, oss.str() );
  oscServer->setHive( this );
  oscServer->addBasicMethods();
}

int MiniXHive::setTargetAddress( const char * host, const char * port ){
    oscServer->setTargetAddress( host, port );
}


// ---------- osc messages to minibees ----------

int MiniXHive::send_output_to_minibee(int minibeeID, std::vector< int > * data)
{
  if ( minibees[ minibeeID ] == NULL ){
      return -1;
  }
  return minibees[ minibeeID ]->send_output( data );
}

int MiniXHive::send_custom_to_minibee(int minibeeID, std::vector< int > * data)
{
  if ( minibees[ minibeeID ] == NULL ){
      return -1;
  }
  return minibees[ minibeeID ]->send_custom( data );
}

int MiniXHive::send_loopback_to_minibee(int minibeeID, int onoff)
{
  if ( minibees[ minibeeID ] == NULL ){
      return -1;
  }
  return minibees[ minibeeID ]->send_loopback( onoff );  
}

int MiniXHive::send_running_to_minibee(int minibeeID, int onoff)
{
  if ( minibees[ minibeeID ] == NULL ){
      return -1;
  }
  return minibees[ minibeeID ]->send_running( onoff );  
}

int MiniXHive::send_reset_to_minibee(int minibeeID)
{
  if ( minibees[ minibeeID ] == NULL ){
      return -1;
  }
  return minibees[ minibeeID ]->send_reset();
}

int MiniXHive::send_save_id_to_minibee(int minibeeID)
{
  if ( minibees[ minibeeID ] == NULL ){
      return -1;
  }
  return minibees[ minibeeID ]->send_save_id();
}

int MiniXHive::send_announce_to_minibee(int minibeeID)
{
  if ( minibees[ minibeeID ] == NULL ){
      return -1;
  }
  return minibees[ minibeeID ]->send_announce_message();
}
