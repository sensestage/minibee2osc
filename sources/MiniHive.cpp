

#include "MiniHive.h"

#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace libminibee;

void miniXHiveConnection::xbee_conCallback(libxbee::Pkt **pkt) {
	std::cout << "MiniXBee Modem Callback!!\n";
	int i;
	
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
	  std::cout << "rx data size: " <<  (*pkt)->size() << "\n"; 
	}
	  
	for (i = 0; i < (*pkt)->size(); i++) {
		std::cout << (**pkt)[i];
	}
	std::cout << "\n";
}

void miniXHiveTXConnection::xbee_conCallback(libxbee::Pkt **pkt) {
	std::cout << "MiniXBee Modem Callback!!\n";
	int i;
	
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
	  std::cout << "rx data size: " <<  (*pkt)->size() << "\n"; 
	}
	  
	for (i = 0; i < (*pkt)->size(); i++) {
		std::cout << (**pkt)[i];
	}
	std::cout << "\n";
}

void miniXHiveModemConnection::xbee_conCallback(libxbee::Pkt **pkt) {
	std::cout << "MiniXBee Modem Callback!!\n";
	int i;
	
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
	  std::cout << "rx data size: " <<  (*pkt)->size() << "\n"; 
	}
	  
	for (i = 0; i < (*pkt)->size(); i++) {
		std::cout << (**pkt)[i];
	}
	std::cout << "\n";
}
