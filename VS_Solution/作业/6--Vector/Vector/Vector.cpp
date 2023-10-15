#pragma warning(disable:4996)
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <iostream>
#include <Windows.h>
using namespace std;
//������ɫ
static void SetColor(string color) {
	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
	if (color == "Red")SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_INTENSITY);
	else if (color == "Blue")SetConsoleTextAttribute(hdl, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	else if (color == "Green")SetConsoleTextAttribute(hdl, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	else if (color == "Yellow")SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}
static ostream& red(ostream& os) {
	SetColor("Red");
	return os;
}
static ostream& blue(ostream& os) {
	SetColor("Blue");
	return os;
}
static ostream& yellow(ostream& os) {
	SetColor("Yellow");
	return os;
}

template <class T>
class Vector {
private:
	//�����е�Ԫ�ظ���
	int m_nSize;
	//�������������
	int m_nCapacity;
	//ָ��̬���ٵ��ڴ�
	T* m_pElements;
	//����������������ʱ,������������ 
	void inflate() {
		cout << "inflate, size = " << yellow << m_nCapacity << blue << endl;
		T* now = new T[m_nCapacity * 2]; //nowָ���¿��ٵ��ڴ�
		T* plast = m_pElements;			 //plastָ��ԭ�����ڴ�
		T* pnow = now;					 //pnow ָ���¿��ٵ��ڴ�
		
		int capacity = m_nCapacity;
		while (capacity > 0) {//��ԭ����Ԫ�ؿ������µ��ڴ���
			*pnow = *plast;
			plast++, pnow++;
			capacity--;
		}
			
		delete m_pElements;
		m_pElements = now;
		m_nCapacity *= 2;
	}
public:
	//����һ���յ�vector
	Vector() {
		m_pElements = new T;
		m_nCapacity = 1;
		m_nSize = 0;
		cout << "vector()" << endl;
	}
	//����һ����ʼ����Ϊsize��vector
	Vector(int size) {
		m_pElements = new T[size];
		m_nCapacity = size;
		m_nSize = 0;
	}
	//����һ��vector,��r�����в�����ͬ
	Vector(const Vector& r) {
		m_nSize = r.m_nSize;
		m_nCapacity = r.m_nCapacity;

		T* now = new T[m_nCapacity]; //nowָ���Լ����ٵ��ڴ�
		T* plast = r.m_pElements;	 //plastָ��r���ڴ�
		T* pnow = now;				 //pnowָ���Լ����ٵ��ڴ�
		
		int capacity = m_nCapacity;
		while (capacity > 0) {//��r�е�Ԫ�ؿ������Լ����ڴ���
			*pnow = *plast;
			plast++, pnow++;
			capacity--;
		}
		m_pElements = now;
	}
	//��������
	~Vector() {
		delete m_pElements;
	}
	//��ȡ��index��Ԫ��,�������߽���
	T& operator[](int index) {
		return m_pElements[index];
	}
	//��ȡ��index��Ԫ��,���index<0 || index>=m_nSize,����쳣'std::out_of_range'
	T& at(int index) {
		if (index >= m_nSize || index < 0) {
			throw std::out_of_range("index out of range");
		}
		else return m_pElements[index];
	}
	//���������Ĵ�С
	int size() const {
		return m_nSize;
	}
	//��������ĩβ���һ��Ԫ��
	void push_back(const T& x) {
		if (m_nSize >= m_nCapacity) inflate();
		m_pElements[m_nSize] = x;
		m_nSize++;
	} 
	//��������е�����Ԫ��
	void clear() {
		m_nSize = 0;
	}
	//�ж������Ƿ�Ϊ��
	bool empty() const {
		return m_nSize == 0;
	}
	//��������������
	int capacity()const {
		return m_nCapacity;
	}
	//������Ϣ
	void Debug() {
		cout << "size:" << yellow << m_nSize << blue << "\t";
		cout << "capacity:" << yellow << m_nCapacity << blue << endl;
		cout << "element:";
		for (int i = 0; i < m_nSize; i++)
			cout << (int)m_pElements[i] << " ";
		cout << "\n";
	}
};
int main() {
	//���:Vector()
	cout << red << "test for Vector()\n" << blue;
	Vector<int>v;
	v.Debug();
	cout << blue << "\n\n";

	//���:Vector(int size)
	cout << red << "test for Vector(int size)\n"<< blue;
	Vector<int>v1(100);
	v1.Debug();
	cout << blue << "\n\n";

	//���:push_back(),inflate
	cout << red << "test for push_back(const &T x) and inflate\n"<< blue;
	cout << "please input a number\n" << red;
	int size = 0;
	cin >> size;
	cout << blue;
	for (int i = 1; i <= size; i++)
		v.push_back(i);
	v.Debug();
	cout << blue << "\n\n";
	
	//���:Vector(const Vector& r)
	cout << red << "test for Vector(const Vector& r)\n" << blue;
	Vector<int>v2(v);
	v2.Debug();
	cout << blue << "\n\n";

	//���:operator[], size()
	cout << red << "test for operator[] and size()\n" << blue;
	v.Debug();
	cout << "please input a number between " << yellow << "0" << blue << " and " << yellow << v.size() - 1 << ":\n" << red;
	int index = 0;
	cin >> index;
	cout << blue << "v["  << index  << "] = " << yellow << (int)v[index] << blue << endl;
	cout << blue << "\n\n";

	//���:at()
	cout << red << "test for at() and size()\n" << blue;
	v.Debug();
	cout << "please input a number between " << yellow << "0" << blue << " and " << yellow << v.size() - 1 << ":\n" << red;
	cin >> index;
	cout << blue << "v[" << index << "] = " << yellow << (int)v.at(index) << blue << endl;
	cout << blue << "\n\n";

	//���:empty()
	cout << red << "test for empty()\n" << blue;
	v.Debug();
	cout << "v.empty() = " << yellow << v.empty() << blue << "\n";
	cout << blue << "\n\n";
	
	//���:clear()
	cout << red << "test for clear()\n" << blue;
	v.clear();
	v.Debug();
	cout << "v.empty() = " << yellow << v.empty() << blue << "\n";
	cout << blue << "\n\n";

	return 0;
}