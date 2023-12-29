//DiverseBlockBufferPool: 在原有内存池策略的基础上, 所有Block块的大小不唯一
#pragma once
#include <cstdio>
#include <cstring>
#include <iostream>
#include <climits>
#include <cstddef>
using namespace std;

// 内存池, 以Block为单位
struct DiverseBlock {
	char* start_address; // 当前Block的起始地址
	DiverseBlock* next;	 // Block链表的下一个元素
}; 
// 内存块链表的头指针, 分为32B, 64B, 128B, 256B, >256B
const int BigMemorySize = 256;
const int cnt_of_diverseBlock = 5;
const int BigMemoryIndex = cnt_of_diverseBlock - 1;
DiverseBlock* BigBlock[cnt_of_diverseBlock];
DiverseBlock* used_DiverseBlock[cnt_of_diverseBlock];
DiverseBlock* free_DiverseBlock[cnt_of_diverseBlock];
int convert_size_to_size(int size) {
	if (size <= 32)   return 32;
	if (size <= 64)   return 64;
	if (size <= 128)  return 128;
	if (size <= 256)  return 256;
	return size;
}
int convert_size_to_index(int size) {
	if (size <= 32)   return 0;
	if (size <= 64)   return 1;
	if (size <= 128)  return 2;
	if (size <= 256)  return 3;
	return 4;
}
int convert_index_to_size(int index) {
	if (index == 0)return 32;
	if (index == 1)return 64;
	if (index == 2)return 128;
	if (index == 3)return 256;
	if (index == 4)return 512;
	return 1024;
}

void FreeAll() {
	for (int i = 0; i < cnt_of_diverseBlock; i++) {
		DiverseBlock* now = BigBlock[i];
		while (now != nullptr) {
			DiverseBlock* next = now->next;
			free(now->start_address);
			now = next;
		}
		BigBlock[i] = nullptr;
		used_DiverseBlock[i] = nullptr;
		free_DiverseBlock[i] = nullptr;
	}
}
// 将p指向的内存插入free_DiverseBlock[index]中
void InsertToDiverseBlock(int index, char* p) {
	DiverseBlock* now = new DiverseBlock();
	now->start_address = p;
	now->next = free_DiverseBlock[index];
	free_DiverseBlock[index] = now;
}
// 取出free_DiverseBlock[index]的第一个元素, 给到used_DiverseBlock[index]
// 返回被改变的DiverseBlock
DiverseBlock* convert_free_to_used(int index) {
	DiverseBlock* now = free_DiverseBlock[index];
	free_DiverseBlock[index] = now->next;

	now->next = used_DiverseBlock[index];
	used_DiverseBlock[index] = now;
	return now;
}
// 将free_DiverseBlock[index]分裂成两个free_DiverseBlock[index-1]
// 返回是否分裂成功
bool SplitToSmallBlock(int index) {
	if (index <= 0 || index >= BigMemoryIndex)return false;
	if (free_DiverseBlock[index] == nullptr)return false;

	DiverseBlock* now = free_DiverseBlock[index];
	free_DiverseBlock[index] = now->next;

	DiverseBlock* newBlock = new DiverseBlock();
	newBlock->start_address = now->start_address + convert_index_to_size(index - 1);
	newBlock->next = now;
	now->next = free_DiverseBlock[index - 1];
	free_DiverseBlock[index - 1] = newBlock;
}

template <typename T, size_t BlockSize = BigMemorySize>
class DiverseBlockBufferPool {
public:
	// typedefs...
	typedef T                   value_type;
	typedef value_type*			pointer;
	typedef value_type&			reference;
	typedef value_type const*	const_pointer;
	typedef value_type const&	const_reference;
	typedef size_t              size_type;
	typedef ptrdiff_t           difference_type;

	// rebind...
	template <typename _other> struct rebind {
		typedef DiverseBlockBufferPool<_other> other;
	};

private:
	void allocateDiverseBlock(int size = BigMemorySize);

public:
	// 返回val的地址
	pointer address(reference val)const {
		return &val;
	}
	const_pointer address(const_reference val)const {
		return &val;
	}

	pointer allocate(size_type cnt, const_pointer pHint = 0);
	void deallocate(pointer p, size_type size);

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
		p->~value_type();
	}

public:
	DiverseBlockBufferPool() throw() {}
	DiverseBlockBufferPool(const_reference val) throw() {}
	template<typename _other>
	DiverseBlockBufferPool(DiverseBlockBufferPool<_other> const& val) throw() {}
	~DiverseBlockBufferPool() noexcept {}
public:
	bool operator==(const DiverseBlockBufferPool& A)const {
		return true;
	}
	bool operator!=(const DiverseBlockBufferPool& A)const {
		return false;
	}
};

// 新建内存块, 并将其放入内存块链表中
// 申请一块大小为size的内存
// 将申请到的内存分裂开, 分别接到free_DiverseBlock[0~7]中
// 返回当前内存块的地址
template <typename T, size_t BlockSize>
void DiverseBlockBufferPool<T,BlockSize>::allocateDiverseBlock(int size) {
	//printf("\033[31mallocateBlock\033[0m size=%d\n", size);

	// 申请大内存, 直接转为调用malloc
	if (size > BigMemorySize) {
		char* p = reinterpret_cast<char*>(malloc(size));
		if (p == nullptr) { printf("malloc %d is failed\n", size); throw bad_alloc(); }
		InsertToDiverseBlock(BigMemoryIndex, p);
		return;
	}

	// 先判断是否能由更大的内存分裂出来
	int index = convert_size_to_index(size);
	for (int i = index + 1; i < BigMemoryIndex; i++) {
		// 大内存i分裂成功, 此时获取了两个i-1的内存, 要继续分解到有i为止
		if (SplitToSmallBlock(i)) {
			for (int j = i - 1; j >= index + 1; j--)
				SplitToSmallBlock(j);
			return;
		}
	}


	// 申请1Block, 并将其分解为
	// 2个32B, 1个64B, 1个128B, 1个256B
	char* p = reinterpret_cast<char*>(malloc(BigMemorySize * 2));
	if (p == nullptr) { printf("malloc %d is failed\n", BigMemorySize * 2); throw bad_alloc(); }
	InsertToDiverseBlock(0, p); p += 32;
	InsertToDiverseBlock(0, p); p += 32;
	InsertToDiverseBlock(1, p); p += 64;
	InsertToDiverseBlock(2, p); p += 128;
	InsertToDiverseBlock(3, p); p += 256;
}

// 分配cnt个元素, 类似于malloc
template <typename T, size_t BlockSize>
typename DiverseBlockBufferPool<T, BlockSize>::pointer
DiverseBlockBufferPool<T, BlockSize>::allocate(size_type cnt, const_pointer pHint) {
	//printf("\033[32mallocate\033[0m cnt=%d\n", cnt);

	if (cnt <= 0)return nullptr;
	size_t size = cnt * sizeof(value_type);
	int index = convert_size_to_index(size);

	// 当前内存中还有空余的地方
	if (free_DiverseBlock[index] != nullptr) {
		DiverseBlock* now = convert_free_to_used(index);
		return reinterpret_cast<pointer>(now->start_address);
	}

	// 分配一块新内存
	allocateDiverseBlock(size);
	DiverseBlock* now = convert_free_to_used(index);
	return reinterpret_cast<pointer>(now->start_address);
}

// 释放空间, 类似于free
template <typename T, size_t BlockSize>
void DiverseBlockBufferPool<T, BlockSize>::deallocate(pointer p, size_type size) {
	//printf("\033[33mdeallocate\033[0m p=%X, size=%d\n", (int)p, size);

	if (p == nullptr)return;
	// 将包含p的Block放入free_DiverseBlock[index]中
	int index = convert_size_to_index(size);
	char* address = reinterpret_cast<char*>(p);
	DiverseBlock* now = used_DiverseBlock[index];
	DiverseBlock* last = nullptr;
	while (now != nullptr) {
		if (now->start_address != address) {
			last = now;
			now = now->next;
		}
		else {
			// 从used_DiverseBlock[index]中删除now
			if (last != nullptr)last->next = now->next;
			// 将now放入free_DiverseBlock[index]中
			now->next = free_DiverseBlock[index];
			free_DiverseBlock[index] = now;
			break;
		}
	}
}