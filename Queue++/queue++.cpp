#include <malloc.h>
#include "queue++.h"


/**
* ��QUEUE(int m)�Զ��г�ʼ��ʱ�� Ϊ��elems����m������Ԫ���ڴ棬����ʼ��maxΪm���Լ���ʼ��head=tail=0
* 
* @param m ���������С
* 
*/
QUEUE::QUEUE(int m) :elems((int*)malloc(sizeof(int)* m)), max(m), head(0), tail(0) {};

/*
* ����QUEUE(const QUEUE& q)������캯��
* �����Ѿ����ڵĶ���q��������¶���ʱ
* �¶����ܹ����Ѿ����ڵĶ���q��elems�ڴ�
* �¶����elems��Ҫ�����qΪelems�����ͬ����С���ڴ�
* ���ҽ�q��elems������������¶��������ڴ�
* �¶����max��head��tailӦ���óɺ�q�Ķ�Ӧֵ��ͬ
* 
* @param q �Ӷ���q�����ʼ��
* 
*/
QUEUE::QUEUE(const QUEUE& q) :elems((int*)malloc(sizeof(int)* q.max)), max(q.max), head(q.head), tail(q.tail){
	if (elems == 0)
		return;
	memcpy(elems, q.elems, sizeof(int) * max);
}

/*
* ����QUEUE(QUEUE&& q)�ƶ����캯���������Ѿ����ڵĶ���q�ƶ������¶���ʱ
* �¶������ʹ�ö���qΪelems������ڴ�
* �����¶����max��head��tailӦ���óɺͶ���q�Ķ�Ӧֵ��ͬ
* Ȼ�����q��elems����Ϊ��ָ���Ա�ʾ�ڴ汻���ߣ�ͬʱ��max��head��tail��Ӧ����Ϊ0
* 
* @param q class Queue �Ӷ���q�ƶ���ʼ��
*/
QUEUE::QUEUE(QUEUE&& q)noexcept:elems(q.elems),max(q.max),head(q.head),tail(q.tail) {
	(int*&)(q.elems) = NULL;
	(int&)(q.max) = 0;
	q.head = 0;
	q.tail = 0;
}

/**
* ���ض�����ʵ��Ԫ�ظ���
* 
* @param *this
* 
* @returns ������ʵ��Ԫ�ظ���
*/
QUEUE::operator int() const noexcept {
	if (max == 0)
		return 0;
	return (max+tail-head)%max;
}

/**
* ��������Ĵ�С
* 
* @returns ��������Ĵ�С
*/
int QUEUE::size() const noexcept {
	return max;
}

/**
* ���Ԫ��e
* 
* @param e ���Ԫ��
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
* ����Ԫ�ص�����e
* 
* @param e �洢����Ԫ�صı���
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
* ����QUEUE& operator=(const QUEUE& q)�����ֵ����
* ���õȺ��ұߵĶ���q�����ֵ�Ⱥ���ߵĶ���ʱ���Ⱥ���ߵĶ�����Ϊelems�������ڴ�
* ��Ӧ���ͷ��ڴ��Ա����ڴ�й©���Ⱥ���ߵĶ����ܹ��ö���q��elems��ͬһ���ڴ�
* ӦΪ��elems�����qΪelems�����ͬ����С���ڴ�
* ���ҽ�q��elems�洢�����ݿ������Ⱥ���߶��������ڴ�
* �Ⱥ���߶����max��head��tailӦ���óɺ�q�Ķ�Ӧֵ��ͬ
* 
* @param q ��q�����ֵ����
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
* ����QUEUE& operator=(QUEUE&& q)noexcept�ƶ���ֵ����
* �����Ѿ����ڵĶ���q�ƶ���ֵ���Ⱥ���ߵĶ���ʱ���Ⱥ���ߵĶ�����Ϊelems�������ڴ�
* ��Ӧ���ͷ��ڴ��Ա����ڴ�й©���Ⱥ���ߵĶ������ʹ��qΪelems������ڴ�
* ���ҵȺ���ߵĶ����max��head��tailӦ���óɺͶ���q�Ķ�Ӧֵ��ͬ
* ����q��elemsȻ������Ϊ��ָ���Ա�ʾ�ڴ汻���ߣ�ͬʱ��max��head��tail��Ӧ��Ϊ0
* 
* @param q ��q�ƶ���ֵ����
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
* ��ӡ����ʱ�Ӷ��״�ӡ����β
* 
* @param s �洢��ӡ���ݵ�����
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
* �����������ͷŷ����ڴ�
* 
*/
QUEUE::~QUEUE() {
	free(elems);
	(int*&)elems = NULL;
	(int&)max = 0;
	head = 0;
	tail = 0;
}