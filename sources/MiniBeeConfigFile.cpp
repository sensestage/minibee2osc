#include "MiniBeeConfigFile.h"

#include <algorithm>
#include <iostream>

using namespace libminibee;

std::map<std::string,int> MiniBeeConfigFile::PinNames = { 
	{"D3", 0},
	{"D4", 1}, // me-pin
	{"D5", 2},
	{"D6", 3},
	{"D7", 4},
	{"D8", 5},
	{"D9", 6},
	{"D10", 7},
	{"D11", 8},
	{"D12", 9}, // linked to minibee
	{"D13", 10}, // linked to minibee
	{"A0", 11},
	{"A1", 12},
	{"A2", 13},
	{"A3", 14},
	{"A4", 15}, // twi 
	{"A5", 16}, // twi
	{"A6", 17},
	{"A7", 18}
};

std::map<std::string,int> MiniBeeConfigFile::PinConfigNames = { 
  { "NotUsed", NotUsed },
  { "DigitalIn", DigitalIn },
  { "DigitalOut", DigitalOut },
  { "AnalogIn", AnalogIn },
  { "AnalogOut", AnalogOut },
  { "AnalogIn10bit", AnalogIn10bit },
  { "SHTClock", SHTClock },
  { "SHTData", SHTData },
  { "TWIClock", TWIClock },
  { "TWIData", TWIData },
  { "Ping", Ping },
  { "DigitalInPullup", DigitalInPullup },
  //   OneWire,
  { "Custom", Custom },
  { "MeID", MeID },
  { "UnConfigured", UnConfigured }
};
  
std::map<std::string,int> MiniBeeConfigFile::TWINames = { 
  { "ADXL345", TWI_ADXL345 },
  { "LIS302DL", TWI_LIS302DL },
  { "BMP085", TWI_BMP085 },
  { "TMP102", TWI_TMP102 },
  { "HMC58X3", TWI_HMC58X3 }
};


MiniBeeConfigFile::MiniBeeConfigFile(void){
  doc = NULL;
}

MiniBeeConfigFile::~MiniBeeConfigFile(void){
  if ( doc != NULL ){
    delete doc;
  }
}

int MiniBeeConfigFile::load( const char *filename ){ // success or not
  if ( doc != NULL ){ 
    delete doc; 
//     delete hiveroot;
  };
  doc = new XMLDocument();
  doc->LoadFile( filename );
  int errorID = doc->ErrorID();
  if ( errorID == 0 ){ // no errors
    XMLElement* root = doc->FirstChildElement();
    if ( root != NULL ){
	hiveroot = root->FirstChildElement( "hive" );
    } else {
	return -1;
    }
  }
  return errorID;
}
// 	int MiniBeeConfigFile::save( const char *filename ); // success or not


// analyze file
int MiniBeeConfigFile::analyzeFile( MiniBeeIDAllocator * idallocator ){
  for( const XMLElement* node=hiveroot->FirstChildElement( "minibee" ); node; node=node->NextSiblingElement( "minibee" ) ) {
	  int v0 = 0;      
	  node->QueryIntAttribute( "id", &v0 );
	  idallocator->add( v0 );
    }
//   idallocator->print();
  return 1; // success
}

// reading data from file

void MiniBeeConfigFile::readConfigurationElement( const XMLElement * ele, MiniBeeConfig * config ){
  
  int id=0;
  int spm=1;
  int red=1;
  int mi=50;
  bool rssi=false;
  
  ele->QueryIntAttribute( "id", &id );
  ele->QueryIntAttribute( "samples_per_message", &spm );
  ele->QueryIntAttribute( "message_interval", &mi );
  ele->QueryIntAttribute( "redundancy", &red );
  ele->QueryBoolAttribute( "rssi", &rssi );

  config->setProperties( id, spm, red, rssi, std::string( ele->Attribute( "name" ) ) );
  
  for( const XMLElement* pinnode = ele->FirstChildElement("pin"); pinnode; pinnode = pinnode->NextSiblingElement( "pin" ) ) {
    std::string pinID = std::string( pinnode->Attribute( "id" ) );
    std::string pinConfig = std::string( pinnode->Attribute( "config" ) );
//     std::string pinLabel = std::string( pinnode->Attribute( "name" ) ); // FIXME
    int pid, cid;
    std::map<std::string,int>::iterator pit = PinNames.find( pinID );
    if ( pit != PinNames.end() ){
	pid = pit->second;
    }
    std::map<std::string,int>::iterator cit = PinConfigNames.find( pinConfig );
    if ( cit != PinConfigNames.end() ){
	cid = cit->second;
    }
    config->addPinConfig( pid, cid );
  }

  // count twi elements
  int twiCount = 0;
  for( const XMLElement* subnode=ele->FirstChildElement("twi"); subnode; subnode=subnode->NextSiblingElement( "twi" ) ) {
    twiCount++;
  }
  
  config->setNumberOfTWIs( twiCount );
  for( const XMLElement* subnode=ele->FirstChildElement("twi"); subnode; subnode=subnode->NextSiblingElement( "twi" ) ) {
    int id;
    subnode->QueryIntAttribute( "id", &id );
    std::string device = std::string( subnode->Attribute( "device" ) );
//     std::string twiLabel = std::string( subnode->Attribute( "name" ) ); // FIXME
    int deviceid;
    std::map<std::string,int>::iterator pit = TWINames.find( device );
    if ( pit != TWINames.end() ){
	deviceid = pit->second;
    }
    config->addTWIConfig( id-1, deviceid ); //NOTE: offset of 1 on twi ids
    //FIXME: iterate over twi slots for further labeling
  }

  // count custom elements
  int customCount = 0;
  const XMLElement* customnode = ele->FirstChildElement("customconf");
  if ( customnode != NULL ){
      for( const XMLElement* subnode=customnode->FirstChildElement("data"); subnode; subnode=subnode->NextSiblingElement( "data" ) ) {
	customCount++;
      }
  }
  config->setNumberOfCustomInputs( customCount );
  
  if ( customnode != NULL ){
      for( const XMLElement* subnode=customnode->FirstChildElement("data"); subnode; subnode=subnode->NextSiblingElement( "data" ) ) {
	int id=0, offset=0, scale=1, size=1, type=0;
	subnode->QueryIntAttribute( "id", &id );
	subnode->QueryIntAttribute( "offset", &offset );
	subnode->QueryIntAttribute( "scale", &scale );
	subnode->QueryIntAttribute( "size", &size );
    subnode->QueryIntAttribute( "type", &type ); // 0: unsigned, 1: signed
	config->addCustomConfig( id, offset, scale, size, type );
//      std::string customLabel = std::string( subnode->Attribute( "name" ) ); // FIXME
      }
  }
  config->calcDataProperties();
}

int MiniBeeConfigFile::readConfigurationWithID( int id, MiniBeeConfig * config ){
      for( const XMLElement* node=hiveroot->FirstChildElement( "configuration" ); node; node=node->NextSiblingElement( "configuration" ) ) {
	  int v0 = 0;      
	  node->QueryIntAttribute( "id", &v0 );
// 	  printf( "\tid: %i\n", v0 );
	  if ( v0 == id ){
	    readConfigurationElement( node, config );
	    return 1;
	  }
    }
    return -1;
}

void MiniBeeConfigFile::readMiniBeeElement( const XMLElement * ele, MiniXBee * bee ){
  
  int id=0;
  int cid=0;
  int libv=0;
  int caps=0;
  bee->setName( std::string( ele->Attribute( "name" ) ) );
  ele->QueryIntAttribute( "id", &id );
  bee->setID( id );
  ele->QueryIntAttribute( "configuration", &cid );
  bee->setConfigID( cid );
  ele->QueryIntAttribute( "library_version", &libv );
  ele->QueryIntAttribute( "caps", &caps );
  bee->setProperties( ele->Attribute( "revision" )[0], libv, caps );
  bee->setSerialNumber( std::string( ele->Attribute( "serial" ) ) );
  
  MiniBeeConfig * config = bee->getConfiguration(); // get a reference
  //FIXME: error checking
  
  readConfigurationWithID( cid, config );
  
  //FIXME: custom configuration stuff
  
// 	const XMLElement* customnode = bee->FirstChildElement("custom");
// 	if ( customnode != NULL ){
// 	  for( const XMLElement* subnode=customnode->FirstChildElement("data"); subnode; subnode=subnode->NextSiblingElement( "data" ) ) {
// 	    const char* name = subnode->Name();
// 	    printf( "\t\tcustom data: %s\n", name );
// 	    for( const XMLAttribute* attr = subnode->FirstAttribute(); attr; attr=attr->Next() ) {
// 		const char* attrName = attr->Name();
// 		const char* attValue = attr->Value();
// 		printf( "\t\t\tattribute: %s, %s\n", attrName, attValue );
// 	    }
// 	  }
// 	}
  
}


int MiniBeeConfigFile::readMiniBeeWithID( int id, MiniXBee * bee ){
  XMLElement* foundnode;
  for( const XMLElement* node=hiveroot->FirstChildElement( "minibee" ); node; node=node->NextSiblingElement( "minibee" ) ) {
	  int v0 = 0;      
	  node->QueryIntAttribute( "id", &v0 );
// 	  printf( "\tid: %i\n", v0 );
	  if ( v0 == id ){
	    readMiniBeeElement( node, bee );
	    return 1; // success
	  }
    }
    return -1;
}

int MiniBeeConfigFile::readMiniBeeWithSerialNumber( std::string serialnumber, MiniXBee * bee ){
    std::transform(serialnumber.begin(), serialnumber.end(),serialnumber.begin(), ::toupper);
  
    for( const XMLElement* node=hiveroot->FirstChildElement( "minibee" ); node; node=node->NextSiblingElement( "minibee" ) ) {
	  const char* text = node->Attribute( "serial" );
// 	  printf( "\tserial: %s, %i\n", text, serialnumber.compare( text ) );
// 	  std::string str = std::string( text );
	  if ( serialnumber.compare( text ) == 0 ){
    	    readMiniBeeElement( node, bee );
	    return 1;
	  }
    }
    return -1;
}
	
	// adding new elements
// int MiniBeeConfigFile::addConfiguration( MiniBeeConfig * config );
// int MiniBeeConfigFile::addMinibee( MiniXBee * bee );


