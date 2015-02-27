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

#include <string>

#include "MiniHiveOSC.h"
#include "MiniBee.h"
#include "osc/oscin.h"

#include <map>

// #ifndef EXPORT
// #define EXPORT
// #define XBEE_EXPORT_DEFINED
// #endif

namespace libminibee {
//   class MiniXHive; // handles connections with XBee
//   
//   class miniXHiveConnection: public libxbee::ConCallback {
// 	public:
// 	  explicit miniXHiveConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address = NULL);
// // 	  : libxbee::ConCallback(parent, type, address);
// 	  void xbee_conCallback(libxbee::Pkt **pkt);
// 	  MiniXHive *minihive; // points to our minihive
//   };
// 
//  
//   class miniXHiveTXConnection: public libxbee::ConCallback {
// 	public:
// 	  explicit miniXHiveTXConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address = NULL);
// // 	  : libxbee::ConCallback(parent, type, address) {};
// 	  void xbee_conCallback(libxbee::Pkt **pkt);
// 	  MiniXHive *minihive; // points to our minihive
//   };
// 
//   class miniXHiveModemConnection: public libxbee::ConCallback {
// 	public:
// 	  explicit miniXHiveModemConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address = NULL);
// // 	  : libxbee::ConCallback(parent, type, address) {};
// 	  void xbee_conCallback(libxbee::Pkt **pkt);
// 	  MiniXHive *minihive; // points to our minihive
//   };
  
  class MiniXHive{
    public:
	MiniXHive(void);
	~MiniXHive(void);

	int createOSCServer( const char * port );
	int setTargetAddress( const char * host, const char * port );
	int waitForOSC();

	int createXBee( std::string serialport, int loglevel );
	void parseDataPacket( char type, int msgid, int msgsize, std::vector<unsigned char> data );
	void parseDataPacketCatchall( char type, int msgid, int msgsize, std::vector<unsigned char> data, struct xbee_conAddress *address );
	int waitForPacket();

	// logging
	void setLogLevel( int level );
	int getLogLevel();
	void writeToLog( int level, const char * logstring );
	void writeToLog( int level, std::string logstring );
	
	int send_announce_message_toFFFA();
	
	/// messages to minibee from OSC:
	int send_output_to_minibee( int minibeeID, std::vector<int> * data );
	int send_custom_to_minibee( int minibeeID, std::vector<int> * data );
	
	int send_running_to_minibee( int minibeeID, int onoff );
	int send_loopback_to_minibee( int minibeeID, int onoff );
	
	int send_reset_to_minibee( int minibeeID );
	int send_save_id_to_minibee( int minibeeID );
	int send_announce_to_minibee( int minibeeID );
	
	
	
	HiveOscServer * oscServer;

    private:
      
	MiniXBee * findMiniBeeByAddress( struct xbee_conAddress beeAddress );
	MiniXBee * createNewMiniBee( struct xbee_conAddress beeAddress );
	MiniXBee * createNewMiniBeeWithID( struct xbee_conAddress beeAddress );

	libxbee::XBee * xbee;
	std::map<int,MiniXBee*> minibees;
	int numberOfBees;
	
	libxbee::Con * con;
// 	libxbee::Con * conTXStatus;
	libxbee::Con * conCatchAll;
	libxbee::Con * conCatchAll64;
	
	int mymsgid;
	
	//TODO: catch-all connection to listen for all messages - test this!
		
// 	miniXHiveConnection * conData16;
// 	miniXHiveTXConnection * conTXStatus16;
// 	miniXHiveModemConnection * conModemStatus;
  };
};

#endif /* __cplusplus */
#endif /* __MINIHIVE_H */