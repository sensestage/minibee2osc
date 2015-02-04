#ifndef __MINIBEE_UTILS_H
#define __MINIBEE_UTILS_H


#include <xbeep.h>

#include <string>
#include <sstream>


void streamXBeePacket( libxbee::Pkt *pkt, std::ostringstream * oss );
void streamXBeeAddress( struct xbee_conAddress addr, std::ostringstream * oss );

std::string xbeeAddress64AsString( struct xbee_conAddress addr );

void printXBeePacket( libxbee::Pkt *pkt );
void printXBeeAddress( struct xbee_conAddress addr );


#endif