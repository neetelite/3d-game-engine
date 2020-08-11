#if 0
struct MemAlloc
{
	u32 id;

	char *file;
	u32 line;
};

struct MemAllocArray
{
	u64 count;
	u64 count_max;
	struct MemAlloc *array;

	u64 freed;
};

global struct MemAllocArray global_alloc_array;

void
mem_alloc_array_init(struct MemAllocArray *alloc_array, u32 count_max)
{
	alloc_array->count = 0;
	alloc_array->count_max = count_max;

	/* NOTE(lungu): The only place where we don't use mem_alloc() */
	alloc_array->array = malloc(count_max * sizeof(struct MemAlloc));
}

void
mem_alloc_array_grow(struct MemAllocArray *alloc_array)
{
	alloc_array->count_max *= 2;
	struct MemAlloc *tmp_array = malloc(alloc_array->count_max * sizeof(struct MemAlloc));
	memcpy(tmp_array, alloc_array->array, (alloc_array->count / 2) * sizeof(struct MemAlloc));
	free(alloc_array->array);
	alloc_array->array = tmp_array;
}

void
mem_alloc_array_add(struct MemAllocArray *alloc_array, char *file, u32 line)
{
	if(alloc_array->count == alloc_array->count_max)
	{
		mem_alloc_array_grow(alloc_array);
	}

	struct MemAlloc *alloc = alloc_array->array + alloc_array->count;
	alloc->id = alloc_array->count;

	/* TODO(lungu): Does this work? */
	//alloc->file = CSTR(file);
	alloc->file = file;
	alloc->line = line;

	++alloc_array->count;
}

void
mem_alloc_array_free(struct MemAllocArray *alloc_array)
{
	++alloc_array->freed;
}
#endif

global u64 global_alloc_count;
global u64 global_free_count;

void *
mem_alloc(u32 count, u64 size, bool clear)
{
	void *result = NULL;

	if(clear) result = calloc(count, size);
	else result = malloc(count * size);

	#if BUILD_DEBUG
	++global_alloc_count;
	#endif/* BULD_DEBUG */

	return(result);
}

void
mem_free(void *pointer)
{
	if(!pointer) return;

	#if BUILD_DEBUG
	++global_free_count;
	#endif/* BULD_DEBUG */
	free(pointer);
}
