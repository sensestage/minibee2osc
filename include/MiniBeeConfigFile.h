#ifndef __MINIBEECONFIGFILE_H
#define __MINIBEECONFIGFILE_H

#ifndef __cplusplus
#warning This header file is intended for use with C++
#else

#include <string.h>
#include <list>
#include <map>

#include <tinyxml2.h>

#include "MiniBeeConfig.h"
#include "MiniBee.h"

using namespace tinyxml2;

namespace libminibee {
  
  class MiniBeeIDAllocator{
      public:
	  MiniBeeIDAllocator(void);
	  ~MiniBeeIDAllocator(void);
	  
	  void add( int id );
	  bool isAvailable( int id );
	  int nextAvailable();
	  void print();

    private:
	  int highest;
	  std::list<int> available;
	  std::list<int> assigned;
  };
    
  class MiniBeeConfigFile{
    public:
      
      static std::map<std::string,int> PinNames;
      static std::map<std::string,int> PinConfigNames;
      static std::map<std::string,int> TWINames;
      
	MiniBeeConfigFile(void);
	~MiniBeeConfigFile(void);

	int load( const char *filename ); // success or not
// 	int save( const char *filename ); // success or not
	
	// analyze file
	int analyzeFile( MiniBeeIDAllocator * idallocator );
	
	// reading data from file
	int readConfigurationWithID( int id, MiniBeeConfig * config );
	
	int readMiniBeeWithID( int id, MiniXBee * bee );
	int readMiniBeeWithSerialNumber( std::string serialnumber, MiniXBee * bee );
	
	void readConfigurationElement( const XMLElement * ele, MiniBeeConfig * config );
	void readMiniBeeElement( const XMLElement * ele, MiniXBee * bee );
	
	// adding new elements
// 	int addConfiguration( MiniBeeConfig * config );
// 	int addMinibee( MiniXBee * bee );

    private:
	XMLDocument* doc;
	XMLElement* hiveroot;

  };
};

#endif /* __cplusplus */
#endif /* __MINIBEECONFIGFILE_H */