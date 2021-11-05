#define HEADER
#include<stdio.h>

struct header_t  {
    size_t size;               //8B
    unsigned is_free;          //4B
    struct header_t* next;     //8B
};
struct header_t* g_head = NULL;
struct header_t* g_tail = NULL;         //堆的头尾指针
