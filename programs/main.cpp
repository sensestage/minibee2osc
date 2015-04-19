#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include <xbeep.h>
#include "MiniHive.h"

#include <getopt.h>

#include <signal.h>

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


volatile sig_atomic_t done = 0;
 
void term(int signum)
{
    done = 1;
}

/* ========================================================================== */

int main(int argc, char *argv[]) {
	int ret, res;
	const char * filename = "example_hiveconfig.xml";
	const char * serialport = "/dev/ttyUSB1";
	const char * targetip = "127.0.0.1";
	const char * targetport = "57120";
	const char * listenport = "57600";
	int loglevel = 0;

    int c;
    int digit_optind = 0;

    while (1) {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            {"configfile", required_argument, 0,  0 },
            {"serialport", required_argument, 0,  0 },
            {"listenport", no_argument,       0,  0 },
            {"targetport", no_argument,       0,  0 },
            {"targetip",   no_argument,       0,  0 },
            {"loglevel",   no_argument,       0,  0 },
	    {"help",   no_argument,       0,  0 },
            {0,            0,             0,  0 }
        };

       c = getopt_long(argc, argv, "c:s:l:p:i:v:h", long_options, &option_index);
       if (c == -1)
            break;

       switch (c) {
        case 0:
            printf("option %s", long_options[option_index].name);
            if (optarg)
                printf(" with arg %s", optarg);
            printf("\n");
            break;
	case 'c':
	    filename = optarg;
//             printf("option c with value '%s'\n", optarg);
            break;

	case 's':
	    serialport = optarg;
//             printf("option s with value '%s'\n", optarg);
            break;

	case 'l':
	    listenport = optarg;
//             printf("option l with value '%s'\n", optarg);
            break;

	case 'p':
	    targetport = optarg;
//             printf("option p with value '%s'\n", optarg);
            break;

	case 'i':
	    targetip = optarg;
//             printf("option i with value '%s'\n", optarg);
            break;

	case 'v':
	    loglevel = atoi( optarg );
//             printf("option v with value '%s'\n", optarg);
            break;

	case 'h':
	    printf("option help\n");
            break;

	case '?':
// 	    printf("getopt returned ?\n");
            break;

// 	default:
//             printf("?? getopt returned character code 0%o ??\n", c);
        }
    }

   if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }
	
	
	std::cout << "=============================================================" << std::endl;
	std::cout << "Minibee to OSC" << std::endl;
	std::cout << "configuration file: " << filename << std::endl;
	std::cout << "serial port: " << serialport << std::endl;
	std::cout << "OSC listening port: " << listenport << std::endl;
	std::cout << "OSC target ip: " << targetip << ", target port: " << targetport << std::endl;
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
	  std::cout << "Read configuration file...\n";

	  hive->createOSCServer(listenport);
	  hive->setTargetAddress( targetip, targetport );
	  std::cout << "Created OSC interface...\n";
	  
	  if ( loglevel > 0 ){
	    hive->oscServer->debug(true);
	  }
	  
	  struct sigaction action;
	  memset(&action, 0, sizeof(struct sigaction));
	  action.sa_handler = term;
	  sigaction(SIGTERM, &action, NULL);
	  
	  char s[10];
	  while ( !done ){
	    hive->waitForPacket();
	    res = hive->waitForOSC();
// 	    std::cout << "Number of received OSC messages: " << res << std::endl;
	    //FIXME unfortunately this blocks:
// 	    std::cin.getline(s,10);
// // 	    std::cout << "input: " << s << "\n";
// 	    if(strcmp(s,"quit")==0){ 
// 	      std::cout << "Got request to quit!\n";
// 	      done = 1; 	      
// 	    }
	    usleep(500);
  // 		  usleep(60000000);
	  }

	  std::cout << "Exiting minibee2osc...\n";
	  std::cout << "=============================================================" << std::endl;
	}	
	return 0;
}
