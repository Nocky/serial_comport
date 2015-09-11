#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "hashfun.h"

static unsigned int hash_seed = 5381;
//-----------------------------------------------------------
unsigned int Str_Hash(const void *key, size_t len)
{
     unsigned int seed = hash_seed;
     unsigned int m = 0x5bd1e995;
     int r = 24;

     unsigned int h = seed ^ len;

     unsigned char *data = (unsigned char *) key;

     while (len >= 4)
     {
	 unsigned int k = *(unsigned int *) data;

	 k *= m;
	 k ^= k >> r;
  	 k *= m;

	 h *= m;
	 h ^= k;

	 data += 4;
	 len -= 4;
     }

     switch (len)
     {
	case 3:
	  h ^= data[2] << 16;
        break;
	case 2:
	  h ^= data[1] << 8;
        break;
	case 1:
 	  h ^= data[0];
	  h *= m;
        break;
     };

     h ^= h >> 13;
     h *= m;
     h ^= h >> 15;

     return (unsigned int) h;
}
//-----------------------------------------------------------
/* And a case insensitive hash function (based on djb hash) */
unsigned int GenCase_Hash(const unsigned char *buf, int len)
{
    unsigned int hash = (unsigned int)hash_seed;

    while (len--)
        hash = ((hash << 5) + hash) + (tolower(*buf++)); /* hash * 33 + c */
    return hash;
}
//-----------------------------------------------------------
//FNV HASH
unsigned int Hash4Bytes(const void *key,size_t len)  
{  
    const char * first =  (const char *)key;  
    unsigned int result = 2166136261UL;  
    for(; len > 0; --len) {  
        result ^= (size_t)*first++;  
        result *= 16777619;  
    }  
    return result;  
}  

