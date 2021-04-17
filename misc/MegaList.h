/*
(c) 2013 Nicolaus Anderson

This is a doubly-linked list structure for HUGE lists in which it is helpful to start from a pointer saved near the nodes desiring to be accessed at random. Imagine, within your list, having a waypoint pointer placed every 10 nodes (change "spacing" value to see what works best for you). When accessing a node, the selector pointer jumps to the nearest waypoint pointer before iterating over to the node of the desired index.
*/

#include <irrlicht.h>

#ifndef _MEGA_LISTIX_H_
#define _MEGA_LISTIX_H_

namespace irr
{
namespace core
{

	template<class Unk>
	class MListixNode
	{
	public:
		MListixNode<Unk>* Prior; // the node preceding this one
		MListixNode<Unk>* Post; // the node after this one
		
		Unk ThisItem; // the item stored here

		//! constructor
		MListixNode( Unk item )
		{
			ThisItem = item;
			Prior = 0;
			Post = 0;
		}

		//! deconstructor
		void deleteMe() { delete this; }
	};

	template<class Unk>
	class MListixIter
	{
		irr::s64 pos;
		MListixNode<Unk>* node;

	public:
		MListixIter()
		{
			pos = 0;
			node = 0;
		}

		bool hasNode()
		{
			return !(node == 0);
		}

		irr::s64 getPos()
		{
			return pos;
		}

		//! Set Node
		/* Sets the node to the given one. You shouldn't need to use
		this. Note: The old node is not deleted in the process. */
		void setNode( MListixNode<Unk>* new_node )
		{
			node = new_node;
		}

		MListixNode<Unk>* getNode()
		{
			return node;
		}

		//! Goto Node
		/* Moves this iterator to the node whose index is given.
		\param index - The index to go to.
		\param relative - The iterator whose node is the starting point for
		this iterator.
		\return - True if the node was reached. */
		bool goTo( irr::s64 index, MListixIter<Unk>* relative )
		{
			if ( !relative )
				return false;

			pos = relative->getPos();
			node = relative->getNode();

			if ( !node )
				return false;

			if ( index > pos )
			{
				while ( node && index > pos )
				{
					if ( node->Post )
					{
						node = node->Post;
						pos++;
					} else {
						return false;
					}
				}

			} else if ( index < pos )
			{
				while ( node && index < pos )
				{
					if ( node->Prior )
					{
						node = node->Prior;
						pos--;
					} else {
						return false;
					}
				}
			}

			return true;
		}

		//! Adjust position
		/* Adjusts this iterator's position based on the index of the
		most recently inserted node, given by the parameter.
		YOU SHOULD NOT USE THIS FUNCTION!
		\return - True if the adjustment could be made. */
		bool adjust( irr::s64 insert, bool removal=false )
		{
			if ( !node )
				return false;

			if ( insert <= pos )
			{
				if ( !removal )
				{
					pos++;
				}
				else {
					pos--;

					if ( !node )
						return false;
				}
			}

			return true;
		}

		//! Adjust Position Retain
		/* Same as adjust() but it attempts to retain the position of the
		iterator in the list.
		\param insert - The location of the new node.
		\param removal - If this was actually the removal of a node.
		\return - True if the node was reached. */
		bool adjustPosRetain( irr::s64 insert, bool removal=false )
		{
			if ( !node )
				return false;

			bool safe = true;

			if ( insert <= pos )
			{
				if ( !removal )
					pos++;
				else
					pos--;

				safe = goTo( (pos - 1), this );
			}

			return safe;
		}
	};

	template<class Unk>
	class MegaListix
	{
	private:
		irr::s64 size; // list size

		// Accessor spacing - keep constant for optimization purposes
		/* You need to optimize this for your machine, but that requires
		speed testing runs. */
		static const irr::s64 spacing=10;

		// List of accessors points
		irr::core::array<MListixIter<Unk>*> waypoints;

		// The main accessor
		MListixIter<Unk>* selector;

	protected:
		void adjustIterators( irr::s64 insert, bool removal=false )
		{
			bool keep = true;

			for ( irr::s32 i=(irr::s32)waypoints.size()-1; (irr::s32)i >= 0; i-- )
			{
				keep = waypoints[(irr::u32)i]->adjustPosRetain( insert, removal );

				if ( !keep )
				{
					waypoints.erase(i);
				}

				keep = true;
			}
		}

	public:
		MegaListix()
		{
			size = 0;
			selector = new MListixIter<Unk>();
			waypoints.push_back( new MListixIter<Unk>() );
		}

		~MegaListix()
		{
			clear();
		}

		irr::s64 Size()
		{
			return size;
		}

		void spawnIterators()
		{
			// Correct for a missing first one
			if ( waypoints[0]->getPos() != 0 )
			{
				// Create this first one
				waypoints.push_front( new MListixIter<Unk>() );

				// Assign it its rightful position
				waypoints[0]->goTo( 0, selector );
			}

			while (
				size > waypoints[ waypoints.size()-1 ]->getPos() + spacing
				)
			{
				// create a new accessor
				waypoints.push_back( new MListixIter<Unk>() );

				// Set it to the new position
				waypoints[ waypoints.size()-1 ]->goTo(
					waypoints[ waypoints.size()-2 ]->getPos() + spacing,
					waypoints[ waypoints.size()-2 ]
					);
			}
		}

			//! Goto Node
		/* Attempts to bring the main iterator (selector) to the node whose
		index is given.
		\return - True if the node could be reached. */
		bool goTo( irr::s64 index )
		{
			if ( size == 0 || index > size || waypoints.size() == 0 )
				return false;

			if ( index < 0 )
				index += size;

			if ( index < 0 )
				return false;

			/* Shortcut - Use the current node if it is closer than the
			nearest of the waypoints. */
			if ( selector->getPos() % spacing <= spacing / 2 )
				return selector->goTo( index, selector );


			MListixIter<Unk>* lastIter;
			
			if ( size >= 1 )
				lastIter = waypoints[0];

			// find the nearest
			for ( irr::u32 s=1; s < waypoints.size(); s++ )
			{
				/* Go to the range within which the desired point lies. */
				if ( s+1 < waypoints.size() && index > waypoints[s]->getPos() )
				{
					lastIter = waypoints[s];
					continue;
				}

				/* If the index comes before this node, find which node
				is closest to it. */
				if ( index <= waypoints[s]->getPos() )
				{
					// closer than the node at the start of this range?
					if (
						irr::core::abs_( selector->getPos() - index )
						<=
						irr::core::abs_( waypoints[s-1]->getPos() - index )
						)
					{
						// closer than the node at the end of this range?
						if (
							irr::core::abs_( selector->getPos() - index )
							<=
							irr::core::abs_( waypoints[s]->getPos() - index )
							)
						{
						} else {
							/* selector not closer than the node at the end,
							so use the one at the end */
							return selector->goTo( index, waypoints[s] );
						}

					} else {
						/* selector not closer than the node at the start,
						so use the one at the start */
						return selector->goTo( index, waypoints[s-1] );
					}
				}

				lastIter = waypoints[s];
			}

			// last resort
			return selector->goTo( index, lastIter );
		}

			//! Insert
		/* Inserts the object at the given position.
		\return - True if the object could be inserted. */
		bool insert( irr::s64 index, Unk item )
		{
			// two causes of failure - index is to negative or > size
			if ( index < 0 )
				index += size;

			// First node
			if ( size == 0 )
			{
				waypoints[0]->setNode( new MListixNode<Unk>( item ) );
				selector->setNode( waypoints[0]->getNode() );
				size++; // account for the newly added node

				return true;
			}

			if ( index < 0 || index > size ) // still too small or too big
				return false;

			// Now go to the node
			goTo( index );

			MListixNode<Unk>* postNode;

			if ( index == 0 ) // insertion at the beginning
			{
				selector->getNode()->Prior = new MListixNode<Unk>( item );

				// tie new prior node to this node
				selector->getNode()->Prior->Post = selector->getNode();

			} else {
				if ( index == size ) // insertion at the end
				{
					selector->getNode()->Post = new MListixNode<Unk>( item );

					// tie new post node to this node
					selector->getNode()->Post->Prior = selector->getNode();
				}
				else
				{
					// save the next node (it will come after the new one)
					postNode = selector->getNode()->Post;

					// create the new node as the next one
					selector->getNode()->Post = new MListixNode<Unk>( item );

					// tie the new next node back to this one
					selector->getNode()->Post->Prior = selector->getNode();

					// tie the new next node to the old post node
					selector->getNode()->Post->Post = postNode;
					postNode->Prior = selector->getNode()->Post;
				}
			}

			size++; // account for the newly added node


			adjustIterators( index );
			spawnIterators();

			return true;
		}

		void push_back( Unk item )
		{
			insert( size, item );
		}

		void push_front( Unk item )
		{
			insert( 0, item );
		}

		Unk& get( irr::s64 index )
		{
			bool can = goTo(index);

			_IRR_DEBUG_BREAK_IF( !can );

			return selector->getNode()->ThisItem;
		}

		Unk& operator[] ( irr::s64 index )
		{
			return get( index );
		}

		Unk copyOf( irr::s64 index )
		{
			if ( !goTo(index) )
				return (Unk)0;

			return selector->getNode()->ThisItem;
		}

		void erase( irr::s64 index )
		{
			if ( !goTo( index ) )
				return;

			/* For this operation, there are three key scenarios:
			1) the node is at one of the waypoints,
			2) the node is before one of the waypoints,
			3) the node is after one of the waypoints.
			For the last two cases, a safe deletion can occur without adjusting
			the affected selector. For the first case, we have to reset it. */

			// Variables
			MListixNode<Unk>* mid;
			MListixIter<Unk>** affected;

			// For convenience of code
			mid = selector->getNode();

			if ( mid->Prior )
			{
				if ( mid->Post )
				{
					mid->Prior->Post = mid->Post;
					mid->Post->Prior = mid->Prior;
				} else {
					mid->Prior->Post = 0;
				}
			}

			if ( index % spacing == 0 ) // on an iterator
			{
				// Find the affected iterator
				affected = &(waypoints[ (irr::u32) index/spacing ]);

				// Set its new node
				if ( mid->Post )
				{
					(*affected)->setNode( mid->Post );
				}
			}

			adjustIterators( index, true );
			spawnIterators();
		}

		void clear()
		{
			for ( irr::s64 i = size-1; i >= 0; i-- )
			{
				erase(i);
			}
		}

	};

} // end namespace core
} // end namespace irr

#endif // #ifndef _MEGA_LLISTIX_H_
