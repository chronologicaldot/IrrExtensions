#include <irrlicht.h>
#include "irrJSON.h"
#include <iostream>

#ifdef _MSC_VER
#pragma comment(lib,"Irrlicht.lib")
#endif

using namespace irr::core;
using namespace irr::io;

using std::cout;
using std::cin;

irr::core::stringc out;

void logNode( /*irr::ILogger* logger,*/ irrTreeNode* node )
{
	irrJSONElement* elem = (irrJSONElement*)(node->getElem());
	if ( elem )
	{
		//logger->log( "Node name", elem->getName().c_str() );
		out.append( "Node name: " ).append( elem->getName().c_str() );
		list<irrJSONElement::Attribute>::Iterator i = elem->getAttributes().begin();
		for ( ; i != elem->getAttributes().end(); ++i )
		{
			//logger->log( " Attribute", (*i).name.c_str() );
			//logger->log( " Value", (*i).value.c_str() );
			out.append( "\nAttribute: " )
				.append( (*i).name.c_str() )
				.append( ", value: " )
				.append( (*i).value.c_str() );
		}
	}
	out.append("\nCHILDREN {\n");
	for ( u32 c=0; c < node->children.size(); c++ )
	{
		logNode( /*logger,*/ node->children[c] );
	}
	out.append("}\n");
}

int main()
{
	irr::IrrlichtDevice* dev = irr::createDevice(video::EDT_NULL,dimension2du(640,480));

	irrJSON* json = new irrJSON( dev->getFileSystem() );
	irrTreeNode* tree;
	bool done = json->parseFile( io::path("json_test.json"), tree );
	if ( !done )
		return 1;

	// Print everything in the tree
	/*irr::ILogger* logger = dev->getLogger();*/
	logNode( /*logger,*/ tree );

	cout << out.c_str();

	char k;
	cin >> k;

	return 0;
}
