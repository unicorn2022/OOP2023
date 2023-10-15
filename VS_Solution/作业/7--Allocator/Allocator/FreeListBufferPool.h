//FreeListBufferPool: ����16��freelist�Ż���allocator
#pragma once
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;
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
    // ����val�ĵ�ַ
    pointer address(reference val)const {
        return &val;
    }
    const_pointer address(const_reference val)const {
        return &val;
    }

    pointer allocate(size_type cnt, char* pHint = 0);

    void deallocate(pointer p, size_type n);

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

const int ALIGN = 8;        //С�������Ӧ�Ĵ�СΪ8�ı���
const int MAX_MEMORY = 128; // С���������Ϊ128B
const int CNT_OF_LIST = MAX_MEMORY / ALIGN; //free-listsһ����16��

//��size�ϵ�ΪALIGN�ı���
static size_t convert_size_to_size(size_t size) {
    return (size + ALIGN - 1) & ~(ALIGN - 1);
}

//����size��С�жϵ�ǰ���������ĸ�FreeList
static size_t convert_size_to_index(size_t size) {
    return (size + ALIGN - 1) / ALIGN - 1;
}

union Block {
    union Block* next; // Free_List����ָ�����һ���ڴ�
    char  data;        // ��ǰ�ڴ��ŵ�����
};
Block* Free_List[CNT_OF_LIST];
char* start_free = nullptr;   // �ڴ�ؿ�ʼ��ַ
char* end_free = nullptr;     // �ڴ�ؽ�����ַ
size_t heap_size;             // �ڴ�صĴ�С 
//���ڴ���з���size*n��С�Ŀռ�
char* chunk_alloc(size_t size, int& n) {
    size_t total_size = n * size;
    size_t bytes_left = end_free - start_free;

    // �ڴ��ʣ��ռ���ȫ��������
    if (bytes_left >= total_size) {
        char* ans = start_free;
        start_free += total_size;
        return ans;
    }

    // �ڴ��ʣ��ռ䲻����ȫ��������, �������ܷ���һ����
    else if (bytes_left >= size) {
        n = bytes_left / size;
        total_size = n * size;
        char* ans = start_free;
        start_free += total_size;
        return ans;
    }

    // �ڴ��ʣ��ռ��޷�����һ������
    else {
        // �Ƚ��ڴ����ʣ��Ŀռ������ʵ���Free_List
        if (bytes_left > 0) {
            int index = convert_size_to_index(bytes_left);
            reinterpret_cast<Block*>(start_free)->next = Free_List[index];
            Free_List[index] = reinterpret_cast<Block*>(start_free);
        }

        size_t bytes_to_get = 2 * total_size + convert_size_to_size(heap_size >> 4);
        start_free = (char*)malloc(bytes_to_get);

        // �ѿռ䲻��, mallocʧ��
        if (start_free == nullptr) {
            // ��鵱ǰFree_List, ���Ƿ��б�size��Ŀ��пռ�, ���������
            for (int i = size; i <= MAX_MEMORY; i += ALIGN) {
                int index = convert_size_to_index(i);
                Block* now = Free_List[index];
                if (now != nullptr) {
                    Free_List[index] = now->next;
                    // ����ǰ�Ŀռ�����ڴ��
                    start_free = reinterpret_cast<char*>(now);
                    end_free = start_free + i;
                    // �ݹ����chunk_alloc, �������·���
                    return chunk_alloc(size, n);
                }
            }
            // Free_List��Ҳû�п���ռ�, ���Է���������
            throw bad_alloc();
        }

        // malloc�ɹ�
        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;
        // �ݹ�����Լ�, ����malloc�����ڴ�, �޸�n
        return chunk_alloc(size, n);
    }
}
//����СΪsize�Ŀ�������
void* refill(size_t size) {
    int n = 20;
    // ��������n*size�Ŀռ�
    char* chunk = chunk_alloc(size, n);
    
    // ���ֻ�����뵽1*size�Ŀռ�, ��ֱ�ӷ����������, Free_Listû���½ڵ�
    if (n == 1)return reinterpret_cast<void*>(chunk);

    // ���뵽>=2*size�Ŀռ�
    int index = convert_size_to_index(size);
    // ����0������������
    Block* ans = reinterpret_cast<Block*>(chunk);
    // ����1~n-1��ռ�ŵ�Free_List��
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

// ����ռ�, ������malloc
template <typename T>
typename FreeListBufferPool<T>::pointer
FreeListBufferPool<T>::allocate(size_type cnt, char* pHint) {
    int size = cnt * sizeof(value_type);
    // ����MAX_MEMORY, ֱ����malloc�����ڴ�
    if (size > MAX_MEMORY) {
        return reinterpret_cast<pointer>(malloc(size));
    }

    int index = convert_size_to_index(size);
    Block* now = Free_List[index];
    // �������޿��пռ�, ����refill����һ���ڴ�
    if (now == nullptr) {
        void* ans = refill(convert_size_to_size(size));
        return reinterpret_cast<pointer>(ans);
    }
    // �������п��пռ�, ֱ�ӷ���
    Free_List[index] = Free_List[index]->next;
    return reinterpret_cast<pointer>(now);
}

// �ͷſռ�, ������free
template <typename T>
void FreeListBufferPool<T>::deallocate(pointer p, size_type n) {
    int size = n * sizeof(value_type);
    // ����MAX_MEMORY, ֱ����free�ͷ��ڴ�
    if (size > MAX_MEMORY) {
        free(p);
        return;
    }
    // ��p��Ӧ���ڴ�ŵ�Free_List��
    int index = convert_size_to_index(size);
    Block* now = reinterpret_cast<Block*>(p);
    now->next = Free_List[index];
    Free_List[index] = now;
}
