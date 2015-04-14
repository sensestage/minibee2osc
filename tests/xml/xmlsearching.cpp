
#include <tinyxml2.h>

#include <string>

#include <list>
#include <algorithm>

#include <iostream>


using namespace std;
using namespace tinyxml2;

// IDAllocator

class IDAllocator{
    public:
	IDAllocator(void);
	~IDAllocator(void);
	
	void add( int id );
	bool isAvailable( int id );
	int nextAvailable();
	void print();

  private:
	int highest;
	list<int> available;
	list<int> assigned;
};

IDAllocator::IDAllocator(){
    highest = 0;
}

IDAllocator::~IDAllocator(){
  
}

void IDAllocator::add( int id ){
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
  
bool IDAllocator::isAvailable( int id ){
  if ( id > highest ){
      return true;
  } else {
      std::list<int>::iterator findIter = std::find(available.begin(), available.end(), id );
      if ( findIter == available.end() ){ return false; }{ return true; }
//       return *findIter;
  }
}

int IDAllocator::nextAvailable(){
  if ( available.size() == 0 ){
    return highest+1;
  } else {
    list<int>::iterator next = available.begin();
    return *next;
  }
}

void IDAllocator::print(){
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

// end of IDAllocator


void analyzeFile( XMLElement* hive ){
  IDAllocator * ids = new IDAllocator();
  for( const XMLElement* node=hive->FirstChildElement( "minibee" ); node; node=node->NextSiblingElement( "minibee" ) ) {
	  int v0 = 0;      
	  node->QueryIntAttribute( "id", &v0 );
// 	  printf( "\tid: %i\n", v0 );
	  ids->add( v0 );
// 	  ids->print();
    }
  ids->print();
  
  printf( "\t next available: %i", ids->nextAvailable() );
  if ( ids->isAvailable( 20 ) ){
    printf( "\t 20 is available" );
  } else {
    printf( "\t 20 is not available" );
  }
  if ( ids->isAvailable( 60 ) ){
    printf( "\t 60 is available" );
  } else {
    printf( "\t 60 is not available" );
  }
}

const XMLElement * findMinibeeWithID( XMLElement* hive, int id ){
    for( const XMLElement* node=hive->FirstChildElement( "minibee" ); node; node=node->NextSiblingElement( "minibee" ) ) {
	  int v0 = 0;      
	  node->QueryIntAttribute( "id", &v0 );
	  printf( "\tid: %i\n", v0 );
	  if ( v0 == id ){
	    return node;
	  }
    }
    return NULL;
}

const XMLElement * findMinibeeWithSerial( XMLElement* hive, string serialnumber ){
    for( const XMLElement* node=hive->FirstChildElement( "minibee" ); node; node=node->NextSiblingElement( "minibee" ) ) {
	  const char* text = node->Attribute( "serial" );
// 	  const char* text = attr->Value();
	  printf( "\tserial: %s, %i\n", text, serialnumber.compare( text ) );
	  if ( serialnumber.compare( text ) == 0 ){
	    return node;
	  }
    }
    return NULL;
}

void printMinibeeElement( const XMLElement *bee ){
	const char* config1text = bee->Name();
	printf( "\tnode: %s\n", config1text );
// 	string( bee->Attribute( "name" ) );
// 	printf( "\trevision: %c\n", bee->Attribute( "revision" )[0] );
	for( const XMLAttribute* attr = bee->FirstAttribute(); attr; attr=attr->Next() ) {
	    const char* attrName = attr->Name();
	    const char* attValue = attr->Value();
	    printf( "\t\tattribute: %s, %s\n", attrName, attValue );
	}
	const XMLElement* customnode = bee->FirstChildElement("custom");
	if ( customnode != NULL ){
	  for( const XMLElement* subnode=customnode->FirstChildElement("data"); subnode; subnode=subnode->NextSiblingElement( "data" ) ) {
	    const char* name = subnode->Name();
	    printf( "\t\tcustom data: %s\n", name );
	    for( const XMLAttribute* attr = subnode->FirstAttribute(); attr; attr=attr->Next() ) {
		const char* attrName = attr->Name();
		const char* attValue = attr->Value();
		printf( "\t\t\tattribute: %s, %s\n", attrName, attValue );
	    }
	  }
	}
}

const XMLElement * findConfigurationWithID( XMLElement* hive, int id ){
    for( const XMLElement* node=hive->FirstChildElement( "configuration" ); node; node=node->NextSiblingElement( "configuration" ) ) {
	  int v0 = 0;      
	  node->QueryIntAttribute( "id", &v0 );
	  printf( "\tid: %i\n", v0 );
	  if ( v0 == id ){
	    return node;
	  }
    }
    return NULL;
}

void printMinibeeConfiguration( const XMLElement *node ){
    const char* config1text = node->Name();
    printf( "\tnode: %s\n", config1text );
    for( const XMLAttribute* attr = node->FirstAttribute(); attr; attr=attr->Next() ) {
	const char* attrName = attr->Name();
	const char* attValue = attr->Value();
	printf( "\t\tattribute: %s, %s\n", attrName, attValue );
    }
    for( const XMLElement* subnode=node->FirstChildElement("pin"); subnode; subnode=subnode->NextSiblingElement( "pin" ) ) {
	const char* name = subnode->Name();
	printf( "\tpins: %s\n", name );
	for( const XMLAttribute* attr = subnode->FirstAttribute(); attr; attr=attr->Next() ) {
	    const char* attrName = attr->Name();
	    const char* attValue = attr->Value();
	    printf( "\t\tattribute: %s, %s\n", attrName, attValue );
	}
    }
    for( const XMLElement* subnode=node->FirstChildElement("twi"); subnode; subnode=subnode->NextSiblingElement( "twi" ) ) {
	const char* name = subnode->Name();
	printf( "\ttwi: %s\n", name );
	for( const XMLAttribute* attr = subnode->FirstAttribute(); attr; attr=attr->Next() ) {
	    const char* attrName = attr->Name();
	    const char* attValue = attr->Value();
	    printf( "\t\tattribute: %s, %s\n", attrName, attValue );
	}


	for( const XMLElement* ssubnode=subnode->FirstChildElement("twislot"); ssubnode; ssubnode=ssubnode->NextSiblingElement( "twislot" ) ) {
	  const char* name = ssubnode->Name();
	  printf( "\tsub twi: %s\n", name );
	  for( const XMLAttribute* attr = ssubnode->FirstAttribute(); attr; attr=attr->Next() ) {
	      const char* attrName = attr->Name();
	      const char* attValue = attr->Value();
	      printf( "\t\tattribute: %s, %s\n", attrName, attValue );
	  }
	}
    }
    const XMLElement* customnode = node->FirstChildElement("customconf");
    if ( customnode != NULL ){
      for( const XMLElement* subnode=customnode->FirstChildElement("data"); subnode; subnode=subnode->NextSiblingElement( "data" ) ) {
	const char* name = subnode->Name();
	printf( "\t\tcustom data: %s\n", name );
	for( const XMLAttribute* attr = subnode->FirstAttribute(); attr; attr=attr->Next() ) {
	    const char* attrName = attr->Name();
	    const char* attValue = attr->Value();
	    printf( "\t\t\tattribute: %s, %s\n", attrName, attValue );
	}
      }
    }
}

int main(int argc, char *argv[]) {
	if ( argc > 1 ) {
		XMLDocument* doc = new XMLDocument();
		doc->LoadFile( argv[1] );
		int errorID = doc->ErrorID();
		printf( "Test file '%s' loaded. ErrorID=%d\n", argv[1], errorID );
		
// 		doc->Print();
		
		XMLElement* root = doc->FirstChildElement();
		const char* roottext = root->Name();
		printf( "Root: %s\n", roottext );

		XMLElement* hive = root->FirstChildElement( "hive" );
		const char* hivetext = hive->Name();
		printf( "Hive: %s\n", hivetext );
		
		analyzeFile( hive );
		
		const XMLElement* minibeeElement = findMinibeeWithID( hive, atoi( argv[2] ) );
		
		if ( minibeeElement != NULL ){
		  printMinibeeElement( minibeeElement );
		  
	  	  int v1 = 0;
		  minibeeElement->QueryIntAttribute( "configuration", &v1 );
		  printf( "\tconfiguration: %i\n", v1 );

		  const XMLElement* configElement = findConfigurationWithID( hive, v1 );
		  if ( configElement != NULL ){
		    printMinibeeConfiguration( configElement );
		  }
		}
		
		const XMLElement* minibeeElement2 = findMinibeeWithSerial( hive, argv[3] );
		if ( minibeeElement2 != NULL ){
		  printMinibeeElement( minibeeElement2 );
		  
	  	  int v2 = 0;
		  minibeeElement2->QueryIntAttribute( "configuration", &v2 );
		  printf( "\tconfiguration: %i\n", v2 );

		  const XMLElement* configElement2 = findConfigurationWithID( hive, v2 );
		  if ( configElement2 != NULL ){
		    printMinibeeConfiguration( configElement2 );
		  }
		}
		
		delete doc; doc = 0;
	}
	exit(0);
}