/*
(c) Nicolaus Anderson
*/

#include "Searchable.h"

Searchable::Searchable()
{
}

Searchable::~Searchable()
{
	tags.clear();
}

void Searchable::addTag(const wchar_t* tag)
{
	tags.push_back(tag);
}

void Searchable::addTag( const irr::core::stringw& tag )
{
	tags.push_back(tag);
}

void Searchable::addTags( const irr::core::stringw& list )
{
	// parse the string
	irr::core::stringw next;

	irr::u32 s=0;
	for ( ; s < list.size(); s++ )
	{
		if ( list[s] == ',' )
		{
			if ( next.size() > 0 )
				tags.push_back(next);

			next.clear();
		}
		else {
			if ( list[s] != ' ' && list[s] != '\n' && list[s] != '\r' && list[s] != '\t' )
			{
				next.push_back(list[s]);
			}
		}
	}
}

irr::core::stringw Searchable::getTags()
{
	irr::core::stringw out;

	irr::u32 t=0;
	for ( ; t < tags.size(); t++ )
	{
		out.append( tags[t] );

		if ( t != tags.size() - 1 )
			out.append( irr::core::stringw(", ") );
	}

	return out;
}

bool Searchable::hasMatchingTagExactly( irr::core::stringw& tag )
{
	irr::u32 t=0;
	for ( ; t < tags.size(); t++ )
	{
		if ( tags[t].size() == tag.size() && tags[t].equalsn(tag, tag.size()) )
			return true;
	}

	return false;
}

bool Searchable::hasMatchingTagPartial( irr::core::stringw& tag )
{
	irr::u32 t=0;
	for ( ; t < tags.size(); t++ )
	{
		if ( tags[t].equalsn(tag, tag.size()) )
			return true;
	}

	return false;
}

void Searchable::shouldShow( bool setting )
{
}