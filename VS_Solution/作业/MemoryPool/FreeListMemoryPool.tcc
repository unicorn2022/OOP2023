enum {ALIGN = 8};             //С��������ϵ��߽�
enum {MAX_MEMORY = 128};       //С�����������
enum {CNT_OF_LIST = MAX_MEMORY/ALIGN};  //free-lists����

class MemoryPool{
private:
    static size_t ROUND_UP(size_t bytes){       //��bytes�ϵ���8�ı���
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
    static void *refill(size_t n);          //����Сn�Ŀ�������
    static char *chunk_alloc(size_t size, int &nobjs);  //���ڴ���з���size*nobjs��С���ڴ�ռ�

    static char *start_free;    //�ڴ�ؿ�ʼ��ַ
    static char *end_free;      //�ڴ�ؽ�����ַ
    static size_t heap_size;

public:
    static void * allocate(size_t n);
    static void deallocate(void *p, size_t n);
    static void * reallocate(void *p, size_t old_sz, size_t new_sz);
};

void * MemoryPool::allocate(size_t n){
    obj * volatile *my_free_list;
    obj * result;
    if(n > (size_t)MAX_MEMORY){    //������ֵ��ͨ����һ������������
        return __malloc_alloc::allocate(n);
    }

    my_free_list = free_list + FREELIST_INDEX(n);   //Ѱ�Һ��ʵĿ�������
    result = *my_free_list;
    if(result==0){         //�����������޿��пռ�
        void *r = refill(ROUND_UP(n));  //����������
        return r;
    }
    *my_free_list = result->next;
    return result;
}

void  MemoryPool::deallocate(void *p, size_t n){
    obj *q = (obj*) p;
    obj * volatile * my_free_list;
    if(n > (size_t)MAX_MEMORY){    //������ֵ��ͨ����һ������������
        __malloc_alloc::deallocate(p, n);
        return;
    }
    my_free_list = free_list + FREELIST_INDEX(n);   //Ѱ�Ҷ�Ӧ�Ŀ�������
    q->next = *my_free_list;      //���ڴ���������������
    *my_free_list = q;
    return;
}

void *MemoryPool::refill(size_t size){
    int n = 20;
    char *chunk = chunk_alloc(size, n); //��������n*nobjs�ռ䣬ʵ�ʵõ�������Ϊnobjs
    obj *volatile * my_free_list;
    obj *result;
    obj* current_obj, *next_obj;
    int i;

    //���ֻ���һ�����飬�������ͷ����������ʹ�ã�free_list���½��
    if(1==n) return chunk;
    my_free_list = free_list + FREELIST_INDEX(size);

    result = (obj *)chunk;
    *my_free_list = next_obj = (obj *)(chunk + size);
    for(i=1;;i++){      //�����������
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

    if(bytes_left >= total_bytes){ //�ڴ��ʣ��ռ���ȫ����������
        result = start_free;
        start_free += total_bytes;
        return result;
    }
    else if(bytes_left>=size){      //�ڴ��ʣ��ռ䲻����ȫ���������������㹻��Ӧһ�����ϵ�����
        nobjs = bytes_left/size;
        total_bytes = size*nobjs;
        result = start_free;
        start_free += total_bytes;
        return result;
    }
    else{   //�ڴ��ʣ��ռ���һ������Ĵ�С���޷��ṩ
        size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size>>4);
        if(bytes_left>0){ //�ڴ���л���һЩ��ͷ��������ʵ���free_list;
            obj * volatile *my_free_list = free_list + FREELIST_INDEX(bytes_left);
            ((obj *)start_free) -> next = *my_free_list;
            *my_free_list = (obj *)start_free;
        }
        start_free = (char*)malloc(bytes_to_get); //����heap�ռ䣬���������ڴ��
        if(0==start_free){  //heap�ռ䲻�㣬mallocʧ��
            int i;
            obj *volatile *my_free_list, *p;
            //��鵱ǰfree_list�����Ƿ��б�size��Ŀ��еĿռ䣬�����������������
            for(i = size; i <= MAX_MEMORY; i += ALIGN){
                my_free_list = free_list + FREELIST_INDEX(i);
                p = *my_free_list;
                if(0 != p){         //��free_list�ҵ����ʵ�����
                    *my_free_list = p->next;
                    start_free = (char *)p;
                    end_free = start_free + i;
                    return chunk_alloc(size, nobjs); //�ݹ�����Լ�������nobjs
                    //�κβ������ͷ�ܽ��������ʵ���free_list�б���
                }
            }
            end_free = 0; //ִ�е���һ��˵��ɽ��ˮ����
            //���õ�һ��������������out-of-memory�����ܷ񾡵���
            start_free = (char *)__malloc_alloc::allocate(bytes_to_get);
        }
        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;
        return chunk_alloc(size, nobjs); //�ݹ�����Լ���Ϊ������nobjs
    }
}
