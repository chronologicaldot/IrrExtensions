

#include <irrlicht.h>
#include "irrVFreader.cpp"

#include <iostream>
using std::cin;
using std::cout;

#ifdef _MSC_VER
#pragma comment( lib, "irrlicht.lib" )
#endif



void main()
{
	irr::IrrlichtDevice* dev;

	dev = createDevice();

	irrVFreader reader(
		dev->getFileSystem()->createAndOpenFile( "test.txt" )
		);

	irr::core::stringc text = " ";
	irr::core::stringc schars = " \n\r/*\"\"";
	irr::c8* chars = (irr::c8*)schars.c_str();

	while ( text != "end_main" )
	{
		text = reader.readFromFile_sys_nse( 6, chars );

		//if ( text == "\"" )
		//	cout << "\n(Q)";
		//else
			cout << "\n(" << (char*)text.c_str() << ")";
	}

	char h;
	cin >> h;
}