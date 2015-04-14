#include <tinyxml2.h>


using namespace tinyxml2;

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

void printMinibeeElement( const XMLElement *bee ){
	const char* config1text = bee->Name();
	printf( "\tnode: %s\n", config1text );
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

		XMLElement* hive = root->FirstChildElement();
		const char* hivetext = hive->Name();
		printf( "Hive: %s\n", hivetext );
		
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
		

		delete doc; doc = 0;
	}
	exit(0);
}