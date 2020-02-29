/*
(c) 2012 Nicolaus Anderson

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Since this uses the irrlicht engine, see irrlicht.h for more details.
*/

#ifndef IRRTREE_H
#define IRRTREE_H

#include <irrlicht.h>

using namespace irr;

class irrTreeElement
{
public:
	/* A virtual deconstructor is required for this class (and consequently for inheriting
	from this class) because the "delete" operator is used. */
	virtual ~irrTreeElement() {}

	virtual bool matches( irrTreeElement * other )
	{
		return false;
	}

	bool operator== ( irrTreeElement * other )
	{
		return matches( other );
	}
};


class irrTreeNode
{
public:
	core::array<irrTreeNode*> children;
	s32 ID; // ID of this node
	irrTreeElement* element; // what this node is carrying
	irrTreeNode* parent; // parent node of this node

	irrTreeNode( s32 id=0, irrTreeNode* parent_node=0,
				irrTreeElement* new_element = new irrTreeElement() )
			: ID(id)
			, element( new_element )
			, parent( parent_node )
	{
	}

	~irrTreeNode()
	{
		u32 c = 0;
		for ( ; c < children.size(); c++ )
		{
			children[c]->parent = 0;
		}
		children.clear();
		if ( element )
			delete element;

		if ( parent )
			parent->removeChild(this);
	}

	void setElem( irrTreeElement* new_element )
	{
		if ( element )
			delete element;
		element = new_element;
	}

	void operator= ( irrTreeElement* new_element )
	{
		setElem( new_element );
	}

	irrTreeElement* getElem()
	{
		return element;
	}

	irrTreeNode& addNode( s32 id=0 )
	{
		children.push_back( new irrTreeNode( id, this ) );
		return *(children[children.size()-1]);
	}

	irrTreeNode& addNode( irrTreeElement* elem, s32 id=0 )
	{
		children.push_back( new irrTreeNode( id, this ) );
		children[children.size()-1]->element = elem;
		return *(children[children.size()-1]);
	}

	// Takes a node from another tree node
	void stealNode( irrTreeNode& pTakeMe )
	{
		if ( pTakeMe.parent )
			pTakeMe.parent->removeChild(&pTakeMe);
		children.push_back( &pTakeMe );
		pTakeMe.parent = this;
	}

	void removeChild( irrTreeNode* child )
	{
		u32 c = 0;
		for ( ; c < children.size(); c++ )
		{
			if ( children[c] == child )
			{
				children.erase(c);
				break;
			}
		}
	}

	//! Size of tree
	/* Returns the size of the entire tree, including all nodes. */
	s32 Size()
	{
		s32 size = children.size();

		for ( s32 i=0 ; i < (s32)children.size(); i++ )
			size += getChildByIndex(i)->Size();

		return size;
	}

		//! Get child by index
	/* Returns only one of this node's children based on index. */
	irrTreeNode* getChildByIndex( s32 child )
	{
		return children[child];
	}

	irrTreeNode& operator[] ( s32 index )
	{
		return *(children[index]);
	}

	irrTreeNode* get( s32 index )
	{
		return getChildByIndex( index );
	}

	irrTreeNode* getParent()
	{
		return parent;
	}

	bool matches( irrTreeElement* match );	

	bool getNodeWithID( s32 id, irrTreeNode*& node );

	bool getAnyMatching( irrTreeElement* match, irrTreeNode*& node );

	s32 getChildIndexByMatch( irrTreeElement* match );

	bool getChildByMatch( irrTreeElement* match, irrTreeNode*& node );
};

#endif
