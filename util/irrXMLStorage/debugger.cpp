/*
(c) 2012 Nic Anderson
*/

#include <iostream>

#include <irrlicht.h>
#include "irrXMLStorage.h"
#include <irrWinWriter.h>

using std::cin;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

using namespace irr;

void main()
{
	IrrlichtDevice * irrdevice = createDevice(video::EDT_NULL);
	irrXMLStorage * unit = new irrXMLStorage();
	irrWinWriter writer;

	writer.print( "Starting test...\n" );

	if ( unit->LoadFile( irrdevice->getFileSystem(), "text.html" ) )
	{
		writer.print( "\nFile loaded" );

		if ( unit->ParseFile() )
		{
			writer.print( "\nFile parsed\n\n" );
			unit->WriteAll( &writer );
		}
	}

	unit[0].get(0)->element;

	//writer.finalize(); the unit already calls finalize

	char c;
	cin >> c;
}