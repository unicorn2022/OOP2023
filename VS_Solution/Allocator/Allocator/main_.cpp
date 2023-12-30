#include <iostream>
#include <random>
#include <vector>
#include <ctime>

const bool Debug = false;
#define fprintf if(Debug)fprintf

//template <class T>
//using MyAlloc = std::allocator<T>;


//#include "BasicAllocator.h"
//template <class T>
//using MyAlloc = BasicAllocator<T>;

//#include "BlockBufferPool.h"
//template <class T>
//using MyAlloc = BlockBufferPool<T>;

//#include "FreeListBufferPool.h"
//template <class T>
//using MyAlloc = FreeListBufferPool<T>;

//#include "myallocator.hpp"
//template <class T>
//using MyAlloc = MemoryPool<T>;

#include "allocator.h"
template <class T>
using MyAlloc = MAllocator<T>;

const int TestSize = 10000;
const int PickSize = 1000;

template <class T>
using standard = std::allocator<T>;
using Point2D = std::pair<int, int>;

void Work() {
	std::cout << "-----------------------------------Work-----------------------------------\n";
	clock_t start = clock();
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, TestSize);


	// vector creation
	fprintf(stderr, "-----------------------------------vec ints:-----------------------------------\n");
	using IntVec = std::vector<int, MyAlloc<int>>;
	std::vector<IntVec, MyAlloc<IntVec>> vecints(TestSize); // this line
	for (int i = 0; i < TestSize; i++) {
		int size = dis(gen);
		fprintf(stderr, "[%d] resize %d\n", i, size);
		vecints[i].resize(size);
	}

	fprintf(stderr, "-----------------------------------vec points:-----------------------------------\n");
	using PointVec = std::vector<Point2D, MyAlloc<Point2D>>;
	std::vector<PointVec, MyAlloc<PointVec>> vecpts(TestSize);
	for (int i = 0; i < TestSize; i++) {
		int size = dis(gen);
		fprintf(stderr, "[%d] resize %d\n", i, size);
		vecpts[i].resize(size);
	}

	// vector resize
	fprintf(stderr, "-----------------------------------vec resize:-----------------------------------\n");
	for (int i = 0; i < PickSize; i++) {
		int idx = dis(gen) - 1;
		int size = dis(gen);

		fprintf(stderr, "vec ints   [%d] resize %d\n", idx, size);
		vecints[idx].resize(size);
		fprintf(stderr, "vec points [%d] resize %d\n", idx, size);
		vecpts[idx].resize(size);
	}

	fprintf(stderr, "-----------------------------------vec assign:-----------------------------------\n");
	// vector element assignment
	{
		int val = 10;
		int idx1 = dis(gen) - 1;
		int idx2 = vecints[idx1].size() / 2;
		vecints[idx1][idx2] = val;
		if (vecints[idx1][idx2] == val)
			std::cout << "correct assignment in vecints: " << idx1 << std::endl;
		else
			std::cout << "incorrect assignment in vecints: " << idx1 << std::endl;
	}
	{
		Point2D val(11, 15);
		int idx1 = dis(gen) - 1;
		int idx2 = vecpts[idx1].size() / 2;
		vecpts[idx1][idx2] = val;
		if (vecpts[idx1][idx2] == val)
			std::cout << "correct assignment in vecpts: " << idx1 << std::endl;
		else
			std::cout << "incorrect assignment in vecpts: " << idx1 << std::endl;
	}
	std::cout << "Allocator with memory pool cost: " << (clock() - start) * 1.0 / CLOCKS_PER_SEC << " seconds" << std::endl;
	std::cout << "--------------------------------------------------------------------------\n";
}


void Work_STD() {
	std::cout << "-----------------------------------Work_STD-----------------------------------\n";
	clock_t start = clock();
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, TestSize);
	// vector creation

	fprintf(stderr, "-----------------------------------vec int:-----------------------------------\n");
	using IntVec1 = std::vector<int, standard<int>>;
	std::vector<IntVec1, standard<IntVec1>> vecints1(TestSize); // this line
	for (int i = 0; i < TestSize; i++) {
		int size = dis(gen);
		fprintf(stderr, "[%d] resize %d\n", i, size);
		vecints1[i].resize(size);
	}

	fprintf(stderr, "-----------------------------------vec point:-----------------------------------\n");
	using PointVec1 = std::vector<Point2D, standard<Point2D>>;
	std::vector<PointVec1, standard<PointVec1>> vecpts1(TestSize);
	for (int i = 0; i < TestSize; i++) {
		int size = dis(gen);
		fprintf(stderr, "[%d] resize %d\n", i, size);
		vecpts1[i].resize(size);
	}

	fprintf(stderr, "-----------------------------------vec resize:-----------------------------------\n");
	// vector resize
	for (int i = 0; i < PickSize; i++)
	{
		int idx = dis(gen) - 1;
		int size = dis(gen);
		fprintf(stderr, "vec ints   [%d] resize %d\n", idx, size);
		vecints1[idx].resize(size);
		fprintf(stderr, "vec points [%d] resize %d\n", idx, size);
		vecpts1[idx].resize(size);
	}

	// vector element assignment
	fprintf(stderr, "-----------------------------------vec assign:-----------------------------------\n");
	{
		int val = 10;
		int idx1 = dis(gen) - 1;
		int idx2 = vecints1[idx1].size() / 2;
		vecints1[idx1][idx2] = val;
		if (vecints1[idx1][idx2] == val)
			std::cout << "correct assignment in vecints: " << idx1 << std::endl;
		else
			std::cout << "incorrect assignment in vecints: " << idx1 << std::endl;
	}
	{
		Point2D val(11, 15);
		int idx1 = dis(gen) - 1;
		int idx2 = vecpts1[idx1].size() / 2;
		vecpts1[idx1][idx2] = val;
		if (vecpts1[idx1][idx2] == val)
			std::cout << "correct assignment in vecpts: " << idx1 << std::endl;
		else
			std::cout << "incorrect assignment in vecpts: " << idx1 << std::endl;
	}
	std::cout << "Allocator with memory pool cost: " << (clock() - start) * 1.0 / CLOCKS_PER_SEC << " seconds" << std::endl;
	std::cout << "--------------------------------------------------------------------------\n";
}

int main() {
	std::cout << "Work Start\n";
	Work();
	std::cout << "Work End\n";

	std::cout << "Work STD Start\n";
	Work_STD();
	std::cout << "Work End\n";

	return 0;
}
