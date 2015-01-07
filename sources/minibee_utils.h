#ifndef __MINIBEE_UTILS_H
#define __MINIBEE_UTILS_H


#include <xbeep.h>

void printXBeePacket( libxbee::Pkt *pkt );
void printXBeeAddress( struct xbee_conAddress addr );

#endif