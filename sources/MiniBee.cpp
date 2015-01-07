// #include "xbee.h"
// #include "xbeep.h"

#include "MiniBee.h"

#include <iostream>
#include <string.h>
#include <unistd.h>


using namespace libminibee;
// namespace libminibee{

#include "minibee_utils.h"

miniXBeeConnection::miniXBeeConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address ): libxbee::ConCallback(parent, type, address) {};
miniXBee16Connection::miniXBee16Connection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address ): libxbee::ConCallback(parent, type, address) {};
miniXBeeTXConnection::miniXBeeTXConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address ): libxbee::ConCallback(parent, type, address) {};
miniXBeeATConnection::miniXBeeATConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address ): libxbee::ConCallback(parent, type, address) {};

void miniXBeeConnection::xbee_conCallback(libxbee::Pkt **pkt) {
	std::cout << "MiniXBee Callback!!\n";
	printXBeePacket( *pkt );

	if ((*pkt)->size() > 0) {
	  if ((*pkt)->size() > 2) { // type and msg id
	    char type = (**pkt)[0];
	    int msgid = (int) (**pkt)[1];
	    minibee->parseDataPacket( type, msgid, (*pkt)->size(), (*pkt)->getVector() );
	  }
	}
}

void miniXBee16Connection::xbee_conCallback(libxbee::Pkt **pkt) {
	std::cout << "MiniXBee 16 Callback!!\n";
	printXBeePacket( *pkt );

	if ((*pkt)->size() > 0) {
	  if ((*pkt)->size() > 2) { // type and msg id
	    char type = (**pkt)[0];
	    int msgid = (int) (**pkt)[1];
	    minibee->parseDataPacket( type, msgid, (*pkt)->size(), (*pkt)->getVector() );
	  }
	}
}

void miniXBeeTXConnection::xbee_conCallback(libxbee::Pkt **pkt) {
	std::cout << "MiniXBee TX Callback!!\n";
	printXBeePacket( *pkt );
	
	if ((*pkt)->size() > 0) {
	  std::cout << "tx status data size: " <<  (*pkt)->size() << "\n"; 
	}
	  
}

void miniXBeeATConnection::xbee_conCallback(libxbee::Pkt **pkt) {
	std::cout << "MiniXBee Remote AT Callback!!\n";
	printXBeePacket( *pkt );
	
	if ((*pkt)->size() > 0) {
	  std::cout << "remote at data size: " <<  (*pkt)->size() << "\n"; 
	}
}


MiniXBee::MiniXBee(){
  conData16 = NULL;
  conData64 = NULL;
  conTXStatus16 = NULL;
  conTXStatus64 = NULL;
  conRemoteAT = NULL;
  
  memset(&addr, 0, sizeof(addr));
  addr.addr16_enabled = 0;
  addr.addr64_enabled = 0;
  
  id = 0;
  mymsgid = 0;
  configid = 0;
  
  configuration = new MiniBeeConfig(); //FIXME: this should be set from outside
}

MiniXBee::MiniXBee( int newid ){
  MiniXBee();  
  setID( newid );
}

MiniXBee::~MiniXBee(){
//   free(&addr);
}

void MiniXBee::parseDataPacket( char type, int msgid, int msgsize, std::vector<unsigned char> data ){
  switch ( type ){
    case MINIBEE_N_DATA: // data
//       node ID + msg ID + N values
      std::cout << "minibee data message" << std::endl;
      parse_data( msgsize, data );
      status = SENDING;
      break;
    case MINIBEE_N_TRIGGER: // trigger data
//       node ID + msg ID + N values
      std::cout << "minibee trigger message" << std::endl;
      status = SENDING;
      break;
    case MINIBEE_N_EXTRA: // extra data
//       node ID + msg ID + N values
      std::cout << "minibee extra data message" << std::endl;
      status = SENDING;
      break;
    case MINIBEE_N_INFO:
      std::cout << "minibee info message" << std::endl;
      break;
    case MINIBEE_N_SER: // serial number
      // Serial High (SH) + Serial Low (SL) + library version + board revision + capabilities
      std::cout << "minibee serial message" << std::endl;
      parse_serial_message( msgsize, data );
      break;
    case MINIBEE_N_WAIT: // waiting
      //node ID + config ID
      std::cout << "minibee wait message" << std::endl;
	send_config_message();
	status = WAIT_FORCONFIRM;
      break;
    case MINIBEE_N_CONF: // confirm config
      // node ID + config ID + smpMsg + msgInt + datasize + outsize + (*custom*) + customInputs + customDataSize + N x (custom pin, data size)
      std::cout << "minibee config message" << std::endl;
	check_configuration_message( msgsize, data );
	status = WAIT_FORDATA;
      break;
    case MINIBEE_N_ACTIVE: // active
//     node ID + msg ID
      std::cout << "minibee active message" << std::endl;
      status = ACTING;
      break;
    case MINIBEE_N_PAUSED: // pausing
//       node ID + msg ID
      std::cout << "minibee pausing message" << std::endl;
      status = PAUSING;
      break;
  } 
}

void MiniXBee::parse_serial_message( int msgsize, std::vector<unsigned char> data ){
  if ( msgsize > 10 ){
    struct xbee_conAddress newAddress;
    memset(&newAddress, 0, sizeof(newAddress));
    addr.addr64_enabled = 1;
    std::vector<unsigned char>::iterator it = data.begin();
    it++; it++; // start at index 2
    for ( int i=0; i<8; i++ ){
      newAddress.addr64[i] = *it;
      ++it;
    }
    parse_serial_message_noaddress( msgsize, data );
  }
}

void MiniXBee::parse_serial_message_noaddress( int msgsize, std::vector<unsigned char> data ){
    if ( msgsize > 12 ){
	library_version = data[10];
	board_revision = data[11];
	capabilities = data[12];
	remote_config = data[13];
    }
    set_remote_id();
    send_id_message();
    status = WAIT_FORCONFIG;  
}

void MiniXBee::parse_data( int msgsize, std::vector<unsigned char> data ){
  int i = 0;
  // iterate over the dataBitSizes
  unsigned char curbyte = data[0];
  std::vector<int>::iterator it = configuration->dataBitSizes.begin();
  std::vector<int>::iterator it2 = configuration->dataScales.begin();
  std::vector<int>::iterator it3 = configuration->dataOffsets.begin();
  // TODO
  
  	for (auto n : data) {
	  std::cout << i << "," << n << std::endl;
	  addr.addr64[i] = n;
	  i++;
	}
  
}

void MiniXBee::set_remote_id(){
  // let's see if this works...
  *conRemoteAT << "MY" << id;
}

void MiniXBee::send_id_message(){
//TODO: send id message (msgtype I:msg ID + SH + SL + node ID + (*config ID*)
  std::vector<unsigned char> mydata;
  mydata.push_back( 'I' );
  mydata.push_back( mymsgid );
  for ( int i=0; i<8; i++ ){
    mydata.push_back( addr.addr64[i] );
  }
  mydata.push_back( id );
  mydata.push_back( configid );
  
  unsigned char frameid = 1;
  if ( !conData64->Tx( &frameid, mydata ) ) {
    std::cout << "error transmitting" << std::endl;
  }
  mymsgid++;
  mymsgid = mymsgid%256;
}

void MiniXBee::send_config_message(){
  std::vector<unsigned char> mydata = configuration->getConfigMessage( mymsgid, id );
  
  unsigned char frameid = 2;
  if ( !conData64->Tx( &frameid, mydata ) ) {
    std::cout << "error transmitting" << std::endl;
  }
  mymsgid++;
  mymsgid = mymsgid%256;
  
//   delete mydata;
}

void MiniXBee::check_configuration_message( int msgsize, std::vector<unsigned char> data ){
   // node ID + config ID + smpMsg + msgInt + datasize + outsize
   // + (*custom*) + customInputs + customDataSize + N x (custom pin, data size)
   //TODO: check whether it matches the configuration
   //TODO: add the custom data
}

void MiniXBee::setConfiguration( MiniBeeConfig * conf ){
    configuration = conf;
}

MiniBeeConfig * MiniXBee::getConfiguration(){
    return configuration;
}

void MiniXBee::setID( int newid ){
    id = newid;
}

void MiniXBee::setConfigID( int newid ){
    configid = newid;
}

void MiniXBee::createConnections( libxbee::XBee * xbee ){
  if ( addr.addr16_enabled == 1 ){
//     if ( conData16 != NULL ){
//       delete conData16;
//     }
//     if ( conTXStatus16 != NULL ){
//       delete conTXStatus16;
//     }
    conData16 = new miniXBee16Connection( *xbee, "16-bit Data", &addr );
    conData16->minibee = this;
    conTXStatus16 = new miniXBeeTXConnection( *xbee, "Transmit Status", &addr );
    conTXStatus16->minibee = this;
  }
  if ( addr.addr64_enabled == 1 ){
//     if ( conData64 != NULL ){
//       delete conData64;
//     }
//     if ( conTXStatus64 != NULL ){
//       delete conTXStatus64;
//     }
//     if ( conRemoteAT != NULL ){
// 	delete conRemoteAT;
//     }
    conData64 = new miniXBeeConnection( *xbee, "64-bit Data", &addr );
    conData64->minibee = this;
//     conTXStatus64 = new miniXBeeTXConnection( *xbee, "Transmit Status", &addr );
//     conTXStatus64->minibee = this;
    conRemoteAT = new miniXBeeATConnection( *xbee, "Remote AT", &addr );
    conRemoteAT->minibee = this;
  }
}

bool MiniXBee::set16bitAddress( struct xbee_conAddress xbaddr, libxbee::XBee * xbee ){
    if ( xbaddr.addr16_enabled == 0 ){
	return false;
    }
    addr.addr16_enabled = 1;
    addr.addr16[0] = xbaddr.addr16[0];
    addr.addr16[1] = xbaddr.addr16[1];
    
//     createConnections( xbee );
    return true;
}

bool MiniXBee::set64bitAddress( struct xbee_conAddress xbaddr, libxbee::XBee * xbee ){
    if ( xbaddr.addr64_enabled == 0 ){
	return false;
    }
    addr.addr64_enabled = 1;
    int i;
    for ( i=0; i<8; i++ ){
      addr.addr64[i] = xbaddr.addr64[i];
    }
    
//     createConnections( xbee );
    return true;
}

bool MiniXBee::matchAddress( struct xbee_conAddress xbaddr ){
  bool isenabled = false;
    if ( xbaddr.addr16_enabled == 1 ){
      isenabled = true;
      if ( (addr.addr16[0] != xbaddr.addr16[0]) || (addr.addr16[1] != xbaddr.addr16[1]) ){
	return false;
      }
    }
    if ( xbaddr.addr64_enabled == 1 ){
      int i;
      isenabled = true;
      for ( i=0; i<8; i++ ){
	if ( (addr.addr64[i] != xbaddr.addr16[i])){
	  return false;
	}
      }
    }
    return isenabled;
}
