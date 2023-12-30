//FreeListBufferPool: 借助16个freelist优化的allocator
#pragma once
#include <cstdio>
#include <cstring>
#include <iostream>
#include "SetColor.h"
using namespace std;

void FreeAll() {
    SetConsoleColor(ConsoleColor::Cyan);
    fprintf(stderr, "FreeAll\n");
    SetConsoleColor(ConsoleColor::Clear);
}

template <typename T>
class FreeListBufferPool {
public:
    // typedefs...
    typedef T                   value_type;
    typedef value_type*         pointer;
    typedef value_type&         reference;
    typedef value_type const*   const_pointer;
    typedef value_type const&   const_reference;
    typedef size_t              size_type;
    typedef ptrdiff_t           difference_type;

    // rebind...
    template <typename _other> struct rebind {
        typedef FreeListBufferPool<_other> other;
    };

public:
    // 返回val的地址
    pointer address(reference val)const {
        return &val;
    }
    const_pointer address(const_reference val)const {
        return &val;
    }

    pointer allocate(size_type cnt, char* pHint = 0);

    void deallocate(pointer p, size_type n);

    // 可容纳的最多元素
    size_type max_size() const throw() {
        return static_cast<size_type>(-1) / sizeof(value_type);
    }

    // 在地址p所指向的空间, 使用val进行填充
    // 需要使用到placement new, 以遍保证调用到构造函数
    // placement new: 在用户指定的内存位置上构建新的对象, 构建过程中不需要额外分配内存, 只需要调用对象的构造函数 
    void construct(pointer p, const_reference val) {
        new (p) value_type(val);
    }

    // 析构p指向的内存块中的内容
    // 一般通过显式调用析构函数来执行
    void destory(pointer p) {
        p->~T();
    }

public:
    FreeListBufferPool() throw() {}
    FreeListBufferPool(const_reference val) throw() {}
    template<typename _other>
    FreeListBufferPool(FreeListBufferPool<_other> const& val) throw() {}
    ~FreeListBufferPool() noexcept {}
public:
    bool operator==(const FreeListBufferPool& A)const {
        return true;
    }
    bool operator!=(const FreeListBufferPool& A)const {
        return false;
    }
};

const int ALIGN = 8;        //小型区块对应的大小为8的倍数
const int MAX_MEMORY = 128; // 小型区块最大为128B
const int CNT_OF_LIST = MAX_MEMORY / ALIGN; //free-lists一共有16个

//将size上调为ALIGN的倍数
static size_t convert_size_to_size(size_t size) {
    return (size + ALIGN - 1) & ~(ALIGN - 1);
}

//根据size大小判断当前区块属于哪个FreeList
static size_t convert_size_to_index(size_t size) {
    return (size + ALIGN - 1) / ALIGN - 1;
}

union Block {
    union Block* next; // Free_List链表指向的下一块内存
    char  data;        // 当前内存存放的数据
};
Block* Free_List[CNT_OF_LIST];
char* start_free = nullptr;   // 内存池开始地址
char* end_free = nullptr;     // 内存池结束地址
size_t heap_size;             // 内存池的大小 
//从内存池中分配size*n大小的空间
char* chunk_alloc(size_t size, int& n) {
    size_t total_size = n * size;
    size_t bytes_left = end_free - start_free;

    // 内存池剩余空间完全满足需求
    if (bytes_left >= total_size) {
        char* ans = start_free;
        start_free += total_size;
        return ans;
    }

    // 内存池剩余空间不能完全满足需求, 但至少能分配一个块
    else if (bytes_left >= size) {
        n = bytes_left / size;
        total_size = n * size;
        char* ans = start_free;
        start_free += total_size;
        return ans;
    }

    // 内存池剩余空间无法分配一个区块
    else {
        // 先将内存池中剩余的空间分配给适当的Free_List
        if (bytes_left > 0) {
            int index = convert_size_to_index(bytes_left);
            reinterpret_cast<Block*>(start_free)->next = Free_List[index];
            Free_List[index] = reinterpret_cast<Block*>(start_free);
        }

        size_t bytes_to_get = 2 * total_size + convert_size_to_size(heap_size >> 4);
        start_free = (char*)malloc(bytes_to_get);

        // 堆空间不足, malloc失败
        if (start_free == nullptr) {
            // 检查当前Free_List, 看是否有比size大的空闲空间, 并将其分裂
            for (int i = size; i <= MAX_MEMORY; i += ALIGN) {
                int index = convert_size_to_index(i);
                Block* now = Free_List[index];
                if (now != nullptr) {
                    Free_List[index] = now->next;
                    // 将当前的空间放入内存池
                    start_free = reinterpret_cast<char*>(now);
                    end_free = start_free + i;
                    // 递归调用chunk_alloc, 进行重新分配
                    return chunk_alloc(size, n);
                }
            }
            // Free_List中也没有空余空间, 可以放弃治疗了
            throw bad_alloc();
        }

        // malloc成功
        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;
        // 递归调用自己, 分配malloc到的内存, 修改n
        return chunk_alloc(size, n);
    }
}
//填充大小为size的空闲链表
void* refill(size_t size) {
    int n = 20;
    // 尝试申请n*size的空间
    char* chunk = chunk_alloc(size, n);
    
    // 如果只能申请到1*size的空间, 则直接分配给调用者, Free_List没有新节点
    if (n == 1)return reinterpret_cast<void*>(chunk);

    // 申请到>=2*size的空间
    int index = convert_size_to_index(size);
    // 将第0块分配给调用者
    Block* ans = reinterpret_cast<Block*>(chunk);
    // 将第1~n-1块空间放到Free_List中
    Block* now_Block = nullptr;
    Block* next_Block = reinterpret_cast<Block*>(chunk + size);
    Free_List[index] = next_Block;
    for (int i = 1;; i++) {
        now_Block = next_Block;
        next_Block = reinterpret_cast<Block*>((char*)next_Block + size);
        if (i == n - 1) {
            now_Block->next = nullptr;
            break;
        }
        else {
            now_Block->next = next_Block;
        }
    }
    return reinterpret_cast<void*>(ans);
}

// 分配空间, 类似于malloc
template <typename T>
typename FreeListBufferPool<T>::pointer
FreeListBufferPool<T>::allocate(size_type cnt, char* pHint) {
    int size = cnt * sizeof(value_type);
    // 大于MAX_MEMORY, 直接用malloc分配内存
    if (size > MAX_MEMORY) {
        return reinterpret_cast<pointer>(malloc(size));
    }

    int index = convert_size_to_index(size);
    Block* now = Free_List[index];
    // 链表中无空闲空间, 调用refill申请一块内存
    if (now == nullptr) {
        void* ans = refill(convert_size_to_size(size));
        return reinterpret_cast<pointer>(ans);
    }
    // 链表中有空闲空间, 直接返回
    Free_List[index] = Free_List[index]->next;
    return reinterpret_cast<pointer>(now);
}

// 释放空间, 类似于free
template <typename T>
void FreeListBufferPool<T>::deallocate(pointer p, size_type n) {
    int size = n * sizeof(value_type);
    // 大于MAX_MEMORY, 直接用free释放内存
    if (size > MAX_MEMORY) {
        free(p);
        return;
    }
    // 将p对应的内存放到Free_List中
    int index = convert_size_to_index(size);
    Block* now = reinterpret_cast<Block*>(p);
    now->next = Free_List[index];
    Free_List[index] = now;
}
