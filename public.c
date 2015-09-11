#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "public.h"
//-----------------------------------------------------------
void DebugInfo(unsigned char *puc, int nLen) 
{
	int i;
	for (i = 0; i < nLen; i++)
         {
	  printf("%.2x ", puc[i]);
	}
	printf("\n");
}
//-----------------------------------------------------------------
unsigned short SwapWordHighLow(unsigned short wDestID) 
{
	unsigned short wID = 0;
	wID = (wDestID >> 8) | (wDestID << 8);

	return wID;
}
//------------------------------------------------------------
unsigned char GetXor(unsigned char *lpBuf, int nLen) 
{
	unsigned char byData = 0;
	int i = 0;
	for (i = 0; i < nLen; i++)
	{
	    byData = byData ^ lpBuf[i];
	}

	return byData;
}
//------------------------------------------------------------
