#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

#include <cstdio>
#include <cstring>
#include <iostream>
#include <climits>
#include <cstddef>

// Memory pool
struct Pool
{
	char *addr_start; // Starting address of the current memory pool
	char *addr_left;  // Address of the remaining space in the current memory pool
	int left_size;	  // Remaining space in the current memory pool
	bool is_malloc;	  // Indicate whether the current memory pool was allocated using malloc
	Pool *next;		  // Next element in the memory block linked list
};
Pool *cur_ptr; // Head pointer of the memory block linked list

template <typename T, size_t BlockSize = 0x800>
class MemoryPool
{
public:
	typedef T value_type;
	typedef value_type *pointer;
	typedef value_type &reference;
	typedef value_type const *const_pointer;
	typedef value_type const &const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	template <typename _other>
	struct rebind
	{
		typedef MemoryPool<_other> other;
	};

	MemoryPool() throw() {}
	MemoryPool(const_reference value) throw() {}
	template <typename _other>
	MemoryPool(MemoryPool<_other> const &value) throw() {}
	~MemoryPool() throw() {}

	// Return the address of the value
	pointer address(reference value) const;
	const_pointer address(const_reference value) const;

	// Allocate memory and deallocate memory
	pointer allocate(size_type size_cnt, const_pointer const_p [[maybe_unused]] = nullptr);
	void deallocate(pointer p, size_type size);

	// Maximum number of elements that can be accommodated
	size_type max_size() const throw();

	// Construct and destruct objects
	void construct(pointer p, const_reference val);
	void destroy(pointer p);

	bool operator==(const MemoryPool &A) const { return true; }
	bool operator!=(const MemoryPool &A) const { return false; }

private:
	Pool *ManagePool(char *p = nullptr, int size = BlockSize);
};

#endif