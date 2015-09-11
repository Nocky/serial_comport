#include <stdio.h>
#include <stdlib.h>
#include "minheap.h"

struct tagMinHeap
{
    void *data;
};

struct tagMinHeapObj
{
   MinHeap **minheap;
   void (*Swap)(void *src,void *dst);
   void (*Insert)(MinHeapObj *minheap,void *data);
   void (*MoveUp)(MinHeapObj *minheap,int data);
   void (*MoveDown)(MinHeap *minheap,void *data);
   void (*Free)(MinHeap *minheap);
   int size;
   int index;
};
//--------------------------------------------------------------------------------------------------------
MinHeapObj *MinHeap_Create(int size)
{
   MinHeapObj *min_heap_obj = NULL;

   min_heap_obj = (MinHeapObj *)malloc(sizeof(MinHeapObj));


  if (NULL == min_heap_obj){
     printf("MinHeap_Create:MinHeapObj malloc fail!\n");
     return NULL;
  }

  min_heap_obj->minheap  = MinHeap_Init(size);
  min_heap_obj->Swap     = MinHeap_Swap;
  min_heap_obj->Insert   = MinHeap_Insert;
  min_heap_obj->MoveUp   = MinHeap_MoveUp;
  min_heap_obj->MoveDown = MinHeap_MoveDown;
  min_heap_obj->Free     = MinHeap_Free;
  min_heap_obj->size     = size;
  min_heap_obj->index    = 0;

  return min_heap_obj;
}
//--------------------------------------------------------------------------------------------------------
MinHeap **MinHeap_Init(int size)
{
   int i = 0;
   MinHeap **min_heap = NULL;

   min_heap = (MinHeap **)malloc(sizeof(MinHeap *)*size);

   if (NULL == min_heap){
       printf("MinHeap_Init:MinHeap malloc fail!\n");
       return NULL;
   }

   for(;i<size;i++){
      min_heap[i]->data = NULL;
   }

   return min_heap;
}
//--------------------------------------------------------------------------------------------------------
void MinHeap_Swap(void *src,void *dst)
{
   void *temp;

   temp = src;
   dst  = src;
   dst  = temp;

}
//--------------------------------------------------------------------------------------------------------
void MinHeap_Free(MinHeap *minheap)
{

}
//--------------------------------------------------------------------------------------------------------
void MinHeap_Insert(MinHeapObj *heapobj,void *data)
{
    if (NULL != heapobj){
       heapobj->minheap[heapobj->index]->data = data;
    }
}
//--------------------------------------------------------------------------------------------------------
void MinHeap_MoveUp(MinHeapObj *heapobj,int data)
{
   int parent;
   int temp   = data;

   parent = (temp -1)/2;

   while(parent >=0){
      MinHeap_Swap(heapobj->minheap[parent]->data,heapobj->minheap[temp]->data);
      temp = parent;
      parent = (parent-1)/2;
   }
}
//--------------------------------------------------------------------------------------------------------
void MinHeap_MoveDown(MinHeap *heapobj,void *data)
{

}
//--------------------------------------------------------------------------------------------------------
