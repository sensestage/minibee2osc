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
  
  enum MiniBeeStatus{
    WAIT_FORCONFIG,
    WAIT_FORCONFIRM,
    WAIT_FORDATA,
    PAUSING,
    ACTING,
    SENDING
  };
  
  class MiniXBee; // handles connections with XBee

  class miniXBeeConnection: public libxbee::ConCallback {
	public:
	  explicit miniXBeeConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address = NULL);
// 	  : libxbee::ConCallback(parent, type, address) {};
	  void xbee_conCallback(libxbee::Pkt **pkt);
	  MiniXBee *minibee; // points to our minibee
  };

  class miniXBee16Connection: public libxbee::ConCallback {
	public:
	  explicit miniXBee16Connection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address = NULL);
// 	  : libxbee::ConCallback(parent, type, address) {};
	  void xbee_conCallback(libxbee::Pkt **pkt);
	  MiniXBee *minibee; // points to our minibee
  };

  class miniXBeeATConnection: public libxbee::ConCallback {
	public:
	  explicit miniXBeeATConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address = NULL);
// 	  : libxbee::ConCallback(parent, type, address) {};
	  void xbee_conCallback(libxbee::Pkt **pkt);
	  MiniXBee *minibee; // points to our minibee
  };
  
  class miniXBeeTXConnection: public libxbee::ConCallback {
	public:
	  explicit miniXBeeTXConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address = NULL);
// 	  : libxbee::ConCallback(parent, type, address) {};
	  void xbee_conCallback(libxbee::Pkt **pkt);
	  MiniXBee *minibee; // points to our minibee
  };
  
  class MiniXBee{
    public:
	MiniXBee(void);
	MiniXBee(int newid);
	~MiniXBee(void);
	
	void parseDataPacket( char type, int msgid, int msgsize, std::vector<unsigned char> data );

	void createConnections( libxbee::XBee * xbee );
	
	void setConfiguration( MiniBeeConfig * conf );
	MiniBeeConfig * getConfiguration();
	
	void setID( int newid );
	void setConfigID( int newid );
	bool set16bitAddress( struct xbee_conAddress xbaddr, libxbee::XBee * xbee ); // returns false when not a valid address
	bool set64bitAddress( struct xbee_conAddress xbaddr, libxbee::XBee * xbee ); // returns false when not a valid address
	
	bool matchAddress( struct xbee_conAddress xbaddr ); // returns false when address does not match
	
	void parse_serial_message_noaddress( int msgsize, std::vector<unsigned char> data );
    private:
	void parse_data( int msgsize, std::vector<unsigned char> data );
	void parse_serial_message( int msgsize, std::vector<unsigned char> data );
	void set_remote_id();
	void send_id_message();
	void send_config_message();
	void check_configuration_message( int msgsize, std::vector<unsigned char> data );
      
	struct xbee_conAddress addr;
	
	miniXBee16Connection * conData16;
	miniXBeeConnection * conData64;
// 	miniXBeeConnection * conIO16;
// 	miniXBeeConnection * conIO64;
	miniXBeeATConnection * conRemoteAT;
	miniXBeeTXConnection * conTXStatus16;
	miniXBeeTXConnection * conTXStatus64;

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