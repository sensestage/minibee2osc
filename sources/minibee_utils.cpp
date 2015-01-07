#include "minibee_utils.h"

#include <iostream>
#include <string.h>
#include <unistd.h>

void printXBeePacket( libxbee::Pkt *pkt ){
	int i;
	struct xbee_pkt *pkthandle;
	pkthandle = (pkt)->getHnd();
	
	std::cout << "XBEE PACKET" << std::endl;
	if (pkthandle->address.addr16_enabled) {
		printf("    16-bit (0x%02X%02X)\n", pkthandle->address.addr16[0], pkthandle->address.addr16[1]);
	}
	if ( pkthandle->address.addr64_enabled) {
		printf("    64-bit (0x%02X%02X%02X%02X 0x%02X%02X%02X%02X)\n", pkthandle->address.addr64[0], pkthandle->address.addr64[1],
		                                                               pkthandle->address.addr64[2], pkthandle->address.addr64[3],
		                                                               pkthandle->address.addr64[4], pkthandle->address.addr64[5],
		                                                               pkthandle->address.addr64[6], pkthandle->address.addr64[7]);
	}
	std::cout << "    packet data size: " <<  (pkt)->size() << std::endl;
	std::cout << "    packet data: ";
	for (i = 0; i < (pkt)->size(); i++) {
		std::cout << (*pkt)[i];
	}
	std::cout << "\n";

}

void printXBeeAddress( struct xbee_conAddress addr ){
  std::cout << "XBEE ADDRESS" << std::endl;
  if (addr.addr16_enabled) {
	printf("    16-bit (0x%02X%02X)\n", addr.addr16[0], addr.addr16[1]);
  }
  if ( addr.addr64_enabled) {
	printf("    64-bit (0x%02X%02X%02X%02X 0x%02X%02X%02X%02X)\n", 
	       addr.addr64[0], addr.addr64[1],
	addr.addr64[2], addr.addr64[3],
	addr.addr64[4], addr.addr64[5],
	addr.addr64[6], addr.addr64[7]);
  }
  std::cout << "\n";
}