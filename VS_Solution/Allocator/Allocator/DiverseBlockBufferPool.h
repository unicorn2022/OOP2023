//DiverseBlockBufferPool: ��ԭ���ڴ�ز��ԵĻ�����, ����Block��Ĵ�С��Ψһ
#pragma once
#include <cstdio>
#include <cstring>
#include <iostream>
#include <climits>
#include <cstddef>
using namespace std;

// �ڴ��, ��BlockΪ��λ
struct DiverseBlock {
	char* start_address; // ��ǰBlock����ʼ��ַ
	DiverseBlock* next;	 // Block�������һ��Ԫ��
}; 
// �ڴ�������ͷָ��, ��Ϊ32B, 64B, 128B, 256B, >256B
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
// ��pָ����ڴ����free_DiverseBlock[index]��
void InsertToDiverseBlock(int index, char* p) {
	DiverseBlock* now = new DiverseBlock();
	now->start_address = p;
	now->next = free_DiverseBlock[index];
	free_DiverseBlock[index] = now;
}
// ȡ��free_DiverseBlock[index]�ĵ�һ��Ԫ��, ����used_DiverseBlock[index]
// ���ر��ı��DiverseBlock
DiverseBlock* convert_free_to_used(int index) {
	DiverseBlock* now = free_DiverseBlock[index];
	free_DiverseBlock[index] = now->next;

	now->next = used_DiverseBlock[index];
	used_DiverseBlock[index] = now;
	return now;
}
// ��free_DiverseBlock[index]���ѳ�����free_DiverseBlock[index-1]
// �����Ƿ���ѳɹ�
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

// �½��ڴ��, ����������ڴ��������
// ����һ���СΪsize���ڴ�
// �����뵽���ڴ���ѿ�, �ֱ�ӵ�free_DiverseBlock[0~7]��
// ���ص�ǰ�ڴ��ĵ�ַ
template <typename T, size_t BlockSize>
void DiverseBlockBufferPool<T,BlockSize>::allocateDiverseBlock(int size) {
	//printf("\033[31mallocateBlock\033[0m size=%d\n", size);

	// ������ڴ�, ֱ��תΪ����malloc
	if (size > BigMemorySize) {
		char* p = reinterpret_cast<char*>(malloc(size));
		if (p == nullptr) { printf("malloc %d is failed\n", size); throw bad_alloc(); }
		InsertToDiverseBlock(BigMemoryIndex, p);
		return;
	}

	// ���ж��Ƿ����ɸ�����ڴ���ѳ���
	int index = convert_size_to_index(size);
	for (int i = index + 1; i < BigMemoryIndex; i++) {
		// ���ڴ�i���ѳɹ�, ��ʱ��ȡ������i-1���ڴ�, Ҫ�����ֽ⵽��iΪֹ
		if (SplitToSmallBlock(i)) {
			for (int j = i - 1; j >= index + 1; j--)
				SplitToSmallBlock(j);
			return;
		}
	}


	// ����1Block, ������ֽ�Ϊ
	// 2��32B, 1��64B, 1��128B, 1��256B
	char* p = reinterpret_cast<char*>(malloc(BigMemorySize * 2));
	if (p == nullptr) { printf("malloc %d is failed\n", BigMemorySize * 2); throw bad_alloc(); }
	InsertToDiverseBlock(0, p); p += 32;
	InsertToDiverseBlock(0, p); p += 32;
	InsertToDiverseBlock(1, p); p += 64;
	InsertToDiverseBlock(2, p); p += 128;
	InsertToDiverseBlock(3, p); p += 256;
}

// ����cnt��Ԫ��, ������malloc
template <typename T, size_t BlockSize>
typename DiverseBlockBufferPool<T, BlockSize>::pointer
DiverseBlockBufferPool<T, BlockSize>::allocate(size_type cnt, const_pointer pHint) {
	//printf("\033[32mallocate\033[0m cnt=%d\n", cnt);

	if (cnt <= 0)return nullptr;
	size_t size = cnt * sizeof(value_type);
	int index = convert_size_to_index(size);

	// ��ǰ�ڴ��л��п���ĵط�
	if (free_DiverseBlock[index] != nullptr) {
		DiverseBlock* now = convert_free_to_used(index);
		return reinterpret_cast<pointer>(now->start_address);
	}

	// ����һ�����ڴ�
	allocateDiverseBlock(size);
	DiverseBlock* now = convert_free_to_used(index);
	return reinterpret_cast<pointer>(now->start_address);
}

// �ͷſռ�, ������free
template <typename T, size_t BlockSize>
void DiverseBlockBufferPool<T, BlockSize>::deallocate(pointer p, size_type size) {
	//printf("\033[33mdeallocate\033[0m p=%X, size=%d\n", (int)p, size);

	if (p == nullptr)return;
	// ������p��Block����free_DiverseBlock[index]��
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
			// ��used_DiverseBlock[index]��ɾ��now
			if (last != nullptr)last->next = now->next;
			// ��now����free_DiverseBlock[index]��
			now->next = free_DiverseBlock[index];
			free_DiverseBlock[index] = now;
			break;
		}
	}
}