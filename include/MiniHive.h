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

#include "MiniBeeConfigFile.h"

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
	
	int readConfigurationFile( const char* filename );

	int createOSCServer( const char * port );
	int setTargetAddress( const char * host, const char * port );
	int waitForOSC();

	int createXBee( std::string serialport, int loglevel );
	virtual void parseDataPacket( char type, int msgid, int msgsize, std::vector<unsigned char> data );
	virtual void parseDataPacketCatchall( char type, int msgid, int msgsize, std::vector<unsigned char> data, struct xbee_conAddress *address );
	virtual int waitForPacket();

	// logging
	void setLogLevel( int level );
	int getLogLevel();
	void writeToLog( int level, const char * logstring );
	void writeToLog( int level, std::string logstring );
	
	int send_announce_message_toFFFA();
	
	/// messages to minibee from OSC:
	int send_output_to_minibee( int minibeeID, std::vector<int> * data, unsigned char noAck = 0 );
	int send_custom_to_minibee( int minibeeID, std::vector<int> * data, unsigned char noAck = 0 );
	
	int send_running_to_minibee( int minibeeID, int onoff );
	int send_loopback_to_minibee( int minibeeID, int onoff );
	
	int send_reset_to_minibee( int minibeeID );
	int send_save_id_to_minibee( int minibeeID );
	int send_announce_to_minibee( int minibeeID );
	
	HiveOscServer * oscServer;

    protected:
	// moved from private
// 	int numberOfBees;
	int mymsgid;

	libxbee::Con * con;
// 	libxbee::Con * conTXStatus;
	libxbee::Con * conCatchAll;
	libxbee::Con * conCatchAll64;
	libxbee::XBee * xbee;

	virtual void waitForBeePackets();

	void addMinibee( int id, MiniXBee * mbee );

  private:
	MiniXBee * findMiniBeeByAddress( struct xbee_conAddress beeAddress );
	MiniXBee * createNewMiniBee( struct xbee_conAddress beeAddress );
	MiniXBee * createNewMiniBeeWithID( struct xbee_conAddress beeAddress );

	std::map<int,MiniXBee*> minibees;
	
	MiniBeeConfigFile * configFile;
	MiniBeeIDAllocator * idAllocator;

	//TODO: catch-all connection to listen for all messages - test this!
		
// 	miniXHiveConnection * conData16;
// 	miniXHiveTXConnection * conTXStatus16;
// 	miniXHiveModemConnection * conModemStatus;
  };
};

#endif /* __cplusplus */
#endif /* __MINIHIVE_H */