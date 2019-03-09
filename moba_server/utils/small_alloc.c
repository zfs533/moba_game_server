#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "small_alloc.h"
#include "cacke_alloc.h"

#define SMALL_ELEM_NUM 10*1024
#define SMALL_ELEM_SIZE 128

static struct cache_allocer* small_alocer = NULL;

void* small_alloc(int size)
{
	if(small_alocer == NULL)
	{
		small_alocer = create_cache_allocer(SMALL_ELEM_NUM,SMALL_ELEM_SIZE);
	}
	return cache_alloc(small_alocer,size);
}

void small_free(void* mem)
{
	if(small_alocer == NULL)
	{
		small_alocer = create_cache_allocer(SMALL_ELEM_NUM,SMALL_ELEM_SIZE);
	}
	cache_free(small_alocer,mem);
}