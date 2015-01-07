#ifndef __MINIHIVE_H
#define __MINIHIVE_H

#ifndef __cplusplus
#warning This header file is intended for use with C++
#else

#ifndef __XBEE_H
#include <xbee.h>
#endif

#ifndef __XBEE_CPP_H
#include <xbeep.h>
#endif

#include "MiniBee.h"

#include <map>

// #ifndef EXPORT
// #define EXPORT
// #define XBEE_EXPORT_DEFINED
// #endif

namespace libminibee {
  class MiniXHive; // handles connections with XBee
  
  class miniXHiveConnection: public libxbee::ConCallback {
	public:
	  explicit miniXHiveConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address = NULL);
// 	  : libxbee::ConCallback(parent, type, address);
	  void xbee_conCallback(libxbee::Pkt **pkt);
	  MiniXHive *minihive; // points to our minihive
  };

 
  class miniXHiveTXConnection: public libxbee::ConCallback {
	public:
	  explicit miniXHiveTXConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address = NULL);
// 	  : libxbee::ConCallback(parent, type, address) {};
	  void xbee_conCallback(libxbee::Pkt **pkt);
	  MiniXHive *minihive; // points to our minihive
  };

  class miniXHiveModemConnection: public libxbee::ConCallback {
	public:
	  explicit miniXHiveModemConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address = NULL);
// 	  : libxbee::ConCallback(parent, type, address) {};
	  void xbee_conCallback(libxbee::Pkt **pkt);
	  MiniXHive *minihive; // points to our minihive
  };
  
  class MiniXHive{
    public:
	MiniXHive(void);
	~MiniXHive(void);

	int createXBee( std::string serialport, int loglevel );
	void parseDataPacket( char type, int msgid, int msgsize, std::vector<unsigned char> data );
	
	int waitForPacket();

	void setLogLevel( int level );
	int getLogLevel();
	void writeToLog( int level, const char * logstring );
	
    private:
      
	MiniXBee * findMiniBeeByAddress( struct xbee_conAddress beeAddress );
	MiniXBee * createNewMiniBee( struct xbee_conAddress beeAddress );
      
	struct xbee_conAddress addr;
	libxbee::XBee * xbee;
	std::map<int,MiniXBee*> minibees;
	int numberOfBees;
	
	libxbee::Con * con;
	
// 	miniXHiveConnection * conData16;
// 	miniXHiveTXConnection * conTXStatus16;
// 	miniXHiveModemConnection * conModemStatus;
  };
};

#endif /* __cplusplus */
#endif /* __MINIHIVE_H */