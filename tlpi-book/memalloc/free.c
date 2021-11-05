#include<unistd.h>
#include<stdio.h>

#ifndef HEADER
#include"header.h"
#endif


void free(void* block)  {
    if (!block)  {
        return;
    }
    
    struct header_t* header = (struct header_t*)block - 1;
    void* programbreak = sbrk(0);
    
    
    //如果要释放的模块位于链表中的最后一个，则在链表中删除最后一个内存块，并释放内存,否者该内存块只是标记一下为free
    if ((char*)block + header->size == programbreak)  {  //block is the last one in the list
        if (g_head == g_tail)  {     //only one block in the list
            g_head = g_tail = NULL;
        }
        else  {
            struct header_t* tmp = g_head;
            while(tmp)  {
                if (tmp->next == g_tail)  {
                    tmp->next = NULL;
                    g_tail = tmp;
                }
                tmp = tmp->next;
            }
        }
        sbrk(0 - header->size - sizeof(struct header_t));
    }
    else  {
        header->is_free = 1;
    }
}

