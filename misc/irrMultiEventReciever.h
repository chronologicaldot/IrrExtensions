/*
Created by Nicolaus Anderson, 2012

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

#ifndef IRR_MULTI_EVENT_RECEIVER_DISPATCHER_H
#define IRR_MULTI_EVENT_RECEIVER_DISPATCHER_H

#include <IEventReceiver.h>

namespace irr {

class irrMultiEventReceiver : public irr::IEventReceiver
{
protected:
	irr::core::list<irr::IEventReceiver*> receivers;
	irr::core::list<irr::IEventReceiver*>::Iterator iter;

public:

	irrMultiEventReceiver()
	{
	}
	
	bool OnEvent( const irr::SEvent& event )
	{
		for ( iter = receivers.begin(); iter != receivers.end(); iter++ )
		{
			if ( (*iter)->OnEvent(event) )
				break;
		}
		
		return finished;
	}
	
	void registerReceiver( irr::IEventReceiver* rec )
	{
		receivers.push_back( rec );
	}

	bool removeReceiver( irr::IEventReceiver* rec )
	{
		for ( iter = receivers.begin(); iter != receivers.end(); iter++ )
		{
			if ( *iter == rec )
			{
				receivers.erase( iter );
				return true;
			}
		}

		return false;
	}

	irr::IEventReceiver* getReceiver( irr::u32 index )
	{
		if ( index > receivers.size() )
			return 0;

		irr::u32 i = 0;
		iter = receivers.begin();
		for ( ; i < index; iter++, i++ );

		return *iter;
	}

};

}

#endif
