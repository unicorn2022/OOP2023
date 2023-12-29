//BlockBufferPool: 使用内存池策略实现的allocator
#pragma once
#include <cstdio>
#include <cstring>
#include <iostream>
#include <climits>
#include <cstddef>
using namespace std;

// 内存池, 以Block为单位
struct Block {
	char* start_address; // 当前Block的起始地址
	bool  isAllocate;	 // 表明当前Block是否是通过allocate分配的
	char* now_address;	 // 当前Block剩余空间的地址
	int last_size;		 // 当前Block剩余的空间
	Block* next;		 // Block链表的下一个元素
}; 
Block* current_Block; // 内存块链表的头指针;

void FreeAll() {
	Block* now = current_Block;
	while (now != nullptr) {
		Block* next = now->next;
		if (now->isAllocate) { free(now->start_address);}
		delete now;
		now = next;
	}
	current_Block = nullptr;
}

template <typename T, size_t BlockSize = 4096>
class BlockBufferPool {
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
		typedef BlockBufferPool<_other> other;
	};

private:
	Block* allocateBlock(char* p = nullptr, int size = BlockSize);

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
	BlockBufferPool() throw() {}
	BlockBufferPool(const_reference val) throw() {}
	template<typename _other>
	BlockBufferPool(BlockBufferPool<_other> const& val) throw() {}
	~BlockBufferPool() noexcept {}
public:
	bool operator==(const BlockBufferPool& A)const {
		return true;
	}
	bool operator!=(const BlockBufferPool& A)const {
		return false;
	}
};

// 新建内存块, 并将其放入内存块链表中
// 当p==NULL的时候, 表示需要申请一块大小为size的内存
// 当p!=NULL的时候, 表示现在要释放内存, 将p指向的大小为size的内存放入可用列表中
// 返回当前内存块的地址
template <typename T, size_t BlockSize>
Block* BlockBufferPool<T, BlockSize>::allocateBlock(char* p, int size) {
	//printf("\033[31mallocateBlock\033[0m p=%X, size=%d\n", (int)p, size);
	Block* now = new Block();
	if (now == nullptr) {
		cout << "new Block() error\n";
		throw std::bad_alloc();
	}
	// 申请一块新的内存
	if (p == nullptr) {
		p = reinterpret_cast<char*>(malloc(size));
		now->isAllocate = true;
	}
	// deallocate释放掉的内存
	else {
		now->isAllocate = false;
	}
	now->start_address = p;
	now->now_address = p;
	now->last_size = size;
	now->next = current_Block;
	current_Block = now;
	return now;
}

// 分配cnt个元素, 类似于malloc
template <typename T, size_t BlockSize>
typename BlockBufferPool<T, BlockSize>::pointer
BlockBufferPool<T, BlockSize>::allocate(size_type cnt, const_pointer pHint) {
	if (cnt <= 0)return nullptr;
	//printf("\033[32mallocate\033[0m cnt=%d\n", cnt);
	size_t num = cnt * sizeof(value_type);
	pointer ans = nullptr;
	Block* now = nullptr;

	// 待分配的内存比一个Block大, 则直接调用malloc分配内存
	if (num > BlockSize) {
		now = allocateBlock(nullptr, num);
		ans = reinterpret_cast<pointer>(now->start_address);
		now->now_address = nullptr;
		now->last_size = 0;
		return ans;
	}

	// 遍历current_Block, 判断是否有Block块能够放入数据
	//if (current_Block == nullptr)printf("\033[31m------current_Block is empty\n\033[0m");
	//else printf("------current_Block fist size is %d\n", current_Block->last_size);
	now = current_Block;
	while (now != nullptr) {
		if (now->last_size < num) { now = now->next; continue; }
		else break;
	}
	//printf("allocate cnt=%d num=%d\n", cnt, num);
	// 现有的Block都放不进去, 则重新申请一块内存
	if (now == nullptr) now = allocateBlock(nullptr, BlockSize);

	// 将当前元素放入now指向的Block中
	ans = reinterpret_cast<pointer>(now->now_address);
	now->now_address += num;
	now->last_size -= num;
	return ans;
}

// 释放空间, 类似于free
template <typename T, size_t BlockSize>
void BlockBufferPool<T, BlockSize>::deallocate(pointer p, size_type size) {
	//printf("\033[33mdeallocate\033[0m p=%X, size=%d\n", (int)p, size);
	if (p == nullptr)return;
	// 将p指向的size大小的内存, 作为新的Block块放入current_Block中
	//allocateBlock(reinterpret_cast<char*>(p), size);
}