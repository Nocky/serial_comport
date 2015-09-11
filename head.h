/**********************************************
* head.h
*
*  Created on: 2015-04-17
*      Author: xifengming
*       Email: xifengming@vip.sina.com
*    Comments: head
 *********************************************/

#ifndef _HEAD_H
#define _HEAD_H

#include "queue.h"
#include "lock.h"
#include "comport.h"

typedef struct tagHead
{
    Serial    *serial;
    DataQueue *queue;
    LockerObj *lockerobj;
}Generic_Obj;

#endif
