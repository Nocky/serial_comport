/*
 *  hashtable.c - demonstrates a hash table
 *  Copyright (C) 2010 Martin Broadhurst 
 *  www.martinbroadhurst.com
 */

#include <stdio.h>
#include <string.h>

#include "hash.h"
#include "hashfun.h"

/* The SDBM hash */
unsigned int myhashfunc(const char * str)
{
	unsigned int hash = 0;
	int c;

	while ((c = *str++)) {
		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

int main(void)
{
	HashTable * table;
	const char * result;
	unsigned int e;
	char * elements[] = {"A", "@#$V", "C", "123", "E", "A", "G"};
	const unsigned int n = sizeof(elements) / sizeof(const char*);

	table = HashTable_Create(100, (HashFun)Str_Hash, (HashCmpFun)strcmp);

	for (e = 0; e < n; e++) {
		HashTable_Add(table, elements[e],elements[e]);
	}

	for (e = 0; e < n; e++) {
		result = (const char *)HashTable_Get(table, elements[e]);
		if (result) {
			printf("Found: %s\n", result);
		}
		else {
			printf("Couldn't find %s\n", elements[e]);
		}
	}
	printf("The load factor is %.2f\n", HashTable_Loadfactor(table));
	for (e = 0; e < n; e++) {
		result = HashTable_Remove(table, elements[e]);
		if (result) {
			printf("Removed: %s\n", result);
		}
		else {
			printf("Couldn't remove %s\n", elements[e]);
		}
	}
	HashTable_Clear(table);

	return 0;
}
