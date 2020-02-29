/* (c) 2014 Nicolaus Anderson
License: zlib
*/

#ifndef IRRJSON_H
#define IRRJSON_H

#include <irrString.h>
#include <IFileSystem.h>
#include <irrTree.h>
#include "CharFileReader.h"

namespace irr {
namespace io {

using core::string;
using core::list;

/* NOTE:
JSON is meant to support unicode format, so in this case, the irrJSON
supports Unicode8 by avoiding circumstances that require interpreting characters
outside of the ASCII range of Unicode8.
*/
class irrJSONElement : public irrTreeElement
{
protected:
	/* Name of the element (includes any '.' or character that
	precedes the name in the file. */
	string<c8> name;

public:
	struct Attribute
	{
		string<c8> name;
		string<c8> value;

		Attribute()
			: name("")
			, value("")
		{}

		Attribute(string<c8> pName, string<c8> pValue)
			: name( pName )
			, value( pValue )
		{}
	};

protected:
	list<Attribute> attributes;

public:
	irrJSONElement()
		: name("")
	{}

	irrJSONElement( const c8* pName )
		: name(pName)
	{}

	string<c8>& getName() { return name; }
	list<Attribute>& getAttributes() { return attributes; }

	void addAttribute(string<c8> pName, string<c8> pValue)
	{
		getAttributes().push_back( Attribute(pName,pValue) );
	}
};


class irrJSON
{
	string<c8> delimChars;
	string<c8> skipChars;
	string<c8> quoteDelimChars;
	IFileSystem* fileSystem;

	CharFileReader* reader;
	CharFileAccessor* accessor;

public:
	irrJSON( IFileSystem* pFileSystem );
	~irrJSON();

	//! Parse file
	/* Attempts to parse the file as a JSON-formatted file.
	\param pResultTree - The output tree parsed from the JSON.
	IMPORTANT NOTE: This should be an empty tree pointer!
	\return - true if successful. */
	bool parseFile( path pPath, irrTreeNode*& pResultTree );
	bool parseFile( IReadFile* pFile, irrTreeNode*& pResultTree );

protected:
	inline string<c8> getToken( bool pInQuotes=false );
	void getElements( irrTreeNode* pNode );
};

}}

#endif
