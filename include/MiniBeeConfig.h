#ifndef __MINIBEECONFIG_H
#define __MINIBEECONFIG_H

#ifndef __cplusplus
#warning This header file is intended for use with C++
#else

#include <vector>
#include <string>

namespace libminibee {
  
  enum MiniBeePinConfig { 
    NotUsed,
    DigitalIn, DigitalOut,
    AnalogIn, AnalogOut, AnalogIn10bit, 
    SHTClock, SHTData, 
    TWIClock, TWIData,
    Ping,
    DigitalInPullup,
  //   OneWire,
    Custom = 100,
    MeID = 150,
    UnConfigured = 200,
  };
  
  enum TWIDeviceConfig { 
    TWI_ADXL345=10,
    TWI_LIS302DL=11,
    TWI_BMP085=20,
    TWI_TMP102=30,
    TWI_HMC58X3=40
  };

  class MiniBeeDataPoint{
    public:
      MiniBeeDataPoint( int bits=8, int off=0, int sc=1 );
      ~MiniBeeDataPoint();
      
      float getValue( std::vector<unsigned char>::iterator dataVectorIterator, std::vector<unsigned char> data );
      
    private:
      int bitSize;
      int offset;
      int scale;
  };
  
  class MiniBeeConfig{
    public:
	MiniBeeConfig(void);
	~MiniBeeConfig(void);

	std::vector<unsigned char> getConfigMessage( unsigned char msgid, unsigned char nodeid );
	
	void calcDataProperties(void);

	std::vector<MiniBeeDataPoint> dataPoints;
	
	//TODO: remove these:
      std::vector<int> dataBitSizes; // how many bits take up a value
      std::vector<int> dataOffsets; // offset of data
      std::vector<int> dataScales;  // scaling factor of data
      
      unsigned char getConfigID();
      
      int getSamplesPerMessage();
      int getRedundancy();
      bool getSendRSSI();
      
      void setProperties( unsigned char id, int spm, int red, bool rssi, std::string newname );
      
      void addPinConfig( int id, unsigned char config );
      void addTWIConfig( int id, unsigned char config );
      void addCustomConfig( int id, int offset, int scale, unsigned char size );
      
      void setNumberOfTWIs( int no );
      void setNumberOfCustomInputs( int no );

    private:
      int msgTimeInterval;
      int samplesPerMessage;
      int redundancy;
      
      std::string name;
      bool sendRSSI;
      
      unsigned char numberOfCustomInputs;
      
//       std::vector<MiniBeeDataPoint> customPoints;
      // this should be a struct
      unsigned char * customInputSizes;
      unsigned char * customInputPins;
      int * customInputOffsets;
      int * customInputScales;
      
      unsigned char pinConfig[19];
      
      unsigned char numberOfTWIs;
      unsigned char * twiConfig;
      
      unsigned char configid; // config id of the minibee
      
  };
};

#endif /* __cplusplus */
#endif /* __MINIBEECONFIG_H */