#include "queue.h"
#include "malloc.h"

/*
* ��initQueue(Queue *const p, int m)��pָ��Ķ��г�ʼ��ʱ�� 
* Ϊ��elems����m������Ԫ���ڴ棬����ʼ��maxΪm���Լ���ʼ��head=tail=0��
*/
void initQueue(Queue* const p, int m) {
	(int *&)(p->elems) = (int *)malloc(sizeof(int) * m);
	(int&)(p->max) = m;
	p->head = 0;
	p->tail = 0;
	return;

}

/*
* ����initQueue(Queue *const p, const Queue& q)��ʼ����
* ���Ѿ����ڵĶ���q��������¶���*pʱ���¶���*p���ܺͶ���q��elems����ͬһ���ڴ棬
* �¶���*p��elems��Ҫ�����qΪelems�����ͬ����С���ڴ棬
* ���ҽ��Ѿ�����q��elems������������·�����ڴ棻
* �¶���*p��max��head��tailӦ���óɺ��Ѿ����ڵĶ���s��ͬ��
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
* ����initQueue(Queue *const p, Queue&& q)��ʼ����
* ���Ѿ����ڵĶ���q�ƶ������¶����¶���ʹ�ö���qΪelems������ڴ�죬
* ������max��head��tail���óɺ�s�Ķ�Ӧֵ��ͬ��Ȼ��s��elems����Ϊ�ձ�ʾ�ڴ��Ѿ��ƶ����¶���
* ��s��max��head��tail����Ϊ0��
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

//����pָ���е�ʵ��Ԫ�ظ���
int  number(const Queue* const p) {
	if (p->max == 0)
		return 0;
	return (p->max+p->tail - p->head)%p->max;
}

//����pָ������������Ԫ�ظ���max
int  size(const Queue* const p) {
	return p->max;
}

//��e�����β����������p
Queue* const enter(Queue* const p, int e) {
	if (number(p) == p->max - 1) {
		throw "Queue is full!";
		return p;
	}
	p->elems[p->tail] = e;
	p->tail =(p->tail+1)%p->max;
	return p;
}

//�Ӷ��׳�Ԫ�ص�e��������p
Queue* const leave(Queue* const p, int& e) {
	if (p->head == p->tail)
		throw "Queue is empty!";
	e = p->elems[p->head];
	p->head = (p->head + 1) % p->max;
	return p;
}

/*
* ����Queue*const assign(Queue*const p, const Queue&q)�����ֵ��
* �õȺ��ұߵĶ���q�����ֵ���Ⱥ���ߵĶ���
* �Ⱥ���ߵĶ�������Ѿ����ڴ���Ӧ���ͷ��Ա����ڴ�й©��
* Ȼ�����Ͷ���qΪelems�����ͬ����С���ڴ棬
* ����������max��head��tail��q�Ķ�Ӧֵ��ͬ��
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
* ����Queue*const assign(Queue*const p, Queue&&q)�ƶ���ֵ��
* ���Ⱥ���ߵĶ���Ϊelems�������ڴ棬�����ͷŸ��ڴ������ڴ�й©��
* Ȼ��ʹ�õȺ��ұ߶���qΪelems������ڴ棬������max��head��tail���óɺͶ���q�Ķ�Ӧֵ��ͬ��
* ����q��elems����Ϊ��ָ���Ա�ʾ�ڴ汻���߸��Ⱥ���ߵĶ���ͬʱ��max��head��tail��Ӧ����Ϊ0��
*/
Queue* const assign(Queue* const p, Queue&& q) {
	if (p == &q)
		return p;
	if (p->elems != NULL)
		free(p->elems);
	initQueue(p,(Queue &&)q);
	return p;
}

//��ӡpָ������sβ��������s��s[0]=��\0��
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

//����pָ��Ķ���
void destroyQueue(Queue* const p) {
	free(p->elems);
	(int *&)(p->elems) = NULL;
	(int &)p->max = 0;
	p->head = 0;
	p->tail = 0;
	return;
}