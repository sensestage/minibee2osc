// #include "xbee.h"
// #include "xbeep.h"

#include "MiniBeeConfig.h"

#include <iostream>
#include <string.h>
#include <unistd.h>


using namespace libminibee;

MiniBeeDataPoint::MiniBeeDataPoint( int bits, int off, int sc ){
    bitSize = bits;
    offset = off;
    scale = sc;
}

float MiniBeeDataPoint::getValue( std::vector<unsigned char>::iterator dataVectorIterator, std::vector<unsigned char> data ){
    //TODO IMPLEMENT
}

MiniBeeDataPoint::~MiniBeeDataPoint(){
}

MiniBeeConfig::MiniBeeConfig(){
  configid = 1;
  msgTimeInterval = 50;
  samplesPerMessage = 1;
  
  for ( int i=0; i<19; i++ ){
    pinConfig[i] = UnConfigured;
  }
  
  numberOfTWIs = 0;
  numberOfCustomInputs = 0;
  calcDataProperties();
}

void MiniBeeConfig::setNumberOfTWIs( int no ){
  if ( numberOfTWIs > 0 ){
      delete twiConfig;
  }
   numberOfTWIs = (unsigned char) no;
   if ( numberOfTWIs > 0 ){
    pinConfig[14] = TWIData; // A4 : 13 + 4 = 17 -> 14
    pinConfig[15] = TWIClock; // A5
    twiConfig = (unsigned char*) malloc(numberOfTWIs * sizeof( unsigned char ) );    
  }
}

void MiniBeeConfig::setNumberOfCustomInputs( int no ){
  if ( numberOfCustomInputs > 0 ){
      delete customInputSizes;
//       delete customInputPins;
      delete customInputScales;
      delete customInputOffsets;
  }
  numberOfCustomInputs = (unsigned char) no;
  if ( numberOfCustomInputs > 0 ){
    customInputSizes = (unsigned char*) malloc(numberOfCustomInputs * sizeof( unsigned char ) );
    customInputScales = (int*) malloc(numberOfCustomInputs * sizeof( int ) );
    customInputOffsets = (int*) malloc(numberOfCustomInputs * sizeof( int ) );
//     customInputPins = (unsigned char*) malloc(numberOfCustomInputs * sizeof( unsigned char ) );
  }
}

void MiniBeeConfig::addPinConfig( int id, unsigned char config ){
  pinConfig[ id ] = config;
}

void MiniBeeConfig::addTWIConfig( int id, unsigned char config ){
  twiConfig[ id ] = config;
}

void MiniBeeConfig::addCustomConfig( int id, int offset, int scale, unsigned char size ){
//   customInputPins[ id ] = pinid;
  customInputSizes[ id ] = size;
  customInputOffsets[ id ] = offset;
  customInputScales[ id ] = scale;
}

MiniBeeConfig::~MiniBeeConfig(){
}

unsigned char MiniBeeConfig::getConfigID()
{
  return configid;
}

int MiniBeeConfig::getSamplesPerMessage()
{
  return samplesPerMessage;
}


void MiniBeeConfig::setProperties( unsigned char id, int spm, int red, bool rssi, std::string newname ){
  name = newname;
  configid = id;
  samplesPerMessage = spm;
  redundancy = red;
  sendRSSI = rssi; // FIXME: use this in the sending of data too
}


void MiniBeeConfig::calcDataProperties(void){
  int i;
  
  for ( i=0; i<numberOfCustomInputs; i++ ){
    dataScales.push_back( customInputScales[i] );
    dataOffsets.push_back( customInputOffsets[i] );
      switch ( customInputSizes[i] ){
	case 1: 
	  dataBitSizes.push_back(8);
	  break;
	case 2: 
	  dataBitSizes.push_back(16);
	  break;
      }
  }
  
  // data order: digital - analog - twi - sht - ping
  for ( i=0; i<19; i++ ){ // digital first
    switch ( pinConfig[i] ){
      case DigitalOut:
	// dataOutSizes.push_back( 1 );
	break;
      case AnalogOut:
	// dataOutSizes.push_back( 1 );
	break;
      case DigitalIn:
      case DigitalInPullup:
// 	numberOfDigitalIns++;
	dataBitSizes.push_back( 1 );
	dataScales.push_back( 1 );
	dataOffsets.push_back( 0 );
	break;
    }
  }
  for ( i=0; i<19; i++ ){ // then analog
    switch ( pinConfig[i] ){
      case AnalogIn:
	dataBitSizes.push_back( 8 );
	dataScales.push_back( 255 );
	dataOffsets.push_back( 0 );
	break;
      case AnalogIn10bit:
	dataBitSizes.push_back( 16 );
	dataScales.push_back( 1023 );
	dataOffsets.push_back( 0 );
	break;
    }
  }
  // then twi
   for ( i=0; i<numberOfTWIs; i++ ){ // then twis
    switch ( twiConfig[i] ){
      case TWI_ADXL345:
	dataBitSizes.push_back( 16 );
	dataBitSizes.push_back( 16 );
	dataBitSizes.push_back( 16 );
	dataScales.push_back( 8191 );
	dataScales.push_back( 8191 );
	dataScales.push_back( 8191 );
	dataOffsets.push_back( 0 );
	dataOffsets.push_back( 0 );
	dataOffsets.push_back( 0 );
	break;
      case TWI_LIS302DL:
	dataBitSizes.push_back( 8 );
	dataBitSizes.push_back( 8 );
	dataBitSizes.push_back( 8 );
	dataScales.push_back( 255 );
	dataScales.push_back( 255 );
	dataScales.push_back( 255 );
	dataOffsets.push_back( 0 );
	dataOffsets.push_back( 0 );
	dataOffsets.push_back( 0 );
	break;
      case TWI_BMP085:
	dataBitSizes.push_back( 16 );
	dataBitSizes.push_back( 24 );
	dataBitSizes.push_back( 24 );
	dataScales.push_back( 100 );
	dataScales.push_back( 100 );
	dataScales.push_back( 100 );
	dataOffsets.push_back( 27300 );
	dataOffsets.push_back( 0 );
	dataOffsets.push_back( 10000 );
	break;
      case TWI_TMP102:
	dataBitSizes.push_back( 16 );
	dataScales.push_back( 16 );
	dataOffsets.push_back( 2048 );
	break;
      case TWI_HMC58X3:
	dataBitSizes.push_back( 16 );
	dataBitSizes.push_back( 16 );
	dataBitSizes.push_back( 16 );
	dataScales.push_back( 2047 );
	dataScales.push_back( 2047 );
	dataScales.push_back( 2047 );
	dataOffsets.push_back( 2048 );
	dataOffsets.push_back( 2048 );
	dataOffsets.push_back( 2048 );
	break;
    }
  }
  
  for ( i=0; i<19; i++ ){ // then sht
    switch ( pinConfig[i] ){
      case SHTData:
	dataBitSizes.push_back( 16 );
	dataBitSizes.push_back( 16 );
	dataOffsets.push_back( 0 );
	dataOffsets.push_back( 0 );
	dataScales.push_back( 1 );
	dataScales.push_back( 1 );
	break;
    }
  }

  for ( i=0; i<19; i++ ){ // then ping
    switch ( pinConfig[i] ){
      case Ping:
	dataBitSizes.push_back( 16 );
	dataOffsets.push_back( 0 );
	dataScales.push_back( 1 ); // actually 61.9195
	break;
    }
  }

}

std::vector<unsigned char> MiniBeeConfig::getConfigMessage( unsigned char msgid, unsigned char nodeid ){
  //send config message (msgtype C: msg ID + config ID + configuration bytes
  // msg time interval - 2 bytes
  // samples per message - 1 byte
  // 19 bytes pin configuration - 1 byte each
  // number of I2C devices - 1 byte
  // N x I2C device ID - 1 byte each

  std::vector<unsigned char> mydata;
  mydata.push_back( 'C' );
  mydata.push_back( msgid );
//   mydata.push_back( nodeid );
  mydata.push_back( configid );
  
  mydata.push_back( (unsigned char) (msgTimeInterval>>8) );
  mydata.push_back( (unsigned char) (msgTimeInterval%256) );
  mydata.push_back( samplesPerMessage );

  for ( int i=0; i<19; i++ ){
    mydata.push_back( pinConfig[i] );
  }
  
  mydata.push_back( numberOfTWIs );
  for ( int i=0; i<numberOfTWIs; i++ ){
    mydata.push_back( twiConfig[i] );
  }

  return mydata;
}
