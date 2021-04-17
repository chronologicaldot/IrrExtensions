/*
Project name:
GUI Function Fire

Created by Nicolaus Anderson, 2012
For terms and conditions, see GUIfunctionFire.h
*/

#include "GUIFunctionFire.h"

irr::s32 GUIFunctionFire::findFunctionByName( irr::core::stringc funcName )
{
	for ( funcs.resetMidix(); !funcs.iter(true,0) ; )
	{
		if ( ((IDedFunc *)(funcs.getCurrItem()))->matches( funcName ) )
			return funcs.getCurrLoc();
	}

	return -1;
}

void GUIFunctionFire::registerFunctionClass( IDedFunc * funcclass )
{
	/* First, find if the function class with that name exists already.
	If so, replace it. */
	if ( findFunctionByName( funcclass->getName() ) != -1 )
	{
		IDedFunc* f = *(funcs.getCurrItem());
		funcs.replace( funcs.getCurrLoc(), funcclass );
		delete f;
	} else {
		funcs.push_back( funcclass );
	}
}

void GUIFunctionFire::giveToFunction( irr::core::stringc funcName, irr::s32 gui_elem_id )
{
	irr::s32 index;

	index = findFunctionByName( funcName );

	/* First, find if function class with that name exists. */
	if ( index == -1 )
	{
		// Create a dummy function
		funcs.push_back( new IDedFunc(funcName) );
	}

	((IDedFunc*)funcs[index])->addRespondant( gui_elem_id );
}

bool GUIFunctionFire::callFunctionWithRespondant( irr::s32 id )
{
	irr::s32 func_loc = 0;

	for ( ; func_loc < funcs.Size(); func_loc++ )
	{
		if ( funcs[func_loc]->hasRespondant( id ) )
		{
			/* Activate the call function. If it returned false, it may be
			deferring action to another call function. */
			if ( funcs[func_loc]->call() )
				return true;
		}
	}

	return false;
}

bool GUIFunctionFire::OnEvent( const irr::SEvent &event )
{
	switch( event.EventType )
	{
	case irr::EET_GUI_EVENT:

		// The caller's function will be identified based on their ID
		return callFunctionWithRespondant( event.GUIEvent.Caller->getID() );

	default: break;
	}

	return false;
}