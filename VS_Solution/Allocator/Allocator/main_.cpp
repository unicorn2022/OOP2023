#include <iostream>
#include <random>
#include <vector>
#include <ctime>

//template <class T>
//using MyAlloc = std::allocator<T>;


//#include "BasicAllocator.h"
//template <class T>
//using MyAlloc = BasicAllocator<T>;

#include "BlockBufferPool.h"
template <class T>
using MyAlloc = BlockBufferPool<T>;

//#include "FreeListBufferPool.h"
//template <class T>
//using MyAlloc = FreeListBufferPool<T>;

 const int TestSize = 10000;
 const int PickSize = 1000;

 template <class T>
 using standard = std::allocator<T>;
 using Point2D = std::pair<int, int>;


int main()
{
    clock_t start;
    start = clock();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, TestSize);
    // vector creation

    using IntVec = std::vector<int, MyAlloc<int>>;
    std::vector<IntVec, MyAlloc<IntVec>> vecints(TestSize); // this line
    // std::cout << "here\n";
    for (int i = 0; i < TestSize; i++) {
        int temp = 0;
        vecints[i].resize(TestSize);
    }

    using PointVec = std::vector<Point2D, MyAlloc<Point2D>>;
    std::vector<PointVec, MyAlloc<PointVec>> vecpts(TestSize);
    for (int i = 0; i < TestSize; i++)
        vecpts[i].resize(dis(gen));

    // vector resize
    for (int i = 0; i < PickSize; i++)
    {
        int idx = dis(gen) - 1;
        int size = dis(gen);
        vecints[idx].resize(size);
        vecpts[idx].resize(size);
    }

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
    std::cout << "Allocator with memory pool cost: "
              << (clock() - start) * 1.0 / CLOCKS_PER_SEC << " seconds" << std::endl
              << std::endl;


    start = clock();
    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::uniform_int_distribution<> dis(1, TestSize);
    // vector creation

    using IntVec1 = std::vector<int, standard<int>>;
    std::vector<IntVec1, standard<IntVec1>> vecints1(TestSize); // this line
    // std::cout << "here\n";
    for (int i = 0; i < TestSize; i++)
        vecints1[i].resize(dis(gen));

    using PointVec1 = std::vector<Point2D, standard<Point2D>>;
    std::vector<PointVec1, standard<PointVec1>> vecpts1(TestSize);
    for (int i = 0; i < TestSize; i++)
        vecpts1[i].resize(dis(gen));

    // vector resize
    for (int i = 0; i < PickSize; i++)
    {
        int idx = dis(gen) - 1;
        int size = dis(gen);
        vecints1[idx].resize(size);
        vecpts1[idx].resize(size);
    }

    // vector element assignment
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
        int idx2 = vecpts[idx1].size() / 2;
        vecpts1[idx1][idx2] = val;
        if (vecpts1[idx1][idx2] == val)
            std::cout << "correct assignment in vecpts: " << idx1 << std::endl;
        else
            std::cout << "incorrect assignment in vecpts: " << idx1 << std::endl;
    }
    std::cout << "Allocator with memory pool cost: "
              << (clock() - start) * 1.0 / CLOCKS_PER_SEC << " seconds" << std::endl
              << std::endl;

    return 0;
}
