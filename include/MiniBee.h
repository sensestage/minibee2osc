#ifndef __MINIBEE_H
#define __MINIBEE_H

#ifndef __cplusplus
#warning This header file is intended for use with C++
#else

#ifndef __XBEE_H
#include <xbee.h>
#endif

#ifndef __XBEE_CPP_H
#include <xbeep.h>
#endif

// #ifndef EXPORT
// #define EXPORT
// #define XBEE_EXPORT_DEFINED
// #endif

#include <vector>
#include <lo/lo_osc_types.h>

#include "MiniBeeConfig.h"


//node message types
#define MINIBEE_N_ACTIVE 'a'
#define MINIBEE_N_CONF 'c'
#define MINIBEE_N_DATA 'd'
#define MINIBEE_N_EXTRA 'e'
#define MINIBEE_N_INFO 'i'
#define MINIBEE_N_PAUSED 'p'
#define MINIBEE_N_SER 's'
#define MINIBEE_N_TRIGGER 't'
#define MINIBEE_N_WAIT 'w'

namespace libminibee {
  
  class MiniXHive;
  
  enum MiniBeeStatus{
    WAIT_FORCONFIG,
    WAIT_FORCONFIRM,
    WAIT_FORDATA,
    PAUSING,
    ACTING,
    SENDING
  };
  
//   class MiniXBee; // handles connections with XBee
// 
//   class miniXBeeConnection: public libxbee::ConCallback {
// 	public:
// 	  explicit miniXBeeConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address = NULL);
// // 	  : libxbee::ConCallback(parent, type, address) {};
// 	  void xbee_conCallback(libxbee::Pkt **pkt);
// 	  MiniXBee *minibee; // points to our minibee
//   };
// 
//   class miniXBee16Connection: public libxbee::ConCallback {
// 	public:
// 	  explicit miniXBee16Connection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address = NULL);
// // 	  : libxbee::ConCallback(parent, type, address) {};
// 	  void xbee_conCallback(libxbee::Pkt **pkt);
// 	  MiniXBee *minibee; // points to our minibee
//   };
// 
//   class miniXBeeATConnection: public libxbee::ConCallback {
// 	public:
// 	  explicit miniXBeeATConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address = NULL);
// // 	  : libxbee::ConCallback(parent, type, address) {};
// 	  void xbee_conCallback(libxbee::Pkt **pkt);
// 	  MiniXBee *minibee; // points to our minibee
//   };
//   
//   class miniXBeeTXConnection: public libxbee::ConCallback {
// 	public:
// 	  explicit miniXBeeTXConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address = NULL);
// // 	  : libxbee::ConCallback(parent, type, address) {};
// 	  void xbee_conCallback(libxbee::Pkt **pkt);
// 	  MiniXBee *minibee; // points to our minibee
//   };
  
  class MiniXBee{
    public:
	MiniXBee(void);
	MiniXBee(int newid);
	~MiniXBee(void);
	
	void setHive( MiniXHive * inhive );
	
	int waitForPacket();
	void parseDataPacket( char type, int msgid, int msgsize, std::vector<unsigned char> data);

	void createConnections( libxbee::XBee * xbee );
	
	void setConfiguration( MiniBeeConfig * conf );
	MiniBeeConfig * getConfiguration();
	
	void setID( int newid );
	void setConfigID( int newid );
	bool set16bitAddress( struct xbee_conAddress xbaddr, libxbee::XBee * xbee ); // returns false when not a valid address
	bool set64bitAddress( struct xbee_conAddress xbaddr, libxbee::XBee * xbee ); // returns false when not a valid address
	
	bool matchAddress( struct xbee_conAddress xbaddr ); // returns false when address does not match
	
	void parse_serial_message_noaddress( int msgsize, std::vector<unsigned char> data );

	int send_announce_message();
	int send_quit_message();
	int send_output( std::vector<int> * data );
	int send_custom( std::vector<int> * data );
// 	int send_custom( std::vector<unsigned char> * data );
	int send_running( int onoff );
	int send_loopback( int onoff );
	
	int send_reset();
	int send_save_id();
	

  private:
	void initVariables();
	
	void setStatus( int newstatus );
	
	void parse_data( int msgsize, std::vector<unsigned char> data );
	void parse_trigger( int msgsize, std::vector<unsigned char> data );
	void parse_extra( int msgsize, std::vector<unsigned char> data );
	void parse_serial_message( int msgsize, std::vector<unsigned char> data );
	int set_remote_id();
	int send_id_message();
	int send_config_message();
	void check_configuration_message( int msgsize, std::vector<unsigned char> data );
	
	int send_digital_D3( unsigned char val );
	int send_IO_D3( unsigned char val );
      
	int sendTx16( unsigned char frameid, std::vector<unsigned char> * data );
	int sendTx64( unsigned char frameid, std::vector<unsigned char> * data );
	int sendAT( unsigned char frameid, std::vector<unsigned char> * data );

	struct xbee_conAddress addr16;
	struct xbee_conAddress addr64;
	
	MiniXHive * hive;
	
	libxbee::Con * con16;
// 	libxbee::Con * con16TxStatus;
	libxbee::Con * con64;
// 	libxbee::Con * con64TxStatus;	
	libxbee::Con * conAT;
	
// 	miniXBee16Connection * conData16;
// 	miniXBeeConnection * conData64;
// // 	miniXBeeConnection * conIO16;
// // 	miniXBeeConnection * conIO64;
// 	miniXBeeATConnection * conRemoteAT;
// 	miniXBeeTXConnection * conTXStatus16;
// 	miniXBeeTXConnection * conTXStatus64;

	int status; // current state of minibee
	unsigned char id;    // node id of the minibee
	unsigned char configid; // config id of the minibee
	
	int library_version;
	int board_revision;
	int capabilities;
	int remote_config;
	
	unsigned char mymsgid;
	
	MiniBeeConfig * configuration;
  };
};

#endif /* __cplusplus */
#endif /* __MINIBEE_H */