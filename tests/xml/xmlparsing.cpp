#include <tinyxml2.h>


using namespace tinyxml2;


int main(int argc, char *argv[]) {
	if ( argc > 1 ) {
		XMLDocument* doc = new XMLDocument();
		doc->LoadFile( argv[1] );
		int errorID = doc->ErrorID();
		printf( "Test file '%s' loaded. ErrorID=%d\n", argv[1], errorID );
		
		doc->Print();
		
		XMLElement* root = doc->FirstChildElement();
		const char* roottext = root->Name();
		printf( "Root: %s\n", roottext );

		XMLElement* hive = root->FirstChildElement();
		const char* hivetext = hive->Name();
		printf( "Hive: %s\n", hivetext );
		
		for( const XMLElement* node=hive->FirstChildElement( "minibee" ); node; node=node->NextSiblingElement( "minibee" ) ) {
			const char* config1text = node->Name();
			printf( "\tnode: %s\n", config1text );
			for( const XMLAttribute* attr = node->FirstAttribute(); attr; attr=attr->Next() ) {
			    const char* attrName = attr->Name();
			    const char* attValue = attr->Value();
			    printf( "\t\tattribute: %s, %s\n", attrName, attValue );
			}
			const XMLElement* customnode = node->FirstChildElement("custom");
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
		
		for( const XMLElement* node=hive->FirstChildElement( "configuration" ); node; node=node->NextSiblingElement( "configuration" ) ) {
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
// 		XMLElement* config1 = hive->FirstChildElement();
		// Navigate to the title, using the convenience function,
		// with a dangerous lack of error checking.
// 		const char* title = doc->FirstChildElement( "hive" )->GetText();
// 		->FirstChildElement( "configuration" );
// 		->FirstChildElement( "name" )->GetText();
// 		printf( "Name of configuration (1): %s\n", title );

		delete doc; doc = 0;
	}
	exit(0);
}