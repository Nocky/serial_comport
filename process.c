#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <strings.h>
#include "queue.h"
#include "lock.h"
#include "comport.h"
#include "head.h"
#include "threadpool.h"

#define SERIAL_PORT_NUMS    4
//-----------------------------------------------------------
void read_func(void *argv)
{
    Generic_Obj *thiz = (Generic_Obj *)argv;
    int nReadLen      = 0;

    for(;;){
      unsigned char ReadBuffer[2048] = {0};
      nReadLen = Serial_ReadSerialport(thiz->serial,(unsigned char *)ReadBuffer); 
      char *lpData = (char *)malloc(nReadLen+1);
      if (lpData != NULL && nReadLen > 0){
         memset(lpData,0,sizeof(nReadLen));
         memcpy(lpData,ReadBuffer,nReadLen);
         lpData[nReadLen] = '\0';
	 //DebugInfo(lpData,nReadLen);
         thiz->lockerobj->Lock(thiz->lockerobj->locker);
         DataQueue_Push(thiz->queue,lpData);
         thiz->lockerobj->Unlock(thiz->lockerobj->locker);
         thiz->lockerobj->Post(thiz->lockerobj->locker);
     }  
   }
}
//-----------------------------------------------------------
void process_func(void *argv)
{
    Generic_Obj *thiz = (Generic_Obj *)argv;
    char *data = NULL;

    for(;;){
        thiz->lockerobj->Swait(thiz->lockerobj->locker);
        thiz->lockerobj->Lock(thiz->lockerobj->locker);
        data = DataQueue_Pop(thiz->queue);
        if (data != NULL){
           printf("recv data:%s,%d\n",data,DataQueue_Size(thiz->queue));
           free(data);
           data=NULL;
        }
        thiz->lockerobj->Post(thiz->lockerobj->locker);
    }
}
//-----------------------------------------------------------
void clear(Generic_Obj  *thiz)
{
    if (NULL != thiz){
       Serial_Flushport(thiz->serial);
       thiz->lockerobj->Clear(thiz->lockerobj->locker);
       DataQueue_Clear(thiz->queue);
    }
}
//-----------------------------------------------------------
int main(void)
{
   int i = 0;

   Serial_Desc  serial_desc;

   Threadpool  *thread_pool  = NULL;
   Generic_Obj **this        = NULL;

   this = (Generic_Obj **)malloc(SERIAL_PORT_NUMS*sizeof(Generic_Obj *));

   if (NULL == this){
      printf("malloc public class fail!!!\n");
      return -1;
   }

   serial_desc.speed    = 115200;
   serial_desc.port     = 0;
   serial_desc.stopbits = 1;
   serial_desc.databits = 8;
   serial_desc.parity   = 0;
   serial_desc.flow     = 0;

   thread_pool = Threadpool_Create(SERIAL_PORT_NUMS * 2);

   if ( NULL == thread_pool){
       return -1;
   }

   for(;i<SERIAL_PORT_NUMS;i++){

     this[i] = (Generic_Obj *)malloc(sizeof(Generic_Obj));

     if ( NULL == this[i]){
        printf("malloc Generic_Obj obj fail!!!\n");
        exit(1);
     }

     this[i]->lockerobj  = LockerObj_Create();
     this[i]->queue      = DataQueue_Create();

     serial_desc.port = i;
      this[i]->serial = Serial_InitSerialport(serial_desc);

     Threadpool_Addtask(thread_pool,&read_func,"read_func",strlen("read_func"),this[i]);
     Threadpool_Addtask(thread_pool,&process_func,"process_func",strlen("process_func"),this[i]);
   }

   for(;;){
      sleep(1);
   }

   return 0;
}
