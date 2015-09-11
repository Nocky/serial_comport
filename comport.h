/**********************************************
* comport.h
*
*  Created on: 2015-04-17
*      Author: xifengming
*       Email: xifengming@vip.sina.com
*    Comments: serial port 
 *********************************************/

#ifndef _COM_PORT_H
#define _COM_PORT_H

#define MAX_SERIAL_LEN   2048

#ifdef  _x86
#define BAUDRATE         B115200
#define COM1             "/dev/ttyS0"
#else
#define BAUDRATE         B115200
#define COM1             "/dev/ttySAC3"
#endif

typedef unsigned char    BOOL;
typedef unsigned short   WORD;
typedef unsigned long    DWORD;

#define TRUE        1
#define FALSE       0

#define P_EVEN      2
#define P_ODD       1
#define P_NONE      0

#define FC_NONE     0
#define FC_RTSCTS   1
#define FC_XONXOFF  2

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagSerial Serial;

struct tagSerialDesc{
    int port;
    int speed;
    int databits;
    int stopbits;
    int parity;
    int flow;
};

typedef struct tagSerialDesc Serial_Desc;

Serial * Serial_InitSerialport(Serial_Desc serialdesc);
void Serial_CloseSerialport(Serial *serial_obj);
BOOL Serial_WriteSerialport(Serial *serial_obj,unsigned char *databuffer,int nDataLen);
int  Serial_ReadSerialport(Serial *serial_obj,unsigned char *returndata);
void Serial_Flushport(Serial *serial_obj);
void Serial_Freeserial(Serial *serial_obj);

#ifdef __cplusplus
}
#endif

#endif
