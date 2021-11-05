#include <unistd.h>
#include <stdio.h>

#ifndef HEADER
#include"header.h"
#endif
//使用单链表进行管理
 
 
struct header_t* get_free_block(size_t size)  {
    struct header_t* curr = g_head;
    while(curr)  {
        if (curr->is_free && curr->size >= size)  {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}
 
 
void* malloc(size_t size)  {
    if (!size)  {
        return NULL;
    }
    
    struct header_t* header = get_free_block(size);
    //首先尝试寻找空闲空间
    if (header)  {
        header->is_free = 0;
        return (void*)(header + 1);
    }
    
    //没有找到空闲空间，增加堆空间
    size_t total_size = sizeof(struct header_t) + size;
    void* block = sbrk(total_size);    //返回的是之前栈顶的值
    if (block == (void*)-1)  {
        return NULL;
    }
    
    header = block;
    header->size = size;
    header->is_free = 0;
    header->next = NULL;
    
    if (g_head == NULL)  {
        g_head = header;
    }
    if (g_tail)  {
        g_tail->next = header;
    }
    
    g_tail = header;
    return (void*)(header + 1);
}
