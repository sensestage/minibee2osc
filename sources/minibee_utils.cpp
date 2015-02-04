#include "minibee_utils.h"

#include <string.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <iomanip>

void streamXBeePacket( libxbee::Pkt *pkt, std::ostringstream * oss ){
  	int i;
	struct xbee_pkt *pkthandle;
	pkthandle = (pkt)->getHnd();
	
	*oss << "XBEE PACKET from ";
	streamXBeeAddress( pkthandle->address, oss );
// 	if (pkthandle->address.addr16_enabled) {
// 		printf("    16-bit (0x%02X%02X)\n", pkthandle->address.addr16[0], pkthandle->address.addr16[1]);
// 	}
// 	if ( pkthandle->address.addr64_enabled) {
// 		printf("    64-bit (0x%02X%02X%02X%02X 0x%02X%02X%02X%02X)\n", pkthandle->address.addr64[0], pkthandle->address.addr64[1],
// 		                                                               pkthandle->address.addr64[2], pkthandle->address.addr64[3],
// 		                                                               pkthandle->address.addr64[4], pkthandle->address.addr64[5],
// 		                                                               pkthandle->address.addr64[6], pkthandle->address.addr64[7]);
// 	}
	*oss << "XBEE PACKET" << std::endl;
	*oss << "    packet status: " <<  pkthandle->status << std::endl;
	*oss << "    packet options: " <<  pkthandle->options << std::endl;
	*oss << "    packet rssi: " <<  pkthandle->rssi << std::endl;
	*oss << "    packet frameId: " <<  pkthandle->frameId << std::endl;
	*oss << "    packet data size: " <<  (pkt)->size() << std::endl;
	if ( (pkt)->size() > 0 ){
	  *oss << "    packet data: ";
	  for (i = 0; i < (pkt)->size(); i++) {
		  *oss << (*pkt)[i]  << ",";
	  }
	  *oss << "\n";
	  *oss << "    packet data as characters: ";
	  for (i = 0; i < (pkt)->size(); i++) {
		  *oss << (char) (*pkt)[i] << ",";
	  }
	  *oss << "\n";
	  *oss << "    packet data as integers: ";
	  for (i = 0; i < (pkt)->size(); i++) {
		  *oss << (int) (*pkt)[i] << ",";
	  }
	  *oss << "\n";
	  }
}


void streamXBeeAddress( struct xbee_conAddress addr, std::ostringstream * oss ){
  *oss << "XBEE ADDRESS:\n";
  if (addr.addr16_enabled) {
    *oss << "    16-bit ( ";
    *oss << std::hex << std::setfill('0');
    *oss << std::setw(2) << static_cast<unsigned>( addr.addr16[0] );
    *oss << std::setw(2) << static_cast<unsigned>( addr.addr16[1] );
    *oss << ")\n";
//     	printf("    16-bit (0x%02X%02X)\n", addr.addr16[0], addr.addr16[1]);
  }
  if ( addr.addr64_enabled) {
    *oss << "    64-bit ( ";
    *oss << std::hex << std::setfill('0');
    for ( int i=0; i < 8; i++ ){
      *oss << std::setw(2) << static_cast<unsigned>( addr.addr64[i] );
    }
    *oss << ")\n";
// 	printf("    64-bit (0x%02X%02X%02X%02X 0x%02X%02X%02X%02X)\n", 
// 	       addr.addr64[0], addr.addr64[1],
// 	addr.addr64[2], addr.addr64[3],
// 	addr.addr64[4], addr.addr64[5],
// 	addr.addr64[6], addr.addr64[7]);
  }
  *oss << "\n";
}


std::string xbeeAddress64AsString( struct xbee_conAddress addr ){
  std::ostringstream oss;
  if ( addr.addr64_enabled) {
    oss << std::hex << std::setfill('0');
    for ( int i=0; i < 8; i++ ){
      oss << std::setw(2) << static_cast<unsigned>( addr.addr64[i] );
    }
  }
  return oss.str();
}


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
	std::cout << "    packet status: " <<  pkthandle->status << std::endl;
	std::cout << "    packet options: " <<  pkthandle->options << std::endl;
	std::cout << "    packet rssi: " <<  pkthandle->rssi << std::endl;
	std::cout << "    packet frameId: " <<  pkthandle->frameId << std::endl;
	std::cout << "    packet data size: " <<  (pkt)->size() << std::endl;
	if ( (pkt)->size() > 0 ){
	  std::cout << "    packet data: ";
	  for (i = 0; i < (pkt)->size(); i++) {
		  std::cout << (*pkt)[i]  << ",";
	  }
	  std::cout << "\n";
	  std::cout << "    packet data as characters: ";
	  for (i = 0; i < (pkt)->size(); i++) {
		  std::cout << (char) (*pkt)[i] << ",";
	  }
	  std::cout << "\n";
	  std::cout << "    packet data as integers: ";
	  for (i = 0; i < (pkt)->size(); i++) {
		  std::cout << (int) (*pkt)[i] << ",";
	  }
	  std::cout << "\n";
	  }
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
