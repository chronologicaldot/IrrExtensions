// (c) 2014 Nic Anderson

#ifndef GUITREETABLE_H
#define GUITREETABLE_H

#include <IGUIElement.h>
#include <irrTree.h>
#include <IGUIWindow.h>

/* Compile option: _COMPILE_GUI_TREE_TABLE_NICE_
Creates a gradient-background, but does not allow the skin
to control the drawing of the background.
It is also costly for rendering.
*/

namespace irr {
namespace gui {

using irr::core::clamp;
using irr::core::list;
using irr::core::vector2di;
using irr::core::recti;

enum EGUITreeTableActivity
{
	// No activity
	EGTTABLEA_None,

	/* When the user uses their middle mouse button, they can move all of
	the windows. However, they are not allowed to do other activities. */
	EGTTABLEA_Scroll,

	/* When a context menu is open, the user's mouse is monitored for
	whether or not the menu is closed. When the context menu is in use,
	other activities (such as node linking) cannot be done. However, those
	activites can be a consequence of using the menu.

	Since the context menu can close itself, this is used only
	to indicate if the activity should be monitored.
	If removed, the buttons of this menu will not be
	clickable. However, I will have to either
	a) reconstruct the menu every time (takes time)
	b) remove the main table GUI from being the parent until it is needed,
	which means I have to monitor the GUI event of the context menu losing
	focus, which shouldn't be too hard. */
	EGTTABLEA_UsingContextMenu,

	/* When a user has clicked a node and chosen to make it the parent
	of the next selected node. Note that, in order to make this succeed,
	the soon-to-be-child must be delinked from any previous parent. */
	EGTTABLEA_LinkingNodeToChild,

	/* When a user has clicked a node and chosen to make it the child
	of the next selected node. Note that, in order to make this succeed,
	the child must be delinked from any previous parent. */
	EGTTABLEA_LinkingNodeToParent,

	/* When a user has clicked a node and chosen to separate it from its
	child. Then the child must be selected. */
	EGTTABLEA_BreakFromChild,

	/* When the user decides to disconnect this node from its parent node.
	NOTE: This action occurs immediately and therefore does not exist as
	a regular action but is listed here for reference. */
	//EGTTABLEA_BreakFromParent,

	// Number of activities
	EGTTABLEA_COUNT
};

enum EGUITreeTableNodeMenuCommandId
{
	// Add a new node to the list
	EGTTABLE_NMCI_Add=0,

	// Delete the selected node
	EGTTABLE_NMCI_Delete,

	// Duplicate the selected node
	EGTTABLE_NMCI_Duplicate,

	// Link the selected node to a child
	EGTTABLE_NMCI_LinkToChild,

	// Link the selected node to a parent
	EGTTABLE_NMCI_LinkToParent,

	// Break link from the selected node to a selected child
	EGTTABLE_NMCI_BreakFromChild,

	// Break link from the selected node to its parent and vice versa
	EGTTABLE_NMCI_BreakFromParent,

	// Make this node the root node
	EGTTABLE_NMCI_MakeRoot,

	// Number of nodes
	EGTTABLE_NMCI_COUNT
};

/* The GUI event states, which can be checked with
GUITreeTable::getLastEventState(), and that way, we never
have to worry about extending EGUI_EVENT_TYPE for this
generic element. */
enum EGUITreeTableEvent
{
	EGTTE_None,
	EGTTE_SelectNode,
	EGTTE_NoSelectNode, // No node selected (or current selection deselected)
	//EGTTE_MoveNodes, // Not used - it would slow things down
	EGTTE_OpenNodeMenu,
	EGTTE_AddNode,
	EGTTE_DuplicateNode,
	EGTTE_RemoveNode,
	EGTTE_LinkToChild,
	EGTTE_LinkToParent,
	EGTTE_BreakFromChild,
	EGTTE_BreakFromParent,
	EGTTE_PreMakeRoot,
	EGTTE_PostMakeRoot,
	EGTTE_COUNT
};

/* Class GUI Tree Table Element Factory
Inherit this class and pass it to GUITreeTable to allow for creating
the nodes that go in the tree table.
*/
class GUITreeTableElementFactory
{
public:
	/* Meant for the user to take the window in the tree table and
	modify it as they please, returning the tree element that is
	represented by the window. */
	virtual irrTreeElement* buildElementOfWindow( IGUIWindow* pWindow )=0;

	/* Meant for duplicating the given tree element, especially
	when 1) there is no copy-constructor for the element or
	2) the programmer does not want the element to be directy copied. */
	virtual irrTreeElement* buildElementDuplicateOf( irrTreeElement* pElement )=0;

	/* Meant for when duplicating the selected node.
	If something extra needs to be done to the duplicate (or something needs
	to be updated concerning it), it can be done via this function.
	\param pWindow - The window of the duplicate node. */
	virtual void amendDuplicateWindow( IGUIWindow* pWindow )=0;
};

/* Class GUI Tree Table Node
Contains a pointer to the node itself and to the window representing it. */
class GUITreeTableNode
{
	IGUIWindow* window;
	irrTreeNode* node;

public:
	GUITreeTableNode( IGUIWindow* pWindow, irrTreeNode* pNode );
	GUITreeTableNode( const GUITreeTableNode& pOther );
	~GUITreeTableNode();

	IGUIWindow* getWindow();
	irrTreeNode* getTreeNode();
	irrTreeElement* getTreeNodeElement();
	void setWindow( IGUIWindow* pWindow );
	void setTreeNode( irrTreeNode* pNode );
	void setTreeNodeElem( irrTreeElement* pElement );
	bool isAncestorOf( GUITreeTableNode* pNode );

	bool operator== ( const GUITreeTableNode& other );
};

/* Class GUI Tree Table

This class is a table containing a tree and a list of nodes.
All of the nodes are in the list, including the root (which cannot be
deleted), but, with the exception of the root, none of the nodes need
to be in the tree.
The table is merely a control for a bunch of windows that compose the
tree. These windows can be used by the user to connect or disconnect
the tree. The windows also contain whatever the user wants.
*/
class GUITreeTable : public IGUIElement
{
	list<GUITreeTableNode> nodeList;
	irrTreeNode* treeRoot;
	GUITreeTableElementFactory* elementFactory;

	vector2di lastMousePos, firstMousePos;
	GUITreeTableNode* selectedNode;
	IGUIContextMenu* nodeMenu;

	EGUITreeTableActivity activity;
	EGUITreeTableEvent lastEvent;

	bool focusOnHover;
	bool allowLinkingToParent;

	bool canSelectedNodeLoseFocus;
	s32 lineThickness;
	recti nodeWindowSize;

	irr::video::SColor
				linkColor1,
				linkColor2,
				selectedHighlightColor;

#ifdef _COMPILE_GUI_TREE_TABLE_NICE_
	irr::video::SColor backgroundColor2;
#endif

public:
	GUITreeTable( IGUIEnvironment* pEnvironment,
					IGUIElement* pParent,
					recti pRect, s32 id=-1 );
	~GUITreeTable();

	/* Add a node at the last mouse location.
	\param pElement - The element to be contained in this node.
	\return - Window of the new node. */
	IGUIWindow* addNode( irrTreeElement* pElement=0 );

	void removeSelectedNode();
	void removeNodeWithTreeElem( irrTreeElement* pElement );

	/* Duplicate the selected node and add the duplicate at the last mouse location.
	\param pElement - The element to be contained in this node.
	\return - Window of the new node. */
	IGUIWindow* duplicateSelectedNode();

	void clearAll(bool pClearWindows=true); // kills everything
	void clearAllTrees(); // no clearList() because then all would have to die

	void setElementFactory( GUITreeTableElementFactory* pFactory );
	void setFocusOnHover( bool pFocus ); // (sets the Environment focus to this when hovering over it)
	void setLinkLineThickness( s32 pThickness );
	void setNodeWindowRect( recti pRect );
	recti getNodeWindowRect();
	void setLinkColor1( irr::video::SColor pColor );
	void setLinkColor2( irr::video::SColor pColor );
	void setSelectedHighlightColor( irr::video::SColor pColor );

	EGUITreeTableEvent getLastEventState();
	IGUIWindow* getSelectedNodeWindow();
	GUITreeTableNode* getSelectedNode();
	GUITreeTableNode* getTreeRootNode();

	/* Set the tree root by giving the node. */
	void setTreeRootNode( GUITreeTableNode& pNewRoot );
	void setTreeRoot( irrTreeNode* pNewRoot );
	list<GUITreeTableNode>& getNodeList();

protected:
	IGUIContextMenu* getNodeMenu( bool pUpdatePosition=false );
	inline void hideNodeMenu();

public:
	virtual bool OnEvent( const SEvent& event );
	bool sendMenuEvent( EGUITreeTableNodeMenuCommandId pCommand );

protected:
	void sendEvent();

public:
	virtual void draw();

	virtual const c8* getTypeName() const { return staticTypeName(); }
	static const c8* staticTypeName() const { return "treeTable"; }

protected:
	GUITreeTableNode* findListNodeWithTreeNode( irrTreeNode* pNode );
	void drawLink( IGUIElement* pFromElement, IGUIElement* pToElement );
	bool handleMouseEvent( const SEvent& event );
	bool handleKeyEvent( const SEvent& event );
	bool handleGuiEvent( const SEvent& event );

	GUITreeTableNode* getNodeAt( vector2di pPos );
	GUITreeTableNode* getNodeWithWindow( IGUIWindow* pWindow );

	//u32 getSelectedNodeIndex();
	void selectNodeAt( vector2di pPos );
	void selectNodeWithWindow( IGUIWindow* pWindow );

public:
	void moveChildrenBy( vector2di pDistance );
	recti getEnclosingRect(); // Relative (to parent) rectangle that wraps around all node windows
};

}}

#endif
