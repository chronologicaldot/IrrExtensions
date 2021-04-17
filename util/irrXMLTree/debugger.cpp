/*
(c) Nic Anderson
*/

#include <irrlicht.h>

#include "irrXMLTree.h"
#include <irrTree.cpp>

#include <iostream>
using std::cout;
using std::cin;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif


void main()
{
	irrXMLTreeNode tree;

	char h;

	for ( s32 i=0; i < 10; i++ )
	{
		tree.addNode( i );
	}

	cout << "\nCurrent size 1 = " << tree.Size();
	cin >> h;

	for ( s32 i=0; i < 10; i++ )
	{
		tree[0].addNode( i );
	}

	cout << "\nCurrent size 2 = " << tree.Size();
	cin >> h;

	for ( s32 i=0; i < 10; i++ )
	{
		tree[0][0].addNode( i );
	}

	cout << "\nCurrent size 3 = " << tree.Size();
	cin >> h;

	cout << "\n\nNow inserting names...";

	tree[2].setElem( new irrXMLTreeElement( io::EXN_ELEMENT, "t1" ) );
	tree[4] = new irrXMLTreeElement( io::EXN_ELEMENT, "t2" );

	cout << "\nNow attempting to find them...";

	cout << "\nFirst name's node id = "
		<< tree.getChildIndexByMatch( new irrXMLTreeElement( io::EXN_ELEMENT, "t1" ) );
	cout << "\nSecond name's node id = "
		<< tree.getChildIndexByMatch( new irrXMLTreeElement( io::EXN_ELEMENT, "t2" ) );

	cin >> h;

	cout << "\n\n\nNow looking for second set of numbers...";

	tree[0][0][3] = new irrXMLTreeElement( io::EXN_ELEMENT, "t3" );
	((irrXMLTreeElement*)tree[4].getElem())->setNodeName( "t4" );

	cout << "\nThird name's node id = "
		<< tree[0][0].getChildIndexByMatch( new irrXMLTreeElement( io::EXN_ELEMENT, "t3" ) );
	cout << "\nFourth name's node id = "
		<< tree.getChildIndexByMatch( new irrXMLTreeElement( io::EXN_ELEMENT, "t4" ) );

	cin >> h;

	cout << "\n\n\nNow attempting to find them the long way.";


	irrXMLTreeNode* nodexml = new irrXMLTreeNode();
	nodexml->setElem( new irrXMLTreeElement() );
	((irrXMLTreeElement*)( nodexml->getElem() ))->setNodeName( "lt1" );
	
	cout << "...";
	cin >> h;

	irrTreeNode* node;
	irrXMLTreeElement* xmlelem = new irrXMLTreeElement( io::EXN_ELEMENT, "t4" );
	if ( tree.getAnyMatching( xmlelem, (irrTreeNode*&)node ) )
	{
		core::stringc in = ((irrXMLTreeElement*)(node->getElem()))->getNodeName();
		cout << "\nNode name found = " << in.c_str();
	} else {
		cout << "\nCould not find node.";
	}

	//delete xmlelem;

	cin >> h;
}