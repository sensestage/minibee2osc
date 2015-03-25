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
  //FIXME: these are all defaults
//   msgTimeInterval = 28;
//   samplesPerMessage = 4;
  
  // for fastest sampling response
  msgTimeInterval = 10;
  samplesPerMessage = 5;
  
  
  int i;
  for ( i=0; i<19; i++ ){
    pinConfig[i] = UnConfigured;
  }
  
  numberOfTWIs = 1;
  if ( numberOfTWIs > 0 ){
    pinConfig[14] = TWIData; // A4 : 13 + 4 = 17 -> 14
    pinConfig[15] = TWIClock; // A5
    twiConfig = (unsigned char*) malloc(numberOfTWIs * sizeof( unsigned char ) );
    twiConfig[0] = TWI_ADXL345; //FIXME: a default!
  }

  numberOfCustomInputs = 0;
  if ( numberOfCustomInputs > 0 ){
    customInputSizes = (unsigned char*) malloc(numberOfCustomInputs * sizeof( unsigned char ) );
    customInputPins = (unsigned char*) malloc(numberOfCustomInputs * sizeof( unsigned char ) );
  }
  
  configid = 1;
  
  calcDataProperties();
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


void MiniBeeConfig::calcDataProperties(void){
  int i;
  
  for ( i=0; i<numberOfCustomInputs; i++ ){
      switch ( customInputSizes[i] ){
	case 1: 
	  dataBitSizes.push_back(8);
	  dataScales.push_back( 1 );
	  dataOffsets.push_back( 0 );
	  break;
	case 2: 
	  dataBitSizes.push_back(16);
	  dataScales.push_back( 1 );
	  dataOffsets.push_back( 0 );
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
      case SHTData:
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
