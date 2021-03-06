#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

typedef char ALIGN[16];

union header {
	struct {
		size_t size;
		unsigned is_free;
		union header *next;
	} s;
	ALIGN stub;
};
typedef union header header_t;    //使用union,整个header的字节数为16


header_t *head, *tail;            	//空闲链表的头和尾
pthread_mutex_t global_malloc_lock;	//使用互斥锁

header_t *get_free_block(size_t size);


void *malloc(size_t size)
{
	size_t total_size;
	void *block;
	header_t *header;
	if (!size)
		return NULL;
	pthread_mutex_lock(&global_malloc_lock);
	header = get_free_block(size);
	if (header) {
		header->s.is_free = 0;
		pthread_mutex_unlock(&global_malloc_lock);
		return (void*)(header + 1);  //地址向后加了16个字节
	}
	total_size = sizeof(header_t) + size;
	block = sbrk(total_size);
	if (block == (void*) -1) {
		pthread_mutex_unlock(&global_malloc_lock);
		return NULL;
	}
	header = block;
	header->s.size = size;
	header->s.is_free = 0;
	header->s.next = NULL;
	if (!head)
		head = header;
	if (tail)
		tail->s.next = header;
	tail = header;
	pthread_mutex_unlock(&global_malloc_lock);
	return (void*)(header + 1);
}

header_t *get_free_block(size_t size)
{
	header_t *curr = head;
	while(curr) {
		if (curr->s.is_free && curr->s.size >= size)
			return curr;
		curr = curr->s.next;
	}
	return NULL;
}



void free(void *block)
{
	header_t *header, *tmp;
	void *programbreak;

	if (!block)
		return;
	pthread_mutex_lock(&global_malloc_lock);
	header = (header_t*)block - 1;

	programbreak = sbrk(0);
	//如果当前所在的空闲块位于堆的顶部，那么直接将其释放给操作系统，
	if ((char*)block + header->s.size == programbreak) {
		//使用char*是为了能够加header->size个字节
		if (head == tail) {
			head = tail = NULL;  //更新head与tail变量
		} else {
			tmp = head;
			while (tmp) {
				if(tmp->s.next == tail) {
					tmp->s.next = NULL;
					tail = tmp; 
				}  //从空闲列表的头部开始循环，
				   //找到空闲列表的倒数第二个元素，更新tail变量
				tmp = tmp->s.next;
			}
		}
		//括号中的值为最后一个空闲块的实际大小，
		//取相反数使用sbrk来释放堆空间
		sbrk(0 - sizeof(header_t) - header->s.size);
		pthread_mutex_unlock(&global_malloc_lock);
		return;
	}
	header->s.is_free = 1;
	pthread_mutex_unlock(&global_malloc_lock);
}
