//BasicAllocator: ʹ��malloc(), free()ʵ�ֵ�allocator
#pragma once
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

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

// ����ռ�, ������malloc
template <typename T>
typename BasicAllocator<T>::pointer
BasicAllocator<T>::allocate(size_type cnt, char* pHint) {
    if (cnt <= 0)return nullptr;
    if (max_size() < cnt)throw std::bad_alloc();

    void* pMem = malloc(cnt * sizeof(value_type));
    if (pMem == nullptr)throw std::bad_alloc();
    return static_cast<pointer>(pMem);
}

// �ͷſռ�, ������free
template <typename T>
void BasicAllocator<T>::deallocate(pointer p, size_type n) {
    free(p);
}