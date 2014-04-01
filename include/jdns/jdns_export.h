/*
 * Copyright (C) 2014  Ivan Romanov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
   \file qjdns_export.h

   Preprocessor magic to allow export of library symbols. 

   This is strictly internal.

   \note You should not include this header directly from an
   application. You should just use <tt> \#include \<qjdns.h>
   </tt> instead.
*/

#ifndef JDNS_EXPORT_H
#define JDNS_EXPORT_H

#ifdef _WIN32
# ifndef JDNS_STATIC
#  ifdef JDNS_MAKEDLL
#   define JDNS_EXPORT __declspec(dllexport)
#  else
#   define JDNS_EXPORT __declspec(dllimport)
#  endif
# endif
#endif
#ifndef JDNS_EXPORT
# define JDNS_EXPORT
#endif

#endif
