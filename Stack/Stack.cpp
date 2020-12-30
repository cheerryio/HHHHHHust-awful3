
#include "Stack.h"
#include "queue.h"
#include "queue++.h"
#include "malloc.h"
#include "string.h"
#include <iostream>

using namespace std;

/**
* MindStorm:
* The two queue are both going to store elements as stack
* Combine them! Wrap up the stack inside you methods
* 
* I'm going take the inherited father as lower stack and s.q as upper stack
*/

/**
* 在用STACK(int m)对栈初始化时， 为其基类和成员q的elems分配m个整型元素内存
* 并初始化基类和成员q的max为m，以及初始化对应的head=tail=0
* 
* @param m 两个队列数组的大小
* 
*/
STACK::STACK(int m):q(m),QUEUE(m) {}

/**
* 对于STACK(const STACK& s)深拷贝构造函数，在用已经存在的对象s深拷贝构造新对象时
* 新对象不能共用s的基类和成员q为elems分配的内存
* 新对象要为其基类和成员q的elems分配和s为其基类和成员q的elems分配的同样大小的内存
* 并且将s相应的elems的内容深拷贝至新对象为对应elems分配的内存
* 新对象应设置其基类和成员q的max、head、tail和s的对应值相同
* 
* @param s 从s深拷贝初始化栈
* 
*/
STACK::STACK(const STACK& s):q(s.q), QUEUE(s) {}

/**
* 对于STACK(STACK&& s)noexcept移动拷贝构造函数
* 在用已经存在的对象s移动构造新对象时，新对象接受使用s为其基类和成员q的对应elems分配的内存
* 并且新对象的max、head、tail应和s的基类和成员q的对应值相同
* s的基类和成员q的elems设置为空指针以表示内存被移走，同时其对应的max、head、tail均应置为0
* 
* @param s 从s移动初始化栈
* 
*/
STACK::STACK(STACK&& s)noexcept:q((QUEUE &&)(s.q)), QUEUE((QUEUE &&)s) {}

/**
* 得到栈的容量，即两个队列数组的大小相加
* 
* @returns 栈的容量
*/
int STACK::size()const noexcept {
	return q.size()+QUEUE::size();
}

/**
* 栈中实际元素的个数
* 
* @param class Stack
* 
* @returns 栈实际元素个数
*/
STACK::operator int() const noexcept {
	return int(q) + QUEUE::operator int();
}

/**
* 将一个元素入栈
* 
* @param e 待入栈元素
* 
* @returns *this
*/
STACK& STACK::operator<<(int e) {
	if (QUEUE::operator int() < QUEUE::size() - 1)
		QUEUE::operator<<(e);
	else if (int(q) < q.size() - 1)
		q << e;
	else
		throw "STACK is full!";
	return *this;
}

/**
* 将一个元素出栈，保存在变量e中
* 
* @param e 存放出栈元素的变量
* 
* @returns *this
*/
STACK& STACK::operator>>(int& e) {
	int a, b;
	if (!int((STACK)(*this)))
		throw "STACK is empty!";
	for (int i = 0; i<int(*this) - 1; i++) {
		QUEUE::operator>>(a);
		if ((QUEUE *)(&q)->operator int()) {
			q >> b;
			QUEUE::operator<<(b);
			q << a;
		}
		else
			QUEUE::operator<<(a);
	}
	QUEUE::operator>>(e);
	if ((QUEUE*)(&q)->operator int()) {
		q >> a;
		QUEUE::operator<<(a);
	}
	return *this;
}

/*
* 对于STACK& operator=(const STACK& s)深拷贝赋值函数
* 在用等号右边的对象s深拷贝赋值等号左边的对象s时，等号左边对象的基类和成员q不能共用s的基类和成员q为elems分配的内存
* 若等号左边的对象为其基类和成员q的elems分配了内存，则应先释放掉以避免内存泄漏
* 然后为其elems分配和s为其基类和成员q的elems分配的同样大小的内存
* 并且将s对应两个elems的内容拷贝至等号左边对象对应两个elems的内存
* 等号左边对象中的max、head、tail应设置成和s中基类和成员q的对应值相同
* 
* @param s 从s深拷贝赋值到栈
* 
* @returns *this
*/
STACK& STACK::operator=(const STACK& s) {
	q = (QUEUE &)(s.q);
	QUEUE::operator=((QUEUE &)s);
	return *this;
}

/*
* 对于STACK& operator=(STACK&& s)noexcept移动赋值
* 在用等号右边的对象s移动赋值给等号左边的对象时
* 等号左边的对象如果已经为其基类和成员q中的elems分配了内存，则应先释放以避免内存泄漏
* 然后接受使用s的基类和成员q为elems分配的内存
* 并且等号左边对象中的max、head、tail应和s中基类和成员q中的对应值相同
* s中基类和成员q的elems设置为空指针以表示内存被移走，同时其对应的max、head、tail均应设置为0
* 
* @param s 从s移动赋值到栈
* 
* @returns *this
*/
STACK& STACK::operator=(STACK&& s)noexcept {
	q = (QUEUE&&)(s.q);
	QUEUE::operator=((QUEUE&&)s);
	return *this;
}

/* 1,2,3,*,*,*       */
/**
* 打印栈时从栈底打印到栈顶，打印的元素之间以逗号分隔
* 
* @param b 存放打印栈元素内容的数组
* 
* @returns s
*/
char* STACK::print(char* b)const noexcept {
	int a = 0;
	int counters = 0;
	int round;
	int lowerLen = QUEUE::operator int();
	int higherLen = int(q);
	int ret[100];
	for (int i = 0; i < lowerLen; i++) {
		((QUEUE *)(this))->QUEUE::operator>>(a);
		counters+=snprintf(b+counters, sizeof(b), "%d,", a);
		((QUEUE*)(this))->QUEUE::operator<<(a);
	}
	if (QUEUE::size() % lowerLen == 0)
		round = QUEUE::size() / lowerLen - 1;
	else
		round = QUEUE::size()-1;
	for (int i = 0; i < round; i++) {
		for (int m = 0; m < lowerLen; m++) {
			((QUEUE*)(this))->QUEUE::operator>>(a);
			((QUEUE*)(this))->QUEUE::operator<<(a);
		}
	}

	if (higherLen) {
		if (q.size() % higherLen == 0)
			round = q.size() / higherLen - 1;
		else
			round = q.size() - 1;
		for (int i = 0; i < higherLen - 1; i++) {
			((QUEUE*)(&q))->operator>>(a);
			counters += snprintf(b + counters, sizeof(b), "%d,", a);
			((QUEUE*)(&q))->operator<<(a);
		}
		if (higherLen) {
			((QUEUE*)(&q))->operator>>(a);
			counters += snprintf(b + counters, sizeof(b), "%d", a);
			((QUEUE*)(&q))->operator<<(a);
		}
		for (int i = 0; i < round; i++) {
			for (int m = 0; m < lowerLen; m++) {
				((QUEUE*)(&q))->operator>>(a);
				((QUEUE*)(&q))->operator<<(a);
			}
		}
	}
	return b;
}

STACK::~STACK()noexcept {
}