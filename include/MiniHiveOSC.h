#ifndef __MINIHIVEOSC_H
#define __MINIHIVEOSC_H

#ifndef __cplusplus
#warning This header file is intended for use with C++
#else

#include <vector>
#include <string>

#include <lo/lo.h>
#include "osc/lo_extensions.h"
#include "osc/oscin.h"


namespace libminibee {
  class MiniXHive; // handles connections with XBee

  class HiveOscServer : public BlockingOSCServer {
  public:
      HiveOscServer( const char *port );
      HiveOscServer( HiveOscServer *orig);
      ~HiveOscServer();
      void addBasicMethods();
      void debug( bool onoff );

      void setTargetAddress( const char * host, int port );
      void setTargetAddress( const char * host, const char * port );
      
      void sendStatusMessage( int minibeeID, std::string status, int statusid );
      void sendInfoMessage( int minibeeID, std::string serialnumber, int noin=0, int noout=0 );
      void sendOutputMessage( int minibeeID, std::vector<float> * data );
      void sendOutputBlockMessage( int minibeeID, int noSamples, std::vector<float> * data );

      void sendOutputErrorMessage( int minibeeID, std::vector<int> * data, int errorState );
      void sendCustomErrorMessage( int minibeeID, std::vector<int> * data, int errorState );
      void sendRunErrorMessage( int minibeeID, int onoff, int errorState );
      void sendOutputSuccessMessage( int minibeeID, std::vector<int> * data );
      void sendCustomSuccessMessage( int minibeeID, std::vector<int> * data );
      void sendRunSuccessMessage( int minibeeID, int onoff );

      void sendTriggerMessage( int minibeeID, std::vector<unsigned char> * data );
      void sendPrivateMessage( int minibeeID, std::vector<unsigned char> * data );
      void sendTriggerMessage( int minibeeID, std::vector<float> * data );
      void sendPrivateMessage( int minibeeID, std::vector<float> * data );
//       void sendInfoMessage( minibeeID, std::vector<float> * data );
//       void sendConfiguredMessage( minibeeID,  );

      bool postDebug;

      void handle_tick();
      void handle_ping( int id );
      void handle_quit();
      
      void handle_minibee_output( int minibeeID, std::vector<int> * data, unsigned char noAck = 0 );
      void handle_minibee_custom( int minibeeID, std::vector<int> * data, unsigned char noAck = 0 );
      void handle_minibee_config( int minibeeID, int configID );
      void handle_minibee_run( int minibeeID, int onoff );
      void handle_minibee_loopback( int minibeeID, int onoff );
      void handle_minibee_reset( int minibeeID );
      void handle_minibee_saveid( int minibeeID );
      void handle_minibee_announce( int minibeeID );

//       void handle_minihive_saveconfig( std::string filename );
//       void handle_minihive_loadconfig( std::string filename );
      void handle_minihive_reset();
      void handle_minihive_saveid();
      void handle_minihive_announce();
      
      void setHive( MiniXHive * inhive );


  protected:
// moved from private:      
    lo_address targetAddress;

    MiniXHive * hive;

  // ----------- osc handlers ---------------

	  static int minihiveTickHandler( handlerArgs );
	  static int minihivePingHandler( handlerArgs );
	  static int minihiveQuitHandler( handlerArgs );
     
  // 	static int errorHandler( handlerArgs );
	  static int minibeeOutputHandler( handlerArgs );
	  static int minibeeCustomHandler( handlerArgs );
	  static int minibeeOutputNoAckHandler( handlerArgs );
	  static int minibeeCustomNoAckHandler( handlerArgs );
	  static int minibeeConfigHandler( handlerArgs );
	  static int minibeeRunHandler( handlerArgs );
	  static int minibeeLoopbackHandler( handlerArgs );
	  static int minibeeResetHandler( handlerArgs );
	  static int minibeeSaveIDHandler( handlerArgs );
	  static int minibeeAnnounceHandler( handlerArgs );

	  static int minihiveSaveConfigHandler( handlerArgs );
	  static int minihiveLoadConfigHandler( handlerArgs );

	  static int minihiveResetHandler( handlerArgs );
	  static int minihiveSaveIDHandler( handlerArgs );
	  static int minihiveAnnounceHandler( handlerArgs );
	  
	  static int genericHandler( handlerArgs );
  //----------- end osc handlers ------------

  };
};

#endif /* __cplusplus */
#endif /* __MINIHIVEOSC_H */