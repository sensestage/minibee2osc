// #include "xbee.h"
// #include "xbeep.h"

#include "MiniBee.h"

#include "MiniHive.h"

#include <iostream>
// #include <string>
#include <sstream>
// #include <string.h>
#include <unistd.h>
#include <string.h>

using namespace libminibee;
// namespace libminibee{

#include "minibee_utils.h"

// miniXBeeConnection::miniXBeeConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address ): libxbee::ConCallback(parent, type, address) {};
// miniXBee16Connection::miniXBee16Connection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address ): libxbee::ConCallback(parent, type, address) {};
// miniXBeeTXConnection::miniXBeeTXConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address ): libxbee::ConCallback(parent, type, address) {};
// miniXBeeATConnection::miniXBeeATConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address ): libxbee::ConCallback(parent, type, address) {};
// 
// void miniXBeeConnection::xbee_conCallback(libxbee::Pkt **pkt) {
// 	std::cout << "MiniXBee Callback!!\n";
// 	printXBeePacket( *pkt );
// 
// 	if ((*pkt)->size() > 0) {
// 	  if ((*pkt)->size() > 2) { // type and msg id
// 	    char type = (**pkt)[0];
// 	    int msgid = (int) (**pkt)[1];
// 	    minibee->parseDataPacket( type, msgid, (*pkt)->size(), (*pkt)->getVector() );
// 	  }
// 	}
// }
// 
// void miniXBee16Connection::xbee_conCallback(libxbee::Pkt **pkt) {
// 	std::cout << "MiniXBee 16 Callback!!\n";
// 	printXBeePacket( *pkt );
// 
// 	if ((*pkt)->size() > 0) {
// 	  if ((*pkt)->size() > 2) { // type and msg id
// 	    char type = (**pkt)[0];
// 	    int msgid = (int) (**pkt)[1];
// 	    minibee->parseDataPacket( type, msgid, (*pkt)->size(), (*pkt)->getVector() );
// 	  }
// 	}
// }
// 
// void miniXBeeTXConnection::xbee_conCallback(libxbee::Pkt **pkt) {
// 	std::cout << "MiniXBee TX Callback!!\n";
// 	printXBeePacket( *pkt );
// 	
// 	if ((*pkt)->size() > 0) {
// 	  std::cout << "tx status data size: " <<  (*pkt)->size() << "\n"; 
// 	}
// 	  
// }
// 
// void miniXBeeATConnection::xbee_conCallback(libxbee::Pkt **pkt) {
// 	std::cout << "MiniXBee Remote AT Callback!!\n";
// 	printXBeePacket( *pkt );
// 	
// 	if ((*pkt)->size() > 0) {
// 	  std::cout << "remote at data size: " <<  (*pkt)->size() << "\n"; 
// 	}
// }


MiniXBee::MiniXBee(){
  initVariables();
}

MiniXBee::MiniXBee( int newid ){
//   MiniXBee();  
  
  initVariables();
  setID( newid );
}

void MiniXBee::initVariables(void)
{
  nodatacount = 0;
  status = OFF;
  
  memset(&addr16, 0, sizeof(addr16));
  addr16.addr16_enabled = 0;
  addr16.addr64_enabled = 0;
  memset(&addr64, 0, sizeof(addr64));
  addr64.addr16_enabled = 0;
  addr64.addr64_enabled = 0;
  
  id = 0;
  mymsgid = 0;
  configid = 1;
  
  configuration = new MiniBeeConfig(); //FIXME: this should be set from outside
}


MiniXBee::~MiniXBee(){
//   free(&addr);
}

void MiniXBee::parseDataPacket( char type, int msgid, int msgsize, std::vector<unsigned char> data ){
  unsigned char expected_config;
  std::ostringstream oss;
  switch ( type ){
    case MINIBEE_N_DATA: // data
//       node ID + msg ID + N values
      
      oss << "minibee data message" << std::endl;
      this->parse_data( msgsize, data );
      this->setStatus( SENDING );
//       status = SENDING;
      break;
    case MINIBEE_N_TRIGGER: // trigger data
//       node ID + msg ID + N values
      oss << "minibee trigger message" << std::endl;
      this->parse_trigger( msgsize, data );
//       status = SENDING;
      this->setStatus( SENDING );
      break;
    case MINIBEE_N_EXTRA: // extra data
//       node ID + msg ID + N values
      oss << "minibee extra data message" << std::endl;
      this->parse_extra( msgsize, data );
//       status = SENDING;
      this->setStatus( SENDING );
      break;
    case MINIBEE_N_INFO:
      oss << "minibee info message" << std::endl;
      break;
    case MINIBEE_N_SER: // serial number
      // Serial High (SH) + Serial Low (SL) + library version + board revision + capabilities
      oss << "minibee serial message" << std::endl;
      this->parse_serial_message( msgsize, data );
      break;
    case MINIBEE_N_WAIT: // waiting
      //node ID + config ID
      oss << "minibee wait message" << std::endl;
      expected_config = data[3];
      if ( expected_config != configuration->getConfigID() ){
	this->send_id_message();
      } else {
	this->send_config_message();
      }
//       status = WAIT_FORCONFIRM;
      this->setStatus( WAIT_FORCONFIRM );
      break;
    case MINIBEE_N_CONF: // confirm config
      // node ID + config ID + smpMsg + msgInt + datasize + outsize + (*custom*) + customInputs + customDataSize + N x (custom pin, data size)
      oss << "minibee config message" << std::endl;
      this->check_configuration_message( msgsize, data );
//       status = WAIT_FORDATA;
      this->setStatus( WAIT_FORDATA );
      break;
    case MINIBEE_N_ACTIVE: // active
//     node ID + msg ID
      oss << "minibee active message" << std::endl;
//       status = ACTING;
      this->setStatus( ACTING );
      break;
    case MINIBEE_N_PAUSED: // pausing
//       node ID + msg ID
      oss << "minibee pausing message" << std::endl;
//       status = PAUSING;
      this->setStatus( PAUSING );
      break;
  } 
  hive->writeToLog(10, oss.str() );
}

void MiniXBee::parse_serial_message( int msgsize, std::vector<unsigned char> data ){
  if ( msgsize > 10 ){
//     struct xbee_conAddress newAddress;
//     memset(&newAddress, 0, sizeof(newAddress));
    addr64.addr64_enabled = 1;
    std::vector<unsigned char>::iterator it = data.begin();
    it++; it++; // start at index 2
    for ( int i=0; i<8; i++ ){
//       newAddress.addr64[i] = *it;
      addr64.addr64[i] = *it;
      ++it;
    }
    this->parse_serial_message_noaddress( msgsize, data );
  }
}

void MiniXBee::parse_serial_message_noaddress( int msgsize, std::vector<unsigned char> data ){
    if ( msgsize > 12 ){
	library_version = data[10];
	board_revision = data[11];
	capabilities = data[12];
	remote_config = data[13];
    }
    this->set_remote_id();
    this->send_id_message();
//     status = WAIT_FORCONFIG; 
    this->setStatus( WAIT_FORCONFIG );
}


void MiniXBee::parse_trigger( int msgsize, std::vector<unsigned char> data ){
//FIXME
  if ( hive->oscServer != NULL ){
    hive->oscServer->sendTriggerMessage( id, &data );
  }

}

void MiniXBee::parse_extra( int msgsize, std::vector<unsigned char> data ){
//FIXME - test!
  if ( hive->oscServer != NULL ){
    hive->oscServer->sendPrivateMessage( id, &data );
  }

}

#define TAKE_N_BITS_FROM(b, p, n) ((b) >> (p)) & ((1 << (n)) - 1)
  
void MiniXBee::parse_data( int msgsize, std::vector<unsigned char> data ){  
  std::vector<float> parsed_data;
  // iterate over the dataBitSizes
  std::vector<unsigned char>::iterator itData = data.begin();
  itData++; itData++; // 'd', msgid    
  
  for ( int i=0; i < configuration->getSamplesPerMessage(); i++ ){    
    std::vector<int>::iterator it = configuration->dataBitSizes.begin();
    std::vector<int>::iterator it2 = configuration->dataScales.begin();
    std::vector<int>::iterator it3 = configuration->dataOffsets.begin();
    // TODO parse data
    unsigned char curData = (*itData);
    int byteIndex = 0;
    int curBitSize = 0;
    int curBitIndex = 0;
//     unsigned char curBitMask;
    float newdata;
    bool prev1bit = false;
    while ( it != configuration->dataBitSizes.end() ){
  //     std::cout << "data: " << (*itData) << "bitsize: " << (*it) << ", dataScale: " << (*it2) << ", dataOffsets: " << (*it3) << std::endl;
      curBitSize = (*it);
      if ( curBitSize == 1 ){ // FIXME: this does not work correctly yet
	prev1bit = true;
	unsigned char myData = curData;
	newdata = TAKE_N_BITS_FROM( myData, curBitIndex, curBitSize );
// 	if ( curData & curBitMask ){
// 	  newdata = 1;
// 	} else {
// 	  newdata = 0;
// 	}
	newdata = ( (float) newdata + (*it3) / (*it2) );
	parsed_data.push_back(newdata);
	curBitIndex++;
// 	curBitMask << 1;
	if ( curBitIndex == 8 ){
	    prev1bit = false;
	    itData++; byteIndex++;
	    curData = *itData;
	    if ( itData == data.end() ){
	      std::ostringstream oss;
	      oss << "Data too short! Early return from data!" << std::endl;
	      hive->writeToLog( 2, oss.str() );
	      break;
	    }
	    curBitIndex = 0;
// 	    curBitMask = 1;
	}
      } else { // other options are 8, 16, 24 bit for now
	if ( prev1bit ){
	  prev1bit = false;
	  curBitIndex = 0;
	  itData++; byteIndex++;
	  if ( itData == data.end() ){
	      std::ostringstream oss;
	      oss << "Data too short! Early return from data!" << std::endl;
	      hive->writeToLog( 2, oss.str() );
	      break;
	  }
	  curData = *itData;
	}
	switch( curBitSize ){
	  case 8:
	    newdata = ( (float) curData + (*it3) )/ (float) (*it2);
	    parsed_data.push_back(newdata);
	    itData++; byteIndex++;
	    curData = *itData;
	    break;
	  case 16:
	    newdata = (float) curData * 256.;
	    itData++; byteIndex++;
	    curData = *itData;
	    newdata += (float) curData;
	    newdata = (float) (newdata + (*it3)) / (float) (*it2 );
	    parsed_data.push_back(newdata);
	    itData++; byteIndex++;
	    curData = *itData;
	    break;
	  case 24:
	    newdata = (float) curData * 256. * 256.;
	    itData++; byteIndex++;
	    curData = *itData;
	    newdata += (float) curData * 256.;
	    itData++; byteIndex++;
	    curData = *itData;
	    newdata += (float) curData;
	    newdata = (newdata + (float) (*it3)) / (float) (*it2 );
	    parsed_data.push_back(newdata);
	    itData++; byteIndex++;
	    curData = *itData;
	    break;
	  case 32:
	    newdata = (float) curData * 256. * 256. * 256.;
	    itData++; byteIndex++;
	    curData = *itData;
	    newdata += (float) curData * 256. * 256.;
	    itData++; byteIndex++;
	    curData = *itData;
	    newdata += (float) curData * 256.;
	    itData++; byteIndex++;
	    curData = *itData;
	    newdata += (float) curData;
	    newdata = (newdata + (float) (*it3)) / (float) (*it2 );
	    parsed_data.push_back(newdata);
	    itData++; byteIndex++;
	    curData = *itData;
	    break;
	}
      }
      it++; it2++; it3++;
    }
  }
  
  this->process_data( &parsed_data );

  if ( hive->getLogLevel() > 0 ){  
    //   int i = 0;
    std::ostringstream oss;
    oss << "Data minibee: " << id << " : ";
    for (auto n : parsed_data) {
      oss << n << ", ";
  //     i++;
    }
    oss << std::endl;
    hive->writeToLog( 20, oss.str() );
  }
}

void MiniXBee::process_data( std::vector<float> * parsed_data ){
  if ( hive->oscServer != NULL ){
    if ( configuration->getSamplesPerMessage() == 1 ){
      hive->oscServer->sendOutputMessage( id, parsed_data );
    } else {
      hive->oscServer->sendOutputBlockMessage( id, configuration->getSamplesPerMessage(), parsed_data );
    }
  }
}

int MiniXBee::send_id_message(){
  settings16.disableAck = 0; // always get acknowledgement
  con16->setSettings(&settings16);

  //TODO: send id message (msgtype I:msg ID + SH + SL + node ID + (*config ID*)
  if ( con64 == NULL ){
      return -1;
  }
  std::vector<unsigned char> mydata;
  mydata.push_back( 'I' );
  mydata.push_back( mymsgid );
  for ( int i=0; i<8; i++ ){
    mydata.push_back( addr64.addr64[i] );
  }
  mydata.push_back( id );
  mydata.push_back( configuration->getConfigID() );
  
  unsigned char frameid = mymsgid;
  int retval = sendTx64( frameid, &mydata );
  mymsgid++;
  mymsgid = mymsgid%256;
  return retval;
  return 0;
}

int MiniXBee::send_config_message(){
  settings16.disableAck = 0; // always get acknowledgement
  con16->setSettings(&settings16);

  std::vector<unsigned char> mydata = configuration->getConfigMessage( mymsgid, id );
  
//   std::cout << "configuration message: ";
//   for ( auto i = mydata.begin(); i != mydata.end(); i++ ) {
//     std::cout << (int) (*i) << ", ";
//   }
//   std::cout << std::endl;
  unsigned char frameid = mymsgid;
  int retval = sendTx64( frameid, &mydata );
  mymsgid++;
  mymsgid = mymsgid%256;
  return retval;
}


int MiniXBee::send_announce_message(){
  settings16.disableAck = 0; // always get acknowledgement
  con16->setSettings(&settings16);

  std::vector<unsigned char> mydata;
  mydata.push_back( 'A' );
//   std::cout << "announce message: " << std::endl;
  unsigned char frameid = mymsgid;
  
  int retval = sendTx16( frameid, &mydata );
  mymsgid++;
  mymsgid = mymsgid%256;
  return retval;
}

int MiniXBee::send_quit_message(){
  settings16.disableAck = 0; // always get acknowledgement
  con16->setSettings(&settings16);

  std::vector<unsigned char> mydata;
  mydata.push_back( 'Q' );
//   std::cout << "quit message: " << std::endl;
  unsigned char frameid = mymsgid;
  
  int retval = sendTx16( frameid, &mydata );
  mymsgid++;
  mymsgid = mymsgid%256;
  return retval;
}

int MiniXBee::send_loopback(int onoff)
{
  settings16.disableAck = 0; // always get acknowledgement
  con16->setSettings(&settings16);

  std::vector<unsigned char> mydata;
  mydata.push_back( 'L' );
  mydata.push_back( mymsgid );
  mydata.push_back( onoff );
//   std::cout << "loopback message: " << std::endl;
  unsigned char frameid = mymsgid;
  
  int retval = sendTx16( frameid, &mydata );
  mymsgid++;
  mymsgid = mymsgid%256;
  return retval;
}

int MiniXBee::send_running(int onoff)
{
  settings16.disableAck = 0; // always get acknowledgement
  con16->setSettings(&settings16);

  std::vector<unsigned char> mydata;
  mydata.push_back( 'R' );
  mydata.push_back( mymsgid );
  mydata.push_back( onoff );
//   std::cout << "running message: " << std::endl;
  unsigned char frameid = mymsgid;
  
  int retval = sendTx16( frameid, &mydata );
  mymsgid++;
  mymsgid = mymsgid%256;
  return retval;
}

int MiniXBee::send_custom( std::vector<int>* data, unsigned char noAck )
{
//   std::cout << "custom message: " << std::endl;
//   for ( auto i = data->begin(); i != data->end(); i++ ) {
//     std::cout << (int) (*i) << ", ";
//   }
//   std::cout << std::endl;
  settings16.disableAck = noAck;
  con16->setSettings(&settings16);

  std::vector<unsigned char> mydata;
  mydata.push_back( 'E' );
  mydata.push_back( mymsgid );
  for ( auto i = data->begin(); i != data->end(); i++ ) {
    mydata.push_back( (unsigned char) *i );
  }
  unsigned char frameid = mymsgid;
  
  int retval = sendTx16( frameid, &mydata );
  mymsgid++;
  mymsgid = mymsgid%256;
  return retval;
}

int MiniXBee::send_output( std::vector< int >* data, unsigned char noAck)
{
//   std::cout << "send output message: " << id << " : ";
//   for ( auto i = data->begin(); i != data->end(); i++ ) {
//     std::cout << (int) (*i) << ", ";
//   }
//   std::cout << std::endl;
  settings16.disableAck = noAck;
  con16->setSettings(&settings16);

  std::vector<unsigned char> mydata;
  mydata.push_back( 'O' );
  mydata.push_back( mymsgid );
  for ( auto i = data->begin(); i != data->end(); i++ ) {
    mydata.push_back( (unsigned char) *i );
  }
  unsigned char frameid = mymsgid;
  
  int retval = sendTx16( frameid, &mydata );
  
  mymsgid++;
  mymsgid = mymsgid%256;
  
  return retval;
}

int MiniXBee::set_remote_id(){
  if ( conAT == NULL ){ return -1; };
  std::vector<unsigned char> mydata;
  mydata.push_back('M');
  mydata.push_back('Y');
  mydata.push_back(id);
  unsigned char frameid = mymsgid;
  int retval = sendAT( frameid, &mydata );
  mymsgid++;
  mymsgid = mymsgid%256;
  return retval;
}

int MiniXBee::send_reset()
{
  if ( conAT == NULL ){ return -1; };
  // 'D3' 5
  if ( send_digital_D3( 5 ) != 0 ){
    return -1;
  }
  // 'IO' 8
  if ( send_IO_D3( 8 ) != 0 ){
    return -1;
  }
  // 'IO' 0  
  if ( send_IO_D3( 0 ) != 0 ){
    return -1;
  }
  return 0;
}

int MiniXBee::send_digital_D3(unsigned char val)
{
  std::vector<unsigned char> mydata;
  mydata.push_back('D');
  mydata.push_back('3');
  mydata.push_back(val);
  unsigned char frameid = mymsgid;
  int retval = sendAT( frameid, &mydata );
  mymsgid++;
  mymsgid = mymsgid%256;
  return retval;
}

int MiniXBee::send_IO_D3(unsigned char val)
{
  std::vector<unsigned char> mydata;
  mydata.push_back('D');
  mydata.push_back('3');
  mydata.push_back(val);
  unsigned char frameid = mymsgid;
  int retval = sendAT( frameid, &mydata );
  mymsgid++;
  mymsgid = mymsgid%256;
  return retval;
}


int MiniXBee::send_save_id()
{
  if ( conAT == NULL ){ return -1; };
  std::vector<unsigned char> mydata;
  mydata.push_back('W');
  mydata.push_back('R');
  unsigned char frameid = mymsgid;
  int retval = sendAT( frameid, &mydata );
  mymsgid++;
  mymsgid = mymsgid%256;
  return retval;
}



void MiniXBee::check_configuration_message( int msgsize, std::vector<unsigned char> data ){
   // node ID + config ID + smpMsg + msgInt + datasize + outsize
   // + (*custom*) + customInputs + customDataSize + N x (custom pin, data size)
   //TODO: check whether it matches the configuration
   //TODO: add the custom data
   unsigned char datavalue;
   for ( auto i = data.begin(); i != data.end(); i++ ) {
     datavalue = (*i);
     std::cout << (int) (datavalue) << ", ";
   }
   std::cout << std::endl;
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

void MiniXBee::setStatus(int newstatus)
{
  if ( status != newstatus ){
      // send osc message
    if ( hive->oscServer != NULL ){
      std::string statusstring;
      switch( newstatus ){
	case WAIT_FORCONFIRM:
	case WAIT_FORDATA:
	  statusstring = std::string( "waiting" );
	  break;
	case PAUSING:
	  statusstring = std::string( "pausing" );
	  break;
	case ACTING:
	  statusstring = std::string( "acting" );
	  break;
	case SENDING:
	  statusstring = std::string( "receiving" );
	  break;
	case OFF:
	  statusstring = std::string( "off" );
	  break;
      }
      hive->oscServer->sendStatusMessage( id, statusstring, newstatus );
    }
  }
  status = newstatus;
}

int MiniXBee::getStatus()
{
    return status;
}

void MiniXBee::createConnections( libxbee::XBee * xbee ){
  if ( addr16.addr16_enabled == 1 ){
    try{
      con16 = new libxbee::Con( *xbee, "16-bit Data", &addr16 );
//       con16TxStatus = new libxbee::Con( *xbee, "Transmit Status", &addr16 );      
      con16->getSettings( &settings16 );
      std::ostringstream oss;
      oss << "MiniBee: created 16-bit data connection" << std::endl;
      hive->writeToLog(10, oss.str() );      
    } catch (xbee_err err) {
      std::ostringstream oss;
      oss << "MiniBee: Could not create 16-bit data connection: " << err << "\n";
      hive->writeToLog(1, oss.str() );
           
//       return 1;
    }
  }
  if ( addr64.addr64_enabled == 1 ){
    try{
      con64 = new libxbee::Con( *xbee, "64-bit Data", &addr64 );
      conAT = new libxbee::Con( *xbee, "Remote AT", &addr64 );
//       con64TxStatus = new libxbee::Con( *xbee, "Transmit Status", &addr64 );      
//       std::cout << "MiniBee: created 64-bit data connection and remote AT" << std::endl;
      std::ostringstream oss;
      oss << "MiniBee: created 64-bit data connection and remote AT" << std::endl;
      hive->writeToLog(10, oss.str() );
    } catch (xbee_err err) {
      std::ostringstream oss;
      oss << "MiniBee: Could not create 64-bit data connection: " << err << "\n";
      hive->writeToLog(1, oss.str() );
    }
  }
//   if ( addr.addr16_enabled == 1 ){
// //     if ( conData16 != NULL ){
// //       delete conData16;
// //     }
// //     if ( conTXStatus16 != NULL ){
// //       delete conTXStatus16;
// //     }
//     conData16 = new miniXBee16Connection( *xbee, "16-bit Data", &addr );
//     conData16->minibee = this;
//     conTXStatus16 = new miniXBeeTXConnection( *xbee, "Transmit Status", &addr );
//     conTXStatus16->minibee = this;
//   }
//   if ( addr.addr64_enabled == 1 ){
//     if ( conData64 != NULL ){
//       delete conData64;
//     }
//     if ( conTXStatus64 != NULL ){
//       delete conTXStatus64;
//     }
//     if ( conRemoteAT != NULL ){
// 	delete conRemoteAT;
//     }
//     conData64 = new miniXBeeConnection( *xbee, "64-bit Data", &addr );
//     conData64->minibee = this;
//     conTXStatus64 = new miniXBeeTXConnection( *xbee, "Transmit Status", &addr );
//     conTXStatus64->minibee = this;
//     conRemoteAT = new miniXBeeATConnection( *xbee, "Remote AT", &addr );
//     conRemoteAT->minibee = this;
//   }
}

bool MiniXBee::set16bitAddress( struct xbee_conAddress xbaddr, libxbee::XBee * xbee ){
    if ( xbaddr.addr16_enabled == 0 ){
	return false;
    }
    addr16.addr16_enabled = 1;
    addr16.addr16[0] = xbaddr.addr16[0];
    addr16.addr16[1] = xbaddr.addr16[1];
    
//     createConnections( xbee );
    return true;
}

bool MiniXBee::set64bitAddress( struct xbee_conAddress xbaddr, libxbee::XBee * xbee ){
    if ( xbaddr.addr64_enabled == 0 ){
	return false;
    }
    addr64.addr64_enabled = 1;
    int i;
    for ( i=0; i<8; i++ ){
      addr64.addr64[i] = xbaddr.addr64[i];
    }
    
//     createConnections( xbee );
    return true;
}

bool MiniXBee::matchAddress( struct xbee_conAddress xbaddr ){
  bool isenabled = false;
    if ( xbaddr.addr16_enabled == 1 ){
      isenabled = true;
      if ( (addr16.addr16[0] != xbaddr.addr16[0]) || (addr16.addr16[1] != xbaddr.addr16[1]) ){
	return false;
      }
    }
    if ( xbaddr.addr64_enabled == 1 ){
      int i;
      isenabled = true;
      for ( i=0; i<8; i++ ){
	if ( (addr64.addr64[i] != xbaddr.addr64[i])){
	  return false;
	}
      }
    }
    return isenabled;
}

void MiniXBee::setHive(MiniXHive * inhive)
{
  hive = inhive;
}


int MiniXBee::waitForPacket(){
  libxbee::Pkt pkt;
  bool shouldLog = hive->getLogLevel() > 0;

//   if ( con16TxStatus->RxAvailable() > 0 ){
//     try {
// 	    //con >> pkt; /* like this */
// 	    pkt << *con16TxStatus;   /* or this */
//     } catch (xbee_err err) {
// 	    std::cout << "Error on Rx! " << err << "\n";
// 	    return 1;
//     }
//     try {
// 	std::cout << "packet 16 tx status: " << pkt.size() << std::endl;
// 	printXBeePacket( &pkt );
//     } catch (xbee_err err) {
//       std::cout << "Error accessing packet! " << err << "\n";
//       return 1;
//     }
//   }
// 
//   if ( con64TxStatus->RxAvailable() > 0 ){
//     try {
// 	    //con >> pkt; /* like this */
// 	    pkt << *con64TxStatus;   /* or this */
//     } catch (xbee_err err) {
// 	    std::cout << "Error on Rx! " << err << "\n";
// 	    return 1;
//     }
//     try {
// 	std::cout << "packet 64 tx status: " << pkt.size() << std::endl;
// 	printXBeePacket( &pkt );
//     } catch (xbee_err err) {
//       std::cout << "Error accessing packet! " << err << "\n";
//       return 1;
//     }
//   }

  if ( con16 != NULL ){
    if ( con16->RxAvailable() > 0 ){
      try {
	      //con >> pkt; /* like this */
	      pkt << *con16;   /* or this */
      } catch (xbee_err err) {
	      std::ostringstream oss;
	      oss << "MiniBee: Error on Rx of con16! " << err << "\n";
	      hive->writeToLog( 1, oss.str() );
	      return 1;
      }
      try {
	if ( shouldLog ){
	  std::ostringstream oss;
	  oss << "packet 16: " << pkt.size() << std::endl;
	  streamXBeePacket(&pkt, &oss );
	  hive->writeToLog( 10, oss.str() );
	}
// 	printXBeePacket( &pkt );    
	if ( pkt.size() > 0) {
	  if ( pkt.size() > 2) { // type and msg id
	    char type = pkt[0];
	    int msgid = (int) pkt[1];
	    int msgsize = pkt.size();
	    std::vector<unsigned char> data = pkt.getVector();
// 	    std::cout << "packet 16 reaction: " << pkt.size() << std::endl;
	    this->parseDataPacket( type, msgid, pkt.size(), pkt.getVector() );
	    nodatacount = 0;
	  }
	}
      } catch (xbee_err err) {
	std::ostringstream oss;
	oss << "MiniBee: Error accessing packet of con16! " << err << "\n";
	hive->writeToLog( 1, oss.str() );
	return 1;
      }
    }
  }
  
  if ( con64 != NULL ){
    if ( con64->RxAvailable() > 0 ){
      try {
	      //con >> pkt; /* like this */
	      pkt << *con64;   /* or this */
      } catch (xbee_err err) {
	std::ostringstream oss;
	oss << "MiniBee: Error on Rx of con64! " << err << "\n";
	hive->writeToLog( 1, oss.str() );
	return 1;
      }
      try {
	if ( shouldLog ){
	  std::ostringstream oss;
	  oss << "packet 64: " << pkt.size() << std::endl;
	  streamXBeePacket(&pkt, &oss );
	  hive->writeToLog( 10, oss.str() );
	}
	if ( pkt.size() > 0) {
	  if ( pkt.size() > 2) { // type and msg id
	    char type = pkt[0];
	    int msgid = (int) pkt[1];
	    int msgsize = pkt.size();
	    std::vector<unsigned char> data = pkt.getVector();
	    this->parseDataPacket( type, msgid, pkt.size(), pkt.getVector() );
	    nodatacount = 0;
// 	    std::cout << "packet 64 reaction: " << pkt.size() << std::endl;
	  }
	}
      } catch (xbee_err err) {
	std::ostringstream oss;
	oss << "MiniBee: Error accessing packet of con64! " << err << "\n";
	hive->writeToLog( 1, oss.str() );
	return 1;
      }
    }
  }

//   if ( conAT != NULL ){
//     if ( conAT->RxAvailable() > 0 ){
//       try {
// 	      //con >> pkt; /* like this */
// 	      pkt << *conAT;   /* or this */
//       } catch (xbee_err err) {
// 	      std::cout << "Error on Rx! " << err << "\n";
// 	      return 1;
//       }
//       try {
// 	  std::cout << "packet AT: " << pkt.size() << std::endl;
// 	  printXBeePacket( &pkt );
// 	  if ( pkt.size() > 0) {
// 	    if ( pkt.size() > 2) { // type and msg id
// 	      char type = pkt[0];
// 	      int msgid = (int) pkt[1];
// 	      int msgsize = pkt.size();
// 	      std::vector<unsigned char> data = pkt.getVector();
//   // 	    std::cout << "AT reaction: " << pkt.size() << std::endl;
//   // 	    parseATPacket( type, msgid, pkt.size(), pkt.getVector() );
// 	    }
// 	  }
//       } catch (xbee_err err) {
// 	      std::cout << "Error accessing packet! " << err << "\n";
// 	      return 1;
//       }
//     }
//   }

  nodatacount++;
  if ( nodatacount > 10000 ){ // 5 seconds
      setStatus( OFF );
  }
}


int MiniXBee::sendTx16( unsigned char frameid, std::vector<unsigned char> * data ){
  try{
    if ( con16->Tx( &frameid, *data ) != XBEE_ENONE ) {
      std::ostringstream oss;
      oss << "MiniBee: error transmitting 16bit" << std::endl;
      hive->writeToLog(2, oss.str() );
      return -1;
    }
    return 0;
  } catch (xbee_err err) {
      std::ostringstream oss;
      oss << "MiniBee: error transmitting via connection 16bit" << err << "\n";
      hive->writeToLog(1, oss.str() );
      return -1;
  } catch (libxbee::xbee_etx etx ){
      std::ostringstream oss;
      oss << "MiniBee: etx error transmitting via connection 16bit" << etx.ret << "\n";
      hive->writeToLog(1, oss.str() );
      return -1;    
  }
}

int MiniXBee::sendTx64( unsigned char frameid, std::vector<unsigned char> * data ){
  try{
    if ( con64->Tx( &frameid, *data ) != XBEE_ENONE ) {
      std::ostringstream oss;
      oss << "MiniBee: error transmitting 64bit" << std::endl;
      hive->writeToLog(2, oss.str() );
//       std::cout << "MiniBee: error transmitting 64bit" << std::endl;
      return -1;
    }
    return 0;
  } catch (xbee_err err) {
      std::ostringstream oss;
      oss << "MiniBee: error transmitting via connection 64bit" << err << "\n";
      hive->writeToLog(1, oss.str() );
      return -1;
  } catch (libxbee::xbee_etx etx ){
      std::ostringstream oss;
      oss << "MiniBee: etx error transmitting via connection 64bit" << etx.ret << "\n";
      hive->writeToLog(1, oss.str() );
      return -1;    
  }
}

int MiniXBee::sendAT( unsigned char frameid, std::vector<unsigned char> * data ){
  try{
    if ( conAT->Tx( &frameid, *data ) != XBEE_ENONE ) {
      std::ostringstream oss;
      oss << "MiniBee: error transmitting AT via connection" << "\n";
      hive->writeToLog(2, oss.str() );
      return -1;
    }
    return 0;
  } catch (xbee_err err) {
      std::ostringstream oss;
      oss << "MiniBee: error transmitting AT via connection" << err << "\n";
      hive->writeToLog(1, oss.str() );
      return -1;
  } catch (libxbee::xbee_etx etx ){
      std::ostringstream oss;
      oss << "MiniBee: etx error transmitting AT via connection" << etx.ret << "\n";
      hive->writeToLog(1, oss.str() );
      return -1;
  }
}
