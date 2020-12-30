
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
* ����STACK(int m)��ջ��ʼ��ʱ�� Ϊ�����ͳ�Աq��elems����m������Ԫ���ڴ�
* ����ʼ������ͳ�Աq��maxΪm���Լ���ʼ����Ӧ��head=tail=0
* 
* @param m ������������Ĵ�С
* 
*/
STACK::STACK(int m):q(m),QUEUE(m) {}

/**
* ����STACK(const STACK& s)������캯���������Ѿ����ڵĶ���s��������¶���ʱ
* �¶����ܹ���s�Ļ���ͳ�ԱqΪelems������ڴ�
* �¶���ҪΪ�����ͳ�Աq��elems�����sΪ�����ͳ�Աq��elems�����ͬ����С���ڴ�
* ���ҽ�s��Ӧ��elems������������¶���Ϊ��Ӧelems������ڴ�
* �¶���Ӧ���������ͳ�Աq��max��head��tail��s�Ķ�Ӧֵ��ͬ
* 
* @param s ��s�����ʼ��ջ
* 
*/
STACK::STACK(const STACK& s):q(s.q), QUEUE(s) {}

/**
* ����STACK(STACK&& s)noexcept�ƶ��������캯��
* �����Ѿ����ڵĶ���s�ƶ������¶���ʱ���¶������ʹ��sΪ�����ͳ�Աq�Ķ�Ӧelems������ڴ�
* �����¶����max��head��tailӦ��s�Ļ���ͳ�Աq�Ķ�Ӧֵ��ͬ
* s�Ļ���ͳ�Աq��elems����Ϊ��ָ���Ա�ʾ�ڴ汻���ߣ�ͬʱ���Ӧ��max��head��tail��Ӧ��Ϊ0
* 
* @param s ��s�ƶ���ʼ��ջ
* 
*/
STACK::STACK(STACK&& s)noexcept:q((QUEUE &&)(s.q)), QUEUE((QUEUE &&)s) {}

/**
* �õ�ջ����������������������Ĵ�С���
* 
* @returns ջ������
*/
int STACK::size()const noexcept {
	return q.size()+QUEUE::size();
}

/**
* ջ��ʵ��Ԫ�صĸ���
* 
* @param class Stack
* 
* @returns ջʵ��Ԫ�ظ���
*/
STACK::operator int() const noexcept {
	return int(q) + QUEUE::operator int();
}

/**
* ��һ��Ԫ����ջ
* 
* @param e ����ջԪ��
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
* ��һ��Ԫ�س�ջ�������ڱ���e��
* 
* @param e ��ų�ջԪ�صı���
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
* ����STACK& operator=(const STACK& s)�����ֵ����
* ���õȺ��ұߵĶ���s�����ֵ�Ⱥ���ߵĶ���sʱ���Ⱥ���߶���Ļ���ͳ�Աq���ܹ���s�Ļ���ͳ�ԱqΪelems������ڴ�
* ���Ⱥ���ߵĶ���Ϊ�����ͳ�Աq��elems�������ڴ棬��Ӧ���ͷŵ��Ա����ڴ�й©
* Ȼ��Ϊ��elems�����sΪ�����ͳ�Աq��elems�����ͬ����С���ڴ�
* ���ҽ�s��Ӧ����elems�����ݿ������Ⱥ���߶����Ӧ����elems���ڴ�
* �Ⱥ���߶����е�max��head��tailӦ���óɺ�s�л���ͳ�Աq�Ķ�Ӧֵ��ͬ
* 
* @param s ��s�����ֵ��ջ
* 
* @returns *this
*/
STACK& STACK::operator=(const STACK& s) {
	q = (QUEUE &)(s.q);
	QUEUE::operator=((QUEUE &)s);
	return *this;
}

/*
* ����STACK& operator=(STACK&& s)noexcept�ƶ���ֵ
* ���õȺ��ұߵĶ���s�ƶ���ֵ���Ⱥ���ߵĶ���ʱ
* �Ⱥ���ߵĶ�������Ѿ�Ϊ�����ͳ�Աq�е�elems�������ڴ棬��Ӧ���ͷ��Ա����ڴ�й©
* Ȼ�����ʹ��s�Ļ���ͳ�ԱqΪelems������ڴ�
* ���ҵȺ���߶����е�max��head��tailӦ��s�л���ͳ�Աq�еĶ�Ӧֵ��ͬ
* s�л���ͳ�Աq��elems����Ϊ��ָ���Ա�ʾ�ڴ汻���ߣ�ͬʱ���Ӧ��max��head��tail��Ӧ����Ϊ0
* 
* @param s ��s�ƶ���ֵ��ջ
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
* ��ӡջʱ��ջ�״�ӡ��ջ������ӡ��Ԫ��֮���Զ��ŷָ�
* 
* @param b ��Ŵ�ӡջԪ�����ݵ�����
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