

#include "MiniHive.h"

#include <iostream>
#include <string.h>
#include <unistd.h>

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
      
      /* make connections for the first data */
      memset(&addr, 0, sizeof(addr));
      addr.addr16_enabled = 1;
      addr.addr16[0] = 0xFF;
      addr.addr16[1] = 0xFA;
      
    //   conData16 = new miniXHiveConnection( *xbee, "16-bit Data", &addr );
    //   conData16->minihive = this;
      con = new libxbee::Con( *xbee, "16-bit Data", &addr );
      std::cout << "created 16-bit data connection" << std::endl;
      conTXStatus = new libxbee::Con( *xbee, "Transmit Status" );
      std::cout << "created transmit status" << std::endl;
    //   conTXStatus16 = new miniXHiveTXConnection( *xbee, "Transmit Status", &addr );
    //   conTXStatus16->minihive = this;
    //   std::cout << "created tx connection connection" << std::endl;
    //   conModemStatus = new miniXHiveModemConnection( *xbee, "Modem Status", &addr );
    //   conModemStatus->minihive = this;
    //   std::cout << "created modem status connection" << std::endl;
      return 0;
    } catch (xbee_err err) {
      std::cout << "Could not create XBee connection: " << err << "\n";
      return 1;
    }

}

int MiniXHive::waitForPacket(){
  libxbee::Pkt pkt;

  if ( con->RxAvailable() > 0 ){
    try {
	    //con >> pkt; /* like this */
	    pkt << *con;   /* or this */
    } catch (xbee_err err) {
	    std::cout << "Error on Rx! " << err << "\n";
	    return 1;
    }
    try {
	printXBeePacket( &pkt );
	if ( pkt.size() > 0) {
	  if ( pkt.size() > 2) { // type and msg id
	    char type = pkt[0];
	    int msgid = (int) pkt[1];
	    int msgsize = pkt.size();
	    std::vector<unsigned char> data = pkt.getVector();
	    parseDataPacket( type, msgid, pkt.size(), pkt.getVector() );
	  }
	}
    } catch (xbee_err err) {
	    std::cout << "Error accessing packet! " << err << "\n";
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

  std::map<int,MiniXBee*>::iterator beeIt;
//   std::cout << "packets from minibees: " << minibees.size() << std::endl;
  for ( beeIt = minibees.begin(); beeIt != minibees.end(); ++beeIt ){
    MiniXBee * minibee = beeIt->second;
    minibee->waitForPacket();
  }

}

void MiniXHive::parseDataPacket( char type, int msgid, int msgsize, std::vector<unsigned char> data ){
  struct xbee_conAddress newAddress;

  switch ( type ){
    case MINIBEE_N_INFO:
      std::cout << "minibee info message " << type << ", " << msgid << ", " << msgsize << std::endl;
      break;
    case MINIBEE_N_SER: // serial number
      // Serial High (SH) + Serial Low (SL) + library version + board revision + capabilities
      std::cout << "minibee serial message " << type << ", " << msgid << ", " << msgsize <<  std::endl;
      if ( msgsize > 10 ){
	  memset(&newAddress, 0, sizeof(newAddress));
	  newAddress.addr64_enabled = 1;
	  std::vector<unsigned char>::iterator it = data.begin();
	  it++; it++; // start at index 2
	  for ( int i=0; i<8; i++ ){
	    newAddress.addr64[i] = *it;
	    ++it;
	  }
	  printXBeeAddress( newAddress );
	  MiniXBee * minibee = findMiniBeeByAddress( newAddress );
	  if ( minibee == NULL ){
	     // create a new minibee
	    minibee = createNewMiniBee(newAddress);
	    minibee->setHive( this );
	  }
	  minibee->parse_serial_message_noaddress(msgsize, data);
      }
      break;
   case MINIBEE_N_WAIT: // serial number
      // Serial High (SH) + Serial Low (SL) + library version + board revision + capabilities
      std::cout << "minibee waiting message " << type << ", " << msgid << ", " << msgsize <<  std::endl;
      break;
  } 
}

MiniXBee * MiniXHive::createNewMiniBee( struct xbee_conAddress beeAddress ){
 numberOfBees++;
 MiniXBee * minibee = new MiniXBee( numberOfBees );
 minibee->set64bitAddress( beeAddress, xbee );
 
 beeAddress.addr16_enabled = 1;
 beeAddress.addr16[0] = (unsigned char) (numberOfBees/256);
 beeAddress.addr16[1] = (unsigned char) numberOfBees%256;
 minibee->set16bitAddress( beeAddress, xbee );
 
 std::cout << "minibees before insert: " << minibees.size() << std::endl;
 
 minibees[numberOfBees] = minibee;
//  minibees.insert ( std::pair<int,MiniXBee*>(numberOfBees, minibee ) );
 std::cout << "minibees after insert: " << minibees.size() << std::endl;
 minibee->createConnections(xbee);
 return minibee;
}

MiniXBee * MiniXHive::findMiniBeeByAddress( struct xbee_conAddress beeAddress ){
  std::map<int,MiniXBee*>::iterator beeIt;
  std::cout << "minibees: " << minibees.size() << std::endl;
  for ( beeIt = minibees.begin(); beeIt != minibees.end(); ++beeIt ){
      if ( beeIt->second->matchAddress(beeAddress) ){
	// match found
	return beeIt->second;
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
}

int MiniXHive::setTargetAddress( const char * host, const char * port ){
    oscServer->setTargetAddress( host, port );
}