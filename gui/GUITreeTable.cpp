// (c) 2014 Nic Anderson

#ifndef GUITREETABLE_CPP
#define GUITREETABLE_CPP

#include "GUITreeTable.h"
#include <IGUIEnvironment.h>

using core::clamp;

namespace irr {
namespace gui {

GUITreeTableNode::GUITreeTableNode( IGUIWindow* pWindow, irrTreeNode* pNode )
	: window( pWindow )
	, node( pNode )
{
	// Creates a bug for some reason
	//if ( window )
	//	window->grab();
}

GUITreeTableNode::GUITreeTableNode( const GUITreeTableNode& pOther )
	: window( pOther.window )
	, node( pOther.node )
{
	// Is this needed or did it not matter?
	//if ( window )
	//	window->grab();
}

GUITreeTableNode::~GUITreeTableNode()
{
	// Creates a bug for some reason
	//if ( window )
	//	window->drop();
}

IGUIWindow* GUITreeTableNode::getWindow()
{
	return window;
}

irrTreeNode* GUITreeTableNode::getTreeNode()
{
	return node;
}

irrTreeElement* GUITreeTableNode::getTreeNodeElement()
{
	if ( node )
		return node->element;
	return 0;
}

void GUITreeTableNode::setWindow( IGUIWindow* pWindow )
{
	// Creates a bug for some reason
	//if ( pWindow )
	//	pWindow->grab();

	//if ( window )
	//	window->drop();

	window = pWindow;
}

void GUITreeTableNode::setTreeNode( irrTreeNode* pNode )
{
	node = pNode;
}

void GUITreeTableNode::setTreeNodeElem( irrTreeElement* pElement )
{
	if ( node )
		node->element = pElement;
}

bool GUITreeTableNode::isAncestorOf( GUITreeTableNode* pNode )
{
	irrTreeNode* childNode = pNode->getTreeNode();
	while ( childNode->getParent() )
	{
		childNode = childNode->getParent();
		if ( childNode == node )
			return true;
	}
	return false;
}

bool GUITreeTableNode::operator== ( const GUITreeTableNode& other )
{
	return ( node == other.node ) && ( window == other.window );
}


/* &&&&&&&&&&&&&&& Tree Table &&&&&&&&&&&&&& */

GUITreeTable::GUITreeTable( IGUIEnvironment* pEnvironment,
							IGUIElement* pParent, recti pRect, s32 id )
	: IGUIElement( EGUIET_ELEMENT, pEnvironment, pParent, id, pRect )
	, treeRoot(0)
	, elementFactory(0)
	, selectedNode(0)
	, nodeMenu(0)
	, activity( EGTTABLEA_None )
	, lastEvent( EGTTE_None )
	, focusOnHover( false )
	, allowLinkingToParent( false )
	, canSelectedNodeLoseFocus( false )
	, lineThickness(2)
	, nodeWindowSize(0,0,150,100)
	, linkColor1(0xffcdcdcd)
	, linkColor2(0xffdddddd)
	, selectedHighlightColor(0x9900ffde)
{
#ifdef _COMPILE_GUI_TREE_TABLE_NICE_
	backgroundColor2 = Environment->getSkin()->getColor(EGDC_WINDOW);
	// Darken
	backgroundColor2.setRed( clamp(backgroundColor2.getRed() - 20, (u32)0, (u32)255) );
	backgroundColor2.setGreen( clamp(backgroundColor2.getGreen() - 20, (u32)0, (u32)255) );
	backgroundColor2.setBlue( clamp(backgroundColor2.getBlue() - 20, (u32)0, (u32)255) );
#endif
}

GUITreeTable::~GUITreeTable()
{
	// Kill the node menu grabbed on its construction
	nodeMenu->drop();
}

IGUIWindow* GUITreeTable::addNode( irrTreeElement* pElement )
{
	vector2di winHalfSize = nodeWindowSize.LowerRightCorner/2;
	IGUIWindow* window = Environment->addWindow(
										//recti(lastMousePos-winHalfSize,
										//		lastMousePos+winHalfSize),
										recti(firstMousePos-winHalfSize,
												firstMousePos+winHalfSize),
										false, // modal?
										0, // text?
										this//, // parent
										//++windowIds
										);

	window->grab(); /* Bugfix? Requires more looking into.
					The program crashes when the window is not grabbed here. */
	window->setDraggable(true);
	window->getMaximizeButton()->remove();
	window->getMinimizeButton()->remove();
	window->getCloseButton()->remove();

	/* Forces the parent to respond first to window selection,
	but also forces the table to send GUI events to the windows
	by returning false for every unhandled event. */
	window->setSubElement(true);
	
	if ( elementFactory )
	{
		if ( !pElement )
			pElement = elementFactory->buildElementOfWindow( window );
		else
			elementFactory->buildElementOfWindow( window );
	}
	irrTreeNode* node = new irrTreeNode( /*windowIds*/ 0, 0 );
	if ( pElement )
		node->setElem( pElement );

	nodeList.push_back( GUITreeTableNode( window, node ) );

	Environment->setFocus(window);

	return window;
}

void GUITreeTable::removeSelectedNode()
{
	if ( ! selectedNode ) return;

	activity = EGTTABLEA_None;
	canSelectedNodeLoseFocus = true;

	list<GUITreeTableNode>::Iterator i = nodeList.begin();
	irrTreeNode* removableNode;
	for ( ; i != nodeList.end(); ++i )
	{
		if ( *selectedNode == *i )
		{
			removableNode = i->getTreeNode();
			i->setTreeNode(0);
			delete removableNode;
			removeChild( i->getWindow() );
			i = nodeList.erase(i);
			selectedNode = 0;

			Environment->setFocus(this);

			break;
		}
	}
}

void GUITreeTable::removeNodeWithTreeElem( irrTreeElement* pElement )
{
	activity = EGTTABLEA_None;
	canSelectedNodeLoseFocus = true;

	list<GUITreeTableNode>::Iterator i = nodeList.begin();
	irrTreeNode* removableNode;
	for ( ; i != nodeList.end(); ++i )
	{
		if ( (*i).getTreeNodeElement() == pElement )
		{
			removableNode = i->getTreeNode();
			i->setTreeNode(0);
			delete removableNode;
			removeChild( i->getWindow() );
			i = nodeList.erase(i);
			selectedNode = 0;
			break;
		}
	}
}

IGUIWindow* GUITreeTable::duplicateSelectedNode()
{
	GUITreeTableNode* node = getSelectedNode();
	IGUIWindow* returnWindow = 0;
	if ( node )
	{
		if ( elementFactory )
		{
			returnWindow = addNode( elementFactory->buildElementDuplicateOf( node->getTreeNode()->getElem() ) );
			elementFactory->amendDuplicateWindow( returnWindow );
		}
	}
	return returnWindow;
}

void GUITreeTable::clearAll(bool pClearWindows)
{
	activity = EGTTABLEA_None;
	canSelectedNodeLoseFocus = true;

	list<GUITreeTableNode>::Iterator i = nodeList.begin();
	if ( pClearWindows )
	{
		for ( ; i != nodeList.end(); ++i )
		{
			i->getWindow()->remove();
		}
	}
	nodeList.clear();
	treeRoot = 0;
	selectedNode = 0;
}

void GUITreeTable::clearAllTrees()
{
	activity = EGTTABLEA_None;
	canSelectedNodeLoseFocus = true;

	irr::core::array<irrTreeNode*>* kids;
	list<GUITreeTableNode>::Iterator i = nodeList.begin();
	for ( ; i != nodeList.end(); ++i )
	{
		kids = &( (*i).getTreeNode()->children );
		while ( kids->size() )
			kids->erase( kids->size() - 1 );
	}
}

void GUITreeTable::setElementFactory( GUITreeTableElementFactory* pFactory )
{
	elementFactory = pFactory;
}

void GUITreeTable::setFocusOnHover( bool pFocus )
{
	focusOnHover = pFocus;
}

void GUITreeTable::setLinkLineThickness( s32 pThickness )
{
	lineThickness = pThickness;
}

void GUITreeTable::setNodeWindowRect( recti pRect )
{
	nodeWindowSize = pRect;
	nodeWindowSize.repair();
}

recti GUITreeTable::getNodeWindowRect()
{
	return nodeWindowSize;
}

void GUITreeTable::setLinkColor1( irr::video::SColor pColor )
{
	linkColor1 = pColor;
}

void GUITreeTable::setLinkColor2( irr::video::SColor pColor )
{
	linkColor2 = pColor;
}

void GUITreeTable::setSelectedHighlightColor( irr::video::SColor pColor )
{
	selectedHighlightColor = pColor;
}

EGUITreeTableEvent GUITreeTable::getLastEventState()
{
	return lastEvent;
}

IGUIWindow* GUITreeTable::getSelectedNodeWindow()
{
	if ( selectedNode )
		return selectedNode->getWindow();
	return 0;
}

GUITreeTableNode* GUITreeTable::getSelectedNode()
{
	return selectedNode;
}

GUITreeTableNode* GUITreeTable::getTreeRootNode()
{
	list<GUITreeTableNode>::Iterator i = nodeList.begin();
	for ( ; i != nodeList.end(); ++i )
	{
		if ( i->getTreeNode() == treeRoot )
			return &(*i);
	}
	return 0;
}

void GUITreeTable::setTreeRootNode( GUITreeTableNode& pNewRoot )
{
	/* Verify that the node is in the node list (before we blindly set it) */
	list<GUITreeTableNode>::Iterator i = nodeList.begin();
	for ( ; i != nodeList.end(); ++i )
	{
		if ( *i == pNewRoot )
		{
			treeRoot = pNewRoot.getTreeNode();
			break;
		}
	}
}

void GUITreeTable::setTreeRoot( irrTreeNode* pNewRoot )
{
	/* Verify that the node is in the node list (before we blindly set it) */
	list<GUITreeTableNode>::Iterator i = nodeList.begin();
	for ( ; i != nodeList.end(); ++i )
	{
		if ( i->getTreeNode() == pNewRoot )
		{
			treeRoot = pNewRoot;
			break;
		}
	}
}

list<GUITreeTableNode>& GUITreeTable::getNodeList()
{
	return nodeList;
}

IGUIContextMenu* GUITreeTable::getNodeMenu( bool pUpdatePosition )
{
	if ( !nodeMenu )
	{
		nodeMenu = Environment->addContextMenu(
									//recti(0,0,175,175),
									recti(0,0,1,1),
									this,
									-1 );

		// Set the settings for the menu
		nodeMenu->setCloseHandling( ECMC_REMOVE );

		// Add items to the menu
		nodeMenu->addItem( L"Add node", (s32)EGTTABLE_NMCI_Add, false );
		nodeMenu->addItem( L"Delete", (s32)EGTTABLE_NMCI_Delete, false );
		nodeMenu->addItem( L"Duplicate", (s32)EGTTABLE_NMCI_Duplicate, false );
		nodeMenu->addItem( L"Link to Child", (s32)EGTTABLE_NMCI_LinkToChild, false );
		nodeMenu->addItem( L"Link to Parent", (s32)EGTTABLE_NMCI_LinkToParent, false );
		nodeMenu->addItem( L"Break from Child", (s32)EGTTABLE_NMCI_BreakFromChild, false );
		nodeMenu->addItem( L"Break from Parent", (s32)EGTTABLE_NMCI_BreakFromParent, false );
		nodeMenu->addItem( L"Make root", (s32)EGTTABLE_NMCI_MakeRoot, false );

		// Debug test
		//assert( nodeMenu->findItemWithCommandId( EGTTABLE_NMCI_Add )
		//		== EGTTABLE_NMCI_Add );

		// Cancel out the removal process later (when the node calls IGUIElement::remove() )
		nodeMenu->grab();
	}
	if ( pUpdatePosition )
		nodeMenu->setRelativePosition(
					lastMousePos
					- vector2di(0, nodeMenu->getAbsoluteClippingRect().getHeight()/2)
					);

	return nodeMenu;
}

/* NOTE: You need to be careful with how you are using this thing.
The context menu itself will already close when it loses focus.
You don't have to set the focus for it. */
void GUITreeTable::hideNodeMenu()
{
	nodeMenu->remove();
	//canSelectedNodeLoseFocus = true;
}

bool GUITreeTable::OnEvent( const SEvent& event )
{
	if ( isEnabled() && isVisible() )
	switch ( event.EventType )
	{
	case irr::EET_MOUSE_INPUT_EVENT:
		if ( handleMouseEvent( event ) )
			return true;
		break;
	case EET_KEY_INPUT_EVENT:
		if ( handleKeyEvent( event ) )
			return true;
		break;
	case EET_GUI_EVENT:
		if ( handleGuiEvent( event ) )
			return true;
		break;
	default: break;
	}

	// Apparently no recent event, so clear event flag
	lastEvent = EGTTE_None;

	return IGUIElement::OnEvent(event);
}

void GUITreeTable::sendEvent()
{
	if ( !Parent ) return;

	SEvent newEvent;
	newEvent.EventType = EET_GUI_EVENT;
	newEvent.GUIEvent.Caller = this;
	newEvent.GUIEvent.EventType = (EGUI_EVENT_TYPE)(EGET_COUNT+1);
	Parent->OnEvent(newEvent);
}

bool GUITreeTable::handleMouseEvent( const SEvent& event )
{
	lastMousePos.set( event.MouseInput.X, event.MouseInput.Y );

	/* Note: The check on the AbsoluteClippingRect has been removed since
	this is done by the engine(??). It can be added again if need be. */

	switch ( event.MouseInput.Event )
	{
		/* Node selection is done regardless of the window focus. */
	case EMIE_LMOUSE_PRESSED_DOWN:
		if ( activity == EGTTABLEA_Scroll ) // Was accidentally an assignment
		{
			activity = EGTTABLEA_None;
			return true;
		}
		break;

	case EMIE_MMOUSE_PRESSED_DOWN:
		if ( isPointInside( lastMousePos ) )
		{
			// Steal focus
			if ( Environment->getFocus() != this
				&& ! isMyChild(Environment->getFocus()) )
			{
				Environment->setFocus(this);
				lastEvent = EGTTE_NoSelectNode;
				sendEvent();
			}
			// Node menu automatically closes
			activity = EGTTABLEA_Scroll;
			firstMousePos = lastMousePos;
			return true;
		}
		break;

	case EMIE_MMOUSE_LEFT_UP:
		if ( activity == EGTTABLEA_Scroll )
		{
			activity = EGTTABLEA_None;
			return true;
		}
		break;

	case EMIE_MOUSE_MOVED:
		if ( activity == EGTTABLEA_Scroll )
		{
			moveChildrenBy(lastMousePos - firstMousePos);
			firstMousePos = lastMousePos;
			return true;
		}
		break;

	case EMIE_RMOUSE_PRESSED_DOWN:
		activity = EGTTABLEA_None;
		break;

	case EMIE_RMOUSE_LEFT_UP:
		if ( activity == EGTTABLEA_None
			&& ( Environment->getFocus() == this || isMyChild( Environment->getFocus() ) )
			)
		{
			activity = EGTTABLEA_UsingContextMenu; // VERY important

			if ( getNodeMenu()->getParent() != this )
				addChild( getNodeMenu() );

			//canSelectedNodeLoseFocus = false; // MUST come before focus setting
			Environment->setFocus(getNodeMenu(true));

			if ( selectedNode )
			{
				// Set the node menu properties for this specific node
				nodeMenu->setItemEnabled( EGTTABLE_NMCI_Add, false );
				nodeMenu->setItemEnabled( EGTTABLE_NMCI_Delete, true );
				nodeMenu->setItemEnabled( EGTTABLE_NMCI_Duplicate, true );
				nodeMenu->setItemEnabled( EGTTABLE_NMCI_LinkToChild, true );
				nodeMenu->setItemEnabled( EGTTABLE_NMCI_LinkToParent, true );
				nodeMenu->setItemEnabled( EGTTABLE_NMCI_BreakFromChild,
							(selectedNode->getTreeNode()->children.size() != 0 ) );
				nodeMenu->setItemEnabled( EGTTABLE_NMCI_BreakFromParent,
							(selectedNode->getTreeNode()->parent != 0 )
							);
				nodeMenu->setItemEnabled( EGTTABLE_NMCI_MakeRoot, true );

			} else {
				// Set the node menu properties for this specific node
				nodeMenu->setItemEnabled( EGTTABLE_NMCI_Add, true );
				nodeMenu->setItemEnabled( EGTTABLE_NMCI_Delete, false );
				nodeMenu->setItemEnabled( EGTTABLE_NMCI_Duplicate, false );
				nodeMenu->setItemEnabled( EGTTABLE_NMCI_LinkToChild, false );
				nodeMenu->setItemEnabled( EGTTABLE_NMCI_LinkToParent, false );
				nodeMenu->setItemEnabled( EGTTABLE_NMCI_BreakFromChild, false );
				nodeMenu->setItemEnabled( EGTTABLE_NMCI_BreakFromParent, false );
				nodeMenu->setItemEnabled( EGTTABLE_NMCI_MakeRoot, false );
			}
			lastEvent = EGTTE_OpenNodeMenu;
			sendEvent();
			return true;
		}
		else {
				// Kill all activity
			activity = EGTTABLEA_None;
			if ( isPointInside( lastMousePos ) )
			{
				selectedNode = getNodeAt( lastMousePos );
				// Don't care about the window
				if ( selectedNode )
					Environment->setFocus( selectedNode->getWindow() );
				return true;
			}
		}
		break;

	default:
		// Make position correction for allowing adding nodes here
		firstMousePos = lastMousePos;
		break;
	}

	return false;
}

bool GUITreeTable::handleKeyEvent( const SEvent& event )
{
	// Don't handle events when the key is not pressed
	if ( ! event.KeyInput.PressedDown )
		return false;

	/* Selected-node-specific events */
	if ( selectedNode && Environment->getFocus() == selectedNode->getWindow() )
	{
		switch ( event.KeyInput.Key )
		{
		case KEY_DELETE:
			lastEvent = EGTTE_RemoveNode;
			sendEvent();
			removeSelectedNode();
			return true;

		case KEY_KEY_D:
			lastEvent = EGTTE_DuplicateNode;
			sendEvent();
			duplicateSelectedNode();
			return true;

		case KEY_UP:
			selectedNode->getWindow()->move( vector2di(0,-30) );
			return true;

		case KEY_DOWN:
			selectedNode->getWindow()->move( vector2di(0,30) );
			return true;

		case KEY_RIGHT:
			selectedNode->getWindow()->move( vector2di(30,0) );
			return true;

		case KEY_LEFT:
			selectedNode->getWindow()->move( vector2di(-30,0) );
			return true;

		default: break;
		}
	}

	if ( Environment->getFocus() == this )
	switch( event.KeyInput.Key )
	{
	case KEY_ADD: // by the Numpad
	case KEY_PLUS: // near the backspace
		lastEvent = EGTTE_AddNode;
		sendEvent();
		addNode();
		return true;

	case KEY_UP:
		moveChildrenBy( vector2di(0,100) );
		return true;

	case KEY_DOWN:
		moveChildrenBy( vector2di(0,-100) );
		return true;

	case KEY_RIGHT:
		moveChildrenBy( vector2di(-100,0) );
		return true;

	case KEY_LEFT:
		moveChildrenBy( vector2di(100,0) );
		return true;

	default: break;
	}
	return false;
}

bool GUITreeTable::handleGuiEvent( const SEvent& event )
{
	GUITreeTableNode* tableNode;

	switch ( event.GUIEvent.EventType )
	{
	case EGET_ELEMENT_FOCUS_LOST:
		/* Lose the selection only if the mouse is outside of the element. */
		/* Note that the menu passes back the focus-lost event BEFORE it removes
		itself from the parent. */
		if ( selectedNode && selectedNode->getWindow() == event.GUIEvent.Caller )
		{
			switch ( activity )
			{
			case EGTTABLEA_UsingContextMenu: // requires selection be kept
			case EGTTABLEA_LinkingNodeToChild:
			case EGTTABLEA_LinkingNodeToParent:
			case EGTTABLEA_BreakFromChild:
				break;
			default:
				// Don't deselect if the mouse isn't even inside the table area
				if ( isPointInside( lastMousePos ) )
				{
					if ( ! selectedNode->getWindow()->isPointInside(lastMousePos) )
					{
						lastEvent = EGTTE_NoSelectNode;
						sendEvent();
						activity = EGTTABLEA_None;
						selectedNode = 0;
					}
				}
				break;
			}
		}
		else if ( event.GUIEvent.Caller == this )
		{
			activity = EGTTABLEA_None;
			canSelectedNodeLoseFocus = true;
		}
		// Caller is not this OR a child OR the menu (must be outside click)
		return false; // returning "true" prevents environment from changing focus

	case EGET_ELEMENT_FOCUSED:
		/* Focus events are called before mouse events. But rather than
		relying on the focus also being the selection, it is merely
		a means to make a selection. */
		{
			if ( event.GUIEvent.Caller == this && activity == EGTTABLEA_None )
			{
				if ( Parent )
					Parent->bringToFront(this);

				lastEvent = EGTTE_NoSelectNode;
				sendEvent();
				if ( selectedNode )
				{
					// Drop activity
					activity = EGTTABLEA_None;
					canSelectedNodeLoseFocus = true;
					selectedNode = 0;
				}
				break;
			}

			// Intercept selection
			if ( isMyChild( event.GUIEvent.Caller )
				&& event.GUIEvent.Caller->getType() == EGUIET_WINDOW )
			{
				switch( activity )
				{
				case EGTTABLEA_Scroll:
					activity = EGTTABLEA_None;
					break;

				case EGTTABLEA_LinkingNodeToChild:
					tableNode = getNodeWithWindow( (IGUIWindow*) event.GUIEvent.Caller );
					if ( tableNode && (tableNode != selectedNode)
						&& (allowLinkingToParent || !tableNode->isAncestorOf(selectedNode)) )
					{
						selectedNode->getTreeNode()->stealNode( *(tableNode->getTreeNode()) );
					}
					activity = EGTTABLEA_None;
					lastEvent = EGTTE_LinkToChild;
					canSelectedNodeLoseFocus = true;
					sendEvent();
					return true; // Prevent changing focus to clicked node

				case EGTTABLEA_LinkingNodeToParent:
					tableNode = getNodeWithWindow( (IGUIWindow*) event.GUIEvent.Caller );
					if ( tableNode && (tableNode != selectedNode)
						&& (allowLinkingToParent || !selectedNode->isAncestorOf(tableNode)) )
					{
						tableNode->getTreeNode()->stealNode( *(selectedNode->getTreeNode()) );
					}
					activity = EGTTABLEA_None;
					lastEvent = EGTTE_LinkToParent;
					canSelectedNodeLoseFocus = true;
					sendEvent();
					return true; // Prevent changing focus to clicked node

				case EGTTABLEA_BreakFromChild:
					tableNode = getNodeWithWindow( (IGUIWindow*) event.GUIEvent.Caller );
					if ( tableNode && (tableNode != selectedNode) )
					{
						if ( tableNode->getTreeNode()->parent == selectedNode->getTreeNode() )
						{
							tableNode->getTreeNode()->parent = 0;
							selectedNode->getTreeNode()->removeChild( tableNode->getTreeNode() );
						}
					}
					activity = EGTTABLEA_None;
					lastEvent = EGTTE_BreakFromChild;
					canSelectedNodeLoseFocus = true;
					sendEvent();
					return true; // Prevent changing focus to clicked node

				case EGTTABLEA_UsingContextMenu:
				default:
					// Select node with this window
					selectNodeWithWindow( (IGUIWindow*) event.GUIEvent.Caller );
					if ( selectedNode )
					{
						lastEvent = EGTTE_SelectNode;
					} else {
						lastEvent = EGTTE_NoSelectNode;
					}
					sendEvent();
					break;
				}

			}
		}
		break;

	case EGET_ELEMENT_HOVERED:
		if ( focusOnHover )
		if ( event.GUIEvent.Caller == this )
		{
			if ( Environment->getFocus() != this && !isMyChild(Environment->getFocus()) )
				Environment->setFocus(this);
			return true; /* Prevent hovering bugs from a parent setting this to focus
						 by handling the event. */
		}
		break;

	//case EGET_ELEMENT_CLOSED:
	//	/* Called by the menu when it loses focus! */
	//	break;

	case EGET_MENU_ITEM_SELECTED:
		if ( event.GUIEvent.Caller == nodeMenu )
		{
			// Find out what the menu selected
			return sendMenuEvent( (EGUITreeTableNodeMenuCommandId) nodeMenu->getItemCommandId( nodeMenu->getSelectedItem() ) );
		}
		break;

	default:
		break;
	}

	return false;
}

bool GUITreeTable::sendMenuEvent( EGUITreeTableNodeMenuCommandId pCommand )
{
	switch( pCommand )
	{
	case EGTTABLE_NMCI_Add:
		addNode();
		lastEvent = EGTTE_AddNode;
		sendEvent();
		activity = EGTTABLEA_None;
		return true;

	case EGTTABLE_NMCI_Delete:
		lastEvent = EGTTE_RemoveNode;
		sendEvent();
		removeSelectedNode();
		// OPTIMIZATION: refreshLinkCache();
		//activity = EGTTABLEA_None; // unnecessary
		return true;

	case EGTTABLE_NMCI_Duplicate:
		lastEvent = EGTTE_DuplicateNode;
		sendEvent();
		duplicateSelectedNode();
		activity = EGTTABLEA_None;
		return true;

	case EGTTABLE_NMCI_LinkToChild:
		activity = EGTTABLEA_LinkingNodeToChild;
		// OPTIMIZATION: refreshLinkCache();
		return true;

	case EGTTABLE_NMCI_LinkToParent:
		activity = EGTTABLEA_LinkingNodeToParent;
		// OPTIMIZATION: refreshLinkCache();
		return true;

	case EGTTABLE_NMCI_BreakFromChild:
		activity = EGTTABLEA_BreakFromChild;
		// OPTIMIZATION: refreshLinkCache();
		return true;

	case EGTTABLE_NMCI_BreakFromParent:
		if ( selectedNode )
		{
			if ( selectedNode->getTreeNode()->parent )
			{
				selectedNode->getTreeNode()->parent->removeChild(
					selectedNode->getTreeNode()
					);
				// OPTIMIZATION: refreshLinkCache();
			}
			selectedNode->getTreeNode()->parent = 0;
		}
		activity = EGTTABLEA_None;
		return true;

	case EGTTABLE_NMCI_MakeRoot:
		if ( selectedNode )
		{
			lastEvent = EGTTE_PreMakeRoot;
			sendEvent();
			treeRoot = selectedNode->getTreeNode();
			lastEvent = EGTTE_PostMakeRoot;
			sendEvent();
		}
		activity = EGTTABLEA_None;
		return true;

	default:
		activity = EGTTABLEA_None;
		break;
	}

	return false;
}

void GUITreeTable::draw()
{
	if ( !isVisible() )
		return;

	/* Children of this element are not drawn because that would require
	checking to see if such children were windows in the list of nodes.
	This slows the drawing process. Furthermore, there shouldn't be
	other types of GUI elements as direct children of this element anyways. */

	// Draw a background
#ifdef _COMPILE_GUI_TREE_TABLE_NICE_
	Environment->getVideoDriver()->draw2DRectangle( AbsoluteRect,
							Environment->getSkin()->getColor(EGDC_WINDOW),
							Environment->getSkin()->getColor(EGDC_WINDOW),
							backgroundColor2,
							backgroundColor2,
							&AbsoluteClippingRect
							);
#else
	Environment->getSkin()->draw2DRectangle(
							this,
							Environment->getSkin()->getColor(EGDC_WINDOW),
							AbsoluteRect,
							&AbsoluteClippingRect);
#endif

	/* Go through the list and draw all of the links between nodes
	for the tree.
	OPTIMIZATION:
	The links should be cached and drawn. Then the following process
	would only be done when the links change.
	The cached links can be moved (rather than rebuilt) when the middle
	mouse button is pressed and the mouse moves, thereby saving time
	in reconstruction.
	With all of that, the draw process is reduced to a single for-loop,
	which may not need to go through all of the nodes (since some may
	not even be connected). */
	u32 c;
	recti r;
	core::list<GUITreeTableNode>::Iterator nodeIter = nodeList.begin();
	GUITreeTableNode* ttnode;
	core::array<irrTreeNode*>* children;
	//for ( u32 n=0; n < nodeList.size(); n++ )
	for ( ; nodeIter != nodeList.end(); ++nodeIter )
	{
		if ( selectedNode && (nodeIter->getWindow() == selectedNode->getWindow()) )
		{
			// Draw a box around the selected node
			r = nodeIter->getWindow()->getAbsolutePosition();
			r.UpperLeftCorner -= vector2di(2,2);
			r.LowerRightCorner += vector2di(2,2);
			Environment->getSkin()->draw2DRectangle(
							this,
							selectedHighlightColor,
							r,
							&AbsoluteClippingRect
							);
		}

		children = &(nodeIter->getTreeNode()->children);
		for ( c=0; c < children->size(); c++ )
		{
			ttnode = findListNodeWithTreeNode( (*children)[c] );
			if ( ttnode )
			{
				drawLink( nodeIter->getWindow(), ttnode->getWindow() );
			}
		}
	}

	// Draw the windows
	IGUIElement::draw();
}

GUITreeTableNode* GUITreeTable::findListNodeWithTreeNode( irrTreeNode* pNode )
{
	list<GUITreeTableNode>::Iterator i = nodeList.begin();
	for ( ; i != nodeList.end(); ++i )
	{
		if ( i->getTreeNode() == pNode )
			return &(*i);
	}
	return 0;
}

void GUITreeTable::drawLink( IGUIElement* pFromElement, IGUIElement* pToElement )
{
	vector2di pt1 = pFromElement->getAbsolutePosition().getCenter();
	vector2di pt3 = pToElement->getAbsolutePosition().getCenter();

	s32 hf = (pt1.X < pt3.X) ? 1 : -1;
	s32 vf = (pt1.Y < pt3.Y) ? 1 : -1;
	recti line1( pt1.X, pt1.Y, pt3.X, pt1.Y+lineThickness*hf);
	recti line2( pt3.X-lineThickness*vf, pt1.Y+lineThickness*hf, pt3.X, pt3.Y );
	line1.repair();
	line2.repair();

	if ( hf > 0 )
	{
		Environment->getVideoDriver()->draw2DRectangle(
										line1,
										linkColor1,		// left up
										linkColor2,		// right up
										linkColor1,		// left down
										linkColor2,		// right down
										&AbsoluteClippingRect
										);
	} else {
		Environment->getVideoDriver()->draw2DRectangle(
										line1,
										linkColor2,		// left up
										linkColor1,		// right up
										linkColor2,		// left down
										linkColor1,		// right down
										&AbsoluteClippingRect
										);
	}

	if ( vf > 0 )
	{
		Environment->getVideoDriver()->draw2DRectangle(
										line2,
										linkColor1,		// left up
										linkColor1,		// right up
										linkColor2,		// left down
										linkColor2,		// right down
										&AbsoluteClippingRect
										);
	} else {
		Environment->getVideoDriver()->draw2DRectangle(
										line2,
										linkColor2,		// left up
										linkColor2,		// right up
										linkColor1,		// left down
										linkColor1,		// right down
										&AbsoluteClippingRect
										);
	}
}

GUITreeTableNode* GUITreeTable::getNodeAt( vector2di pPos )
{
	list<GUITreeTableNode>::Iterator i = nodeList.begin();
	for ( ; i != nodeList.end(); ++i )
	{
		if ( i->getWindow()->getAbsolutePosition().isPointInside(pPos) )
			return &(*i);
	}
	return 0;
}

void GUITreeTable::selectNodeAt( vector2di pPos )
{
	selectedNode = getNodeAt( pPos );
}

GUITreeTableNode* GUITreeTable::getNodeWithWindow( IGUIWindow* pWindow )
{
	list<GUITreeTableNode>::Iterator i = nodeList.begin();
	for ( ; i != nodeList.end(); ++i )
	{
		if ( i->getWindow() == pWindow )
		{
			return &(*i);
		}
	}
	return 0;
}

void GUITreeTable::selectNodeWithWindow( IGUIWindow* pWindow )
{
	selectedNode = getNodeWithWindow( pWindow );
}

void GUITreeTable::moveChildrenBy( vector2di pDistance )
{
	core::list<IGUIElement*>::Iterator c = Children.begin();
	for ( ; c != Children.end(); ++c )
	{
		(*c)->move( pDistance );
	}

	// OPTIMIZATION: refreshLinkCache();
}

recti GUITreeTable::getEnclosingRect()
{
	recti border, winRect;
	bool set = false;
	list<GUITreeTableNode>::Iterator i = nodeList.begin();
	for ( ; i != nodeList.end(); ++i )
	{
		winRect = i->getWindow()->getRelativePosition();
		if ( set )
		{
			border.addInternalPoint( winRect.UpperLeftCorner );
			border.addInternalPoint( winRect.LowerRightCorner );
		} else {
			border = winRect;
			set = true;
		}
	}
	return border;
}

}}

#endif
