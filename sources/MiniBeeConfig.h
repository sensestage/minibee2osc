#ifndef __MINIBEECONFIG_H
#define __MINIBEECONFIG_H

#ifndef __cplusplus
#warning This header file is intended for use with C++
#else

#include <vector>

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

    private:
      int msgTimeInterval;
      int samplesPerMessage;
      unsigned char pinConfig[19];
      unsigned char numberOfTWIs;
      unsigned char * twiConfig;
      unsigned char configid; // config id of the minibee
      
  };
};

#endif /* __cplusplus */
#endif /* __MINIBEECONFIG_H */