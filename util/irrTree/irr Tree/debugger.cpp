/*
(c) 2012 Nic Anderson
*/

#include <iostream>

#include <irrlicht.h>
#include "irrTree.h"

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

using std::cout;
using std::cin;
using namespace irr;

void test1();
void test2();
void test3();
void test4();


class Int : public irrTreeElement
{
public:
	s32 i;

	Int() : irrTreeElement() { i = 0; }
	Int( s32 val ) : irrTreeElement() { i = val; }
	void set( s32 val ) { i = val; }
	s32 get() { return i; }

	virtual bool matches( irrTreeElement * other )
	{
		cout << "\nIn TreeElement: fixed: i = " << i << ", other = " << ((Int *)other)->i;
		return i == ((Int *)other)->i;
	}
	virtual bool matches( s32 * val ) { return (i==*val)?true:false; }
	virtual bool matches( Int * val ) { return (i==val->i)?true:false; }
};




void main()
{
	test4();
}


void test1()
{
	irrTreeNode tree;

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

	cout << "\n\nNow inserting numbers...";

	tree[2].setElem( new Int(19) );
	tree[4] = new Int(4);

	cout << "\nNow attempting to find them...";

	cout << "\nFirst number's node id = " << tree.getChildIndexByMatch( &Int(19) );
	cout << "\nSecond number's node id = " << tree.getChildIndexByMatch( &Int(4) );

	// Doesn't work because native element does not contain field "i":
	//((Int *)(TreeElement *)tree[0][0][3].element)->i = (s32)12;
	//((Int *)((TreeElement *)tree[0][5].getElem()))->i = (s32)3;

	cout << "\n\n\nNow looking for second set of numbers...";

	tree[0][0][3] = new Int(12);
	((Int *)tree[4].getElem())->i = 3;

	cout << "\nThird number's node id = " << tree[0][0].getChildIndexByMatch( &Int(12) );
	cout << "\nFourth number's node id = " << tree.getChildIndexByMatch( &Int(3) );

	cin >> h;

	cout << "\n\n\nNow attempting to find them the long way.";

	irrTreeNode * node = new irrTreeNode();
	node->setElem( new Int() );
	((Int *)(node->getElem()))->set( 2 );
	
	if ( tree.getAnyMatching( &Int(19), node ) )
	{
		s32 in = ((Int *)(node->getElem()))->i;
		cout << "\nNumber found = " << in;
	} else {
		cout << "\nCould not find first node.";
	}

	cin >> h;
}

void test2()
{
	irrTreeNode tree;

	char h;
	int id = 0;

	cout << "\nNow testing deletion...\nAdding node.";
	tree = new Int(id*10);
	tree.addNode(++id);
	tree[0] = new Int(id*10);
	tree[0].addNode(++id);
	tree[0][0] = new Int(id*10);
	/* note that assigning inline, as in the following, causes id to not increment
	for the Int class:
	tree[0].addNode(++id) = new Int(id*10); */

	cout << "\nTree = ";
	cout << "\n" << tree.ID << " (" << ((Int*)(tree.getElem()))->get() << ")";
	cout << "\n" << tree[0].ID << " (" << ((Int*)(tree[0].getElem()))->get() << ")";
	cout << "\n" << tree[0][0].ID << " (" << ((Int*)(tree[0][0].getElem()))->get() << ")";

	irrTreeNode* last = & (tree[0][0]);

	cout << "\n\nRemoving node (middle)...";
	tree[0].~irrTreeNode();
	cout << "\nTree (from root) = ";
	cout << "\n" << tree.ID << " (" << ((Int*)tree.getElem())->get() << ")";
	cout << "\n\nHas children? ";
	if ( tree.children.size() )
		cout << "Yes";
	else
		cout << "No";
	cout << "\n\nDoes last still exist? ";
	if ( last )
	{
		cout << "Yes";
		cout << "\nand it == " << last->ID << " (" << ((Int*)(last->getElem()))->get() << ")";
	} else
		cout << "No";

	cout << "\n";

	cin >> h;
}

void test3()
{
	char h;

	cout << "\nTest begun.";

	irrTreeNode* root = new irrTreeNode();
	root->setElem(new Int(0));

	root->addNode() = new Int(12);
	root->addNode() = new Int(22);

	irrTreeNode other;
	other.stealNode( (*root)[0] );
	other.stealNode( (*root)[0] );

	delete root;

	cout << "\nTest halfway.";

	cout << "\nother[0] == " << ((Int*) other[0].getElem() )->get();
	cout << "\nother[1] == " << ((Int*) other[1].getElem() )->get();

	cout << "\nTest complete.";

	cin >> h;
}

void test4()
{
	char h;

	cout << "\nTest begun.";

	irrTreeNode* root = new irrTreeNode();
	root->setElem( new Int(0) );

	root->addNode() = new Int(11);
	root->addNode() = new Int(21);

	root->get(0)->addNode() = new Int(111);

	cout << "\nDeleting...";

	delete root->get(0);

	cout << "\nDelete complete.";

	cin >> h;
}