//BasicAllocator: 使用malloc(), free()实现的allocator
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
class BasicAllocator{
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
        typedef BasicAllocator<_other> other; 
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
    BasicAllocator() throw() {}
    BasicAllocator(const_reference val) throw() {}
    template<typename _other>
    BasicAllocator(BasicAllocator<_other> const &val) throw() {}
    ~BasicAllocator() noexcept {}
public:
    bool operator==(const BasicAllocator& A)const {
        return true;
    }
    bool operator!=(const BasicAllocator& A)const {
        return false;
    }
};

// 分配空间, 类似于malloc
template <typename T>
typename BasicAllocator<T>::pointer
BasicAllocator<T>::allocate(size_type cnt, char* pHint) {
    if (cnt <= 0)return nullptr;
    if (max_size() < cnt)throw std::bad_alloc();

    void* pMem = malloc(cnt * sizeof(value_type));
    if (pMem == nullptr)throw std::bad_alloc();
    return static_cast<pointer>(pMem);
}

// 释放空间, 类似于free
template <typename T>
void BasicAllocator<T>::deallocate(pointer p, size_type n) {
    free(p);
}