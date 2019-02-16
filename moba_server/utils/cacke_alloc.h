#ifndef __CACKE_ALLOC_H
#define __CACKE_ALLOC_H

#ifdef __cplusplus
extern "C"
{
#endif
	//初始化一个指定容量和段大小的内存池
	struct cache_allocer* create_cache_allocer(int capacity,int elem_size);//容量，内存单元大小
	void destroy_cache_allocer(struct cache_allocer* allocer);
	//从内存池中分配一块内存
	void* cache_alloc(struct cache_allocer* allocer,int elem_size);
	void cache_free(struct cache_allocer* allocer,void* mem);
#ifdef __cplusplus
}
#endif
#endif