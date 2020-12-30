#include <malloc.h>
#include "queue++.h"

//��QUEUE(int m)�Զ��г�ʼ��ʱ�� Ϊ��elems����m������Ԫ���ڴ棬����ʼ��maxΪm���Լ���ʼ��head=tail=0��
QUEUE::QUEUE(int m) :elems((int*)malloc(sizeof(int)* m)), max(m), head(0), tail(0) {};

/*
* ����QUEUE(const QUEUE& q)������캯����
* �����Ѿ����ڵĶ���q��������¶���ʱ��
* �¶����ܹ����Ѿ����ڵĶ���q��elems�ڴ棬
* �¶����elems��Ҫ�����qΪelems�����ͬ����С���ڴ棬
* ���ҽ�q��elems������������¶��������ڴ棻
* �¶����max��head��tailӦ���óɺ�q�Ķ�Ӧֵ��ͬ��
*/
QUEUE::QUEUE(const QUEUE& q) :elems((int*)malloc(sizeof(int)* q.max)), max(q.max), head(q.head), tail(q.tail){
	if (elems == 0)
		return;
	memcpy(elems, q.elems, sizeof(int) * max);
}

/*
* ����QUEUE(QUEUE&& q)�ƶ����캯���������Ѿ����ڵĶ���q�ƶ������¶���ʱ��
* �¶������ʹ�ö���qΪelems������ڴ棬
* �����¶����max��head��tailӦ���óɺͶ���q�Ķ�Ӧֵ��ͬ��
* Ȼ�����q��elems����Ϊ��ָ���Ա�ʾ�ڴ汻���ߣ�ͬʱ��max��head��tail��Ӧ����Ϊ0��
*/
QUEUE::QUEUE(QUEUE&& q)noexcept:elems(q.elems),max(q.max),head(q.head),tail(q.tail) {
	(int*&)(q.elems) = NULL;
	(int&)(q.max) = 0;
	q.head = 0;
	q.tail = 0;
}

QUEUE::operator int() const noexcept {
	if (max == 0)
		return 0;
	return (max+tail-head)%max;
}

int QUEUE::size() const noexcept {
	return max;
}
QUEUE& QUEUE::operator<<(int e) {
	if (max == 0 || QUEUE::operator int() >= max - 1) {
		throw "QUEUE is full!";
		return *this;
	}
	elems[tail] = e;
	tail = (tail + 1) % max;
	return *this;
}
QUEUE& QUEUE::operator>>(int& e) {
	if (head == tail)
		throw "QUEUE is empty!";
	e = elems[head];
	head = (head + 1) % max;
	return *this;
}

/*
* ����QUEUE& operator=(const QUEUE& q)�����ֵ������
* ���õȺ��ұߵĶ���q�����ֵ�Ⱥ���ߵĶ���ʱ���Ⱥ���ߵĶ�����Ϊelems�������ڴ棬
* ��Ӧ���ͷ��ڴ��Ա����ڴ�й©���Ⱥ���ߵĶ����ܹ��ö���q��elems��ͬһ���ڴ棬
* ӦΪ��elems�����qΪelems�����ͬ����С���ڴ棬
* ���ҽ�q��elems�洢�����ݿ������Ⱥ���߶��������ڴ棻
* �Ⱥ���߶����max��head��tailӦ���óɺ�q�Ķ�Ӧֵ��ͬ��
**/
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

/*
* ����QUEUE& operator=(QUEUE&& q)noexcept�ƶ���ֵ������
* �����Ѿ����ڵĶ���q�ƶ���ֵ���Ⱥ���ߵĶ���ʱ���Ⱥ���ߵĶ�����Ϊelems�������ڴ棬
* ��Ӧ���ͷ��ڴ��Ա����ڴ�й©���Ⱥ���ߵĶ������ʹ��qΪelems������ڴ棬
* ���ҵȺ���ߵĶ����max��head��tailӦ���óɺͶ���q�Ķ�Ӧֵ��ͬ��
* ����q��elemsȻ������Ϊ��ָ���Ա�ʾ�ڴ汻���ߣ�ͬʱ��max��head��tail��Ӧ��Ϊ0��
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

//��ӡ����ʱ�Ӷ��״�ӡ����β
char* QUEUE::print(char* s) const noexcept {
	int i = 0;
	int counters = 0;
	for (i = head; i != tail; i=(i+1)%max) {
		counters += snprintf(s+counters, sizeof(s),"%d ", elems[i]);
	}
	return s;
}

QUEUE::~QUEUE() {
	free(elems);
	(int*&)elems = NULL;
	(int&)max = 0;
	head = 0;
	tail = 0;
}