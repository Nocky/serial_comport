/**********************************************
* minheap.h
*
*  Created on: 2015-06-01
*      Author: xifengming
*       Email: xifengming@vip.sina.com
*    Comments: min heap
 *********************************************/
#ifndef _MIN_HEAP_H
#define _MIN_HEAP_H


#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagMinHeap    MinHeap;
typedef struct tagMinHeapObj MinHeapObj;

MinHeapObj *MinHeap_Create(int size);
MinHeap **MinHeap_Init(int size);
void MinHeap_Swap(void *src,void *dst);
void MinHeap_Free(MinHeap *minheap);
void MinHeap_Insert(MinHeapObj *heapobj,void *data);
void MinHeap_MoveUp(MinHeapObj *heapobj,int data);
void MinHeap_MoveDown(MinHeap *heapobj,void *data);


#ifdef __cplusplus
}
#endif

#endif
