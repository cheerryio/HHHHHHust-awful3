#include "queue.h"
#include "malloc.h"

/*
* 用initQueue(Queue *const p, int m)对p指向的队列初始化时， 
* 为其elems分配m个整型元素内存，并初始化max为m，以及初始化head=tail=0。
*/
void initQueue(Queue* const p, int m) {
	(int *&)(p->elems) = (int *)malloc(sizeof(int) * m);
	(int&)(p->max) = m;
	p->head = 0;
	p->tail = 0;
	return;

}

/*
* 对于initQueue(Queue *const p, const Queue& q)初始化，
* 用已经存在的对象q深拷贝构造新对象*p时，新对象*p不能和对象q的elems共用同一块内存，
* 新对象*p的elems需要分配和q为elems分配的同样大小的内存，
* 并且将已经存在q的elems的内容深拷贝至新分配的内存；
* 新对象*p的max、head、tail应设置成和已经存在的对象s相同。
*/
void initQueue(Queue* const p, const Queue& q) {
	(int&)(p->max) = q.max;
	p->head = q.head;
	p->tail = q.tail;
	void* mem = (void*)malloc(sizeof(int) * q.max);
	memcpy(mem, q.elems, sizeof(int) * q.max);
	(int*&)(p->elems) = (int *)mem;
	return;
}

/*
* 对于initQueue(Queue *const p, Queue&& q)初始化，
* 用已经存在的对象q移动构造新对象，新对象使用对象q为elems分配的内存快，
* 并将其max、head、tail设置成和s的对应值相同，然后将s的elems设置为空表示内存已经移动给新对象，
* 将s的max、head、tail设置为0。
*/
void initQueue(Queue* const p, Queue&& q) {
	(int*&)(p->elems) = q.elems;
	(int&)(p->max) = q.max;
	p->head = q.head;
	p->tail = q.tail;
	(int *&)q.elems = NULL;
	(int &)q.max = 0;
	q.head = 0;
	q.tail = 0;
	return;
}

//返回p指队列的实际元素个数
int  number(const Queue* const p) {
	if (p->max == 0)
		return 0;
	return (p->max+p->tail - p->head)%p->max;
}

//返回p指队列申请的最大元素个数max
int  size(const Queue* const p) {
	return p->max;
}

//将e入队列尾部，并返回p
Queue* const enter(Queue* const p, int e) {
	if (number(p) == p->max - 1) {
		throw "Queue is full!";
		return p;
	}
	p->elems[p->tail] = e;
	p->tail =(p->tail+1)%p->max;
	return p;
}

//从队首出元素到e，并返回p
Queue* const leave(Queue* const p, int& e) {
	if (p->head == p->tail)
		throw "Queue is empty!";
	e = p->elems[p->head];
	p->head = (p->head + 1) % p->max;
	return p;
}

/*
* 对于Queue*const assign(Queue*const p, const Queue&q)深拷贝赋值，
* 用等号右边的对象q深拷贝赋值给等号左边的对象，
* 等号左边的对象如果已经有内存则应先释放以避免内存泄漏，
* 然后分配和对象q为elems分配的同样大小的内存，
* 并且设置其max、head、tail和q的对应值相同。
*/
Queue* const assign(Queue* const p, const Queue& q) {
	if (p == &q)
		return p;
	if (p->elems != NULL)
		free(p->elems);
	initQueue(p, q);
	return p;
}

/*
* 对于Queue*const assign(Queue*const p, Queue&&q)移动赋值，
* 若等号左边的对象为elems分配了内存，则先释放改内存以免内存泄漏，
* 然后使用等号右边对象q为elems分配的内存，并将其max、head、tail设置成和对象q的对应值相同；
* 对象q的elems设置为空指针以表示内存被移走给等号左边的对象，同时其max、head、tail均应设置为0。
*/
Queue* const assign(Queue* const p, Queue&& q) {
	if (p == &q)
		return p;
	if (p->elems != NULL)
		free(p->elems);
	initQueue(p,(Queue &&)q);
	return p;
}

//打印p指队列至s尾部并返回s：s[0]=’\0’
char* print(const Queue* const p, char* s) {
	int i = 0;
	int counter = 0;
	int offset = 0;
	for(offset=0;offset<number(p);offset++) {
		i = p->elems[(p->head + offset)%p->max];
		counter+=sprintf(s+counter, "%d ", i);
	}
	return s;
}

//销毁p指向的队列
void destroyQueue(Queue* const p) {
	free(p->elems);
	(int *&)(p->elems) = NULL;
	(int &)p->max = 0;
	p->head = 0;
	p->tail = 0;
	return;
}