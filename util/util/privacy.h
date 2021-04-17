// (c) 2019 Nicolaus Anderson
/*
	AGREEMENT
	1) This software is provided AS-IS without any expressed guarantee of any kind of usability or fitness.
	2) You may use, modify, and/or distribute this software to your heart's content provided:
	a) You do not misrepresent the software as having been created by you. (That would be lying.)
	b) You transmit all copies of this software with this original agreement completely intact.
*/
/*
	USAGE
	These macros are intended to replace the standard "private" and "protected" labels of classes.
	Use them in place of the "private" and "protected" labels.
	This allows for accessing and checking the private and protected members of a class for MOMENTARY debugging
	purposes.
	[code]
	class MyClass
	{
	loose_protected:
		int protectedMember;

	loose_private:
		int privateMember;
	};
	[/code]
*/

#ifndef PRIVACY_UTIL_H
#define PRIVACY_UTIL_H

#define ENFORCE_PRIVATE
#ifdef UNENFORCE_PROTECTED
#undef ENFORCE_PRIVATE
#endif

#define ENFORCE_PROTECTED
#ifdef UNENFORCE_PROTECTED
#undef ENFORCE_PROTECTED
#endif

#ifdef ENFORCE_PRIVATE
#define  loose_private  private
#else
#define  loose_private  public
#endif

#ifdef ENFORCE_PROTECTED
#define  loose_protected  protected
#else
#define  loose_protected  public
#endif

#endif // end PRIVACY_UTIL_H
