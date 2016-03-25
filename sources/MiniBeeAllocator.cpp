#include "MiniBeeAllocator.h"
// start of MiniBeeIDAllocator

using namespace libminibee

MiniBeeIDAllocator::MiniBeeIDAllocator(){
    highest = 0;
}

MiniBeeIDAllocator::~MiniBeeIDAllocator(){
  
}

bool MiniBeeIDAllocator::free( int id ){
    if ( isAssigned( id ) ){ // free it
      assigned.remove( id );
      available.push_back( id );
      return( true ); // it was assigned
    } // otherwise, it was not assigned in the first place
    return( false );
}

void MiniBeeIDAllocator::add( int id ){
  if ( id > highest ){
      for ( int i=highest+1; i < id; i++ ){
// 	std::cout << "highest " << highest << ", id " << id << ", i " << i << std::endl;
	available.push_back( i );
      }
      assigned.push_back( id );
      highest = id;
  } else { // <= highest
      available.remove( id );
      assigned.push_back( id );
  }
  assigned.sort();
  assigned.unique();
}

bool MiniBeeIDAllocator::isAssigned( int id ){
  std::list<int>::iterator findIter = std::find(assigned.begin(), assigned.end(), id );
  if ( findIter == assigned.end() ){ return false; }{ return true; }
}
  
bool MiniBeeIDAllocator::isAvailable( int id ){
  if ( id > highest ){
      return true;
  } else {
      std::list<int>::iterator findIter = std::find(available.begin(), available.end(), id );
      if ( findIter == available.end() ){ return false; }{ return true; }
//       return *findIter;
  }
}

int MiniBeeIDAllocator::nextAvailable(){
  if ( available.size() == 0 ){
    return highest+1;
  } else {
    std::list<int>::iterator next = available.begin();
    return *next;
  }
}

void MiniBeeIDAllocator::print(){
  std::cout << "available contains:";
  for (std::list<int>::iterator it=available.begin(); it!=available.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';  
  std::cout << "assigned contains:";
  for (std::list<int>::iterator it=assigned.begin(); it!=assigned.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';
  std::cout << "highest value: " << highest << std::endl;  
}

// end of MiniBeeIDAllocator
