// testallocator.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "SetColor.h"
#include <vector>
#include <iostream>
#include <ctime>
#include <string>
using namespace std;

#define INT 1
#define FLOAT 2
#define DOUBLE 3
#define CLASS  4

//#define MyAllocator std::allocator
//const string NowAllocator = "std::allocator";
//void FreeAll(){}

//#include "BasicAllocator.h"
//#define MyAllocator BasicAllocator
//const string NowAllocator = "BasicAllocator";
//void FreeAll(){}

#include "BlockBufferPool.h"
#define MyAllocator BlockBufferPool
const string NowAllocator = "BlockBufferPool";

//#include "DiverseBlockBufferPool.h"
//#define MyAllocator DiverseBlockBufferPool
//const string NowAllocator = "DiverseBlockBufferPool";

//#include "FreeListBufferPool.h"
//#define MyAllocator FreeListBufferPool
//const string NowAllocator = "FreeListBufferPool";
//void FreeAll(){}

const int testTime = 50;
const bool Debug = false;


class vecWrapper
{
public:
	vecWrapper() {
		m_pVec = NULL;
		m_type = INT;
	}
	virtual ~vecWrapper()	{
	}
public:
	void setPointer(int type, void *pVec) { m_type = type; m_pVec = pVec; }
	virtual void visit(int index) = 0;
	virtual int size() = 0;
	virtual void resize(int size) = 0;
	virtual bool checkElement(int index, void *value) = 0;
	virtual void setElement(int idex, void *value) = 0;
protected: 
	int m_type;
	void *m_pVec;
};

template<class T>
class vecWrapperT : public vecWrapper
{
public:
	vecWrapperT(int type, std::vector<T, MyAllocator<T> > *pVec)
	{ 
		m_type = type;
		m_pVec = pVec;
	}
	virtual ~vecWrapperT() {
		if (m_pVec)
			delete ((std::vector<T, MyAllocator<T> > *)m_pVec);
	}
public:
	virtual void visit(int index)
	{
		T temp = (*(std::vector<T, MyAllocator<T> > *)m_pVec)[index];
	}
	virtual int size()
	{
		return ((std::vector<T, MyAllocator<T> > *)m_pVec)->size();
	}
	virtual void resize(int size)
	{
		((std::vector<T, MyAllocator<T> > *)m_pVec)->resize(size);
	}
	virtual bool checkElement(int index, void *pValue)
	{
		T temp = (*(std::vector<T, MyAllocator<T> > *)m_pVec)[index];
		if (temp == (*((T *)pValue)))
			return true;
		else
			return false;
	}

	virtual void setElement(int index, void *value)
	{
		(*(std::vector<T, MyAllocator<T> > *)m_pVec)[index] = *((T *)value);
	}
};

class myObject
{
public:
	myObject() : m_X(0), m_Y(0) {}
	myObject(int t1, int t2) :m_X(t1), m_Y(t2) {}
	myObject(const myObject &rhs) { m_X = rhs.m_X; m_Y = rhs.m_Y;}
	~myObject() { /*std::cout << "my object destructor called" << std::endl;*/ }
	bool operator == (const myObject &rhs)
	{
		if ((rhs.m_X == m_X) && (rhs.m_Y == m_Y))
			return true;
		else
			return false;
	}
protected:
	int m_X;
	int m_Y;
};

#define TESTSIZE 10000

bool Test() {
	bool ans = true;
	vecWrapper** testVec;
	testVec = new vecWrapper * [TESTSIZE];

	int tIndex, tSize;
	//test allocator
	if (Debug) cout << red << "test allocator\n" << blue;
	if (Debug) cout << red << "test vector<int>\n" << blue;
	for (int i = 0; i < TESTSIZE - 4; i++) {
		tSize = (int)((float)rand() / (float)RAND_MAX * 100);
		
		if (i < 5 && Debug)cout << "No." << i << endl;
		if (i < 5 && Debug)cout << "allocator " << red << tSize << "\tint\t" << blue << "for testVec<int>[" << yellow << i << blue << "]\n";
		
		vecWrapperT<int>* pNewVec = new vecWrapperT<int>(INT, new std::vector<int, MyAllocator<int>>(tSize));
		testVec[i] = (vecWrapper*)pNewVec;
	}
	if (Debug) cout << "\n\n";

	if (Debug) cout << red << "test vector<myObject>\n" << blue;
	for (int i = 0; i < 4; i++) {
		tSize = (int)((float)rand() / (float)RAND_MAX * 10000);
		
		if (i < 5 && Debug)cout << "No." << i << endl;
		if (i < 5 && Debug)cout << "allocator " << red << tSize << "\tmyObject\t" << blue << "for testVec<myObject>[" << yellow << TESTSIZE - 4 + i << blue << "]\n";
		
		vecWrapperT<myObject>* pNewVec = new vecWrapperT<myObject>(CLASS, new std::vector<myObject, MyAllocator<myObject>>(tSize));
		testVec[TESTSIZE - 4 + i] = (vecWrapper*)pNewVec;

	}
	if (Debug) cout << "\n\n";

	//test resize
	if (Debug) cout << red << "test resize\n" << blue;
	for (int i = 0; i < 100; i++){
		tIndex = (int)((float)rand() / (float)RAND_MAX * (float)TESTSIZE);
		tSize = (int)((float)rand() / (float)RAND_MAX * (float)TESTSIZE) + 1;
		
		if (i < 5 && Debug) cout << "resize testVec[" << yellow << tIndex << blue << "]\t" << "to size:" << red << tSize << blue << "\n";
		
		testVec[tIndex]->resize(tSize);
	}
	if (Debug) cout << "\n\n";


	//test assignment
	//test assignment vector<int>
	if (Debug) cout << red << "test assignment vector<int>\n" << blue;
	tIndex = (int)((float)rand() / (float)RAND_MAX * (TESTSIZE - 4 - 1));
	int tIntValue = 10;
	
	int size = testVec[tIndex]->size();
	if (Debug) cout << "testVec[" << yellow << tIndex << blue << "] size = " << yellow << size << blue << "\n";
	int valueIndex = testVec[tIndex]->size() / 2;
	if (Debug) cout << "assign testVec[" << yellow << tIndex << blue << "](" << yellow << valueIndex << blue << ")\t";
	if (Debug) cout << "to " << red << tIntValue << blue << "\n";
	testVec[tIndex]->setElement(valueIndex, &tIntValue);


	if (!testVec[tIndex]->checkElement(testVec[tIndex]->size() / 2, &tIntValue)) {
		if (Debug) cout << red << "incorrect " << blue << "assignment in vector<int> " << yellow << tIndex << blue << endl;
		ans = false;
	}
	else {
		if (Debug) cout << "correct assignment in vector<int> " << yellow << tIndex << blue << endl;
	}
	if (Debug) cout << "\n\n";

	//test allocate vector<myObject>
	if (Debug) cout << red << "test allocate vector<myObject>\n" << blue;
	tIndex = TESTSIZE - 4 + 2;
	myObject tObj(11, 15);
	testVec[tIndex]->setElement(testVec[tIndex]->size() / 2, &tObj);
	if (!testVec[tIndex]->checkElement(testVec[tIndex]->size() / 2, &tObj)) {
		if (Debug) cout << red << "incorrect " << blue << "assignment in vector<myObject> " << yellow << tIndex << blue << endl;
		ans = false;
	}
	else {
		if (Debug) cout << "correct assignment in vector<myObject> " << yellow << tIndex << blue << endl;
	}


	myObject tObj1(13, 20);
	if (testVec[tIndex]->checkElement(testVec[tIndex]->size() / 2, &tObj1)) {
		if (Debug) cout << red << "the same object " << blue << "in vector " << yellow << tIndex << blue << " for object (13,20)" << endl;
		ans = false;
	}
	else {
		if (Debug) cout << "not the same object " << "in vector " << yellow << tIndex << blue << " for object(13, 20)" << endl;
	}
	if (Debug) cout << "\n\n";

	for (int i = 0; i < TESTSIZE; i++)
		delete testVec[i];

	delete[]testVec;
	return ans;
}

int main(){
	cout << "c++版本:" << __cplusplus << endl;
	auto nowTime = clock();
	for (int i = 1; i <= testTime; i++) {
		bool test = Test();
		FreeAll();
		if (i == 1 || i == 5 || i == 10 || i == 20 || i == 30 || i == 40 || i == 50) {
			cout << "use " << yellow << NowAllocator << blue;
			cout << " for " << yellow << i << blue << " tests used ";
			cout << red << clock() - nowTime << blue << " ms\n";
		}
		if (test)continue;
		cout << red << "test failed\n";
		break;
	}
	//system("pause");
    return 0;
	
}

