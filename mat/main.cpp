#include "mat.h"
#include <iostream>

using namespace std;


template MAT<int>;			//����ʵ����
template MAT<long long>;		//����ʵ����
extern const char* TestMAT(int& s);	//����ʵ����

int main() 					//����չmain()������������
{
	/*
	MAT<int>   a(1, 2), b(2, 2), c(1, 2);
	char t[2048];
	a[0][0] = 1;			//���Ƶس�ʼ�����������Ԫ��
	a[0][1] = 2; 			//�ȼ��ڡ�*(a.operator[ ](0)+1)=2;�����ȼ��ڡ�*(a[0]+1)=2;��
	a.print(t);			//��ʼ�����������þ���
	cout << "matrix a" << endl;
	cout << t << endl;
	b[0][0] = 3; 	b[0][1] = 4;		//����T* const operator[ ](int r)��ʼ������Ԫ��
	b[1][0] = 5; 	b[1][1] = 6;
	b.print(t);
	cout << "matrix b" << endl;
	cout << t << endl;
	c = a * b;						//���Ծ���˷�����
	c.print(t);
	cout << "matrix c=a*b" << endl;
	cout << t << endl;
	(a + c).print(t);					//���Ծ���ӷ�����
	cout << "matrix a+c" << endl;
	cout << t << endl;
	c = c - a;						//���Ծ����������
	c.print(t);
	cout << "matrix c=c-a" << endl;
	cout << t << endl;
	c += a;							//���Ծ���+=������
	cout << "matrix c+=a" << endl;
	c.print(t);
	cout << t << endl;
	c = ~a;							//���Ծ���ת������
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
