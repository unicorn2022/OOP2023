#pragma once
#include <cstddef>
#include <cstdlib>
#include <limits>
#include <utility>
#include <cstddef>   // For size_t and ptrdiff_t
#include <type_traits>  // For std::true_type and std::forward
#include <new>  // For operator new and placement new

template <typename T, size_t BlockSize = 0x10000>
class MyAllocator
{
public:
    typedef void _Not_user_specialized;

    typedef T value_type; // 要分配空间的类型

    typedef T *pointer;
    typedef const T *const_pointer;

    typedef T &reference;
    typedef const T &const_reference;

    typedef size_t size_type;          // unsigned long long
    typedef ptrdiff_t difference_type; // long long

    typedef std::true_type propagate_on_container_move_assignment;
    typedef std::true_type is_always_equal;

    MyAllocator() throw() : currBlock(nullptr), currSlot(nullptr), lastSlot(nullptr), freeSlots(nullptr) {}
    // 拷贝构造函数
    MyAllocator(const MyAllocator& other) throw() : currBlock(nullptr), currSlot(nullptr), lastSlot(nullptr), freeSlots(nullptr) {}
    template<class U>
    MyAllocator(const MyAllocator<U>& other) throw() : currBlock(nullptr), currSlot(nullptr), lastSlot(nullptr), freeSlots(nullptr) {}
    MyAllocator(MyAllocator&& myalloc) throw() : currBlock(myalloc.currBlock), currSlot(myalloc.currSlot), lastSlot(myalloc.lastSlot), freeSlots(myalloc.freeSlots) 
    {
        myalloc.currBlock = nullptr;
        myalloc.currSlot = nullptr;
        myalloc.lastSlot = nullptr;
        myalloc.freeSlots = nullptr;
    }
    
    ~MyAllocator() noexcept;

    MyAllocator &operator=(MyAllocator &&myalloc) throw();

    pointer address(reference _Val) const throw() { return &_Val; }
    const_pointer address(const_reference _Val) const throw() { return &_Val; }

    pointer allocate(size_type _Count = 1);
    void deallocate(pointer _Ptr, size_type _Count = 1);

    size_type MaxSlot() const throw();
    
    template <class _Uty>
    void destroy(_Uty *_Ptr) { _Ptr->~_Uty(); } // 销毁方法 _Ptr指向要销毁对象

    template <class _Objty, class... _Types>
    void construct(_Objty *_Ptr, _Types &&..._Args) { new (static_cast<void *>(_Ptr)) _Objty(std::forward<_Types>(_Args)...); } // 构造方法

    template <class... Args>
    pointer newElem(Args &&...args);
    void deleteElem(pointer p);

    template <typename U>
    struct rebind
    {
        using other = MyAllocator<U, BlockSize>;
    };

private:
    union Slot
    {
        value_type element;
        Slot *next;
    };
    Slot *currBlock;
    Slot *currSlot;
    Slot *lastSlot;
    Slot *freeSlots;

    size_type SlotAlign(char *p, size_type align) const throw()
    {
        size_t memSpace = reinterpret_cast<size_t>(p);
        size_t offBytes = (align - memSpace) % align;
        return offBytes;
    }
    void allocateBlock()
    {
        try
        {
            char* newBlock = static_cast<char*>(std::malloc(BlockSize));
            memset(newBlock, 0, BlockSize);
            Slot* slotBlock = reinterpret_cast<Slot*>(newBlock);
            slotBlock->next = currBlock;
            currBlock = slotBlock;

            char* slotArea = newBlock + sizeof(Slot*);
            size_type alignPadding = SlotAlign(slotArea, sizeof(Slot));
            currSlot = reinterpret_cast<Slot*>(slotArea + alignPadding);
            lastSlot = reinterpret_cast<Slot*>(newBlock + BlockSize - sizeof(Slot) + 1);
        }
        catch (const std::bad_alloc& e)
        {
            // 处理内存分配失败的情况
            // 在此处添加适当的错误处理逻辑
            // 例如，可以向上层报告错误或进行其他清理操作
            std::cerr << "Failed to allocate memory for block: " << e.what() << std::endl;
            // 抛出异常或采取其他适当的操作
            throw; // 重新抛出异常，将其传递给上层调用者
        }
    }
};

template <typename T, size_t BlockSize>
MyAllocator<T, BlockSize> &MyAllocator<T, BlockSize>::operator=(MyAllocator &&myalloc) noexcept
{
    if (this != &myalloc)
    {
        currBlock = myalloc.currBlock;
        currSlot = myalloc.currSlot;
        lastSlot = myalloc.lastSlot;
        freeSlots = myalloc.freeSlots;
    }
    return *this;
}

template <typename T, size_t BlockSize>
MyAllocator<T, BlockSize>::~MyAllocator() noexcept
{
    Slot *curr = currBlock;
    while (curr)
    {
        Slot *prev = curr->next;
        operator delete(reinterpret_cast<void *>(curr));
        curr = prev;
    }
}

template <typename T, size_t BlockSize>
typename MyAllocator<T, BlockSize>::size_type MyAllocator<T, BlockSize>::MaxSlot() const noexcept
{
    size_type MaxBlockNum = std::numeric_limits<unsigned long long>::max() / BlockSize;
    size_type MaxBlockSize = (BlockSize - sizeof(Slot *)) / (sizeof(Slot) + sizeof(Slot *)) * MaxBlockNum;
    return MaxBlockSize;
}

template <typename T, size_t BlockSize>
typename MyAllocator<T, BlockSize>::pointer MyAllocator<T, BlockSize>::allocate(size_type _Count)
{
    if (freeSlots)
    {
        pointer addr = reinterpret_cast<pointer>(freeSlots);
        freeSlots = freeSlots->next;
        // construct<value_type>(addr); // 构造对象
        return addr;
    }
    else
    {
        if (currSlot >= lastSlot)
            allocateBlock();
        return reinterpret_cast<pointer>(currSlot++);
    }
}

template <typename T, size_t BlockSize>
typename MyAllocator<T, BlockSize>::_Not_user_specialized MyAllocator<T, BlockSize>::deallocate(pointer _Ptr, size_type _Count)
{
    if (_Ptr)
    {
        reinterpret_cast<Slot *>(_Ptr)->next = freeSlots;
        freeSlots = reinterpret_cast<Slot *>(_Ptr);
    }
    return _Not_user_specialized(); // 返回 _Not_user_specialized 类型的值
}

template <typename T, size_t BlockSize>
template <class... Args>
typename MyAllocator<T, BlockSize>::pointer MyAllocator<T, BlockSize>::newElem(Args &&...args)
{
    pointer addr = allocate();
    construct<value_type>(addr, std::forward<Args>(args)...);
    return addr;
}

template <typename T, size_t BlockSize>
void MyAllocator<T, BlockSize>::deleteElem(pointer p)
{
    if (p)
    {
        p->~value_type();
        deallocate(p);
    }
}