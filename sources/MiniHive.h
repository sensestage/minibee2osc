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

#include <list>

// #ifndef EXPORT
// #define EXPORT
// #define XBEE_EXPORT_DEFINED
// #endif


namespace libminibee {
  class MiniXHive; // handles connections with XBee
  
  class miniXHiveConnection: public libxbee::ConCallback {
	public:
	  explicit miniXHiveConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address = NULL): libxbee::ConCallback(parent, type, address) {};
	  void xbee_conCallback(libxbee::Pkt **pkt);
	  MiniXHive *minihive; // points to our minihive
  };

 
  class miniXHiveTXConnection: public libxbee::ConCallback {
	public:
	  explicit miniXHiveTXConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address = NULL): libxbee::ConCallback(parent, type, address) {};
	  void xbee_conCallback(libxbee::Pkt **pkt);
	  MiniXHive *minihive; // points to our minihive
  };

  class miniXHiveModemConnection: public libxbee::ConCallback {
	public:
	  explicit miniXHiveModemConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address = NULL): libxbee::ConCallback(parent, type, address) {};
	  void xbee_conCallback(libxbee::Pkt **pkt);
	  MiniXHive *minihive; // points to our minihive
  };
  
  class MiniXHive{
    public:
	MiniXHive(void);
	~MiniXHive(void);
	
    private:
	struct xbee_conAddress addr;
	libxbee::XBee xbee;
	
	miniXHiveConnection * conData16;
	miniXHiveTXConnection * conTXStatus16;
	miniXHiveModemConnection * conModemStatus;
  };
};

#endif /* __cplusplus */
#endif /* __MINIHIVE_H */