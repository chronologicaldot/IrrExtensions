/*
Project name:
CSS Library

Created by Nicolaus Anderson, 2012
see CSSLibrary.h for terms and conditions.

Necessary functions.
*/

#include "CSSLibrary.h"


irr::s32 CSSLibrary::findCSSClass(irr::core::stringc name)
{
	if ( tags.Size() == 0 )
		return -1;

	CSSTagData * data = tags[0];

	tags.resetMidix();
	
	do {
		// the data must be a class and have the correct name
		if ( (data)->isClass() && (data)->getTagName() == name )
		{
			return (irr::s32)( tags.getCurrLoc() );
		}
	} while ( !tags.iter(true,&data) );

	return -1;
}


irr::s32 CSSLibrary::findCSSID( irr::core::stringc name )
{
	if ( tags.Size() == 0 )
		return -1;

	CSSTagData * data = tags[0];

	tags.resetMidix();

	do {
		// the data must be an ID and have the correct name
		if ( (data)->isID() && (data)->getTagName() == name )
		{
			return (irr::s32)( tags.getCurrLoc() );
		}
	} while ( !tags.iter(true,&data) );

	return -1;
}

irr::s32 CSSLibrary::findCSSRandom( irr::core::stringc name )
{
	if ( tags.Size() == 0 )
		return -1;

	CSSTagData * data = tags[0];

	tags.resetMidix();

	do {
		// the data merely must have the correct name
		if ( (data)->getTagName() == name )
		{
			return (irr::s32)( tags.getCurrLoc() );
		}
	} while ( !tags.iter(true,&data) );

	return -1;
}

bool CSSLibrary::ParseCSSFile( irr::io::path filename )
{
	irr::io::IReadFile* file = irrdev->getFileSystem()->createAndOpenFile( filename );

	bool result = ParseCSSFile( file );

	if ( file != 0 )
		file->drop();

	return result;
}


bool CSSLibrary::ParseCSSFile(irr::io::IReadFile * file)
{
	if ( file == 0 ) return false;

	// Create a reader for reading from the file
	irrVFreader reader( file ) ;

	// String for collecting what was returned from file
	irr::core::stringc token = " ";

	// Contents of a tag to be passed to the interpreting function
	irr::core::stringc contents;

	// Last tag data
	CSSTagType tag_type = CSSTYPE_NEITHER;
	irr::core::stringc tag_name;

	// Editable flag - if the tag has a body already
	bool editable = false;

	

	// Continue while the file returns something
	while ( token != "end_main" )
	{
		token = reader.readFromFile_sys_nse( _maxsystemchars, system_chars );

		// Escape from the loop if the next token indicates the end of the file
		if ( token == "end_main" ) break;


		// Determine what to do based on the type found in file
		if ( token == "." )
		{
			editable = true;

			// Assume this is a class
			tag_type = CSSTYPE_CLASS;

			// Next token should be the name
			tag_name = reader.readFromFile_sys_nse( _maxsystemchars, system_chars );
		}
		else if ( token == "#" )
		{
			editable = true;

			// Assume this is an ID
			tag_type = CSSTYPE_ID;

			// Next token should be the name
			tag_name = reader.readFromFile_sys_nse( _maxsystemchars, system_chars );
		}
		else if ( token == "{" )
		{
			// Destroy the old string
			contents = "";

			// CSS tag body - search for the rest
			while ( token != "end_main" && token != "}" )
			{
				if ( token != "{" ) // ignore the first character
					contents.append( token );
				token = reader.readFromFile(
					bodysyschars.size(),
					(irr::c8*)bodysyschars.c_str()
					);
				//token = reader.readFromFile_sys_nse( _maxsystemchars, system_chars );
			}

			// Should this be added to the current tag (or was it an accident)
			if ( editable )
			{
				editable = false;

				addStyle( contents, tag_name, tag_type );
			}
		}
		else if ( token == "/" )
		{
			/* This might need to change because it wastes a word to check
			if this is a comment. Then again, what else could it be? */

			// Check for * to indicate this is a comment
			token = reader.readFromFile_sys_nse( _maxsystemchars, system_chars );

			if ( token == "*" )
			{
				// Destroy the old string
				token = "";

				do {
					if ( token != "*" )
					{
						token = "";
					}
					token.append(
							reader.readFromFile_sys_nse( _maxsystemchars, system_chars )
						);
				} while ( token != "*/" );
			}
		}
		else if ( !editable )
		{
			// Unknown tag type
			tag_type = CSSTYPE_NEITHER;

			// Assume the token was the name
			tag_name = token;
		}
	}

	return true;
}
