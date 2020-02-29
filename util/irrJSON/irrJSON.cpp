/* (c) 2014 Nicolaus Anderson
License: zlib
*/

#ifndef IRRJSON_CPP
#define IRRJSON_CPP

#include "irrJSON.h"

namespace irr {
namespace io {

using core::string;

irrJSON::irrJSON( IFileSystem* pFileSystem )
	: fileSystem( pFileSystem )
	, delimChars( "{}:,;\"\'\\" )
	, skipChars( " \t\r\n" )
	, quoteDelimChars( "\"\'\\" )
	, reader(0)
	, accessor(0)
{
}

irrJSON::~irrJSON()
{
}

bool irrJSON::parseFile(path pPath, irrTreeNode*& pResultTree)
{
	IReadFile* file = fileSystem->createAndOpenFile( pPath );

	bool result = parseFile( file, pResultTree );

	if ( file != 0 )
		file->drop();

	return result;
}

bool irrJSON::parseFile( IReadFile* pFile, irrTreeNode*& pResultTree )
{
	if ( !pFile || !pFile->getSize() )
		return false;

	reader = new CharFileReader(pFile);
	accessor = new CharFileAccessor(reader);

	/*
	IMPORTANT NOTE:

	We want to be able to load JSON files that don't necessarily follow
	standard format. For example, they may have multiple root bodies.
	In that case, a single root is used that represents the file.

	JSONroot
	{
		node0{
			info ( is tree root )
			node1,
			node2,
			node3
		}
		node1 {
			info
		}
		node2 {
			info
		}
		node3 {
			info
		}
	}
	*/

	pResultTree = new irrTreeNode();
	pResultTree->setElem( new irrJSONElement("root") );

	while ( ! reader->atEOF() )
		getElements(pResultTree);
		//findAndStartBody(pResultTree);

	delete accessor;
	delete reader;

	return true;
}

inline string<c8> irrJSON::getToken( bool pInQuotes )
{
	if ( pInQuotes )
		return accessor->getToken(quoteDelimChars.c_str(), quoteDelimChars.size(), 0, 0 );

	/* Ironically, it doesn't matter if unicode-8 characters are grabbed
	here or ASCII characters are since only ASCII characters are the
	deliminators. */
	return accessor->getToken(delimChars.c_str(), delimChars.size(), skipChars.c_str(), skipChars.size());
}

void irrJSON::getElements( irrTreeNode* pNode )
{
	string<c8> token;
	bool findName = true;
	bool add = false; // (flag) append the token to name or value
		// quote types 1==' 2=="
	bool inQuote1 = false;
	bool inQuote2 = false;
	bool out;
	c8 temp;

	string<c8> name;
	string<c8> value;
	irrJSONElement* elem = (irrJSONElement*)(pNode->getElem());

	token = getToken();
	while ( ! reader->atEOF() && token[0] != '}' )
	{
		add = false;

		// Handle quotes situations
		if ( inQuote1 || inQuote2 )
		{
			switch ( token[0] )
			{
			case '\'':
				if ( !inQuote2 ) {
					inQuote1 = false;
				} else {
					add = true;
				}
				break;

			case '\"':
				if ( !inQuote1 ) {
					inQuote2 = false;
				} else {
					add = true;
				}
				break;

			case '\\': // Escape character
				{
					if ( reader->atEOF() )
						break;
					temp = reader->next();
					switch( temp )
					{
					case '\\': // backslash
						if ( findName )
							name.append('\\');
						else
							value.append('\\');
						break;
					case 'n': // new line (Windows)
						if ( findName )
							name.append('\n');
						else
							value.append('\n');
						break;
					case 'r': // new line / return (Linux)
						if ( findName )
							name.append('\r');
						else
							value.append('\r');
						break;
					case 't': // tab
						if ( findName )
							name.append('\t');
						else
							value.append('\t');
						break;
					default: break;
					}
				}
				break;

			default:
				add = true;
				break;
			}
		// Handle normal situations
		} else {
			switch ( token[0] )
			{
			case ':':
				findName = false;
				break;

			case '{':
				if ( ! findName )
				{
					getElements( &(pNode->addNode( new irrJSONElement(name.c_str()) )) );
				}
				break;

			case '}': // <- unreachable anyways
			case ';':
			case ',':
				findName = true;
				if ( name.size() && name != "" )
					elem->addAttribute(name,value);
				name = "";
				value = "";
				break;

			case '\'':
				inQuote1 = true;
				break;

			case '\"':
				inQuote2 = true;
				break;

			case '/': // Comments
				token = getToken();
				out = false;
				if ( ! reader->atEOF() && token[0] == '*' )
				{
					token = getToken();
					while ( ! reader->atEOF() && token[0] != '}' )
					{
						if ( out )
						{
							// Comment ended
							if ( token[0] == '/' )
							{
								// Continue normal activity
								token = getToken();

								// Collect values terminated by '}'
								// Must be looking for value
								if ( (token[0] == '}' || reader->atEOF()) && !findName )
								{
									elem->addAttribute(name,value);
								}

								continue;
							}
							else
								out = false;
						}
						if ( token[0] == '*' )
							out = true;
						token = getToken();
					}
					// Continue normal activity

					// Collect values terminated by '}'
					// Must be looking for value
					if ( (token[0] == '}' || reader->atEOF()) && !findName )
					{
						elem->addAttribute(name,value);
					}

					continue;
				}
				add = true;
				break;

			default: // regular words
				add = true;
				break;
			}
		}

		if ( add )
		{
			if ( findName )
			{
				name.append(token);
			} else {
				value.append(token);
			}
		}

		if ( token[0] != '}' )
			token = getToken( inQuote1 || inQuote2 );

		// Collect values terminated by '}'
		// Must be looking for value
		if ( (token[0] == '}' || reader->atEOF()) && !findName )
		{
			elem->addAttribute(name,value);
		}
	} // end while loop
}


}}

#endif
