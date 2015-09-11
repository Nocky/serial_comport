/**********************************************
* public.h
*
*  Created on: 2015-04-17
*      Author: xifengming
*       Email: xifengming@vip.sina.com
*    Comments: generic
 *********************************************/

#ifndef _PUBLIC_H
#define _PUBLIC_H


#ifdef __cplusplus
extern "C" {
#endif

unsigned short SwapWordHighLow(unsigned short wDestID);
void DebugInfo(unsigned char * puc,int nLen);
unsigned char GetXor(unsigned char *lpBuf,int nLen);

#ifdef __cplusplus
}
#endif
#endif
