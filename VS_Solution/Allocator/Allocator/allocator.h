#pragma once
#include <iostream>
#include <random>
#include <vector>
#include <ctime>

#define BlockSize 0x1000
class MemoryPool
{
public:
    void* allocate(std::size_t n);
    void deallocate(void* p, std::size_t size);

    // 内存池, 以Block为单位
    struct Pool
    {
        char* addr_start; // 当前内存池的起始地址
        char* addr_left;  // 当前内存池剩余空间的地址
        int left_size;    // 当前内存池剩余的空间
        bool is_malloc;   // 表明当前内存池是否是通过malloc分配的
        Pool* next;       // 内存块链表的下一个元素
    };
    Pool* cur_ptr = nullptr; // 内存块链表的头指针;
    Pool* ManagePool(char* p = nullptr, std::size_t size = BlockSize);
    static MemoryPool& getInstance()
    {
        static MemoryPool instance;
        return instance;
    }
};
void* MemoryPool::allocate(std::size_t n)
{
    fprintf(stderr, "MemoryPool allocate %d\n", n);
    if (n <= 0)
        return nullptr;
    void* result = nullptr;
    Pool* NewMemory = nullptr;
    if (n > BlockSize)
    {
        NewMemory = reinterpret_cast<Pool*>(this->ManagePool(nullptr, n));
        result = reinterpret_cast<void*>(NewMemory->addr_start);
    }
    else
    {
        NewMemory = cur_ptr;
        while (NewMemory != nullptr)
        {
            if (NewMemory->left_size < n)
                NewMemory = NewMemory->next;
            else
                break;
        }
        if (NewMemory == nullptr)
            NewMemory = ManagePool(nullptr, BlockSize);
        result = (char*)NewMemory->addr_left;
        NewMemory->left_size -= n;
        NewMemory->addr_left += n;
    }
    return result;
}
void MemoryPool::deallocate(void* p, std::size_t size)
{
    if (p == nullptr)
        return;
    // 将p指向的size大小的内存, 作为新的Block块放入current_Block中
    ManagePool(reinterpret_cast<char*>(p), size);
}
MemoryPool::Pool* MemoryPool::ManagePool(char* p, std::size_t size)
{
    Pool* NewPool = new Pool();
    if (NewPool == nullptr)
        throw std::bad_alloc();

    // 申请一块新的内存
    if (p == nullptr)
    {
        p = reinterpret_cast<char*>(malloc(size));
        NewPool->is_malloc = true;
    }
    // deallocate释放掉的内存
    else
        NewPool->is_malloc = false;

    NewPool->addr_start = p;
    NewPool->addr_left = p;
    NewPool->left_size = size;
    NewPool->next = cur_ptr;
    cur_ptr = NewPool;
    return NewPool;
}

template <typename T>
class MAllocator
{
private:
    /* data */
public:
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T* const_pointer;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef char data_type;
    typedef char* data_pointer;

    typedef std::false_type propagate_on_container_copy_assignment;
    typedef std::true_type propagate_on_container_move_assignment;
    typedef std::true_type propagate_on_container_swap;

    template <typename U>
    struct rebind
    {
        typedef MAllocator<U> other;
    };

    MAllocator() {}

    template <typename U>
    MAllocator(const MAllocator<U>& obj) {}

    MAllocator(MAllocator& obj) {}
    MAllocator(MAllocator&& obj) {}
    ~MAllocator() {}

    MAllocator& operator=(MAllocator&& obj) { return *this; }
    MAllocator& operator=(const MAllocator&& obj) { return *this; }

    pointer address(reference _Val) const noexcept { return &_Val; }
    const_pointer address(const_reference _Val) const noexcept { return &_Val; }

    template <typename U>
    bool operator==(const MAllocator<U>&) { return true; }
    template <typename U>
    bool operator!=(const MAllocator<U>&) { return false; }

    pointer allocate(size_type n = 1)
    {
        fprintf(stderr, "allocator allocate %d\n", n);
        if (auto p = reinterpret_cast<pointer>(MemoryPool::getInstance().allocate(n * sizeof(value_type))))
            return p;
        throw std::bad_alloc();
        // return reinterpret_cast<pointer>(MemoryPool::allocate(n));
    }
    void deallocate(pointer p, size_type n = 1)
    {
        MemoryPool::getInstance().deallocate(p, n);
    }
    template <class _Objty, class... _Types>
    void construct(_Objty* _Ptr, _Types &&..._Args)
    {
        ::new (const_cast<void*>(static_cast<const volatile void*>(_Ptr)))
            _Objty(std::forward<_Types>(_Args)...);
    }
    template <class _Uty>
    void destroy(_Uty* _Ptr)
    {
        _Ptr->~_Uty();
    }
};
