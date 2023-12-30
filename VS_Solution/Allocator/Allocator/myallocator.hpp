#ifndef _MEMORY_POOL_TCC_
#define _MEMORY_POOL_TCC_

#include "myallocator.h"

void FreeAll() {
    std::cout << "FreeAll\n";
}

class Manager {
public:
    static Manager& GetInstance() {
	    static Manager instance;
        return instance;
    }

    Pool* ManagePool(char* p, int size)
    {
        Pool* NewPool = nullptr;

        // There is still remaining memory in the memory pool available for allocation
        if (cur_ptr != nullptr && cur_ptr->left_size >= size)
        {
            NewPool = cur_ptr;
            cur_ptr->left_size -= size;
            cur_ptr->addr_left += size;
            return NewPool;
        }
        NewPool = new Pool();
        if (NewPool == nullptr)
            throw std::bad_alloc();

        // Allocate a new block of memory
        if (p == nullptr)
        {
            p = reinterpret_cast<char*>(malloc(size));
            NewPool->is_malloc = true;
            // Indicate that this node is allocated with malloc for later use in freeing memory
        }

        // After deallocating, free the allocated memory
        else
            NewPool->is_malloc = false;

        NewPool->addr_start = p;
        NewPool->addr_left = p;
        NewPool->left_size = size;
        NewPool->next = cur_ptr;
        cur_ptr = NewPool;
        return NewPool;
    }

private:
	Manager() = default;
    ~Manager() {
        FreeAll();
    }
};





template <typename T, size_t BlockSize>
typename MemoryPool<T, BlockSize>::pointer
MemoryPool<T, BlockSize>::address(reference value) const
{
    return &value;
}

template <typename T, size_t BlockSize>
typename MemoryPool<T, BlockSize>::const_pointer
MemoryPool<T, BlockSize>::address(const_reference value) const
{
    return &value;
}

// Allocate an element within the memory pool
template <typename T, size_t BlockSize>
typename MemoryPool<T, BlockSize>::pointer
MemoryPool<T, BlockSize>::allocate(size_type size_cnt, const_pointer const_p [[maybe_unused]])
{
    if (size_cnt <= 0)
        return nullptr;
    pointer result = nullptr;
    Pool *NewMemory = nullptr;
    size_type total = size_cnt * sizeof(value_type);

    // When allocating large memory, use malloc directly
    if (total > BlockSize)
    {
        NewMemory = ManagePool(nullptr, total);
        result = reinterpret_cast<pointer>(NewMemory->addr_start);
        NewMemory->left_size = 0;
        NewMemory->addr_left = nullptr;
    }

    // Check if there is a memory block available to accommodate the data
    else
    {
        NewMemory = cur_ptr;
        while (NewMemory != nullptr)
        {
            if (NewMemory->left_size < static_cast<decltype(NewMemory->left_size)>(total))
                NewMemory = NewMemory->next;
            else
                break;
        }

        // If the existing memory blocks cannot accommodate the current element, then reapply for memory
        if (NewMemory == nullptr)
            NewMemory = Manager::GetInstance().ManagePool(nullptr, BlockSize);

        // Place the current element into the memory block pointed to by NewMemory
        result = reinterpret_cast<pointer>(NewMemory->addr_left);
        NewMemory->left_size -= total;
        NewMemory->addr_left += total;
    }
    return result;
}

template <typename T, size_t BlockSize>
void MemoryPool<T, BlockSize>::deallocate(pointer p, size_type size)
{
    if (p == nullptr)
        return;

    // Find the memory block containing the memory to be freed in the memory pool linked list
    char *addr = reinterpret_cast<char *>(p);
    Pool *block = nullptr;
    Pool *cur = cur_ptr;
    while (cur != nullptr)
    {
        if (addr >= cur->addr_start && addr < (cur->addr_start + BlockSize))
        {
            block = cur;
            break;
        }
        cur = cur->next;
    }

    // If it is not in the memory pool, use the default release function for deallocation
    if (block == nullptr)
    {
        if (size > BlockSize)
            free(addr);
        else
            delete p;
    }

    // Find the position in the memory pool and update the pointer and remaining space
    else
    {
        block->addr_left = addr;
        block->left_size = BlockSize - (addr - block->addr_start);
    }
}

template <typename T, size_t BlockSize>
typename MemoryPool<T, BlockSize>::size_type
MemoryPool<T, BlockSize>::max_size() const throw()
{
    return static_cast<size_type>(-1) / sizeof(value_type);
}

template <typename T, size_t BlockSize>
void MemoryPool<T, BlockSize>::construct(pointer p, const_reference val)
{
    new (p) value_type(val);
}

template <typename T, size_t BlockSize>
void MemoryPool<T, BlockSize>::destroy(pointer p)
{
    p->~value_type();
}


#endif