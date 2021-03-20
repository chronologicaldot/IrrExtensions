/* (c) 2014 Nicolaus Anderson
License: zlib
*/

#include <irrString.h>
#include <IReadFile.h>
#include <IFileSystem.h>

// Based on (but not compatible with): InputStream.h

/*
Note that c8 is used everywhere, representing characters, which are
always supposed to be 8-bits, except on Linux systems.
Even still, that feature of Linux is never taken advantage of here.
Instead, everything is treated as a u8 (unsigned 8-bit), but does
not use u8 in order to keep the compiler happy with irrString.
*/

namespace irr {
namespace io {

using core::string;

enum ESeekPosition
{
	// Indicates offset from the current token
	ESeek_CURRENT=0,

	// Indicates offset from the beginning
	ESeek_BEGIN,

	// Indicates offset from the end
	ESeek_END,

	// Count
	ESeek_COUNT
};

class CharFileReader
{
	c8* file;
	long fileSize;
	long readPos;

	/* NOTE: readPos starts at -1 and ends at fileSize (1 after the last character)
	to allow the easy, continuous usage of next() */

public:
	CharFileReader( IReadFile* pFile=0 )
		: file(0)
		, fileSize(0)
		, readPos(-1)
	{
		if ( pFile )
		{
			fileSize = pFile->getSize();
			file = new c8[fileSize];
			pFile->read( (void*)file, fileSize ); // copy contents into the array
			//fileSize /= sizeof(c8); // Account for data type
		}
	}

	~CharFileReader()
	{
		if ( file )
		{
			delete[] file;
		}
	}

	void setFile( io::IReadFile* pFile )
	{
		if ( file )
		{
			delete[] file;
		}
		fileSize = 0;
		readPos = -1;

		if ( pFile )
		{
			fileSize = pFile->getSize();
			file = new c8[fileSize];
			pFile->read( (void*)file, fileSize ); // copy contents into the array
			//fileSize /= sizeof(c8); // Account for data type
		}
	}

	//! Set the file to a pointer
	/* NOTE: Requires that the pointer end in zero.
	NOTE: The contents of the pointer are NOT copied,
	but the pointer size is at least calculated. */
	void setFile( c8* ptr )
	{
		if ( file )
		{
			delete[] file;
		}
		fileSize = 0;
		readPos = -1;

		if ( ptr )
		{
			while ( ptr[fileSize] != 0 )
				fileSize++;
			file = ptr;
		}
	}

	c8 curr()
	{
		if ( readPos >= 0 && readPos < fileSize ) // implies fileSize > 0
			return file[readPos];
		return 0;
	}

	c8 next()
	{
		if ( readPos < fileSize )
		{
			readPos++;
			if ( readPos < fileSize )
				return file[readPos];
		}
		return 0;
	}

	c8 prev()
	{
		if ( readPos > 0 )
		{
			readPos--;
			return file[readPos];
		}
		return 0;
	}

	void seek( long pPosition, ESeekPosition pSeek=ESeek_CURRENT )
	{
		switch( pSeek )
		{
		case ESeek_BEGIN:
			if ( pPosition > fileSize )
				readPos = fileSize;
			else
				readPos = pPosition;
			break;

		case ESeek_END:
			if ( fileSize < pPosition )
				readPos = -1;
			else
				readPos = fileSize - pPosition;
			break;

		default:
			if ( readPos + pPosition > -1 )
			{
				if ( readPos + pPosition < fileSize )
				{
					readPos += pPosition;
				} else {
					readPos = fileSize;
				}
			} else {
				readPos = -1;
			}
		}
	}

	//! Indicates if the reading position is prior to the first character
	bool atPreBegin()
	{
		return (readPos == -1);
	}

	//! Indicates if the reading position is after the last character in file
	bool atEOF()
	{
		// If another character cannot be extracted via next()
		return (fileSize == readPos);
	}
};

class CharFileAccessor
{
	CharFileReader* reader;

	c8 unicode8SizeMark;
	c8 unicode8SizeBits;
	/* It is assumed that all numbers in the text file are, in fact,
	stored as characters and not numbers. */

	bool bigEndian; /* Flag indicating if the file is big-endian.
					This is usually identified via:
					1) The first character in file (which takes precedence
					over user setting) or
					2) User setting */

protected:
	/* Sets the endianness of the accessor based on the Byte Order Mark.
	Note that this mark is two characters long for U16.
	U16BE = FE FF
	U16LE = FF FE */
	void setEndianFromBom( char* bom )
	{
		bigEndian = bom[0] == 0xfe;
	}

public:
	/* Checks the system's endianness.
	Idea thanks to http://www.codeproject.com/Articles/4804/Basic-concepts-on-Endianness
	*/
	static bool isSystemBigEndian()
	{
		s16 w = 0x0201;
		return ((c8*)&w)[0] != 0x01;
	}


	CharFileAccessor( CharFileReader* pReader=0 )
		: reader( pReader )
		, unicode8SizeMark( (c8)0xc0 )
				/* rather than 80 (1000 0000), check for the existence of a second
				byte via c0 (1100 0000) */
		, unicode8SizeBits( (c8)0xf0 )
		, bigEndian( false )
	{}

	void setReader( CharFileReader* pReader )
	{
		reader = pReader;
	}

	CharFileReader* getReader()
	{
		return reader;
	}

	bool atEOF()
	{
		if ( reader )
			return reader->atEOF();
		return true;
	}

	/* Sets the endianness for U16 and U32 reading.
	(Not required for U8) */
	void setEndianness( bool pBigEndian )
	{
		bigEndian = pBigEndian;
	}

	/* Sets the endianness for U16 and U32 reading.
	(Not required for U8) */
	void useSystemEndian()
	{
		bigEndian = isSystemBigEndian();
	}

	/* Returns the next unicode-8 character as a string. */
	const string<c8> getNextU8Char()
	{
		u8 i = 1;
		string<c8> out;

		if ( !reader )
			return out;

		u8 c = reader->next();

		if ( (c & unicode8SizeMark) > 0 )
		{
			i = (c & unicode8SizeBits) >> 4;
		}
		out.append(c);
		for ( ; i > 1; i>>=2 )
		{
			out.append(reader->next());
		}
		//reader->seek(-3, ESeek_CURRENT);
		return out;
	}

	/* Returns a string deliminated by the given characters.
	Skips the characters given as skipChars. */
	const string<c8> getToken( const c8* delims, u32 numDelims, const c8* skipChars, u32 numSkipChars )
	{
		c8 c;
		u32 d;
		string<c8> out; // may need to be const
		bool add;

		if ( !reader )
			return out;

		while ( ! reader->atEOF() )
		{
			c = reader->next();
			for ( d = 0; d < numDelims; d++ )
			{
				if ( c == delims[d] )
				{
					if ( out.size() == 0 )
						out.append(c);
					else
						reader->seek(-1); // Deliminator should be put in the next string
					return out;
				}
			}
			add = true;
			for ( d = 0; d < numSkipChars; d++ )
			{
				if ( c == skipChars[d] )
				{
					add = false;
					if ( out.size() )
						return out;
				}
			}
			if ( add )
				out.append(c);
		}
		return out;
	}

};

}}
