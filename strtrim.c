
/*
 * strtrim.c remove leading and trailing whitespace from a string
 *
 * Copyright (C) Dennis Clarke 2012
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * https://www.gnu.org/licenses/gpl-3.0.txt
 */

#define _XOPEN_SOURCE 600

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strtrim.h"

/* WARNING : If the caller provides a NULL pointer then
 * we return a valid char pointer to 8 bytes of memory
 * which shall be nothing more than a nul string.
 *
 * It is the responsibility of the caller to deal with a
 * free() of their pointer. We do not own it but we will
 * ensure it is not a NULL.
 *
 * 20 Jul 2022  a few words from the wild Greek who says :
 *
 *    GCC doesn't complain at all but my intuition says this is UB
 *    but I've been wrong before so I'm looking into it.
 *    I know for instance that char *foo="bar"; foo[0]='B'; is UB.
 *    but I am not sure what happens if you allocate an array of
 *    pointers.
 *    Best bet is to look at the assembly I guess.
 *    If the strings go in the stack it is fine
 *    since char[] foo="bar"; foo[0]='B'; well defined.
 *    The difference is pretty much where the string memory goes
 *
 *    I'm saying you can't pass string literals to that.
 *
 * 20 Jul 2022 marsen says
 *   "garbage in, garbage out". If the caller passes NULL either fail
 *   (like strlen, strdup, strcpy do) or return NULL again.
 *
 * some geek says :
 *    It may be far more sane to return a pointer to a new string
 *    in memory and not ever modify the callers string pointer.
 */
char *strtrim( char *str ) {
    /* Given a string pointer, toss out all
     * leading and trailing whitespace.
     */
    size_t  len;
    char    *frontp;
    char    *temp = NULL;

    /* Are we given NULL ?
     * If so then the caller is a jerk. Return the NULL
     * right back at them and let them deal with it.
     */
    if ( str == NULL ) {
        return str;
    }

    /* we were given nothing. */
    if ( str[0] == '\0' )
        return str;

    /* Bail out on the condition that this is only one byte
     * AND it is a whitespace.
     *
     * Another simple condition is that we are given a single
     * byte string.  If it is not whitespace just bail out.
     */
    len = strlen(str);
    if ( len == 1 ) {
        if (isspace(str[0])) {
            str[0] = '\0';
        }
        return str;
    }

    frontp = str - 1; /* will need to pre-increment this */
    temp = str + len;

    /* Move the front and back pointers to address
     * of the first non-whitespace characters from
     * each end. Note that frontp gets pre-incremented
     * which is fine given its definition as (str - 1)
     */
    while ( isspace(*(++frontp)) ); /* n.b: frontp = str - 1 */
    while ( isspace(*(--temp)) && ( temp != frontp ) );

    /* clean up in case we removed all characters */
    if ( ( str + len - 1 ) != temp )
        *(temp + 1) = '\0';
    else
        if ( ( frontp != str ) && ( temp == frontp ) )
            *str = '\0';

    /* now let's start over from the front of the
     * first non-whitespace char and move one char
     * at a time to the front of the input buffer
     */
    temp = str;  /* the front of the input buffer */
    if ( frontp != str ) {
        while ( *frontp )
            *temp++ = *frontp++;
        *temp = '\0';
    }

    return str;

}

