#pragma warning(disable:4996)
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <iostream>
#include <Windows.h>
using namespace std;
//更改颜色
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
	//容器中的元素个数
	int m_nSize;
	//容器的最大容量
	int m_nCapacity;
	//指向动态开辟的内存
	T* m_pElements;
	//当容器的容量不够时,倍增容器容量 
	void inflate() {
		cout << "inflate, size = " << yellow << m_nCapacity << blue << endl;
		T* now = new T[m_nCapacity * 2]; //now指向新开辟的内存
		T* plast = m_pElements;			 //plast指向原来的内存
		T* pnow = now;					 //pnow 指向新开辟的内存
		
		int capacity = m_nCapacity;
		while (capacity > 0) {//将原来的元素拷贝到新的内存中
			*pnow = *plast;
			plast++, pnow++;
			capacity--;
		}
			
		delete m_pElements;
		m_pElements = now;
		m_nCapacity *= 2;
	}
public:
	//创建一个空的vector
	Vector() {
		m_pElements = new T;
		m_nCapacity = 1;
		m_nSize = 0;
		cout << "vector()" << endl;
	}
	//创建一个初始容量为size的vector
	Vector(int size) {
		m_pElements = new T[size];
		m_nCapacity = size;
		m_nSize = 0;
	}
	//创建一个vector,与r的所有参数相同
	Vector(const Vector& r) {
		m_nSize = r.m_nSize;
		m_nCapacity = r.m_nCapacity;

		T* now = new T[m_nCapacity]; //now指向自己开辟的内存
		T* plast = r.m_pElements;	 //plast指向r的内存
		T* pnow = now;				 //pnow指向自己开辟的内存
		
		int capacity = m_nCapacity;
		while (capacity > 0) {//将r中的元素拷贝到自己的内存中
			*pnow = *plast;
			plast++, pnow++;
			capacity--;
		}
		m_pElements = now;
	}
	//析构函数
	~Vector() {
		delete m_pElements;
	}
	//获取第index个元素,不包含边界检查
	T& operator[](int index) {
		return m_pElements[index];
	}
	//获取第index个元素,如果index<0 || index>=m_nSize,输出异常'std::out_of_range'
	T& at(int index) {
		if (index >= m_nSize || index < 0) {
			throw std::out_of_range("index out of range");
		}
		else return m_pElements[index];
	}
	//返回容器的大小
	int size() const {
		return m_nSize;
	}
	//向容器的末尾添加一个元素
	void push_back(const T& x) {
		if (m_nSize >= m_nCapacity) inflate();
		m_pElements[m_nSize] = x;
		m_nSize++;
	} 
	//清空容器中的所有元素
	void clear() {
		m_nSize = 0;
	}
	//判断容器是否为空
	bool empty() const {
		return m_nSize == 0;
	}
	//返回容器的容量
	int capacity()const {
		return m_nCapacity;
	}
	//调试信息
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
	//检测:Vector()
	cout << red << "test for Vector()\n" << blue;
	Vector<int>v;
	v.Debug();
	cout << blue << "\n\n";

	//检测:Vector(int size)
	cout << red << "test for Vector(int size)\n"<< blue;
	Vector<int>v1(100);
	v1.Debug();
	cout << blue << "\n\n";

	//检测:push_back(),inflate
	cout << red << "test for push_back(const &T x) and inflate\n"<< blue;
	cout << "please input a number\n" << red;
	int size = 0;
	cin >> size;
	cout << blue;
	for (int i = 1; i <= size; i++)
		v.push_back(i);
	v.Debug();
	cout << blue << "\n\n";
	
	//检测:Vector(const Vector& r)
	cout << red << "test for Vector(const Vector& r)\n" << blue;
	Vector<int>v2(v);
	v2.Debug();
	cout << blue << "\n\n";

	//检测:operator[], size()
	cout << red << "test for operator[] and size()\n" << blue;
	v.Debug();
	cout << "please input a number between " << yellow << "0" << blue << " and " << yellow << v.size() - 1 << ":\n" << red;
	int index = 0;
	cin >> index;
	cout << blue << "v["  << index  << "] = " << yellow << (int)v[index] << blue << endl;
	cout << blue << "\n\n";

	//检测:at()
	cout << red << "test for at() and size()\n" << blue;
	v.Debug();
	cout << "please input a number between " << yellow << "0" << blue << " and " << yellow << v.size() - 1 << ":\n" << red;
	cin >> index;
	cout << blue << "v[" << index << "] = " << yellow << (int)v.at(index) << blue << endl;
	cout << blue << "\n\n";

	//检测:empty()
	cout << red << "test for empty()\n" << blue;
	v.Debug();
	cout << "v.empty() = " << yellow << v.empty() << blue << "\n";
	cout << blue << "\n\n";
	
	//检测:clear()
	cout << red << "test for clear()\n" << blue;
	v.clear();
	v.Debug();
	cout << "v.empty() = " << yellow << v.empty() << blue << "\n";
	cout << blue << "\n\n";

	return 0;
}