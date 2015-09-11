#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include <sys/select.h>
#include "public.h"
#include "comport.h"

typedef struct termios DCB;

struct tagSerial{
    int  handle;
    DCB  *dcb;
    char serial_name[50];
};

int speed_arr[] = {B38400,B19200,B9600,B4800,B2400,B1200,B300,
                   B38400,B19200,B9600,B4800,B2400,B1200,B300,};

int name_arr[]  = {38400,19200,9600,4800,2400,1200,300,38400,  
                   19200,9600,4800,2400,1200,300,};
//-----------------------------------------------------------
static int Serial_TermSet_Baudrate(Serial *serial,int speed)
{
    int i=0;
    int ret=0;

    if (NULL == serial){
        printf("Serial_TermSet_Baudrate: serial obj is null!!!\n");
        return -1;
    }

    tcgetattr(serial->handle,serial->dcb); 

    for (;i < sizeof(speed_arr) / sizeof(int);i++) { 

       if  (speed == name_arr[i]) {     
         tcflush(serial->handle, TCIOFLUSH);  
         cfsetispeed(serial->dcb, speed_arr[i]);  
         cfsetospeed(serial->dcb, speed_arr[i]);

         ret = tcsetattr(serial->handle, TCSANOW,serial->dcb);

       if  (ret != 0) {        
          printf("Serial_TermSet_Baudrate:error:%d\n",speed);
          return -1;     
        }    

        tcflush(serial->handle,TCIOFLUSH);   
    }  
  }

  return 0;
}
//-----------------------------------------------------------
static int Serial_TermSet_Flow(Serial *serial,int flow)
{
    if (NULL == serial){
        printf("Serial_TermSet_Flow: serial obj is null!!!\n");
        return -1;
    }

    switch(flow)
    {
       case FC_RTSCTS:
      {
          serial->dcb->c_cflag |= CRTSCTS;
 	  serial->dcb->c_iflag &= ~(IXON | IXOFF | IXANY);
      }
      break;
       case FC_XONXOFF:
      {
          serial->dcb->c_cflag &= ~(CRTSCTS);
          serial->dcb->c_iflag |= IXON | IXOFF;
      }
       case FC_NONE:
      {
          serial->dcb->c_cflag &= ~(CRTSCTS);
    	  serial->dcb->c_iflag &= ~(IXON | IXOFF | IXANY);
      }
      default:
      break;
   }
    return 0;
}
//-----------------------------------------------------------
static int Serial_TermSet_Stopbit(Serial *serial,int stopbit)
{
    if (NULL == serial){
        printf("Serial_TermSet_Stopbit: serial obj is null!!!\n");
        return -1;
    }

    switch(stopbit)
    {
        case 1:
          serial->dcb->c_cflag &= ~CSTOPB;  
        break;
        case 2:
          serial->dcb->c_cflag |= CSTOPB;
        break;
        default:
           printf("Serial_TermSet_Stopbit:set error:%d\n",stopbit);
           return -1;
        break;
    }

    return 0;
}
//-----------------------------------------------------------
static int Serial_TermSet_Databits(Serial *serial,int databits)
{
    if (NULL == serial){
        printf("Serial_TermSet_Databits: serial obj is null!!!\n");
        return -1;
    }

    switch(databits)
    {
        case 5:
          serial->dcb->c_cflag = (serial->dcb->c_cflag & ~CSIZE) | CS5;
        break;
        case 6:
          serial->dcb->c_cflag = (serial->dcb->c_cflag & ~CSIZE) | CS6;
        break;
        case 7:
          serial->dcb->c_cflag = (serial->dcb->c_cflag & ~CSIZE) | CS7;
        break;
        case 8:
          serial->dcb->c_cflag = (serial->dcb->c_cflag & ~CSIZE) | CS8;
        break;
        default:
          serial->dcb->c_cflag = (serial->dcb->c_cflag & ~CSIZE) | CS8;
        break;
    }

    return 0;
}
//-----------------------------------------------------------
static int Serial_TermSet_Parity(Serial *serial,int parity)
{
    if (NULL == serial){
        printf("Serial_TermSet_Parity: serial obj is null!!!\n");
        return -1;
    }

    switch(parity)
    {
       case P_EVEN:
       {
         serial->dcb->c_cflag &= ~PARODD;
         serial->dcb->c_cflag |= PARENB;
       }
       break;
       case P_ODD:
       {
         serial->dcb->c_cflag |= PARENB | PARODD;
       }
       break;
       case P_NONE:
       {
         serial->dcb->c_cflag &= ~(PARENB | PARODD); 
       }
       break;
       default:
         serial->dcb->c_cflag &= ~(PARENB | PARODD); 
       break;
    }

    return 0;
}
//-----------------------------------------------------------
Serial *Serial_InitSerialport(Serial_Desc serialdesc)
{
	struct termios dcb;
        Serial *serial_obj = NULL;

	memset(&dcb, 0, sizeof(dcb));

        serial_obj = (Serial *)malloc(sizeof(Serial));

        if (serial_obj == NULL){
             printf("Serial_InitSerialport:serial malloc fail!!!\n");
             return NULL;
        }

        serial_obj->dcb =(DCB *)malloc(sizeof(DCB));

        if (NULL == serial_obj->dcb){
             printf("Serial_InitSerialport:serial struct dcb malloc fail!!!\n");
             return NULL;
        }

	memset(serial_obj->dcb, 0, sizeof(DCB));

        sprintf(serial_obj->serial_name,"/dev/ttyS%d",serialdesc.port);
	serial_obj->handle = open(serial_obj->serial_name, O_RDWR | O_NOCTTY);

	if (-1 == serial_obj->handle) {
	     printf("Serial_InitSerialport:serial handle -1,Please check com port,%s!\n",serial_obj->serial_name);
	     return NULL;
	}

        //cfsetispeed(&dcb, B115200);
        //cfsetospeed(&dcb, B115200);
        //
        Serial_TermSet_Baudrate(serial_obj,serialdesc.speed);
        Serial_TermSet_Databits(serial_obj,serialdesc.databits);
        Serial_TermSet_Stopbit(serial_obj,serialdesc.stopbits);
        Serial_TermSet_Parity(serial_obj,serialdesc.parity);
        Serial_TermSet_Flow(serial_obj,serialdesc.flow);

	//dcb.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD | HUPCL;
	//dcb.c_iflag = IGNPAR;
	//
	serial_obj->dcb->c_oflag = 0;
	serial_obj->dcb->c_lflag = 0;
	serial_obj->dcb->c_cc[VMIN]  = 0;
	serial_obj->dcb->c_cc[VTIME] = 0;

         //tcsetattr(g_SerialPortHand, TCSAFLUSH, &dcb); 
	//tcflush(g_SerialPortHand, TCIFLUSH); 

	tcsetattr(serial_obj->handle, TCSANOW, serial_obj->dcb);

	tcsetattr(serial_obj->handle, TCSAFLUSH,serial_obj->dcb); 
	tcflush(serial_obj->handle, TCIFLUSH); 

	return serial_obj;
}
//-----------------------------------------------------------
void Serial_Freeserial(Serial *serial_obj)
{
      if (serial_obj != NULL){
           free(serial_obj);
           serial_obj = NULL;
      }
}
//-----------------------------------------------------------
void Serial_CloseSerialport(Serial *serial_obj)
{
    if (serial_obj != NULL){
       close(serial_obj->handle);
    }
}
//-----------------------------------------------------------
BOOL Serial_WriteSerialport(Serial *serial_obj,unsigned char *databuffer, int nDataLen)
{
        if (serial_obj == NULL){
             return FALSE;
        }

	ssize_t ret=0;
        unsigned char *ptr = databuffer;
        int send_len=0;
        int nlen =0;

	ret = write(serial_obj->handle, databuffer, nDataLen);

        nlen = nDataLen;
     
        while(nlen>0){
	  ret = write(serial_obj->handle,ptr,nlen);
          nlen -= ret;
          ptr += ret;
          send_len +=ret;
        }

	DebugInfo(databuffer, ret);

	printf("Send Len:%d\n",send_len);

	if (send_len != nDataLen) {
	    printf("write data to device error!\n");
	    return FALSE;
	}

	return TRUE;
}
//-----------------------------------------------------------
int Serial_ReadSerialport(Serial *serial_obj,unsigned char *returndata)
{
        if ( NULL == returndata || serial_obj == NULL){
             return -1;
        }

	int nRead, nRet;
	fd_set fs_read;

	struct timeval timeOut;

	timeOut.tv_sec  =3;
	timeOut.tv_usec =0;

	FD_ZERO(&fs_read);
	if (-1 == serial_obj->handle)
		return -1;	//·ÀÖ¹¶Î´íÎó
	FD_SET(serial_obj->handle, &fs_read);

      	nRet = select(serial_obj->handle + 1, &fs_read, NULL, NULL, &timeOut);

	if (-1 == nRet) {
		perror("select failed");
		return -1;
	} else if (0 == nRet)
        {
		printf("%s:timeout:com no data!!\n",serial_obj->serial_name);
                return 0;
	}
        else 
        {
                if (FD_ISSET(serial_obj->handle,&fs_read))
               {
		 nRead = read(serial_obj->handle, returndata,MAX_SERIAL_LEN);
		 return nRead;
               }
               return -1;
	}
}
//-----------------------------------------------------------
void Serial_Flushport(Serial *serial_obj)
{
   if (serial_obj != NULL){
     tcflush(serial_obj->handle, TCIFLUSH);
     tcflush(serial_obj->handle, TCIOFLUSH);
   }
}
//-----------------------------------------------------------
