#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include <xbeep.h>
#include "MiniHive.h"


/* ========================================================================== */

/*
class mainConnection: public libxbee::ConCallback {
	public:
		explicit mainConnection(libxbee::XBee &parent, std::string type, struct xbee_conAddress *address = NULL): libxbee::ConCallback(parent, type, address) {};
		void xbee_conCallback(libxbee::Pkt **pkt);
		std::string myData;
};

void mainConnection::xbee_conCallback(libxbee::Pkt **pkt) {
	std::cout << "Callback!!\n";
	int i;
	
	struct xbee_conAddress newAddress;
	struct xbee_pkt *pkthandle;
	pkthandle = (*pkt)->getHnd();
	
	if (pkthandle->address.addr16_enabled) {
		printf("    16-bit (0x%02X%02X)\n", pkthandle->address.addr16[0], pkthandle->address.addr16[1]);
	}
	if ( pkthandle->address.addr64_enabled) {
		printf("    64-bit (0x%02X%02X%02X%02X 0x%02X%02X%02X%02X)\n", pkthandle->address.addr64[0], pkthandle->address.addr64[1],
		                                                               pkthandle->address.addr64[2], pkthandle->address.addr64[3],
		                                                               pkthandle->address.addr64[4], pkthandle->address.addr64[5],
		                                                               pkthandle->address.addr64[6], pkthandle->address.addr64[7]);
	}

	if ((*pkt)->size() > 0) {
	  std::cout << "rx data size: " <<  (*pkt)->size();
	  if ((*pkt)->size() > 2) { // type and msg id
	    char type = (**pkt)[0];
	    int msgid = (int) (**pkt)[1];
	    std::cout << ", type: " << type <<  ", msg id: " << msgid;
	    switch ( type ){
	      case 's':
		if ( (*pkt)->size() > 10 ){
		  memset(&newAddress, 0, sizeof(newAddress));
		  newAddress.addr64_enabled = 1;
		  for ( i=2; i<10; i++ ){
// 		    std::cout << (*pkt)[i];
		    newAddress.addr64[i-2] = (**pkt)[i];
		  }
		}
		break;
	    }
	  }
	  std::cout << "\n"; 
	}
	  
	for (i = 0; i < (*pkt)->size(); i++) {
		std::cout << (**pkt)[i];
	}
	std::cout << "\n";

	std::cout << myData;

}
*/

/* ========================================================================== */

int main(int argc, char *argv[]) {
	int ret, res;
	const char * filename = "example_hiveconfig.xml";
	const char * serialport = "/dev/ttyUSB1";
	const char * targetip = "127.0.0.1";
	const char * targetport = "57120";
	const char * listenport = "57600";
	int loglevel = 0;
// 	// 1: filename, 2: serialport, 3: listenport, 4: sendport, 5: sendip, 6: loglevel
	if ( argc > 6 ){
	    loglevel = atoi( argv[6] );
	}
	if ( argc > 5 ){
	  targetip = argv[5];
	}
	if ( argc > 4 ){
	  targetport = argv[4];
	}
	if ( argc > 3 ){
	  listenport = argv[3];
	}
	if ( argc > 2 ){
	  serialport = argv[2];
	}
	if ( argc > 1 ){
	  filename = argv[1];
	}
	
	std::cout << "=============================================================" << std::endl;
	std::cout << "XBee to OSC" << std::endl;
	std::cout << "filename: " << filename << std::endl;
	std::cout << "serial port: " << serialport << std::endl;
	std::cout << "listening port: " << listenport << std::endl;
	std::cout << "target ip: " << targetip << ", target port: " << targetport << std::endl;
	std::cout << "loglevel: " << loglevel << std::endl;
	std::cout << "=============================================================" << std::endl;
	
	  /* setup libxbee */
	libminibee::MiniXHive * hive;
	hive = new libminibee::MiniXHive();
	std::cout << "Creating XBee...\n";
	
	ret = hive->createXBee(serialport, loglevel); //TODO: serial port should be an argument to the start of program, log level too

	if ( ret == 0 ){
	  std::cout << "Opened connection...\n";

	  hive->readConfigurationFile( filename );

	  hive->createOSCServer(listenport);
	  hive->setTargetAddress( targetip, targetport );
	  
	  if ( loglevel > 0 ){
	    hive->oscServer->debug(true);
	  }
	  
	  while ( true ){
	    hive->waitForPacket();
	    res = hive->waitForOSC();
// 	    std::cout << "Number of received OSC messages: " << res << std::endl;
	    usleep(500);
  // 		  usleep(60000000);
	  }

	  std::cout << "Closing connection...\n";
	}	
	return 0;
}
