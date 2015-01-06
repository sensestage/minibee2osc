#include <iostream>
#include <string.h>
#include <unistd.h>

#include <xbeep.h>

#define USE_CALLBACKS

// #ifdef USE_CALLBACKS

/* ========================================================================== */

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

/* ========================================================================== */

int main(int argc, char *argv[]) {
	int i;
	try {
		/* setup libxbee */
		libxbee::XBee xbee("xbee1", "/dev/ttyUSB0", 57600); //TODO: serial port should be an argument to the start of program
		std::cout << "Running libxbee in mode '" << xbee.mode() << "'\n";
		xbee.setLogLevel( 100 ); //TODO: loglevel should be an argument to the start of the program		

		/* make a connection for the first info data */
		struct xbee_conAddress addr;
		memset(&addr, 0, sizeof(addr));
		addr.addr16_enabled = 1;
		addr.addr16[0] = 0xFF;
		addr.addr16[1] = 0xFA;
		mainConnection con(xbee, "16-bit Data", &addr); /* with a callback */
		con.myData = "Testing, 1... 2... 3...\n";

		std::cout << "Opened connection...\n";

		usleep(60000000);

		std::cout << "Closing connection...\n";

	} catch (xbee_err err) {
		std::cout << "Error " << err << "\n";
	}
	
	return 0;
}
