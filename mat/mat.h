#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iomanip> 
#include <exception>
#include <string.h>
#include <iostream>

using namespace std;

template <typename T>
class MAT {
	T* e;							//指向所有整型矩阵元素的指针
	const int r, c;							//矩阵的行r和列c大小
public:
	MAT(int r, int c);						//矩阵定义
	MAT(const MAT& a);				//深拷贝构造
	MAT(MAT&& a)noexcept;			//移动构造
	virtual ~MAT()noexcept;
	virtual T* const operator[ ](int r);//取矩阵r行的第一个元素地址，r越界抛异常
	virtual MAT operator+(const MAT& a)const;	//矩阵加法，不能加抛异常
	virtual MAT operator-(const MAT& a)const;	//矩阵减法，不能减抛异常
	virtual MAT operator*(const MAT& a)const;	//矩阵乘法，不能乘抛异常
	virtual MAT operator~()const;					//矩阵转置
	virtual MAT& operator=(const MAT& a);		//深拷贝赋值运算
	virtual MAT& operator=(MAT&& a)noexcept;	//移动赋值运算
	virtual MAT& operator+=(const MAT& a);		//“+=”运算
	virtual MAT& operator-=(const MAT& a);		//“-=”运算
	virtual MAT& operator*=(const MAT& a);		//“*=”运算
//print输出至s并返回s：列用空格隔开，行用回车结束
	virtual char* print(char* s)const noexcept;
};

/**
* 初始化矩阵行数列数，分配内存空间
* 
* @param r 矩阵行数
* @param c 矩阵列数
* 
*/
template<typename T>
MAT<T>::MAT<T>(int r, int c) :e((T*)malloc(sizeof(T)*r*c)), r(r), c(c) {}

/**
* 用矩阵a深拷贝初始化矩阵
* 
* @param a 用来被深拷贝的矩阵
* 
*/
template<typename T>
MAT<T>::MAT<T>(const MAT& a):e((T*)malloc(sizeof(T)*(a.r)*(a.c))), r(a.r), c(a.c) {
	memcpy(e, a.e, sizeof(T) * a.r * a.c);
}

/**
* 用矩阵a移动初始化矩阵
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
* 析构函数
* 
*/
template<typename T>
MAT<T>::~MAT()noexcept {}

/**
* 重载[]运算符，返回第r行首地址
* 
* @param r 返回首地址的行数
* 
* @returns r行首地址指针
*/
template<typename T>
T* const MAT<T>::operator[](int r) {
	if (r < 0 || r >= this->r)
		throw "line out!";
	return e+r*c;
}

/**
* 重载+运算符，实现两个矩阵的加法
* 
* @param a 加矩阵
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
* 重载-运算符，实现两个矩阵的减法
* 
* @param a 减矩阵
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
* 重载*运算符，实现两个矩阵的乘法
* 
* @param a 乘数
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
* 实现矩阵转置
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
* 深拷贝赋值矩阵
* 
* @param a 从a深拷贝赋值矩阵
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
* 移动赋值矩阵
* 
* @param a 从a移动赋值矩阵
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
* 矩阵+=
* 
* @param a 加矩阵
* 
* @returns *this
*/
template<typename T>
MAT<T>& MAT<T>::operator+=(const MAT& a) {
	*this=*this + a;
	return *this;
}

/**
* 矩阵-=
* 
* @param a 减矩阵
* 
* @returns *this
*/
template<typename T>
MAT<T>& MAT<T>::operator-=(const MAT& a) {
	*this = *this - a;
	return *this;
}

/**
* 矩阵*=
* 
* @param a 乘矩阵
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
* 打印输出矩阵元素的内容
* 
* @param s 打印内容存放的数组
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