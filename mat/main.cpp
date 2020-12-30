#include "mat.h"
#include <iostream>

using namespace std;


template MAT<int>;			//用于实验四
template MAT<long long>;		//用于实验四
extern const char* TestMAT(int& s);	//用于实验四

int main() 					//请扩展main()测试其他运算
{
	/*
	MAT<int>   a(1, 2), b(2, 2), c(1, 2);
	char t[2048];
	a[0][0] = 1;			//类似地初始化矩阵的所有元素
	a[0][1] = 2; 			//等价于“*(a.operator[ ](0)+1)=2;”即等价于“*(a[0]+1)=2;”
	a.print(t);			//初始化矩阵后输出该矩阵
	cout << "matrix a" << endl;
	cout << t << endl;
	b[0][0] = 3; 	b[0][1] = 4;		//调用T* const operator[ ](int r)初始化数组元素
	b[1][0] = 5; 	b[1][1] = 6;
	b.print(t);
	cout << "matrix b" << endl;
	cout << t << endl;
	c = a * b;						//测试矩阵乘法运算
	c.print(t);
	cout << "matrix c=a*b" << endl;
	cout << t << endl;
	(a + c).print(t);					//测试矩阵加法运算
	cout << "matrix a+c" << endl;
	cout << t << endl;
	c = c - a;						//测试矩阵减法运算
	c.print(t);
	cout << "matrix c=c-a" << endl;
	cout << t << endl;
	c += a;							//测试矩阵“+=”运算
	cout << "matrix c+=a" << endl;
	c.print(t);
	cout << t << endl;
	c = ~a;							//测试矩阵转置运算
	cout << "matrix c=~a" << endl;
	c.print(t);
	cout << t << endl;
	*/
	int score;
	const char* info = TestMAT(score);
	cout << "--------mat--------" << endl;
	cout << info << "  " << score << endl;
	return 0;
}
