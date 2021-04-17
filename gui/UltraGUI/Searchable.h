/*
(c) Nicolaus Anderson
*/

#include <irrlicht.h>

#pragma once

//! Class Searchable
/*
An object that can be searched.
*/
class Searchable
{
protected:
	irr::core::array<irr::core::stringw> tags;

public:
	Searchable();
	~Searchable();

	virtual void addTag( const wchar_t* tag );

	virtual void addTag( const irr::core::stringw tag );
	
	/* Add comma-separated tags. */
	virtual void addTags( const irr::core::stringw list );

	/* Get tags */
	virtual irr::core::stringw getTags();

	/* Has a tag that matches exactly the given one. */
	virtual bool hasMatchingTagExactly( irr::core::stringw& tag );

	/* Has a partially matching tag. */
	virtual bool hasMatchingTagPartial( irr::core::stringw& tag );

	/* Called by search engines to indicate whether this object
	should show up in the search results. For listed GUI elements,
	this may mean being excluded from the drawing order. */
	virtual void shouldShow( bool setting );
};