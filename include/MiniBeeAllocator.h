#ifndef __MINIBEEALLOCATOR_H
#define __MINIBEEALLOCATOR_H

#ifndef __cplusplus
#warning This header file is intended for use with C++
#else

#include <string.h>
#include <list>
// #include <map>

// #include "MiniBeeConfig.h"
// #include "MiniBee.h"

namespace libminibee {
  
  class MiniBeeIDAllocator{
      public:
	  MiniBeeIDAllocator(void);
	  ~MiniBeeIDAllocator(void);
	  
	  bool free( int id );
	  void add( int id );
	  bool isAvailable( int id );
	  bool isAssigned( int id );
	  int nextAvailable();
	  void print();

    private:
	  int highest;
	  std::list<int> available;
	  std::list<int> assigned;
  };
    
};

#endif /* __cplusplus */
#endif /* __MINIBEEALLOCATOR_H */