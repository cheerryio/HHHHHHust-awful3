#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iomanip> 
#include <exception>
#include <string.h>
#include <iostream>

using namespace std;

template <typename T>
class MAT {
	T* e;							//ָ���������;���Ԫ�ص�ָ��
	const int r, c;							//�������r����c��С
public:
	MAT(int r, int c);						//������
	MAT(const MAT& a);				//�������
	MAT(MAT&& a)noexcept;			//�ƶ�����
	virtual ~MAT()noexcept;
	virtual T* const operator[ ](int r);//ȡ����r�еĵ�һ��Ԫ�ص�ַ��rԽ�����쳣
	virtual MAT operator+(const MAT& a)const;	//����ӷ������ܼ����쳣
	virtual MAT operator-(const MAT& a)const;	//������������ܼ����쳣
	virtual MAT operator*(const MAT& a)const;	//����˷������ܳ����쳣
	virtual MAT operator~()const;					//����ת��
	virtual MAT& operator=(const MAT& a);		//�����ֵ����
	virtual MAT& operator=(MAT&& a)noexcept;	//�ƶ���ֵ����
	virtual MAT& operator+=(const MAT& a);		//��+=������
	virtual MAT& operator-=(const MAT& a);		//��-=������
	virtual MAT& operator*=(const MAT& a);		//��*=������
//print�����s������s�����ÿո���������ûس�����
	virtual char* print(char* s)const noexcept;
};

/**
* ��ʼ���������������������ڴ�ռ�
* 
* @param r ��������
* @param c ��������
* 
*/
template<typename T>
MAT<T>::MAT<T>(int r, int c) :e((T*)malloc(sizeof(T)*r*c)), r(r), c(c) {}

/**
* �þ���a�����ʼ������
* 
* @param a ����������ľ���
* 
*/
template<typename T>
MAT<T>::MAT<T>(const MAT& a):e((T*)malloc(sizeof(T)*(a.r)*(a.c))), r(a.r), c(a.c) {
	memcpy(e, a.e, sizeof(T) * a.r * a.c);
}

/**
* �þ���a�ƶ���ʼ������
* 
* @param a
* 
*/
template<typename T>
MAT<T>::MAT<T>(MAT&& a)noexcept :e(a.e),r(a.r),c(a.c) {
	(T*&)(a.e) = NULL;
	(int &)(a.r) = 0;
	(int &)(a.c) = 0;
}

/**
* ��������
* 
*/
template<typename T>
MAT<T>::~MAT()noexcept {}

/**
* ����[]����������ص�r���׵�ַ
* 
* @param r �����׵�ַ������
* 
* @returns r���׵�ַָ��
*/
template<typename T>
T* const MAT<T>::operator[](int r) {
	if (r < 0 || r >= this->r)
		throw "line out!";
	return e+r*c;
}

/**
* ����+�������ʵ����������ļӷ�
* 
* @param a �Ӿ���
* 
* @returns *this
*/
template<typename T>
MAT<T> MAT<T>::operator+(const MAT& a)const {
	if (r != a.r || c != a.c)
		throw "cant add!";
	MAT<T> ret(r, c);
	for (int i = 0; i < r * c; i++)
		(ret.e)[i] = e[i] + (a.e)[i];
	return ret;
}

/**
* ����-�������ʵ����������ļ���
* 
* @param a ������
* 
* @returns *this
*/
template<typename T>
MAT<T> MAT<T>::operator-(const MAT& a)const {
	if (r != a.r || c != a.c)
		throw "cant sub!";
	MAT<T> ret(r, c);
	for (int i = 0; i < r * c; i++)
		(ret.e)[i] = e[i] - (a.e)[i];
	return ret;
}

/**
* ����*�������ʵ����������ĳ˷�
* 
* @param a ����
* 
* @returns *this
*/
template<typename T>
MAT<T> MAT<T>::operator*(const MAT& a)const {
	if (c != a.r)
		throw "cant mul!";
	MAT<T> ret(r, a.c);
	for (int i1 = 0; i1 < r; i1++) {
		for (int i2 = 0; i2 < a.c; i2++) {
			int val = 0;
			for (int i3 = 0; i3 < c; i3++) {
				val += e[i1 * c + i3] * (a.e)[i2 + i3 * c];
			}
			ret[i1][i2] = val;
		}
	}
	return ret;
}

/**
* ʵ�־���ת��
* 
* @returns *this
*/
/*   1 2 3 4
*    5 6 7 8
*    9 1 2 3
*/
template<typename T>
MAT<T> MAT<T>::operator~()const {
	MAT<T> ret(c,r);
	for (int i1 = 0; i1 < r; i1++) {
		for (int i2 = 0; i2 < c; i2++) {
			(ret.e)[i2*r+i1] = e[i1 * c + i2];
		}
	}
	return ret;
}

/**
* �����ֵ����
* 
* @param a ��a�����ֵ����
* 
* @returns *this
*/
template<typename T>
MAT<T>& MAT<T>::operator=(const MAT& a) {
	if (this == &a)
		return *this;
	if (r != a.r || c != a.c)
		throw "cant deep copy!";
	if (e)
		free(e);
	T* p = (T*)malloc(sizeof(T) * r * c);
	memcpy(p, a.e, sizeof(T) * r * c);
	(T*&)e = p;
	(int&)r = a.r;
	(int&)c = a.c;
	return *this;
}

/**
* �ƶ���ֵ����
* 
* @param a ��a�ƶ���ֵ����
* 
* @returns *this
*/
template<typename T>
MAT<T>& MAT<T>::operator=(MAT&& a)noexcept {
	if (this == &a)
		return *this;
	if (e != NULL)
		free(e);
	(T*&)e = a.e;
	(int&)r = a.r;
	(int&)c = a.c;
	(T*&)(a.e) = NULL;
	(int&)(a.r) = 0;
	(int&)(a.c) = 0;
	return *this;
}

/**
* ����+=
* 
* @param a �Ӿ���
* 
* @returns *this
*/
template<typename T>
MAT<T>& MAT<T>::operator+=(const MAT& a) {
	*this=*this + a;
	return *this;
}

/**
* ����-=
* 
* @param a ������
* 
* @returns *this
*/
template<typename T>
MAT<T>& MAT<T>::operator-=(const MAT& a) {
	*this = *this - a;
	return *this;
}

/**
* ����*=
* 
* @param a �˾���
* 
* @returns *this
*/
template<typename T>
MAT<T>& MAT<T>::operator*=(const MAT& a) {
	*this = (*this) * a;
	return *this;
}

int printMat(char* s, int number, int counter) {
	counter += sprintf(s+counter, "%6ld ", number);
	return counter;
};

int printMat(char* s, long long number, int counter) {
	counter += sprintf(s + counter, "%6lld ", number);
	return counter;
};

int printMat(char* s, float number, int counter) {
	counter += sprintf(s + counter, "%8f ", number);
	return counter;
};

int printMat(char* s, long float number, int counter) {
	counter += sprintf(s + counter, "%8lf ", number);
	return counter;
};

/**
* ��ӡ�������Ԫ�ص�����
* 
* @param s ��ӡ���ݴ�ŵ�����
* 
* @returns s
*/
template<typename T>
char* MAT<T>::print(char* s)const noexcept {
	int counter = 0;
	char LF = '\n';
	for (int i1 = 0; i1 < r; i1++) {
		for (int i2 = 0; i2 < c; i2++) {
			counter=printMat(s, e[i1 * c + i2], counter);
		}
		counter += sprintf(s + counter, "%c",'\n');
	}
	return s;
}