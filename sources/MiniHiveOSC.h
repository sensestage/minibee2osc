#ifndef __MINIHIVEOSC_H
#define __MINIHIVEOSC_H

#ifndef __cplusplus
#warning This header file is intended for use with C++
#else

#include <vector>

#include <lo/lo.h>
#include "osc/lo_extensions.h"
#include "osc/oscin.h"


namespace libminibee {
  class HiveOscServer : public NonBlockOSCServer {
  public:
      HiveOscServer( const char *port );
      ~HiveOscServer();
      void addBasicMethods();
      void debug( bool onoff );

      void setTargetAddress( const char * host, int port );
      void setTargetAddress( const char * host, const char * port );
      
      void sendOutputMessage( int minibeeID, std::vector<float> * data );
      void sendTriggerMessage( int minibeeID, std::vector<float> * data );
      void sendPrivateMessage( int minibeeID, std::vector<float> * data );
//       void sendInfoMessage( minibeeID, std::vector<float> * data );
//       void sendConfiguredMessage( minibeeID,  );

      bool postDebug;

  private:
    lo_address targetAddress;

  // ----------- osc handlers ---------------

  // 	static int errorHandler( handlerArgs );

	  static int minibeeOutputHandler( handlerArgs );
	  static int minibeeCustomHandler( handlerArgs );
	  static int minibeeConfigHandler( handlerArgs );

	  static int genericHandler( handlerArgs );
  //----------- end osc handlers ------------

  };
};

#endif /* __cplusplus */
#endif /* __MINIHIVEOSC_H */