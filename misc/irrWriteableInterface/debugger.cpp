
#include <iostream>
#include <irrlicht.h>
#include "TextContainer.h"
#include "irrWinWriter.h"

using std::cin;

using namespace irr;


void main()
{

	//
	irrWinWriter writer;

	writer.print( "Is this working?" );

	writer.finalize();

	writer.writeConsoleInputToStdOut();

	char stopper;
	cin >> stopper;
}