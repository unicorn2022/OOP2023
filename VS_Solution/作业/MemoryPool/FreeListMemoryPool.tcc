enum {ALIGN = 8};             //小型区块的上调边界
enum {MAX_MEMORY = 128};       //小型区块的上限
enum {CNT_OF_LIST = MAX_MEMORY/ALIGN};  //free-lists个数

class MemoryPool{
private:
    static size_t ROUND_UP(size_t bytes){       //将bytes上调至8的倍数
        return (((bytes) + ALIGN-1) & ~(ALIGN - 1));
    }
    union obj{
        union obj * next;
        char client_data[1];
    };
    static obj * volatile free_list[CNT_OF_LIST];
    static size_t FREELIST_INDEX(size_t bytes){
        return (((bytes) + ALIGN-1)/ALIGN - 1);
    }
    static void *refill(size_t n);          //填充大小n的空闲链表
    static char *chunk_alloc(size_t size, int &nobjs);  //从内存池中分配size*nobjs大小的内存空间

    static char *start_free;    //内存池开始地址
    static char *end_free;      //内存池结束地址
    static size_t heap_size;

public:
    static void * allocate(size_t n);
    static void deallocate(void *p, size_t n);
    static void * reallocate(void *p, size_t old_sz, size_t new_sz);
};

void * MemoryPool::allocate(size_t n){
    obj * volatile *my_free_list;
    obj * result;
    if(n > (size_t)MAX_MEMORY){    //大于阈值则通过第一级分配器分配
        return __malloc_alloc::allocate(n);
    }

    my_free_list = free_list + FREELIST_INDEX(n);   //寻找合适的空闲链表
    result = *my_free_list;
    if(result==0){         //空闲链表中无空闲空间
        void *r = refill(ROUND_UP(n));  //填充空闲链表
        return r;
    }
    *my_free_list = result->next;
    return result;
}

void  MemoryPool::deallocate(void *p, size_t n){
    obj *q = (obj*) p;
    obj * volatile * my_free_list;
    if(n > (size_t)MAX_MEMORY){    //大于阈值则通过第一级分配器分配
        __malloc_alloc::deallocate(p, n);
        return;
    }
    my_free_list = free_list + FREELIST_INDEX(n);   //寻找对应的空闲链表
    q->next = *my_free_list;      //将内存区域插入空闲链表
    *my_free_list = q;
    return;
}

void *MemoryPool::refill(size_t size){
    int n = 20;
    char *chunk = chunk_alloc(size, n); //尝试请求n*nobjs空间，实际得到的数量为nobjs
    obj *volatile * my_free_list;
    obj *result;
    obj* current_obj, *next_obj;
    int i;

    //如果只获得一个区块，这个区块就分配给调用者使用，free_list无新结点
    if(1==n) return chunk;
    my_free_list = free_list + FREELIST_INDEX(size);

    result = (obj *)chunk;
    *my_free_list = next_obj = (obj *)(chunk + size);
    for(i=1;;i++){      //构造空闲链表
        current_obj = next_obj;
        next_obj = (obj *)((char *)next_obj + size);
        if(n-1==i){
            current_obj->next = 0;
            break;
        }
        else{
            current_obj->next = next_obj;
        }
    }   
    return result; 
}

char * MemoryPool::chunk_alloc(size_t size, int &nobjs){
    char * result;
    size_t total_bytes = size*nobjs;
    size_t bytes_left = end_free - start_free;

    if(bytes_left >= total_bytes){ //内存池剩余空间完全满足需求量
        result = start_free;
        start_free += total_bytes;
        return result;
    }
    else if(bytes_left>=size){      //内存池剩余空间不能完全满足需求量，但足够供应一个以上的区块
        nobjs = bytes_left/size;
        total_bytes = size*nobjs;
        result = start_free;
        start_free += total_bytes;
        return result;
    }
    else{   //内存池剩余空间连一个区块的大小都无法提供
        size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size>>4);
        if(bytes_left>0){ //内存池中还有一些零头，先配给适当的free_list;
            obj * volatile *my_free_list = free_list + FREELIST_INDEX(bytes_left);
            ((obj *)start_free) -> next = *my_free_list;
            *my_free_list = (obj *)start_free;
        }
        start_free = (char*)malloc(bytes_to_get); //配置heap空间，用来补充内存池
        if(0==start_free){  //heap空间不足，malloc失败
            int i;
            obj *volatile *my_free_list, *p;
            //检查当前free_list，看是否有比size大的空闲的空间，并对其进行重新配置
            for(i = size; i <= MAX_MEMORY; i += ALIGN){
                my_free_list = free_list + FREELIST_INDEX(i);
                p = *my_free_list;
                if(0 != p){         //在free_list找到合适的区域
                    *my_free_list = p->next;
                    start_free = (char *)p;
                    end_free = start_free + i;
                    return chunk_alloc(size, nobjs); //递归调用自己，修正nobjs
                    //任何残余的零头总将被编入适当的free_list中备用
                }
            }
            end_free = 0; //执行到这一步说明山穷水尽了
            //调用第一级配置器，看看out-of-memory机制能否尽点力
            start_free = (char *)__malloc_alloc::allocate(bytes_to_get);
        }
        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;
        return chunk_alloc(size, nobjs); //递归调用自己，为了修正nobjs
    }
}
