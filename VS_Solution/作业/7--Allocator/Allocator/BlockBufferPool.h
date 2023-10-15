//BlockBufferPool: ʹ���ڴ�ز���ʵ�ֵ�allocator
#pragma once
#include <cstdio>
#include <cstring>
#include <iostream>
#include <climits>
#include <cstddef>
using namespace std;

// �ڴ��, ��BlockΪ��λ
struct Block {
	char* start_address; // ��ǰBlock����ʼ��ַ
	bool  isAllocate;	 // ������ǰBlock�Ƿ���ͨ��allocate�����
	char* now_address;	 // ��ǰBlockʣ��ռ�ĵ�ַ
	int last_size;		 // ��ǰBlockʣ��Ŀռ�
	Block* next;		 // Block�������һ��Ԫ��
}; 
Block* current_Block; // �ڴ�������ͷָ��;

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
	// ����val�ĵ�ַ
	pointer address(reference val)const {
		return &val;
	}
	const_pointer address(const_reference val)const {
		return &val;
	}

	pointer allocate(size_type cnt, const_pointer pHint = 0);
	void deallocate(pointer p, size_type size);

	
	// �����ɵ����Ԫ��
	size_type max_size() const throw() {
		return static_cast<size_type>(-1) / sizeof(value_type);
	}
	
	// �ڵ�ַp��ָ��Ŀռ�, ʹ��val�������
	// ��Ҫʹ�õ�placement new, �Ա鱣֤���õ����캯��
	// placement new: ���û�ָ�����ڴ�λ���Ϲ����µĶ���, ���������в���Ҫ��������ڴ�, ֻ��Ҫ���ö���Ĺ��캯��
	void construct(pointer p, const_reference val) {
		new (p) value_type(val);
	}

	// ����pָ����ڴ���е�����
	// һ��ͨ����ʽ��������������ִ��
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

// �½��ڴ��, ����������ڴ��������
// ��p==NULL��ʱ��, ��ʾ��Ҫ����һ���СΪsize���ڴ�
// ��p!=NULL��ʱ��, ��ʾ����Ҫ�ͷ��ڴ�, ��pָ��Ĵ�СΪsize���ڴ��������б���
// ���ص�ǰ�ڴ��ĵ�ַ
template <typename T, size_t BlockSize>
Block* BlockBufferPool<T, BlockSize>::allocateBlock(char* p, int size) {
	//printf("\033[31mallocateBlock\033[0m p=%X, size=%d\n", (int)p, size);
	Block* now = new Block();
	if (now == nullptr) {
		cout << "new Block() error\n";
		throw std::bad_alloc();
	}
	// ����һ���µ��ڴ�
	if (p == nullptr) {
		p = reinterpret_cast<char*>(malloc(size));
		now->isAllocate = true;
	}
	// deallocate�ͷŵ����ڴ�
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

// ����cnt��Ԫ��, ������malloc
template <typename T, size_t BlockSize>
typename BlockBufferPool<T, BlockSize>::pointer
BlockBufferPool<T, BlockSize>::allocate(size_type cnt, const_pointer pHint) {
	if (cnt <= 0)return nullptr;
	//printf("\033[32mallocate\033[0m cnt=%d\n", cnt);
	size_t num = cnt * sizeof(value_type);
	pointer ans = nullptr;
	Block* now = nullptr;

	// ��������ڴ��һ��Block��, ��ֱ�ӵ���malloc�����ڴ�
	if (num > BlockSize) {
		now = allocateBlock(nullptr, num);
		ans = reinterpret_cast<pointer>(now->start_address);
		now->now_address = nullptr;
		now->last_size = 0;
		return ans;
	}

	// ����current_Block, �ж��Ƿ���Block���ܹ���������
	//if (current_Block == nullptr)printf("\033[31m------current_Block is empty\n\033[0m");
	//else printf("------current_Block fist size is %d\n", current_Block->last_size);
	now = current_Block;
	while (now != nullptr) {
		if (now->last_size < num) { now = now->next; continue; }
		else break;
	}
	//printf("allocate cnt=%d num=%d\n", cnt, num);
	// ���е�Block���Ų���ȥ, ����������һ���ڴ�
	if (now == nullptr) now = allocateBlock(nullptr, BlockSize);

	// ����ǰԪ�ط���nowָ���Block��
	ans = reinterpret_cast<pointer>(now->now_address);
	now->now_address += num;
	now->last_size -= num;
	return ans;
}

// �ͷſռ�, ������free
template <typename T, size_t BlockSize>
void BlockBufferPool<T, BlockSize>::deallocate(pointer p, size_type size) {
	//printf("\033[33mdeallocate\033[0m p=%X, size=%d\n", (int)p, size);
	if (p == nullptr)return;
	// ��pָ���size��С���ڴ�, ��Ϊ�µ�Block�����current_Block��
	//allocateBlock(reinterpret_cast<char*>(p), size);
}