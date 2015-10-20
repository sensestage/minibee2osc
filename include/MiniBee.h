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

#include "MiniBeeConfig.h"

#include <string.h>
#include <vector>
#include <lo/lo_osc_types.h>



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
    SENDING,
    OFF
  };
  
  class MiniXBee; // handles connections with XBee
  
  class BeeConnection: public libxbee::ConCallback {
	public:
		explicit BeeConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address = NULL): libxbee::ConCallback(parent, type, address) {};
		void xbee_conCallback(libxbee::Pkt **pkt);
		MiniXBee * bee;
  };
  
  class MiniXBee{
    public:
	MiniXBee(void);
	MiniXBee(int newid);
	~MiniXBee(void);
	
	virtual void setHive( MiniXHive * inhive );
	
	void tick();
	
	void parsePacket( libxbee::Pkt * pkt );
// 	virtual int waitForPacket();
	virtual void parseDataPacket( char type, int msgid, int msgsize, std::vector<unsigned char> data);

	void createConnections( libxbee::XBee * xbee );
	
	void setConfiguration( MiniBeeConfig * conf );
	MiniBeeConfig * getConfiguration();
	
	void setSerialNumber( std::string newserial );
	void setName( std::string newname );
	int getID();
	void setID( int newid );
	void setConfigID( int newid );
	void setProperties( char rev, int libv, int caps );
	
	bool set16bitAddress( struct xbee_conAddress xbaddr, libxbee::XBee * xbee ); // returns false when not a valid address
	bool set64bitAddress( struct xbee_conAddress xbaddr, libxbee::XBee * xbee ); // returns false when not a valid address
	
	bool matchAddress( struct xbee_conAddress xbaddr ); // returns false when address does not match
	
	void parse_serial_message_catchall( int msgsize, std::vector<unsigned char> data );

	int send_announce_message();
	int send_quit_message();
	int send_output( std::vector<int> * data, unsigned char noAck = 0 );
	int send_custom( std::vector<int> * data, unsigned char noAck = 0 );
// 	int send_custom( std::vector<unsigned char> * data );
	int send_running( int onoff );
	int send_loopback( int onoff );
	
	int send_reset();
	int send_save_id();

	void setStatus( int newstatus );
	void check_configuration_message( int msgsize, std::vector<unsigned char> data );
	
  protected:
	int nodatacount;
	// moved from private:
	void initVariables();
	MiniBeeConfig * configuration;
	unsigned char id;    // node id of the minibee

	virtual void process_data( std::vector<float> * parsed_data );
	
	virtual void parse_data( int msgsize, std::vector<unsigned char> data );
	virtual void parse_trigger( int msgsize, std::vector<unsigned char> data );
	virtual void parse_extra( int msgsize, std::vector<unsigned char> data );
	virtual void parse_serial_message( int msgsize, std::vector<unsigned char> data );
	int set_remote_id();
	int send_id_message();
	int send_config_message();
	
	int send_digital_D3( unsigned char val );
	int send_IO_D3( unsigned char val );
      
	int sendTx16( unsigned char frameid, std::vector<unsigned char> * data );
	int sendTx64( unsigned char frameid, std::vector<unsigned char> * data );
	int sendAT( unsigned char frameid, std::vector<unsigned char> * data );
	int sendAT64( unsigned char frameid, std::vector<unsigned char> * data );

	struct xbee_conAddress addr16;
	struct xbee_conAddress addr64;
	
	struct xbee_conSettings settings16;
	
	MiniXHive * hive;
	
	BeeConnection * con16;
	BeeConnection * con64;
	BeeConnection * conAT;
	BeeConnection * conAT64;
	
// 	libxbee::Con * con16;
// // 	libxbee::Con * con16TxStatus;
// 	libxbee::Con * con64;
// // 	libxbee::Con * con64TxStatus;	
// 	libxbee::Con * conAT;
	
	int status; // current state of minibee
	unsigned char configid; // config id of the minibee
	
	std::string serial_number;
	std::string name;
	int library_version;
	char board_revision;
	int capabilities;
	int remote_config;
	
	unsigned char mymsgid;
	
  };
};

#endif /* __cplusplus */
#endif /* __MINIBEE_H */