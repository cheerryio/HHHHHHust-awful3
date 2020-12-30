#include <malloc.h>
#include "queue++.h"


/**
* 用QUEUE(int m)对队列初始化时， 为其elems分配m个整型元素内存，并初始化max为m，以及初始化head=tail=0
* 
* @param m 队列数组大小
* 
*/
QUEUE::QUEUE(int m) :elems((int*)malloc(sizeof(int)* m)), max(m), head(0), tail(0) {};

/*
* 对于QUEUE(const QUEUE& q)深拷贝构造函数
* 在用已经存在的对象q深拷贝构造新对象时
* 新对象不能共用已经存在的对象q的elems内存
* 新对象的elems需要分配和q为elems分配的同样大小的内存
* 并且将q的elems的内容深拷贝至新对象分配的内存
* 新对象的max、head、tail应设置成和q的对应值相同
* 
* @param q 从队列q深拷贝初始化
* 
*/
QUEUE::QUEUE(const QUEUE& q) :elems((int*)malloc(sizeof(int)* q.max)), max(q.max), head(q.head), tail(q.tail){
	if (elems == 0)
		return;
	memcpy(elems, q.elems, sizeof(int) * max);
}

/*
* 对于QUEUE(QUEUE&& q)移动构造函数，在用已经存在的对象q移动构造新对象时
* 新对象接受使用对象q为elems分配的内存
* 并且新对象的max、head、tail应设置成和对象q的对应值相同
* 然后对象q的elems设置为空指针以表示内存被移走，同时其max、head、tail均应设置为0
* 
* @param q class Queue 从队列q移动初始化
*/
QUEUE::QUEUE(QUEUE&& q)noexcept:elems(q.elems),max(q.max),head(q.head),tail(q.tail) {
	(int*&)(q.elems) = NULL;
	(int&)(q.max) = 0;
	q.head = 0;
	q.tail = 0;
}

/**
* 返回队列中实际元素个数
* 
* @param *this
* 
* @returns 队列中实际元素个数
*/
QUEUE::operator int() const noexcept {
	if (max == 0)
		return 0;
	return (max+tail-head)%max;
}

/**
* 队列数组的大小
* 
* @returns 队列数组的大小
*/
int QUEUE::size() const noexcept {
	return max;
}

/**
* 入队元素e
* 
* @param e 入队元素
* 
* @returns *this
*/
QUEUE& QUEUE::operator<<(int e) {
	if (max == 0 || int(*this) >= max - 1)
		throw "QUEUE is full!";
	elems[tail] = e;
	tail = (tail + 1) % max;
	return *this;
}

/**
* 出队元素到变量e
* 
* @param e 存储出队元素的变量
* 
* @returns *this
*/
QUEUE& QUEUE::operator>>(int& e) {
	if (head == tail)
		throw "QUEUE is empty!";
	e = elems[head];
	head = (head + 1) % max;
	return *this;
}

/**
* 对于QUEUE& operator=(const QUEUE& q)深拷贝赋值函数
* 在用等号右边的对象q深拷贝赋值等号左边的对象时，等号左边的对象若为elems分配了内存
* 则应先释放内存以避免内存泄漏。等号左边的对象不能共用对象q的elems的同一块内存
* 应为其elems分配和q为elems分配的同样大小的内存
* 并且将q的elems存储的内容拷贝至等号左边对象分配的内存
* 等号左边对象的max、head、tail应设置成和q的对应值相同
* 
* @param q 从q深拷贝赋值队列
* 
* @returns *this
*/
QUEUE& QUEUE::operator=(const QUEUE& q) {
	if (this == &q)
		return *this;
	if (elems != NULL)
		free(elems);
	void* p = malloc(sizeof(int) * q.max);
	if (p == 0)
		return *this;
	memcpy(p, q.elems, sizeof(int) * q.max);
	(int*&)elems = (int*)p;
	(int&)max = q.max;
	head = q.head;
	tail = q.tail;
	return *this;
}

/**
* 对于QUEUE& operator=(QUEUE&& q)noexcept移动赋值函数
* 在用已经存在的对象q移动赋值给等号左边的对象时，等号左边的对象若为elems分配了内存
* 则应先释放内存以避免内存泄漏。等号左边的对象接受使用q为elems分配的内存
* 并且等号左边的对象的max、head、tail应设置成和对象q的对应值相同
* 对象q的elems然后设置为空指针以表示内存被移走，同时其max、head、tail均应置为0
* 
* @param q 从q移动赋值队列
* 
* @returns *this
*/
QUEUE& QUEUE::operator=(QUEUE&& q)noexcept {
	if (this == &q)
		return *this;
	if (elems != NULL)
		free(elems);
	(int*&)elems = q.elems;
	(int&)max = q.max;
	head = q.head;
	tail = q.tail;
	(int*&)(q.elems) = NULL;
	(int&)(q.max) = 0;
	q.head = 0;
	q.tail = 0;
	return *this;
}

/**
* 打印队列时从队首打印至队尾
* 
* @param s 存储打印内容的数组
* 
* @returns s
*/
char* QUEUE::print(char* s) const noexcept {
	int i = 0;
	int counters = 0;
	for (i = head; i != tail; i=(i+1)%max) {
		counters += snprintf(s+counters, sizeof(s),"%d ", elems[i]);
	}
	return s;
}

/**
* 析构函数，释放分配内存
* 
*/
QUEUE::~QUEUE() {
	free(elems);
	(int*&)elems = NULL;
	(int&)max = 0;
	head = 0;
	tail = 0;
}