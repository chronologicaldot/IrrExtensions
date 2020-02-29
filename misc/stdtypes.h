/*
(c) 2013 Nicolaus Anderson
License: zlib

Number data type templates

Purpose:
Sometimes you may not want to use irrlicht types but you still want
your classes to be usable with or without irrlicht types.
Hence, this is available.

*/

#pragma once

#if defined(__IRR_TYPES_H_INCLUDED__) //&& defined(__USE_IRRLICHT_NUMBERS__)

typedef irr::c8				stdc8;
typedef wchar_t				stdwc;
typedef irr::s8				stds8;
typedef irr::s16			stds16;
typedef irr::s32			stds32;
typedef irr::s64			stds64;
typedef irr::u8				stdu8;
typedef irr::u16			stdu16;
typedef irr::u32			stdu32;
typedef irr::u64			stdu64;
typedef irr::f32			stdf32;
typedef irr::f64			stdf64;

typedef long double			stdf128;

#ifdef __IRR_STRING_H_INCLUDED__
typedef irr::core::stringc	stdstr;
typedef irr::core::stringw	stdstrw;
#endif

#else // built-in types

typedef char				stdc8;
typedef wchar_t				stdwc;
typedef signed char			stds8;
typedef short				stds16;
typedef int					stds32;
typedef long int			stds64;
typedef unsigned char		stdu8;
typedef unsigned short		stdu16;
typedef unsigned int		stdu32;
typedef unsigned long		stdu64;
typedef float				stdf32;
typedef double				stdf64;
typedef long double			stdf128;

#if defined( _INC_STRING ) && defined( __USE_MSC_STRING__ )
typedef string				stdstr;
#endif
#if defined( _INC_WCHAR ) && defined( __USE_MSC_STRING_W__ )
typedef wstring				stdstrw;
#endif

/* Note the type definitions in xstring, lines 2209 to 2212

typedef basic_string<char, char_traits<char>, allocator<char> >
	string;
typedef basic_string<wchar_t, char_traits<wchar_t>,
	allocator<wchar_t> > wstring;
*/

#endif

// Unicode character
#ifdef( _MSC_VER )
#define __int8 utf8	// UTF-8 character
#define wchar_t utf16	// UTF-16 character
#define __int32 utf32	// UTF-32 character
#else
#define char utf8		// linux handles unicode in char as UTF-8
#define short utf16
#define int utf32
#endif


#ifndef __ENUMTYPE__
#define __ENUMTYPE__

//! Enumeration "Number Type"
/* Used to indicate the type of number stored in a particular location. */
enum ENumType
{
	ENT_null=0,
	ENT_bool,
	ENT_c8,
	ENT_wc,
	ENT_s16,
	ENT_s32,
	ENT_s64,
	ENT_u8,
	ENT_u16,
	ENT_u32,
	ENT_u64,
	ENT_f32,
	ENT_f64,
	ENT_f128,
	ENT_string,
	ENT_other
};

#endif


#define BOOLMAX			1
#define STDC8MAX		255
#define STDU8MAX		255
#define STDS16MAX		32767
#define STDS32MAX		2147483647
#define STDS64MAX		9223372036854775807
#define STDU16MAX		65536
#define STDU32MAX		4294967296
//#define STDU64MAX		18446744073709551616 // constant too big
#define STDU64			0xffffffffffffffffU
#define STDF32MAX		2147483646
#define STDF64MAX		9223372036854775806


#define BOOLMIN			0
#define STDC8MIN		0
#define STDU8MIN		0
#define STDS16MIN		-32767
#define STDS32MIN		-2147483647
#define STDS64MIN		-9223372036854775807
#define STDU16MIN		-65536
#define STDU32MIN		0
#define STDU64MIN		0
#define STDF32MIN		-2147483646
#define STDF64MIN		-9223372036854775806
