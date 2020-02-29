/*
(C) 2011-2012 Nicolaus Anderson
This file subject to license as given in PES_Plot_main.cpp
*/

#pragma once

#include "IReadFile.h"
#include "irrString.h"

using namespace irr;


class irrVFreader
{

s32 read_position;
io::IReadFile * theFile;

	class infile
	{
		bool usable;
		c8 * filePtr;
		long filesize;
		c8* faPtrEnd; // the last location the file pointer points to

	public:
		infile()
		{
			usable = false;
		}

		void infileInst( io::IReadFile * file )
		{
			filesize = file->getSize();
			filePtr = new c8[filesize];
			memset( filePtr, 0, filesize );
			faPtrEnd = filePtr + filesize; // final pointer location = curr + assoc. items

			file->read( (void*)filePtr, filesize ); // copy contents into the array

			usable = true;
		}

		long Size() { return filesize; }

		c8 Read( s32 readpos )
		{
			if (usable)
				return filePtr[ readpos ];
			else return ' ';
		}

	} infiler;

public:

// constructor
irrVFreader(io::IReadFile * file)
{
	theFile = file; // save a pointer to the file to read

	infiler.infileInst(file); // the file in c8 characters

	read_position = 0; // start from the beginning
}


/* Functions used to call readFromFile functions repeatedly when spaces are not
desired. */
core::string<c8> readFromFile_ns(bool delim)
{
	core::string<c8> ret;

	c8 * s_chars;
	s32 numchars = 1;

	if ( delim ) {
		s_chars = new c8[3];
		numchars = 3;
		s_chars[1] = (c8)'\n';
		s_chars[2] = (c8)'\r';
	} else
		s_chars = new c8[1];
	
	s_chars[0] = (c8)' ';

	do {
		ret = readFromFile( numchars, s_chars );
	} while ( ret == " " );
	
	return ret;
}

core::string<c8> readFromFile_sys_ns(s32 _MaxSystemChars, c8 * system_chars)
{
	core::string<c8> ret;

	do {
		ret = readFromFile( _MaxSystemChars, system_chars );
	} while ( ret == " " );
	
	return ret;
}

core::string<c8> readFromFile_sys_nse(s32 _MaxSystemChars, c8 * system_chars)
{
	core::string<c8> ret;

	do {
		ret = readFromFile( _MaxSystemChars, system_chars );
	} while ( ret == " " || ret == "\n" || ret == "\r" );
	
	return ret;
}

//====================================================

/* Function used to obtain objects (tokens) from files
considering special characters. */
core::string<c8> readFromFile(s32 _MaxSystemChars, c8 * system_chars)
{     
     /* read individual characters from the file */

	      // prep: create necessary variables
     c8 character = ' ';	/* This stores the character to be placed in a
							core::string and returned to the function that called this
							function */
	 c8 peek;				/* For looking at the next character in file. */
	 bool set = false;		/* Indicates whether or not the loop that collects
							characters from file (and puts them in a core::string) should
							be stopped. */
	 bool set_fail = false;	/* Indicates whether or not to reset the set variable so
							that the character-collecting loop can continue. */
	 s32 count = 0;			/* Used in for-loop to compare system characters with
							each new character extracted from file. */

     // initialize a core::string to return to the function that called this one
     core::string<c8> strng;

	 //**********************************


	// get characters from file
	do
	{
		// WHAT TO DO IF AT THE END OF MAIN
		if ( read_position >= infiler.Size() )
		{
			 /* assign "end_main" to the return core::string to indicate the
			 end of the file has been reached */
			 strng = "end_main";

			 // end the readFromFile() use here
			 return strng;
		} // endif WHAT TO DO IF AT THE END OF MAIN

        // get a character from the file
		character = infiler.Read( read_position );
		read_position++;

		/* Get rid of any tabs. Turn them into spaces. */
		if ( character == '	' )
		{ character = ' '; }

		/* check to make sure that the character is not a system command
		character */
		for (count = 0; count < _MaxSystemChars; count++)
		{
			/* If the character is a system character, plan to get rid of it. */
			if ( character == system_chars[count] )
		    {
				set = true; break;
			}
		}

		// Add the character if it is going to be the only one in the word.
		if ( set == false || strng.size() == 0 )
		{
			// Add the character to the word
			strng.append( character );

			/* Consider adding another character to the word ONLY if it
			is a number, since this might be a floating point decimal
			number. */
			if ( character == '.' )
			{
				// peek at the next character from the file
				peek = infiler.Read( read_position );

				//if ( peek == '0' || peek == '1'
				//	|| peek == '2' || peek == '3'
				//	|| peek == '4' || peek == '5'
				//	|| peek == '6' || peek == '7'
				//	|| peek == '8' || peek == '9' )
				if ( peek >= '0' && peek <= '9' )
				{
					set = false;
				}
			}
		} else {
			// Apparently, set was true, so a system character was found
			/* Only let a period be added to the word as long as there
			is no other period in the word already. */
			//if ( character == '.'
			//	&& ( strng[0] == '0' || strng[0] == '1'
			//	|| strng[0] == '2' || strng[0] == '3'
			//	|| strng[0] == '4' || strng[0] == '5'
			//	|| strng[0] == '6' || strng[0] == '7'
			//	|| strng[0] == '8' || strng[0] == '9'
			//	) )
			if ( character == '.' && (strng[0] >= '0' && strng[0] <= '9') )
				/* NOTE: Only the first character in the word needs
				to be looked at, since the period will be added if it
				is going to be the first character in the word anyways.
				A number at the beginning of the word automatically
				indicates a floating decimal number.
				
				Minus signs are to be handled under the '-' data manipulator. */
			{
				// The system character is a period.
					// Check for a period already in the word
				set_fail = false; // assume there is no period in the word
					// start looking for a period in the word
				for (u32 w = 0; w < strng.size(); w++)
				{
					if (strng[w] == '.')
					{ set_fail = true; break; }
				}
				if ( set_fail == true )
				{
					/* If there is a period in the word already, allow
					the second period to be picked up next time. */
					read_position--;
				} else {
					// Add the period to the end of the word
					strng.append( character );

					// peek at the next character from the file
					peek = infiler.Read( read_position );

					/* only consider adding another character to the core::string
					if the character is a decimal place or a number */
					//if ( peek == '0' || peek == '1'
					//	|| peek == '2' || peek == '3'
					//	|| peek == '4' || peek == '5'
					//	|| peek == '6' || peek == '7'
					//	|| peek == '8' || peek == '9' )
					if ( peek >= '0' && peek <= '9' )
					{
						set = false;
					}
				}
			} else {
			/* The core::string length was not zero, or the character was
			not a period. */

				/* The system character was not added to the end of a word,
				so allow it to be extracted next time. */
				read_position--;
			}
		}

		/* Don't collect more characters if the file is at its end.
		Simply return what has been collected already. */
		if ( read_position >= infiler.Size() && strng.size() > 0 )
		{
			/* Return what has been acquired. */
			return strng;
		}

	} while ( set == false && read_position < infiler.Size() );
     
     // return the core::string
     return strng;
}


//==============================================================

/* Function for getting a select number of characters from file */
core::string<c8> readFileChars( s32 obtain )
{     
     /* read individual characters from the file */

	      // prep: create necessary variables
     c8 character = ' ';	/* This stores the character to be placed in a
							core::string and returned to the function that called this
							function */
	 s32 count = 0;			/* Used in while loop to compare the number of characters
							stored in the core::string with the number of characters
							desired in the core::string. */

      // initialize a core::string to return to the function that called this one
     core::string<c8> strng;


	 //**********************************

	// get characters from file
	do
	{
		// WHAT TO DO IF AT THE END OF MAIN
		if ( read_position >= infiler.Size() )
		{
			 /* assign "end_main" to the return core::string to indicate the
			 end of the file has been reached */
			 strng = "end_main";

			 // end the readFromFile() use here
			 return strng;
		} // endif WHAT TO DO IF AT THE END OF MAIN


        // get a character from the file
		character = infiler.Read( read_position );
		read_position++;

		// store the character in the core::string
		strng.append( character );

		// indicate another character has been put in the core::string
		count++;

	} while ( read_position < infiler.Size() && count < obtain );
     
    // return the core::string
    return strng;
}

};
