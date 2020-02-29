// From:
// https://code.woboq.org/qt5/include/c++/7.3.0/type_traits.html#std::enable_if

// C++11 <type_traits> -*- C++ -*-
// Copyright (C) 2007-2017 Free Software Foundation, Inc.
// Distributed under the GNU GPL v3

 // Primary template.
/// Define a member typedef @c type only if a boolean constant is true.
template<bool, typename _Tp = void>
struct enable_if
{ };
// Partial specialization for true.
template<typename _Tp>
struct enable_if<true, _Tp>
{ typedef _Tp type; };
