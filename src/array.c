/*
 *   DCAP - dCache Access Protocol client interface
 *
 *   Copyright (C) 2000,2004 DESY Hamburg DMG-Division.
 *
 *   AUTHOR: Tigran Mkrtchayn (tigran.mkrtchyan@desy.de)
 *
 *   This program can be distributed under the terms of the GNU LGPL.
 *   See the file COPYING.LIB
 *
 */


/*
 * $Id: array.c,v 1.8 2004-11-01 19:33:28 tigran Exp $
 */


#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sysdep.h"
#include "char2crc.h"

static MUTEX(gLock);


typedef struct {
	unsigned long   key;
	int             value;
}               arrayElement;

static arrayElement *array;
static int      len;

unsigned long
array_char2crc(const unsigned char * key)
{
   unsigned long kh;
   size_t keysize = strlen((const char *)key);
   char * new_key = (char *)malloc(sizeof(char)*(keysize+10));
   if (new_key == NULL) {
     /* Malloc failed; return original function */
     return char2crc(key);
   }
   snprintf(new_key, keysize, "%i %s", getpid(), key);
   kh = char2crc((const unsigned char *)new_key);
   free(new_key);
   return kh;
}

unsigned short
isMember(char *key)
{

	unsigned long   kh;
	int             i;

	kh = array_char2crc((const unsigned char *)key);
	for (i = 0; i < len; i++) {
		if (array[i].key == kh) {
			m_unlock(&gLock);
			return 1;
		}
	}

	return 0;
}

void
addMember(char *key, int value)
{

	arrayElement   *tmpArray;

	tmpArray = (arrayElement *) realloc(array, sizeof(arrayElement) * (len + 1));
	if (tmpArray == NULL) {
		return;
	}

	array = tmpArray;
	array[len].value = value;
	array[len].key = array_char2crc((const unsigned char *)key);

	len++;
	return;
}


int
getMember(char *key)
{

	unsigned long   kh;
	int             i;
	int             value;

	kh = array_char2crc((const unsigned char *)key);

	for (i = 0; i < len; i++) {
		if (array[i].key == kh) {
			value = array[i].value;
			return value;
		}
	}

	return -1;
}

void
deleteMember(char *key)
{
	arrayElement   *tmpArray;
	int             i, j;
	unsigned long   kh;


	if (array != NULL) {
		return;
	}
	tmpArray = (arrayElement *) malloc(sizeof(arrayElement) * (len - 1));
	if (tmpArray == NULL) {
		return;
	}
	kh = array_char2crc((const unsigned char *)key);
	for (i = 0, j = 0; i < len; i++) {

		if (array[i].key == kh) {
			continue;
		}
		array[i] = tmpArray[j];
		j++;
	}

	free(array);
	array = tmpArray;

	len--;
	return;
}

void
deleteMemberByValue(int value)
{
	arrayElement   *tmpArray;
	int             i, j;


	/* empty array */
	if (( array == NULL ) || ( len == 0 ) ) {
		return;
	}

	tmpArray = (arrayElement *) malloc(sizeof(arrayElement) * (len - 1));
	if (tmpArray == NULL) {
		return;
	}

	for (i = 0, j = 0; i < len; i++) {

		if (array[i].value == value) {
			continue;
		}
		array[i] = tmpArray[j];
		j++;
	}

	free(array);
	array = tmpArray;

	len--;
	return;
}

void
lockMember()
{
	m_lock(&gLock);
}


void
unlockMember()
{
	m_unlock(&gLock);
}
